
TARGET = test-sqlite-encrypt
TEMPLATE = app

include(../sqlite_encrypt_common.pri)

QT -= qt
CONFIG += console
CONFIG -= app_bundle

DEFINES +=

SRC_ROOT = $$PWD/../../../../src/test-sqlite-encrypt
INCLUDEPATH += $${SRC_ROOT}

HEADERS += $$files($${SRC_ROOT}/*.h*, true)

SOURCES += $$files($${SRC_ROOT}/*.c*, true)

# 链接库
mac {
    LIBS += -lc++
} else: unix {
    LIBS += -lrt
} else: win32 {
    LIBS += -lpthread
}

# sqlite-encrypt
INCLUDEPATH += $$PWD/../../../../src/sqlite
LIBS += -L$$OUT_PWD/../sqlite-encrypt$${OUT_TAIL}
win32: LIBS += -lsqlite-encrypt3
else: LIBS += -lsqlite-encrypt

# nut
INCLUDEPATH += $${NUT_PATH}/src
LIBS += -L$$OUT_PWD/../nut$${OUT_TAIL}
win32: LIBS += -lnut1
else: LIBS += -lnut
