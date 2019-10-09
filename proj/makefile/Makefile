#!/usr/bin/env make

.PHONY: all clean rebuild run valgrind

export NUT_PATH=$(CURDIR)/../../lib/nut.git

all:
	cd ${NUT_PATH}/proj/makefile ; $(MAKE) -f nut.mk
	$(MAKE) -f sqlite.mk
	$(MAKE) -f sqlite-encrypt.mk
	$(MAKE) -f sqlitecpp.mk
	$(MAKE) -f test-sqlite-encrypt.mk
	$(MAKE) -f test-sqlitecpp.mk

clean:
	cd ${NUT_PATH}/proj/makefile ; $(MAKE) -f nut.mk clean
	$(MAKE) -f sqlite.mk clean
	$(MAKE) -f sqlite-encrypt.mk clean
	$(MAKE) -f sqlitecpp.mk clean
	$(MAKE) -f test-sqlite-encrypt.mk clean
	$(MAKE) -f test-sqlitecpp.mk clean

rebuild:
	$(MAKE) clean
	$(MAKE) all

run:
	$(MAKE) -f test-sqlite-encrypt.mk run

run-test-sqlitecpp:
	$(MAKE) -f test-sqlitecpp.mk run

valgrind:
	$(MAKE) -f test-sqlite-encrypt.mk valgrind
