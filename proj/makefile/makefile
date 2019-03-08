#!/user/bin/env make

.PHONY: all clean rebuild run valgrind

export NUT_PATH=$(CURDIR)/../../lib/nut.git

all:
	cd ${NUT_PATH}/proj/makefile ; $(MAKE) -f nut.mk
	$(MAKE) -f sqlite.mk
	$(MAKE) -f sqlite_encrypt.mk
	$(MAKE) -f test_sqlite_encrypt.mk

clean:
	cd ${NUT_PATH}/proj/makefile ; $(MAKE) -f nut.mk clean
	$(MAKE) -f sqlite.mk clean
	$(MAKE) -f sqlite_encrypt.mk clean
	$(MAKE) -f test_sqlite_encrypt.mk clean

rebuild:
	$(MAKE) clean
	$(MAKE) all

run:
	$(MAKE) -f test_sqlite_encrypt.mk run

valgrind:
	$(MAKE) -f test_sqlite_encrypt.mk valgrind
