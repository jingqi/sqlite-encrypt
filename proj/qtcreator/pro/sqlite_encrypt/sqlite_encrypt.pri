
TARGET = sqlite_encrypt
TEMPLATE = lib
VERSION = 3.15.1

include(../sqlite_encrypt_common.pri)

CONFIG -= qt

DEFINES += BUILDING_SQLITE_ENCRYPT

SQLITE_SRC_PATH = $$PWD/../../../../src/sqlite
SRC_ROOT = $$PWD/../../../../src/sqlite_encrypt

INCLUDEPATH += $${SQLITE_SRC_PATH}

HEADERS += \
    $$files($${SQLITE_SRC_PATH}/*.h, true) \
    $$files($${SRC_ROOT}/*.h, true)

# HACK "sqlite3.c" 被包含在了 "codecext.c" 中
# NOTE 要排除 sqlite/shell.c
SOURCES += $$files($${SRC_ROOT}/*.c*, true)

# 链接库
mac {
    LIBS += -lc++
} else: unix {
    LIBS += -lrt
} else: win32 {
    LIBS += -lpthread
}

# nut
INCLUDEPATH += $${NUT_PATH}/src
LIBS += -L$$OUT_PWD/../nut$${OUT_TAIL}
win32: LIBS += -lnut1
else: LIBS += -lnut
