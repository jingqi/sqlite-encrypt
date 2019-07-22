
TEMPLATE = subdirs

SUBDIRS += \
    nut \
    sqlite \
    sqlite-encrypt \
    sqlitecpp \
    test-sqlite-encrypt \
    test-sqlitecpp \

sqlite-encrypt.depends = nut
sqlitecpp.depends = nut
test-sqlite-encrypt.depends = nut sqlite-encrypt
test-sqlitecpp.depends = nut sqlitecpp
