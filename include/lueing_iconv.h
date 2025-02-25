#ifndef LUEING_COMMON_LUEING_ICONV_H
#define LUEING_COMMON_LUEING_ICONV_H
#include <iconv.h>
#include <string>

namespace lueing
{
    class LueingIconv
    {
    private:
        iconv_t conv_;

    public:
        LueingIconv();
        ~LueingIconv();

    public:
        std::string GBK2UTF8(const std::string &src);
    };

} // namespace lueing

#endif // LUEING_COMMON_LUEING_ICONV_H