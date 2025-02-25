#ifndef LUEING_COMMON_LUEING_OS_H
#define LUEING_COMMON_LUEING_OS_H
#include <string>

namespace lueing {
    namespace filesystem {
        bool FilesExists(const std::string &name);
        void Mkdirs(const std::string &name);
    } // namespace os
} // namespace lueing

#endif // LUEING_COMMON_LUEING_OS_H