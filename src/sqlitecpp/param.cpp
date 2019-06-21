
#include <string.h>
#include <regex>

#include <nut/rc/rc_new.h>
#include <nut/util/string/string_utils.h>

#include "param.h"


namespace sqlitecpp
{

Param::Param(int arg) noexcept
    : type(Type::Integer), int_arg(arg)
{}

Param::Param(int64_t arg) noexcept
    : type(Type::BigInteger), int_arg(arg)
{}

Param::Param(double arg) noexcept
    : type(Type::Double), dbl_arg(arg)
{}

Param::Param(const char *arg) noexcept
    : type(Type::String), str_arg(nut::rc_new<nut::enrc<std::string>>(arg))
{}

Param::Param(std::string&& arg) noexcept
    : type(Type::String), str_arg(nut::rc_new<nut::enrc<std::string>>(std::forward<std::string>(arg)))
{}

Param::Param(const std::string& arg) noexcept
    : type(Type::String), str_arg(nut::rc_new<nut::enrc<std::string>>(arg))
{}

Param::Param(Type type_) noexcept
    : type(type_)
{}

const Param& Param::null() noexcept
{
    static Param ret(Type::Null);
    return ret;
}

const Param& Param::nop() noexcept
{
    static Param ret(Type::NOP);
    return ret;
}

std::string Param::datetime_to_str(const nut::DateTime& dt, bool utc)
{
    if (!dt.is_valid())
        return "NULL";
    return dt.format_time("%Y-%m-%d %H:%M:%S", utc);
}

nut::DateTime Param::str_to_datetime(const char* s, bool utc)
{
    nut::DateTime ret;
    ret.set(0); // null
    if (nullptr == s || 0 == s[0])
        return ret;
    if (0 == nut::stricmp(s, "NULL"))
        return ret;

    // 匹配 "2018-09-03 00:31:16.93144"
    static std::regex ptn("(\\d{4})-(\\d{1,2})-(\\d{1,2})\\s+"
                          "(\\d{2})[:](\\d{2})[:](\\d{2}(?:[.]\\d*)?)");
    std::cmatch result;
    if (!std::regex_match(s, result, ptn))
        return ret;
    const uint32_t year = ::atoi(result[1].str().c_str());
    const uint8_t month = ::atoi(result[2].str().c_str());
    const uint8_t day = ::atoi(result[3].str().c_str());
    const uint8_t hour = ::atoi(result[4].str().c_str());
    const uint8_t minute = ::atoi(result[5].str().c_str());
    const double fsecond = ::atof(result[6].str().c_str());
    const uint8_t sec = (uint8_t) fsecond;
    const uint32_t nsec = (uint32_t) ((fsecond - sec) * 1000000000);
    ret.set(year, month, day, hour, minute, sec, nsec, utc);
    return ret;
}

}
