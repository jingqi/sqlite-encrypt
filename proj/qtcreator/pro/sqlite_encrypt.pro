
TEMPLATE = subdirs

SUBDIRS += \
    nut \
    sqlite \
    sqlite_encrypt \
    sqlitecpp \
    test_sqlite_encrypt \
    test_sqlitecpp \

sqlite_encrypt.depends = nut
sqlitecpp.depends = nut
test_sqlite_encrypt.depends = nut sqlite_encrypt
test_sqlitecpp.depends = nut sqlitecpp
