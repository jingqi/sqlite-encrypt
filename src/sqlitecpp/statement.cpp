
#include "statement.h"


namespace sqlitecpp
{

Statement::Statement(sqlite3 *db, const char *sql) noexcept
{
    assert(nullptr != db && nullptr != sql);
    prepare(db, sql);
}

Statement::~Statement() noexcept
{
    clear();
}

bool Statement::prepare(sqlite3 *db, const char *sql) noexcept
{
    assert(nullptr != db && nullptr != sql);
    clear();
    const int rs = ::sqlite3_prepare_v2(db, sql, -1, &_stmt, nullptr);
    if (SQLITE_OK != rs || nullptr == _stmt)
        return false;
    return true;
}

bool Statement::is_valid() const noexcept
{
    return nullptr != _stmt;
}

bool Statement::reset() noexcept
{
    assert(is_valid());
    _strings.clear();
    ::sqlite3_clear_bindings(_stmt);
    return SQLITE_OK == ::sqlite3_reset(_stmt);
}

void Statement::clear() noexcept
{
    _strings.clear();
    if (nullptr != _stmt)
    {
        // NOTE 如果 stmt 之前已经有错误发生，sqlite3_finalize() 返回值不会
        //      是 SQLITE_OK
        ::sqlite3_finalize(_stmt);
        _stmt = nullptr;
    }
}

bool Statement::bind(int pos, const Param& param) noexcept
{
    assert(is_valid());
    switch (param.type)
    {
    case Param::Type::Null:
        return SQLITE_OK == ::sqlite3_bind_null(_stmt, pos);

    case Param::Type::Integer:
        return SQLITE_OK == ::sqlite3_bind_int(_stmt, pos, (int) param.int_arg);

    case Param::Type::BigInteger:
        return SQLITE_OK == ::sqlite3_bind_int64(_stmt, pos, param.int_arg);

    case Param::Type::Double:
        return SQLITE_OK == ::sqlite3_bind_double(_stmt, pos, param.dbl_arg);

    case Param::Type::String:
        // NOTE
        // - sqlite3 只有在用到字符串时才会去取值，这里需要缓存一下字符串, 否则
        //   导致访问内存错误，或者其他未定义错误
        // - 也不能用 std::vector<std::string> 保存字符串，否则容器增长时导致
        //   c_str() 返回值失效的，从而导致崩溃等问题，只能用
        //   std::vector<nut::rc_ptr<nut::enref<std::string>>> 来保存
        assert(!param.str_arg.is_null());
        _strings.push_back(param.str_arg);
        return SQLITE_OK == ::sqlite3_bind_text(_stmt, pos, param.str_arg->c_str(), -1, nullptr);

    default:
        assert(Param::Type::NOP == param.type);
        return true;
    }
}

sqlite3_stmt* Statement::get_raw_stmt() noexcept
{
    return _stmt;
}

}
