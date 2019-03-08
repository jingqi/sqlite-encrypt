
TEMPLATE = subdirs

SUBDIRS += \
    nut \
    sqlite \
    sqlite_encrypt \
    test_sqlite_encrypt

sqlite_encrypt.depends = nut
test_sqlite_encrypt.depends = nut sqlite_encrypt
