# Token Makefile that just calls the real build script
#

all: tests

.PHONY: library
library:
	python3 ../../build.py library

.PHONY: tests
tests:
	python3 ../../build.py ctests

.PHONY: run
run:
	python3 ../../build.py runctests

.PHONY: clean
clean:
	python3 ../../build.py cleantests

