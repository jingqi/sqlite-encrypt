#!/usr/bin/env make

# Preface
include ${NUT_PATH}/proj/makefile/mkinclude/vars.mk
include ${NUT_PATH}/proj/makefile/mkinclude/funcs.mk

# Project vars
TARGET_NAME = test-sqlitecpp
SRC_ROOT = ../../src/${TARGET_NAME}
SQLITE_SRC_PATH = ../../src/sqlite
SQLITECPP_SRC_PATH = ../../src/sqlite-encrypt
OBJ_ROOT = ${OUT_DIR}/obj/${TARGET_NAME}
TARGET = ${OUT_DIR}/${TARGET_NAME}

# Make dirs
$(call make_image_dir_tree,${SRC_ROOT},${OBJ_ROOT})

# C/C++ standard
CFLAGS += -std=c11
CXXFLAGS += -std=c++11

# Defines and flags

# Includes

# Files
SRCS = $(call files,${SRC_ROOT},*.c *.cpp)
OBJS = $(patsubst ${SRC_ROOT}%.cpp,${OBJ_ROOT}%.o,$(patsubst ${SRC_ROOT}%.c,${OBJ_ROOT}%.o,${SRCS}))
DEPS = $(patsubst ${SRC_ROOT}%.cpp,${OBJ_ROOT}%.d,$(patsubst ${SRC_ROOT}%.c,${OBJ_ROOT}%.d,${SRCS}))

# Library sqlitecpp
CPPFLAGS += -I${SRC_ROOT}/.. -I${SQLITE_SRC_PATH} -I${SQLITECPP_SRC_PATH}/..
LDFLAGS += -L${OUT_DIR} -lsqlitecpp
LIB_DEPS += ${OUT_DIR}/libsqlitecpp.${DL_SUFFIX}

# Library nut
CPPFLAGS += -I${NUT_PATH}/src
LDFLAGS += -L${OUT_DIR} -lnut
LIB_DEPS += ${OUT_DIR}/libnut.${DL_SUFFIX}

# Other Libraries
ifeq (${HOST}, Linux)
	LDFLAGS += -lpthread
endif

# Targets
.PHONY: all clean rebuild

all: ${TARGET}

clean:
	rm -rf ${OBJS} ${DEPS} ${TARGET}

rebuild:
	$(MAKE) -f test-sqilte-encrypt.mk clean
	$(MAKE) -f test-sqilte-encrypt.mk all

${OUT_DIR}/libsqlitecpp.${DL_SUFFIX}: FORCE
	$(MAKE) -f sqlitecpp.mk

${NUT_PATH}/proj/makefile/${OUT_DIR_NAME}/libnut.${DL_SUFFIX}: FORCE
	cd ${NUT_PATH}/proj/makefile ; $(MAKE) -f nut.mk

${OUT_DIR}/libnut.${DL_SUFFIX}: ${NUT_PATH}/proj/makefile/${OUT_DIR_NAME}/libnut.${DL_SUFFIX}
	cp -f $< $@

# Rules
include ${NUT_PATH}/proj/makefile/mkinclude/common_rules.mk
include ${NUT_PATH}/proj/makefile/mkinclude/app_rules.mk
