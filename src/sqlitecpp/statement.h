
#ifndef ___HEADFILE_54107F5A_983A_4021_AB0E_0E5422FE5FD9_
#define ___HEADFILE_54107F5A_983A_4021_AB0E_0E5422FE5FD9_

#include <vector>
#include <string>
#include <sqlite3.h>

#include <nut/platform/platform.h>
#include <nut/rc/rc_new.h>
#include <nut/rc/enrc.h>

#include "param.h"


namespace sqlitecpp
{

class Statement
{
    NUT_REF_COUNTABLE

public:
    Statement() = default;
    Statement(sqlite3 *db, const char *sql);

    ~Statement();

    bool prepare(sqlite3 *db, const char *sql);

    bool is_valid() const;

    /**
     * Reset so we can re-query
     */
    bool reset();

    /**
     * Make it invalid
     */
    void clear();

    /**
     * @param pos 1-based
     */
    bool bind(int pos, const Param& param);

    sqlite3_stmt* get_raw_stmt();

private:
    sqlite3_stmt *_stmt = nullptr;
    std::vector<nut::rc_ptr<nut::enrc<std::string>>> _strings;
};

}

#endif
