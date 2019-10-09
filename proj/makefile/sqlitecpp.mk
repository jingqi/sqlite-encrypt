#!/usr/bin/env make

# Preface
include ${NUT_PATH}/proj/makefile/mkinclude/vars.mk
include ${NUT_PATH}/proj/makefile/mkinclude/funcs.mk

# Project vars
TARGET_NAME = sqlitecpp
SRC_ROOT = ../../src/${TARGET_NAME}
SQLITE_SRC_PATH = ../../src/sqlite
SQLITE_ENCRYPT_SRC_PATH = ../../src/sqlite-encrypt
OBJ_ROOT = ${OUT_DIR}/obj/${TARGET_NAME}
TARGET = ${OUT_DIR}/lib${TARGET_NAME}.${DL_SUFFIX}

# Make dirs
$(call make_image_dir_tree,${SRC_ROOT},${OBJ_ROOT})

# C/C++ standard
CFLAGS += -std=c11
CXXFLAGS += -std=c++11

# Defines and flags
CPPFLAGS += -DBUILDING_SQLITECPP -DBUILDING_SQLITE_ENCRYPT

# Includes
CPPFLAGS += -I${SRC_ROOT}/.. -I${SQLITE_SRC_PATH}

# Files
SRCS := $(call files,${SRC_ROOT},*.c *.cpp)
OBJS := $(patsubst ${SRC_ROOT}%.cpp,${OBJ_ROOT}%.o,$(patsubst ${SRC_ROOT}%.c,${OBJ_ROOT}%.o,${SRCS}))
DEPS := $(patsubst ${SRC_ROOT}%.cpp,${OBJ_ROOT}%.d,$(patsubst ${SRC_ROOT}%.c,${OBJ_ROOT}%.d,${SRCS}))

OBJS += ${OBJ_ROOT}/codec.o ${OBJ_ROOT}/codecext.o
DEPS += ${OBJ_ROOT}/codec.d ${OBJ_ROOT}/codecext.d

# Library nut
CPPFLAGS += -I${NUT_PATH}/src
LDFLAGS += -L${OUT_DIR} -lnut
LIB_DEPS += ${OUT_DIR}/libnut.${DL_SUFFIX}

# Libraries
ifeq (${HOST}, Linux)
	LDFLAGS += -lpthread
endif

# Targets
.PHONY: all clean rebuild

all: ${TARGET}

clean:
	rm -rf ${OBJS} ${DEPS} ${TARGET}

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

${NUT_PATH}/proj/makefile/${OUT_DIR_NAME}/libnut.${DL_SUFFIX}: FORCE
	cd ${NUT_PATH}/proj/makefile ; $(MAKE) -f nut.mk

${OUT_DIR}/libnut.${DL_SUFFIX}: ${NUT_PATH}/proj/makefile/${OUT_DIR_NAME}/libnut.${DL_SUFFIX}
	cp -f $< $@

# Rules
include ${NUT_PATH}/proj/makefile/mkinclude/common_rules.mk
include ${NUT_PATH}/proj/makefile/mkinclude/shared_lib_rules.mk
