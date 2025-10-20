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

} // namespace lueing
