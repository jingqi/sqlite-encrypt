
#include <string.h>

#include <nut/rc/rc_new.h>

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

Param::Param(const nut::DateTime& dt, bool utc) noexcept
    : type(dt.is_valid() ? Type::String : Type::Null)
{
    if (dt.is_valid())
    {
        str_arg = nut::rc_new<nut::enrc<std::string>>(
            dt.format_time("%Y-%m-%d %H:%M:%S", utc));
    }
}

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

}
