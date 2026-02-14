#include "gtest/gtest.h"
#include "lueing_time.h"
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std::chrono_literals;

TEST(LueingTimeTest, GetCurrentTimeWithFormat) {
    // Test %Y format - should return current year
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_c);
    
    long long expected_year = now_tm.tm_year + 1900;
    long long got_year = lueing::TimeUtil::GetCurrentTimeWithFormat("%Y");
    EXPECT_EQ(expected_year, got_year);

    // Test %Y%m%d format - should return current YYYYMMDD
    std::ostringstream oss_ymd;
    oss_ymd << std::put_time(&now_tm, "%Y%m%d");
    long long expected_ymd = std::stoll(oss_ymd.str());
    long long got_ymd = lueing::TimeUtil::GetCurrentTimeWithFormat("%Y%m%d");
    // Allow for date change during test by checking within a small window
    EXPECT_TRUE(got_ymd >= expected_ymd - 1 && got_ymd <= expected_ymd + 1) 
        << "Expected ~" << expected_ymd << ", got " << got_ymd;
}

// Helper to build a timestamp in YYYYMMDDHHMMSSmmm format
static long long BuildTimestamp(int year, int month, int day, int hour, int minute, int second, int msec) {
    long long result = 0;
    result += static_cast<long long>(year) * 10000000000000LL;
    result += static_cast<long long>(month) * 100000000000LL;
    result += static_cast<long long>(day) * 1000000000LL;
    result += static_cast<long long>(hour) * 10000000LL;
    result += static_cast<long long>(minute) * 100000LL;
    result += static_cast<long long>(second) * 1000LL;
    result += static_cast<long long>(msec);
    return result;
}

// Helper to compute expected result after adding seconds (mirrors implementation)
static long long ExpectedAddSeconds(int year, int month, int day, int hour, int minute, int second, int msec, long long add_seconds) {
    std::tm tm_time = {};
    tm_time.tm_year = year - 1900;
    tm_time.tm_mon = month - 1;
    tm_time.tm_mday = day;
    tm_time.tm_hour = hour;
    tm_time.tm_min = minute;
    tm_time.tm_sec = second;

    std::time_t tt = std::mktime(&tm_time);
    tt += static_cast<time_t>(add_seconds);
    std::tm *new_tm = std::localtime(&tt);

    long long result = 0;
    result += static_cast<long long>(new_tm->tm_year + 1900) * 10000000000000LL;
    result += static_cast<long long>(new_tm->tm_mon + 1) * 100000000000LL;
    result += static_cast<long long>(new_tm->tm_mday) * 1000000000LL;
    result += static_cast<long long>(new_tm->tm_hour) * 10000000LL;
    result += static_cast<long long>(new_tm->tm_min) * 100000LL;
    result += static_cast<long long>(new_tm->tm_sec) * 1000LL;
    result += static_cast<long long>(msec);
    return result;
}

TEST(LueingTimeTest, CurrentTimeAddSeconds_Simple)
{
    // 2025-01-01 00:00:00.000 + 1 second
    long long start = BuildTimestamp(2025, 1, 1, 0, 0, 0, 0);
    long long got = lueing::TimeUtil::CurrentTimeAddSeconds(start, 1);
    long long expect = ExpectedAddSeconds(2025, 1, 1, 0, 0, 0, 0, 1);
    EXPECT_EQ(expect, got);
}

TEST(LueingTimeTest, CurrentTimeAddSeconds_PreserveMilliseconds)
{
    // 2025-03-14 15:09:26.123 + 5 seconds -> milliseconds should be preserved
    long long start = BuildTimestamp(2025, 3, 14, 15, 9, 26, 123);
    long long got = lueing::TimeUtil::CurrentTimeAddSeconds(start, 5);
    long long expect = ExpectedAddSeconds(2025, 3, 14, 15, 9, 26, 123, 5);
    EXPECT_EQ(expect, got);
}

TEST(LueingTimeTest, CurrentTimeAddSeconds_CrossDay)
{
    // 2025-12-31 23:59:59.999 + 2 seconds -> should cross into next year
    long long start = BuildTimestamp(2025, 12, 31, 23, 59, 59, 999);
    long long got = lueing::TimeUtil::CurrentTimeAddSeconds(start, 2);
    long long expect = ExpectedAddSeconds(2025, 12, 31, 23, 59, 59, 999, 2);
    EXPECT_EQ(expect, got);
}

TEST(LueingTimeTest, CurrentTimeAddSeconds_Negative)
{
    // 2025-06-15 12:00:00.500 - 3600 seconds (go back 1 hour)
    long long start = BuildTimestamp(2025, 6, 15, 12, 0, 0, 500);
    long long got = lueing::TimeUtil::CurrentTimeAddSeconds(start, -3600);
    long long expect = ExpectedAddSeconds(2025, 6, 15, 12, 0, 0, 500, -3600);
    EXPECT_EQ(expect, got);
}

TEST(LueingTimeTest, YearMonthAddMonths_Simple)
{
    EXPECT_EQ(2511, lueing::TimeUtil::YearMonthAddMonths(2510, 1));
}

TEST(LueingTimeTest, YearMonthAddMonths_CrossYear)
{
    EXPECT_EQ(2601, lueing::TimeUtil::YearMonthAddMonths(2510, 3));
}

TEST(LueingTimeTest, YearMonthAddMonths_Negative)
{
    EXPECT_EQ(2508, lueing::TimeUtil::YearMonthAddMonths(2510, -2));
}

TEST(LueingTimeTest, YearMonthAddMonths_NegativeCrossYear)
{
    EXPECT_EQ(2410, lueing::TimeUtil::YearMonthAddMonths(2501, -3));
}

TEST(LueingTimeTest, GetCurrentTimeInSeconds_IsCloseToNow)
{
    auto now_seconds = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    long long got = lueing::TimeUtil::GetCurrentTimeInSeconds();
    EXPECT_NEAR(static_cast<double>(now_seconds), static_cast<double>(got), 1.0); // Allow a tolerance of 1 second
}

TEST(LueingTimeTest, GetXTimeInSeconds_MatchesLocalCalculation)
{
    // Capture now and compute expected trading seconds using the same rules as GetXTimeInSeconds
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_c);

    int hour = now_tm.tm_hour;
    int minute = now_tm.tm_min;
    int second = now_tm.tm_sec;
    long current_seconds = hour * 3600 + minute * 60 + second;

    const long morning_start = 9 * 3600 + 30 * 60;  // 9:30
    const long morning_end = 11 * 3600 + 30 * 60;   // 11:30
    const long afternoon_start = 13 * 3600;         // 13:00
    const long afternoon_end = 15 * 3600;           // 15:00

    long expected = 0;
    if (current_seconds >= morning_start && current_seconds < morning_end) {
        expected = current_seconds - morning_start;
    } else if (current_seconds >= morning_end && current_seconds < afternoon_start) {
        expected = morning_end - morning_start;
    } else if (current_seconds >= afternoon_start && current_seconds < afternoon_end) {
        long morning_duration = morning_end - morning_start;
        expected = morning_duration + (current_seconds - afternoon_start);
    } else {
        expected = 0;
    }

    long got = lueing::TimeUtil::GetXTimeInSeconds();
    // Allow one second tolerance because GetXTimeInSeconds calls system_clock::now() internally
    EXPECT_NEAR(static_cast<double>(expected), static_cast<double>(got), 1.0);
}