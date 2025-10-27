#ifndef LUEING_COMMON_LUEING_TIME_H
#define LUEING_COMMON_LUEING_TIME_H

#include <chrono>
#include <string>

namespace lueing {

class TimeUtil {
public:
    // get current time with format and return long long
    static long long GetCurrentTimeWithFormat(const std::string &format);
    // if a long long value  20251020153531000 represent time and we need to get the time when add some seconds
    static long long CurrentTimeAddSeconds(long long time, long long seconds);
    // if 2510 means 2025-10, then add some months to it, implement the method.
    static long long YearMonthAddMonths(long long year_month, int months);
    // get current time in seconds since epoch
    static long long GetCurrentTimeInSeconds();
};

} // namespace lueing

#endif // LUEING_COMMON_LUEING_TIME_H
