# Token Makefile that just calls the real build script
#

all: headers

.PHONY: headers
headers:
	python3 ../build.py java

.PHONY: clean
clean:
	python3 ../build.py javaclean


.PHONY: test
test:
	python3 ../build.py javatests
