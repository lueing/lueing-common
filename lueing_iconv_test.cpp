#include "gtest/gtest.h"
#include "lueing_iconv.h"

TEST(LueingIconvTest, GBK2UTF8)
{
    lueing::LueingIconv iconv;
    char in[128] = "\xd5\xe2\xd6\xd6\xd6\xd0\xce\xc4\xb1\xc8\xbd\xcf\xbc\xf2\xb5\xa5, hhhhh\xca\xc7\xbc\xf2\xb5\xa5";
    std::string dst = iconv.GBK2UTF8(in);
    EXPECT_EQ("这种中文比较简单, hhhhh是简单", dst);
}