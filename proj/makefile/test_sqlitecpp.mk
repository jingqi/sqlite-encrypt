#!/user/bin/env make

TARGET_NAME = test_sqlitecpp
SRC_ROOT = ../../src/${TARGET_NAME}
SQLITE_SRC_PATH = ../../src/sqlite
SQLITECPP_SRC_PATH = ../../src/sqlite_encrypt

# Preface rules
include ${NUT_PATH}/proj/makefile/preface_rules.mk

# Includes
CPPFLAGS += -I${SRC_ROOT}/.. -I${SQLITE_SRC_PATH} -I${SQLITECPP_SRC_PATH}/.. -I${NUT_PATH}/src

# Defines
CPPFLAGS +=

# C/C++ standard
CFLAGS += -std=c11
CXXFLAGS += -std=c++11

# Libraries
ifeq (${HOST}, Linux)
	LDFLAGS += -lpthread
endif
LDFLAGS += -L${OUT_DIR} -lnut -lsqlitecpp
LIB_DEPS += ${OUT_DIR}/libnut.${DL_SUFFIX} \
			${OUT_DIR}/libsqlitecpp.${DL_SUFFIX}

# TARGET
TARGET = ${OUT_DIR}/${TARGET_NAME}

.PHONY: all clean rebuild

all: ${TARGET}

clean:
	rm -rf ${OBJS} ${DEPS} ${TARGET}

rebuild:
	$(MAKE) -f test_sqilte_encrypt.mk clean
	$(MAKE) -f test_sqilte_encrypt.mk all

${NUT_PATH}/proj/makefile/${OUT_DIR_NAME}/libnut.${DL_SUFFIX}:
	cd ${NUT_PATH}/proj/makefile ; $(MAKE) -f nut.mk

${OUT_DIR}/libnut.${DL_SUFFIX}: ${NUT_PATH}/proj/makefile/${OUT_DIR_NAME}/libnut.${DL_SUFFIX}
	cp -f $< $@

${OUT_DIR}/libsqlitecpp.${DL_SUFFIX}:
	$(MAKE) -f sqlitecpp.mk

# Rules
include ${NUT_PATH}/proj/makefile/common_rules.mk
include ${NUT_PATH}/proj/makefile/app_rules.mk
