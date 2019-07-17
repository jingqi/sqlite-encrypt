
#include <assert.h>
#include <string.h> // for ::strlen()

#include "exception.h"
#include "connection.h"


namespace sqlitecpp
{

namespace
{

class Sqlite3Freer
{
public:
    explicit Sqlite3Freer(void *p = nullptr) noexcept
        : _ptr(p)
    {}

    ~Sqlite3Freer() noexcept
    {
        if (nullptr != _ptr)
            ::sqlite3_free(_ptr);
        _ptr = nullptr;
    }

private:
    Sqlite3Freer(const Sqlite3Freer&) = delete;
    Sqlite3Freer& operator=(const Sqlite3Freer&) = delete;

private:
    void *_ptr = nullptr;
};

}

Connection::Connection(sqlite3 *db) noexcept
    : _sqlite(db)
{
    assert(nullptr != db);
}

Connection::Connection(const char *db_file)
{
    assert(nullptr != db_file);
    open(db_file);
}

Connection::Connection(const std::string& db_file)
{
    open(db_file);
}

Connection::~Connection()
{
    bool rs = close();
    assert(rs); UNUSED(rs);
}

bool Connection::open(const char *db_file)
{
    assert(nullptr != db_file);

    // Close old database if exists
    if (is_valid())
    {
        bool rs = close();
        assert(rs); UNUSED(rs);
    }

    clear_error();

    // Open new database
    _sqlite = nullptr;
    const int rs = ::sqlite3_open(db_file, &_sqlite);
    if (SQLITE_OK != rs)
    {
        on_error(rs);
        _sqlite = nullptr;
        return false;
    }
    assert(is_valid());
    return SQLITE_OK == rs;
}

bool Connection::open(const std::string& db_file)
{
    return open(db_file.c_str());
}

bool Connection::close()
{
    if (nullptr == _sqlite)
        return true;

    clear_error();

    _cached_stmts.clear();

    const int rs = ::sqlite3_close(_sqlite);
    if (SQLITE_OK != rs)
        on_error(rs);
    else
        _sqlite = nullptr;
    return SQLITE_OK == rs;
}

#ifdef SQLITE_HAS_CODEC
/**
 * 设置加密密码，或者用密码打开已加密数据库
 */
bool Connection::set_key(const char *key, int key_len)
{
    assert(nullptr != key);
    assert(is_valid());

    clear_error();

    if(key_len < 0)
        key_len = ::strlen(key);
    const int rs = ::sqlite3_key(_sqlite, key, key_len);
    if (SQLITE_OK != rs)
        on_error(rs);
    return SQLITE_OK == rs;
}

/**
 * 更改密数据库码
 */
bool Connection::change_key(const char *key, int key_len)
{
    assert(nullptr != key);
    assert(is_valid());

    clear_error();

    if (key_len < 0)
        key_len = ::strlen(key);
    const int rs = ::sqlite3_rekey(_sqlite, key, key_len);
    if (SQLITE_OK != rs)
        on_error(rs);
    return SQLITE_OK == rs;
}
#endif

sqlite3* Connection::get_raw_db() const noexcept
{
    return _sqlite;
}

bool Connection::is_valid() const noexcept
{
    return nullptr != _sqlite;
}

bool Connection::is_throw_exceptions() const noexcept
{
    return _throw_exceptions;
}

void Connection::set_throw_exceptions(bool b) noexcept
{
    _throw_exceptions = b;
}

int Connection::get_last_error_code() const noexcept
{
    return _last_error;
}

const std::string& Connection::get_last_error_msg() const noexcept
{
    return _last_error_msg;
}

bool Connection::set_busy_timeout(int ms) noexcept
{
    int rs = ::sqlite3_busy_timeout(_sqlite, ms);
    if (SQLITE_OK != rs)
        on_error(rs);
    return SQLITE_OK == rs;
}

bool Connection::start(bool immediate)
{
    assert(is_valid());

    clear_error();

    char *msg = nullptr;
    const int rs = ::sqlite3_exec(
        _sqlite, (immediate ? "BEGIN IMMEDIATE;" : "BEGIN;"), nullptr, nullptr, &msg);
    Sqlite3Freer _g(msg);
    if (SQLITE_OK != rs)
        on_error(rs, msg);
    return SQLITE_OK == rs;
}

bool Connection::commit()
{
    assert(is_valid());

    clear_error();

    char *msg = nullptr;
    const int rs = ::sqlite3_exec(_sqlite, "COMMIT;", nullptr, nullptr, &msg);
    Sqlite3Freer _g(msg);
    if (SQLITE_OK != rs)
        on_error(rs, msg);
    return SQLITE_OK == rs;
}

bool Connection::rollback()
{
    assert(is_valid());

    clear_error();

    char *msg = nullptr;
    const int rs = ::sqlite3_exec(_sqlite, "ROLLBACK;", nullptr, nullptr, &msg);
    Sqlite3Freer _g(msg);
    if (SQLITE_OK != rs)
        on_error(rs, msg);
    return SQLITE_OK == rs;
}

bool Connection::vacuum()
{
    assert(is_valid());

    clear_error();

    char *msg = nullptr;
    const int rs = ::sqlite3_exec(_sqlite, "VACUUM;", nullptr, nullptr, &msg);
    Sqlite3Freer _g(msg);
    if (SQLITE_OK != rs)
        on_error(rs, msg);
    return SQLITE_OK == rs;
}

bool Connection::execute_update(const char *sql)
{
    assert(nullptr != sql && is_valid());

    clear_error();

    char *msg = nullptr;
    const int rs = ::sqlite3_exec(_sqlite, sql, nullptr, nullptr, &msg);
    Sqlite3Freer _g(msg);
    if (SQLITE_OK != rs)
    {
        on_error(rs, msg);
        return false;
    }
    return true;
}

bool Connection::execute_update(
    const char *sql, const Param& arg1, const Param& arg2, const Param& arg3,
    const Param& arg4, const Param& arg5, const Param& arg6, const Param& arg7,
    const Param& arg8, const Param& arg9)
{
    assert(nullptr != sql && is_valid());

    clear_error();

    // 预编译
    nut::rc_ptr<Statement> stmt = obtain_stmt(sql);
    if (nullptr == stmt)
        return false;

    // 绑定参数
    if (!stmt->reset())
    {
        on_error();
        return false;
    }

#define __BIND(i)                   \
    do                              \
    {                               \
        if (!stmt->bind(i, arg##i)) \
        {                           \
            on_error();             \
            return false;           \
        }                           \
    } while (false)

    __BIND(1);
    __BIND(2);
    __BIND(3);
    __BIND(4);
    __BIND(5);
    __BIND(6);
    __BIND(7);
    __BIND(8);
    __BIND(9);

#undef __BIND

    // 执行
    const int irs = ::sqlite3_step(stmt->get_raw_stmt());
    release_stmt(stmt);
    if (SQLITE_DONE != irs)
    {
        on_error(irs);
        return false;
    }
    return true;
}

bool Connection::execute_update(const char *sql, const std::vector<Param>& args)
{
    assert(nullptr != sql && is_valid());

    clear_error();

    // 预编译
    nut::rc_ptr<Statement> stmt = obtain_stmt(sql);
    if (nullptr == stmt)
        return false;

    // 绑定参数
    if (!stmt->reset())
    {
        on_error();
        return false;
    }
    for (size_t i = 0, size = args.size(); i < size; ++i)
    {
        if (!stmt->bind(i + 1, args.at(i)))
        {
            on_error();
            return false;
        }
    }
    // 执行
    const int irs = ::sqlite3_step(stmt->get_raw_stmt());
    release_stmt(stmt);
    if (SQLITE_DONE != irs)
    {
        on_error(irs);
        return false;
    }
    return true;
}

nut::rc_ptr<ResultSet> Connection::execute_query(
    const char *sql, const Param& arg1, const Param& arg2, const Param& arg3,
    const Param& arg4, const Param& arg5, const Param& arg6, const Param& arg7,
    const Param& arg8, const Param& arg9)
{
    assert(nullptr != sql && is_valid());

    clear_error();

    // 预编译
    nut::rc_ptr<Statement> stmt = obtain_stmt(sql);
    if (nullptr == stmt)
        return nullptr;

    // 绑定参数
    if (!stmt->reset())
    {
        on_error();
        return nullptr;
    }

#define __BIND(i)                       \
    do                                  \
    {                                   \
        if (!stmt->bind(i, arg##i))     \
        {                               \
            on_error();                 \
            return nullptr;             \
        }                               \
    } while (false)

    __BIND(1);
    __BIND(2);
    __BIND(3);
    __BIND(4);
    __BIND(5);
    __BIND(6);
    __BIND(7);
    __BIND(8);
    __BIND(9);

#undef __BIND

    // 执行
    return nut::rc_new<ResultSet>(stmt);
}

nut::rc_ptr<ResultSet> Connection::execute_query(const char *sql, const std::vector<Param>& args) noexcept(false)
{
    assert(nullptr != sql && is_valid());

    clear_error();

    // 预编译
    nut::rc_ptr<Statement> stmt = obtain_stmt(sql);
    if (nullptr == stmt)
        return nullptr;

    // 绑定参数
    if (!stmt->reset())
    {
        on_error();
        return nullptr;
    }
    for (size_t i = 0, size = args.size(); i < size; ++i)
    {
        if (!stmt->bind(i + 1, args.at(i)))
        {
            on_error();
            return nullptr;
        }
    }

    // 执行
    return nut::rc_new<ResultSet>(stmt);
}

nut::rc_ptr<Statement> Connection::obtain_stmt(const char *sql)
{
    assert(nullptr != sql);

    std::string key(sql);
    nut::rc_ptr<Statement> stmt;
    if (_cached_stmts.obtain_object(key, &stmt))
    {
        assert(nullptr != stmt && stmt->is_valid());
        return stmt;
    }

    stmt = nut::rc_new<Statement>(this, sql);
    if (!stmt->is_valid())
    {
        on_error();
        return nullptr;
    }
    return stmt;
}

void Connection::release_stmt(Statement *stmt)
{
    assert(nullptr != stmt && !stmt->get_sql().empty());
    ::sqlite3_reset(stmt->get_raw_stmt()); // Release internal lock
    _cached_stmts.release_object(stmt->get_sql(), stmt);
}

void Connection::clear_error()
{
    _last_error = SQLITE_OK;
    _last_error_msg.clear();
}

void Connection::on_error(int err, const char *msg) noexcept(false)
{
    _last_error = err;
    if (SQLITE_OK == err && nullptr != _sqlite)
        _last_error = ::sqlite3_errcode(_sqlite);

    if (nullptr == msg)
    {
        if (nullptr != _sqlite)
            msg = ::sqlite3_errmsg(_sqlite); // XXX memory of "msg" is managed internally by sqlite3
        else if (SQLITE_OK != _last_error)
            msg = ::sqlite3_errstr(_last_error);
    }
    _last_error_msg = (nullptr == msg ? "no error detected" : msg);

    if (_throw_exceptions)
        throw SqliteException(_last_error, _last_error_msg, __FILE__, __LINE__, __FUNCTION__);
}

}
