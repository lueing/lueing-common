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

} // namespace lueing
