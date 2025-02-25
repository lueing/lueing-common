#include "lueing_os.h"
#include <sys/stat.h>
#include <filesystem>

bool lueing::filesystem::FilesExists(const std::string &name)
{
    return std::filesystem::exists(name);
}

void lueing::filesystem::Mkdirs(const std::string &name)
{
    std::filesystem::create_directories(name);
}
