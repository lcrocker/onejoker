# Makefile for src/tests/ directory of OneJoker project.
#
SHELL=/bin/sh
CC=gcc
LD=gcc
CFLAGS=-Wall -I../include
LDFLAGS=-L../lib -lm -lonejoker
SOURCES=test_random.c test_sequence.c test_iters.c test_004.c

ifeq ($(OS),Windows_NT)
MAKE=make
LIB=onejoker.dll
EXE=.exe
else
LIB=libonejoker.so
EXE=
endif

EXECUTABLES=$(SOURCES:.c=$(EXE))

.SUFFIXES:

.PHONY: all
all: $(EXECUTABLES)

test: $(EXECUTABLES)
	cp ../lib/$(LIB) .
	./test_random
	./test_sequence
	./test_iters

$(EXECUTABLES): %$(EXE) : %.c ../include/onejoker.h $(LIB)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

$(LIB): ../lib/*.c ../lib/*.h ../include/*.h
	$(MAKE) -C ../lib lib
	cp ../lib/$(LIB) .

.PHONY: clean
clean:
	rm -f $(EXECUTABLES) $(LIB)
