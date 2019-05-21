# Makefile for testing
#
# Author: Daniel DiPietro
# Last Edited: 5/20/2019

V = ../lib
L = ../libcs50

# Uncomment to see log messages
#TESTING=-DAPPTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../lib
CC = gcc
MAKE = make
PROG = pointTest
PPROG = mapTest
OBJS = pointTest.o
OOBJS = mapTest.o
LIBS = $L/libcs50.a
LLIBS = $V/lib.a

.PHONY: all test valgrind clean

all: $(PROG) $(PPROG)

# executable depends on object files
$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o $(PROG)

# executable depends on object files
$(PPROG): $(OOBJS)
	$(CC) $(CFLAGS) $(OOBJS) $(LLIBS) $(LIBS) -o $(PPROG)

pointTest.o: pointTest.c
mapTest.o: mapTest.c

test: $(PROG) $(PPROG)
	bash -v testing.sh

valgrind: $(PROG)
	valgrind ./$(PROG);

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o *.t
	rm -f $(PROG) $(PPROG)
