
#ifndef ___HEADFILE_F597ABF0_3C94_490D_A072_5EA0331BFD83_
#define ___HEADFILE_F597ABF0_3C94_490D_A072_5EA0331BFD83_

#include <stdint.h>
#include <string>

#include <nut/rc/enrc.h>


namespace sqlitecpp
{

class Param
{
public:
    enum class Type
    {
        NOP, // no operation
        Null,
        Integer,
        BigInteger,
        Double,
        String
    };

public:
    Param(int arg);
    Param(int64_t arg);
    Param(double arg);
    Param(const char *arg);
    Param(std::string&& arg);
    Param(const std::string& arg);

    static const Param& null();

    static const Param& nop();

private:
    Param(Type type_);

public:
    Type type = Type::NOP;
    nut::rc_ptr<nut::enrc<std::string>> str_arg;
    union
    {
        int64_t int_arg;
        double dbl_arg;
    };
};

}

#endif
