#include "gtest/gtest.h"
#include "lueing_time.h"

using namespace std::chrono_literals;

TEST(LueingTimeTest, YearOnlyMatches) {
    long long expected = 2025LL;
    long long got = lueing::TimeUtil::GetCurrentTimeWithFormat("%Y");
    EXPECT_EQ(expected, got);

    expected = 20251020LL;
    got = lueing::TimeUtil::GetCurrentTimeWithFormat("%Y%m%d");
    EXPECT_EQ(expected, got);

    expected = 202510201914LL;
    got = lueing::TimeUtil::GetCurrentTimeWithFormat("%Y%m%d%H%M");
    EXPECT_EQ(expected, got);
}