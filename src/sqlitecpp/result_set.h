
#ifndef ___HEADFILE_B51CBEE3_B5FB_4481_AE29_559B709E3B26_
#define ___HEADFILE_B51CBEE3_B5FB_4481_AE29_559B709E3B26_

#include <string>

#include <sqlite3.h>

#include <nut/rc/rc_new.h>

#include "statement.h"


namespace sqlitecpp
{

class ResultSet
{
    NUT_REF_COUNTABLE

public:
    ResultSet() = default;
    explicit ResultSet(Statement *stmt);

    /**
     * 获取下一组数据
     */
    bool next();

    /**
     * @param column_index 0-based
     * @return SQLITE_INTEGER, SQLITE_FLOAT, SQLITE_TEXT, SQLITE_BLOB, or SQLITE_NULL
     */
    int get_type(int column_index);
    int get_type(const char *column_name);

    /**
     * @param column_index 0-based
     */
    int get_int(int column_index);
    int get_int(const char *column_name);

    /**
     * @param column_index 0-based
     */
    int64_t get_int64(int column_index);
    int64_t get_int64(const char *column_name);

    /**
     * @param column_index 0-based
     */
    double get_double(int column_index);
    double get_double(const char *column_name);

    /**
     * @param column_index 0-based
     */
    std::string get_string(int column_index);
    std::string get_string(const char *column_name);

private:
    int get_column_index(const char *column_name);

private:
    nut::rc_ptr<Statement> _stmt;
};

}

#endif
