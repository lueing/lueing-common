#include "gtest/gtest.h"
#include "lueing_os.h"

TEST(LueingOsTest, FilesExists)
{
    EXPECT_EQ(true, lueing::filesystem::FilesExists("D:\\workspace\\lueing\\cases\\lueing-qmt\\lueing-common\\lueing_os_test.cpp"));
    EXPECT_EQ(false, lueing::filesystem::FilesExists("D:\\workspace\\lueing\\cases\\lueing-qmt\\lueing-common\\lueing_os_test2.cpp"));
}

TEST(LueingOsTest, Mkdirs)
{
    lueing::filesystem::Mkdirs("dirs/a/b/c");
}