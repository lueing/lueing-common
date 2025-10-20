#ifndef LUEING_COMMON_LUEING_TIME_H
#define LUEING_COMMON_LUEING_TIME_H

#include <chrono>
#include <string>

namespace lueing {

class TimeUtil {
public:
    // get current time with format and return long long
    static long long GetCurrentTimeWithFormat(const std::string &format);
};

} // namespace lueing

#endif // LUEING_COMMON_LUEING_TIME_H
