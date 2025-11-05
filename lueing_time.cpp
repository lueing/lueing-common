#include "lueing_time.h"

#include <iomanip>
#include <sstream>

namespace lueing {
    long long TimeUtil::GetCurrentTimeWithFormat(const std::string &format) {
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm = *std::localtime(&now_c);

        std::ostringstream oss;
        oss << std::put_time(&now_tm, format.c_str());
        std::string time_str = oss.str();

        if (time_str.empty()) return 0LL;
        try {
            return std::stoll(time_str);
        } catch (...) {
            return 0LL;
        }
    }


    long long TimeUtil::CurrentTimeAddSeconds(long long time, long long seconds)
    {
        // Parse the time format: YYYYMMDDHHMMSSmmm
        long long milliseconds = time % 1000;
        time /= 1000;
        
        int second = time % 100;
        time /= 100;
        int minute = time % 100;
        time /= 100;
        int hour = time % 100;
        time /= 100;
        int day = time % 100;
        time /= 100;
        int month = time % 100;
        time /= 100;
        int year = time;
        
        // Create tm structure
        std::tm tm_time = {};
        tm_time.tm_year = year - 1900;
        tm_time.tm_mon = month - 1;
        tm_time.tm_mday = day;
        tm_time.tm_hour = hour;
        tm_time.tm_min = minute;
        tm_time.tm_sec = second;
        
        // Convert to time_t and add seconds
        std::time_t time_t_val = std::mktime(&tm_time);
        time_t_val += seconds;
        
        // Convert back to tm
        std::tm *new_tm = std::localtime(&time_t_val);
        
        // Format back to YYYYMMDDHHMMSSmmm
        long long result = 0;
        result += (new_tm->tm_year + 1900) * 10000000000000LL;
        result += (new_tm->tm_mon + 1) * 100000000000LL;
        result += new_tm->tm_mday * 1000000000LL;
        result += new_tm->tm_hour * 10000000LL;
        result += new_tm->tm_min * 100000LL;
        result += new_tm->tm_sec * 1000LL;
        result += milliseconds;
        
        return result;
    }

    long long TimeUtil::GetCurrentTimeInSeconds()
    {
        return std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    }

    // if 2510 means 2025-10, then add some months to it, implement the method.
    long long TimeUtil::YearMonthAddMonths(long long int year_month, int months) {
        int year = year_month / 100;
        int month = year_month % 100;

        month += months;
        int year_delta = 0;
        if (month > 12) {
            year_delta = (month - 1) / 12;
            month = (month - 1) % 12 + 1;
        } else if (month <= 0) {
            year_delta = month / 12 - 1;
            month = month % 12 + 12;
        }
        year += year_delta;

        return year * 100 + month;
    }

    long TimeUtil::GetXTimeInSeconds() {
        // return trade time in seconds, if current time is in 9:30-11:30 then return seconds since 9:30, 
        // if current time is 13:00-15:00 then return seconds since 9:30 but skip the noon break from 11:30 to 13:00
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm = *std::localtime(&now_c);

        int hour = now_tm.tm_hour;
        int minute = now_tm.tm_min;
        int second = now_tm.tm_sec;

        // Convert current time to seconds since midnight
        long current_seconds = hour * 3600 + minute * 60 + second;

        // Define market times in seconds since midnight
        const long morning_start = 9 * 3600 + 30 * 60;  // 9:30
        const long morning_end = 11 * 3600 + 30 * 60;   // 11:30
        const long afternoon_start = 13 * 3600;          // 13:00
        const long afternoon_end = 15 * 3600;            // 15:00

        // Morning session: 9:30-11:30
        if (current_seconds >= morning_start && current_seconds < morning_end) {
            return current_seconds - morning_start;
        } else if (current_seconds >= morning_end && current_seconds < afternoon_start) {
            // Noon break: 11:30-13:00
            return morning_end - morning_start; // return total seconds of morning session
        }
        // Afternoon session: 13:00-15:00
        else if (current_seconds >= afternoon_start && current_seconds < afternoon_end) {
            // Morning session duration: 2 hours = 7200 seconds
            long morning_duration = morning_end - morning_start;
            return morning_duration + (current_seconds - afternoon_start);
        }

        return 0;
    }

} // namespace lueing
