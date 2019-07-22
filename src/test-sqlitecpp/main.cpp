
#include <nut/nut.h>
#include <sqlitecpp/sqlitecpp.h>


using namespace nut;

int main(int argc, char* argv[])
{
    ConsoleTestLogger l;
    TestRunner runner(&l);
    runner.run_group("quiet");
    // runner.run_fixture("TestSqlitecpp");
    // runner.run_case("TestSqlitecpp", "test_smoke");
}
