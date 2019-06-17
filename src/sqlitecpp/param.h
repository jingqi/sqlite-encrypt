
#ifndef ___HEADFILE_F597ABF0_3C94_490D_A072_5EA0331BFD83_
#define ___HEADFILE_F597ABF0_3C94_490D_A072_5EA0331BFD83_

#include <stdint.h>
#include <string>

#include <nut/rc/enrc.h>
#include <nut/time/date_time.h>

#include "sqlitecpp_config.h"


namespace sqlitecpp
{

class SQLITECPP_API Param
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
    Param(int arg) noexcept;
    Param(int64_t arg) noexcept;
    Param(double arg) noexcept;
    Param(const char *arg) noexcept;
    Param(std::string&& arg) noexcept;
    Param(const std::string& arg) noexcept;

    static const Param& null() noexcept;
    static const Param& nop() noexcept;

    static std::string datetime_to_str(const nut::DateTime& dt, bool utc = false);
    static nut::DateTime str_to_datetime(const char *s, bool utc = false);

private:
    Param(Type type_) noexcept;

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
