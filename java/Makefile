# Token Makefile that just calls the real build script
#

all: headers

.PHONY: headers
headers:
	python3 ../build.py java

.PHONY: clean
clean:
	python3 ../build.py javaclean

.PHONY: tests
tests:
	python3 ../build.py javatests

.PHONY: run
run:
	python3 ../build.py runjavatests
