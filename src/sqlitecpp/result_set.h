
#ifndef ___HEADFILE_B51CBEE3_B5FB_4481_AE29_559B709E3B26_
#define ___HEADFILE_B51CBEE3_B5FB_4481_AE29_559B709E3B26_

#include <string>

#include <nut/rc/rc_new.h>
#include <nut/time/date_time.h>

#include "sqlitecpp_config.h"
#include <sqlite3.h>

#include "statement.h"


namespace sqlitecpp
{

class SQLITECPP_API ResultSet
{
    NUT_REF_COUNTABLE

public:
    ResultSet() = default;
    explicit ResultSet(Statement *stmt) noexcept;

    /**
     * 获取下一组数据
     */
    bool next() noexcept;

    /**
     * @param column_index 0-based
     * @return SQLITE_INTEGER, SQLITE_FLOAT, SQLITE_TEXT, SQLITE_BLOB, or SQLITE_NULL
     */
    int get_type(int column_index) noexcept;
    int get_type(const char *column_name) noexcept;

    /**
     * @param column_index 0-based
     */
    int get_int(int column_index) noexcept;
    int get_int(const char *column_name) noexcept;

    /**
     * @param column_index 0-based
     */
    int64_t get_int64(int column_index) noexcept;
    int64_t get_int64(const char *column_name) noexcept;

    /**
     * @param column_index 0-based
     */
    double get_double(int column_index) noexcept;
    double get_double(const char *column_name) noexcept;

    /**
     * @param column_index 0-based
     */
    std::string get_string(int column_index) noexcept;
    std::string get_string(const char *column_name) noexcept;

    /**
     * @param column_index 0-based
     */
    nut::DateTime get_datetime(int column_index, bool utc = false) noexcept;
    nut::DateTime get_datetime(const char *column_name, bool utc = false) noexcept;

private:
    int get_column_index(const char *column_name) noexcept;

private:
    nut::rc_ptr<Statement> _stmt;
};

}

#endif
