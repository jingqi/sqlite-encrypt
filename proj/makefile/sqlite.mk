#!/usr/bin/env make

# Preface
include ${NUT_PATH}/proj/makefile/mkinclude/vars.mk
include ${NUT_PATH}/proj/makefile/mkinclude/funcs.mk

# Project vars
TARGET_NAME = sqlite
SRC_ROOT = ../../src/${TARGET_NAME}
OBJ_ROOT = ${OUT_DIR}/obj/${TARGET_NAME}
TARGET = ${OUT_DIR}/lib${TARGET_NAME}.${DL_SUFFIX}

# Make dirs
$(call make_image_dir_tree,${SRC_ROOT},${OBJ_ROOT})

# C/C++ standard
CFLAGS += -std=c11

# Defines and flags

# Includes
CPPFLAGS += -I${SRC_ROOT} -I${NUT_PATH}/src

# Files
# NOTE shell.c 含有 main() 函数，应该去除
OBJS := ${OBJ_ROOT}/sqlite3.o
DEPS := ${OBJ_ROOT}/sqlite3.d

# Other libraries

# Targets
.PHONY: all clean rebuild

all: ${TARGET}

clean:
	rm -rf ${OBJS} ${DEPS} ${TARGET}

rebuild:
	$(MAKE) -f sqlite.mk clean
	$(MAKE) -f sqlite.mk all

# Rules
include ${NUT_PATH}/proj/makefile/mkinclude/common_rules.mk
include ${NUT_PATH}/proj/makefile/mkinclude/shared_lib_rules.mk
