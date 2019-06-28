
#ifndef ___HEADFILE_0AF24E91_832D_400C_AACF_556C6311B6A4_
#define ___HEADFILE_0AF24E91_832D_400C_AACF_556C6311B6A4_

#include <nut/debugging/exception.h>

#include "sqlitecpp_config.h"


namespace sqlitecpp
{

class SQLITECPP_API SqliteException : public nut::Exception
{
public:
    SqliteException(long code, std::string&& msg, const char *file = nullptr,
                    int line = -1, const char *func = nullptr) noexcept
        : Exception(code, std::forward<std::string>(msg), file, line, func)
    {}

    SqliteException(long code, const std::string& msg, const char *file = nullptr,
                    int line = -1, const char *func = nullptr) noexcept
        : Exception(code, msg, file, line, func)
    {}

    virtual ~SqliteException() noexcept override = default;
};

}

#endif
