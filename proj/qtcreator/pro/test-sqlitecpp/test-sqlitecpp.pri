
TARGET = test-sqlitecpp
TEMPLATE = app

include(../sqlite_encrypt_common.pri)

QT -= qt
CONFIG += console
CONFIG -= app_bundle

DEFINES +=

SRC_ROOT = $$PWD/../../../../src/test-sqlitecpp
INCLUDEPATH += $${SRC_ROOT}

HEADERS += $$files($${SRC_ROOT}/*.h*, true)

SOURCES += $$files($${SRC_ROOT}/*.c*, true)

# sqlitecpp
INCLUDEPATH += $$PWD/../../../../src/sqlite $$PWD/../../../../src
LIBS += -L$$OUT_PWD/../sqlitecpp$${OUT_TAIL}
win32: LIBS += -lsqlitecpp3
else: LIBS += -lsqlitecpp

# nut
INCLUDEPATH += $${NUT_PATH}/src
LIBS += -L$$OUT_PWD/../nut$${OUT_TAIL}
win32: LIBS += -lnut1
else: LIBS += -lnut

# 其他链接库
mac {
    LIBS += -lc++
} else: unix {
    LIBS += -lrt
} else: win32 {
    LIBS += -lpthread
}
