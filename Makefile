# Top-level Makefile for OneJoker project.
#
ifeq ($(OS),Windows_NT)
MAKE=make
endif

all: lib

.PHONY: lib
lib:
	$(MAKE) -C src/lib lib

.PHONY: test
test: lib
	$(MAKE) -C src/tests test

.PHONY: clean
clean:
	$(MAKE) -C src/lib clean
	$(MAKE) -C src/tests clean
