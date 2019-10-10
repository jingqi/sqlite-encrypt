#!/usr/bin/env make

# Preface
include ${NUT_PATH}/proj/makefile/mkinclude/vars.mk
include ${NUT_PATH}/proj/makefile/mkinclude/funcs.mk

# Project vars
TARGET_NAME = sqlitecpp
SRC_ROOT = ../../src/${TARGET_NAME}
OBJ_ROOT = ${OUT_DIR}/obj/${TARGET_NAME}
TARGET = ${OUT_DIR}/lib${TARGET_NAME}.${DL_SUFFIX}

SQLITE_SRC_PATH = ../../src/sqlite
SQLITE_ENCRYPT_SRC_PATH = ../../src/sqlite-encrypt

# C/C++ standard
CFLAGS += -std=c11
CXXFLAGS += -std=c++11

# Defines and flags
CPPFLAGS += -DBUILDING_SQLITECPP -DBUILDING_SQLITE_ENCRYPT

# Includes
CPPFLAGS += -I${SRC_ROOT}/.. -I${SQLITE_SRC_PATH}

# Files
OBJS = $(call map_files,${SRC_ROOT},c,${OBJ_ROOT},o) \
    $(call map_files,${SRC_ROOT},cpp,${OBJ_ROOT},o)

OBJS += ${OBJ_ROOT}/codec.o ${OBJ_ROOT}/codecext.o

DEPS = ${OBJS:.o=.d}

# Library nut
CPPFLAGS += -I${NUT_PATH}/src
LDFLAGS += -L${NUT_PATH}/proj/makefile/${OUT_DIR_NAME} -lnut
LIB_DEPS += ${NUT_PATH}/proj/makefile/${OUT_DIR_NAME}/libnut.${DL_SUFFIX}

# Other libraries
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
	$(MAKE) -f sqlitecpp.mk clean
	$(MAKE) -f sqlitecpp.mk all

${OBJ_ROOT}/codec.o: ${SQLITE_ENCRYPT_SRC_PATH}/codec.cpp
	$(make-cxx-obj)

${OBJ_ROOT}/codec.d: ${SQLITE_ENCRYPT_SRC_PATH}/codec.cpp
	$(make-cxx-dep)

${OBJ_ROOT}/codecext.o: ${SQLITE_ENCRYPT_SRC_PATH}/codecext.c
	$(make-c-obj)

${OBJ_ROOT}/codecext.d: ${SQLITE_ENCRYPT_SRC_PATH}/codecext.c
	$(make-c-dep)

# Rules
include ${NUT_PATH}/proj/makefile/mkinclude/common_rules.mk
include ${NUT_PATH}/proj/makefile/mkinclude/shared_lib_rules.mk
