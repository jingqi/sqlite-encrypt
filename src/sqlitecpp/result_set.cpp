
#include <assert.h>
#include <string.h>

#include <nut/util/string/string_utils.h>

#include "result_set.h"


namespace sqlitecpp
{

ResultSet::ResultSet(Statement *stmt) noexcept
    : _stmt(stmt)
{
    assert(nullptr != stmt);
}

bool ResultSet::next() noexcept
{
    if (nullptr == _stmt)
        return false;

    return SQLITE_ROW == ::sqlite3_step(_stmt->get_raw_stmt());
}

int ResultSet::get_type(int column_index) noexcept
{
    assert(column_index >= 0 && nullptr != _stmt);
    return ::sqlite3_column_type(_stmt->get_raw_stmt(), column_index);
}

int ResultSet::get_type(const char *column_name) noexcept
{
    assert(nullptr != column_name && nullptr != _stmt);
    const int i = get_column_index(column_name);
    assert(i >= 0);
    return get_type(i);
}

int ResultSet::get_int(int column_index) noexcept
{
    assert(column_index >= 0 && nullptr != _stmt);
    return ::sqlite3_column_int(_stmt->get_raw_stmt(), column_index);
}

int ResultSet::get_int(const char *column_name) noexcept
{
    assert(nullptr != column_name && nullptr != _stmt);
    const int i = get_column_index(column_name);
    assert(i >= 0);
    return get_int(i);
}

int64_t ResultSet::get_int64(int column_index) noexcept
{
    assert(column_index >= 0 && nullptr != _stmt);
    return ::sqlite3_column_int64(_stmt->get_raw_stmt(), column_index);
}

int64_t ResultSet::get_int64(const char *column_name) noexcept
{
    assert(nullptr != column_name && nullptr != _stmt);
    const int i = get_column_index(column_name);
    assert(i >= 0);
    return get_int64(i);
}

double ResultSet::get_double(int column_index) noexcept
{
    assert(column_index >= 0 && nullptr != _stmt);
    return ::sqlite3_column_double(_stmt->get_raw_stmt(), column_index);
}

double ResultSet::get_double(const char *column_name) noexcept
{
    assert(nullptr != column_name && nullptr != _stmt);
    const int i = get_column_index(column_name);
    assert(i >= 0);
    return get_double(i);
}

std::string ResultSet::get_string(int column_index) noexcept
{
    assert(column_index >= 0 && nullptr != _stmt);
    const char *ret = (const char*) ::sqlite3_column_text(_stmt->get_raw_stmt(), column_index);
    return nullptr == ret ? "" : ret;
}

std::string ResultSet::get_string(const char *column_name) noexcept
{
    assert(nullptr != column_name && nullptr != _stmt);
    const int i = get_column_index(column_name);
    assert(i >= 0);
    return get_string(i);
}

int ResultSet::get_column_index(const char *column_name) noexcept
{
    assert(nullptr != column_name && nullptr != _stmt);
    const int c = ::sqlite3_column_count(_stmt->get_raw_stmt());
    for (int i = 0; i < c; ++i)
    {
        const char *n = ::sqlite3_column_name(_stmt->get_raw_stmt(), i);
        assert(nullptr != n);
        if (0 == nut::stricmp(n, column_name))
            return i;
    }
    return -1;
}

}
