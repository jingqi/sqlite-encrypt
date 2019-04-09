
#include <iostream>

#include <nut/nut.h>
#include <sqlitecpp/sqlitecpp.h>

using namespace std;
using namespace nut;
using namespace sqlitecpp;

#define DBNAME "./testdb"

class TestSqlitecpp : public TestFixture
{
    virtual void register_cases() override
    {
        NUT_REGISTER_CASE(test_smoke);
    }

    virtual void set_up() override
    {
        if (Path::exists(DBNAME))
            OS::remove_file(DBNAME);
    }

    virtual void tear_down() override
    {
        if (Path::exists(DBNAME))
            OS::remove_file(DBNAME);
    }

    void test_smoke()
    {
        // Write
        rc_ptr<Connection> conn = rc_new<Connection>(DBNAME);
        conn->set_key("passwd");
        NUT_TA(SQLITE_OK == conn->get_last_error_code());

        conn->set_auto_commit(true);
        conn->execute_update(
            "CREATE TABLE 'test' ("
                "id INTEGER PRIMARY KEY,"
                "str TEXT,"
                "int INTEGER,"
                "bint BIGINT,"
                "dbl REAL,"
                "nil TEXT DEFAULT (NULL)"
            ");");
        // cerr << conn->get_last_error_msg() << endl;
        NUT_TA(SQLITE_OK == conn->get_last_error_code());

        conn->execute_update("INSERT INTO test (str, int, bint, dbl) values (?, ?, ?, ?)",
                             "abc", 12, (int64_t) 123456, 12.345);
        NUT_TA(SQLITE_OK == conn->get_last_error_code());

        conn->close();

        // Read
        conn = rc_new<Connection>(DBNAME);
        conn->set_key("passwd");
        NUT_TA(SQLITE_OK == conn->get_last_error_code());

        rc_ptr<ResultSet> rs = conn->execute_query("SELECT str,int,bint,dbl,nil FROM test WHERE str=?;",
                                                    "abc");
        NUT_TA(SQLITE_OK == conn->get_last_error_code());
        NUT_TA(rs->next());

        NUT_TA(rs->get_type(0) == SQLITE_TEXT);
        NUT_TA(rs->get_string(0) == "abc");

        NUT_TA(rs->get_type(1) == SQLITE_INTEGER);
        NUT_TA(rs->get_int(1) == 12);

        NUT_TA(rs->get_type(2) == SQLITE_INTEGER);
        NUT_TA(rs->get_int64(2) == 123456);

        NUT_TA(rs->get_type(3) == SQLITE_FLOAT);
        NUT_TA(rs->get_double(3) == 12.345);

        NUT_TA(rs->get_type(4) == SQLITE_NULL);
    }
};

NUT_REGISTER_FIXTURE(TestSqlitecpp, "quiet")
