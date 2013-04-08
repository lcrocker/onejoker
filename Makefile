# Token Makefile that just calls the real build script
#

all: library

.PHONY: library
library:
	python3 ./build.py library

.PHONY: tests
tests:
	python3 ./build.py tests

.PHONY: test
test:
	python3 ./build.py tests

.PHONY: clean
clean:
	python3 ./build.py clean

