
#ifndef ___HEADFILE_54107F5A_983A_4021_AB0E_0E5422FE5FD9_
#define ___HEADFILE_54107F5A_983A_4021_AB0E_0E5422FE5FD9_

#include <vector>
#include <string>

#include <nut/platform/platform.h>
#include <nut/rc/rc_new.h>
#include <nut/rc/enrc.h>

#include "sqlitecpp_config.h"
#include <sqlite3.h>

#include "param.h"


namespace sqlitecpp
{

class SQLITECPP_API Statement
{
    NUT_REF_COUNTABLE

public:
    Statement() = default;
    Statement(sqlite3 *db, const char *sql) noexcept;

    ~Statement() noexcept;

    bool prepare(sqlite3 *db, const char *sql) noexcept;

    bool is_valid() const noexcept;

    /**
     * Reset so we can re-query
     */
    bool reset() noexcept;

    /**
     * Make it invalid
     */
    void clear() noexcept;

    /**
     * @param pos 1-based
     */
    bool bind(int pos, const Param& param) noexcept;

    sqlite3_stmt* get_raw_stmt() noexcept;

private:
    sqlite3_stmt *_stmt = nullptr;
    std::vector<nut::rc_ptr<nut::enrc<std::string>>> _strings;
};

}

#endif
