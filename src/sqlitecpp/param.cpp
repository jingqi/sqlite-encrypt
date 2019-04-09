
#include <nut/rc/rc_new.h>

#include "param.h"


namespace sqlitecpp
{

Param::Param(int arg)
    : type(Type::Integer), int_arg(arg)
{}

Param::Param(int64_t arg)
    : type(Type::BigInteger), int_arg(arg)
{}

Param::Param(double arg)
    : type(Type::Double), dbl_arg(arg)
{}

Param::Param(const char *arg)
    : type(Type::String), str_arg(nut::rc_new<nut::enrc<std::string>>(arg))
{}

Param::Param(std::string&& arg)
    : type(Type::String), str_arg(nut::rc_new<nut::enrc<std::string>>(std::forward<std::string>(arg)))
{}

Param::Param(const std::string& arg)
    : type(Type::String), str_arg(nut::rc_new<nut::enrc<std::string>>(arg))
{}

Param::Param(Type type_)
    : type(type_)
{}

const Param& Param::null()
{
    static Param ret(Type::Null);
    return ret;
}

const Param& Param::nop()
{
    static Param ret(Type::NOP);
    return ret;
}

}
