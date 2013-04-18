# OneJoker library <http://lcrocker.github.io/OneJoker>
# Token Makefile that just calls the real build script
#

all: classes

.PHONY: classes
classes:
	python3 ../../build.py javaclasses

.PHONY: headers
headers:
	python3 ../../build.py javaheaders

.PHONY: tests
tests:
	python3 ../../build.py javatests

.PHONY: run
run:
	python3 ../../build.py runjavatests

.PHONY: clean
clean:
	python3 ../../build.py cleanjava
