#include "lueing_iconv.h"

#include <iostream>
#include <cstring>

lueing::LueingIconv::LueingIconv()
{
    conv_ = iconv_open("utf-8", "gbk");
    if (conv_ == (iconv_t) (-1)) {
        std::cout << "Fail to create convertor, errno: " << errno << std::endl;
        exit(1);
    }
}

lueing::LueingIconv::~LueingIconv()
{
    iconv_close(conv_);
}

std::string lueing::LueingIconv::GBK2UTF8(const std::string &src)
{
    size_t inSize = src.size();
    size_t outSize = 2 * inSize;
    char dst[512] = {0};
    
    const char *source_string = src.c_str();    
    const char *dst_ptr = dst;

    iconv(conv_, const_cast<char **>(&source_string),
                       &inSize,
                       const_cast<char **>(&dst_ptr),
                       &outSize);
    return dst;
}
