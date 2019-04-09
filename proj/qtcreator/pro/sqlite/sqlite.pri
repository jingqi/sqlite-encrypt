
TARGET = sqlite
TEMPLATE = lib
VERSION = 3.15.1

include(../sqlite_encrypt_common.pri)

CONFIG -= qt

SRC_ROOT = $$PWD/../../../../src/sqlite

INCLUDEPATH += $${SRC_ROOT}

HEADERS += $$files($${SRC_ROOT}/*.h, true)

SOURCES += $$files($${SRC_ROOT}/*.c*, true)
