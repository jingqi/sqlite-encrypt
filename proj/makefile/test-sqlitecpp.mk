#!/usr/bin/env make

# Preface
include ${NUT_PATH}/proj/makefile/mkinclude/vars.mk
include ${NUT_PATH}/proj/makefile/mkinclude/funcs.mk

# Project vars
TARGET_NAME = test-sqlitecpp
SRC_ROOT = ../../src/${TARGET_NAME}
OBJ_ROOT = ${OUT_DIR}/obj/${TARGET_NAME}
TARGET = ${OUT_DIR}/${TARGET_NAME}

SQLITE_SRC_PATH = ../../src/sqlite
SQLITECPP_SRC_PATH = ../../src/sqlite-encrypt

# C/C++ standard
CFLAGS += -std=c11
CXXFLAGS += -std=c++11

# Defines and flags

# Includes

# Files
OBJS = $(call map_files,${SRC_ROOT},c,${OBJ_ROOT},o) \
    $(call map_files,${SRC_ROOT},cpp,${OBJ_ROOT},o)
DEPS = ${OBJS:.o=.d}

# Library sqlitecpp
CPPFLAGS += -I${SRC_ROOT}/.. -I${SQLITE_SRC_PATH} -I${SQLITECPP_SRC_PATH}/..
LDFLAGS += -L${OUT_DIR} -lsqlitecpp
LIB_DEPS += ${OUT_DIR}/libsqlitecpp.${DL_SUFFIX}

# Library nut
CPPFLAGS += -I${NUT_PATH}/src
LDFLAGS += -L${NUT_PATH}/proj/makefile/${OUT_DIR_NAME} -lnut
LIB_DEPS += ${NUT_PATH}/proj/makefile/${OUT_DIR_NAME}/libnut.${DL_SUFFIX}

# Other Libraries
ifeq (${HOST}, Linux)
    LDFLAGS += -lpthread
endif

# Targets
.PHONY: all
all: ${TARGET}

.PHONY: clean
clean:
	$(RM) ${OBJS} ${DEPS} ${TARGET}

.PHONY: rebuild
rebuild:
	$(MAKE) -f test-sqilte-encrypt.mk clean
	$(MAKE) -f test-sqilte-encrypt.mk all

# Rules
include ${NUT_PATH}/proj/makefile/mkinclude/common_rules.mk
include ${NUT_PATH}/proj/makefile/mkinclude/app_rules.mk
