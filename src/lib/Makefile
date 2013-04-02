# Makefile for src/lib/ directory of OneJoker project.
#
SHELL=/bin/sh
CC=gcc
LD=gcc
CFLAGS=-Wall -I../include
LDFLAGS=-shared -lm
SOURCES=onejoker.c deckinfo.c prng.c sequence.c combinatorics.c poker.c
OBJECTS=$(SOURCES:.c=.o)

ifeq ($(OS),Windows_NT)
LIB=onejoker.dll
LDFLAGS+= -lcrypt32
else
LIB=libonejoker.so
LDFLAGS+= -nostartfiles
endif

$(LIB): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(LIB)

$(OBJECTS): ../include/onejoker.h
combinatorics.o: bctable.h
poker.o: ldctables.h

.SUFFIXES:

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

all: lib

.PHONY: lib
lib: $(LIB)

.PHONY: clean
clean:
	rm -f *.o *.dll *.so
