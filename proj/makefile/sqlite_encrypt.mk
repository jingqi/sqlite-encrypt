#!/user/bin/env make

TARGET_NAME = sqlite_encrypt
SRC_ROOT = ../../src/${TARGET_NAME}
SQLITE_SRC_PATH = ../../src/sqlite

# Preface rules
include ${NUT_PATH}/proj/makefile/preface_rules.mk

# Includes
CPPFLAGS += -I${SRC_ROOT}/.. -I${SQLITE_SRC_PATH} -I${NUT_PATH}/src

# Defines
CPPFLAGS += -DBUILDING_SQLITE_ENCRYPT

# C/C++ standard
CFLAGS += -std=c11
CXXFLAGS += -std=c++11

# Libraries
ifeq (${HOST}, Linux)
	LDFLAGS += -lpthread
endif
LDFLAGS += -L${OUT_DIR} -lnut
LIB_DEPS += ${OUT_DIR}/libnut.${DL_SUFFIX}

# TARGET
TARGET = ${OUT_DIR}/lib${TARGET_NAME}.${DL_SUFFIX}

.PHONY: all clean rebuild

all: ${TARGET}

clean:
	rm -rf ${OBJS} ${DEPS} ${TARGET}

rebuild:
	$(MAKE) -f sqlite_encrypt.mk clean
	$(MAKE) -f sqlite_encrypt.mk all

${NUT_PATH}/proj/makefile/${OUT_DIR_NAME}/libnut.${DL_SUFFIX}:
	cd ${NUT_PATH}/proj/makefile ; $(MAKE) -f nut.mk

${OUT_DIR}/libnut.${DL_SUFFIX}: ${NUT_PATH}/proj/makefile/${OUT_DIR_NAME}/libnut.${DL_SUFFIX}
	cp -f $< $@

# Rules
include ${NUT_PATH}/proj/makefile/common_rules.mk
include ${NUT_PATH}/proj/makefile/shared_lib_rules.mk
