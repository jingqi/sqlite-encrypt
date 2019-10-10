#!/usr/bin/env make

# 3rdparty
export NUT_PATH=$(CURDIR)/../../lib/nut.git

# Preface
include ${NUT_PATH}/proj/makefile/mkinclude/vars.mk

# Targets
.PHONY: all
all: sqlite sqlite-encrypt sqlitecpp test-sqlite-encrypt test-sqlitecpp

.PHONY: nut
nut:
	cd ${NUT_PATH}/proj/makefile ; $(MAKE) -f nut.mk
	mkdir -p ${OUT_DIR}
	cp -f ${NUT_PATH}/proj/makefile/${OUT_DIR_NAME}/libnut.${DL_SUFFIX} ${OUT_DIR}/libnut.${DL_SUFFIX}

.PHONY: sqlite
sqlite:
	$(MAKE) -f sqlite.mk

.PHONY: sqlite-encrypt
sqlite-encrypt: nut
	$(MAKE) -f sqlite-encrypt.mk

.PHONY: sqlitecpp
sqlitecpp: nut
	$(MAKE) -f sqlitecpp.mk

.PHONY: test-sqlite-encrypt
test-sqlite-encrypt: nut sqlite-encrypt
	$(MAKE) -f test-sqlite-encrypt.mk

.PHONY: test-sqlitecpp
test-sqlitecpp: nut sqlitecpp
	$(MAKE) -f test-sqlitecpp.mk

.PHONY: clean
clean:
	cd ${NUT_PATH}/proj/makefile ; $(MAKE) -f nut.mk clean
	$(RM) -r ${OUT_DIR_NAME}

.PHONY: rebuild
rebuild:
	$(MAKE) clean
	$(MAKE) all

.PHONY: run
run: test-sqlite-encrypt
	$(MAKE) -f test-sqlite-encrypt.mk run

.PHONY: run-test-sqlitecpp
run-test-sqlitecpp: test-sqlitecpp
	$(MAKE) -f test-sqlitecpp.mk run

.PHONY: valgrind
valgrind: test-sqlite-encrypt
	$(MAKE) -f test-sqlite-encrypt.mk valgrind
