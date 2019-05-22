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
OBJS = pointTest.o
LIBS = $L/libcs50.a
LLIBS = $V/lib.a

.PHONY: all test valgrind clean

all: $(PROG)

# executable depends on object files
$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o $(PROG)

pointTest.o: pointTest.c

test: $(PROG) $(PPROG)
	bash -v testing.sh

valgrind: $(PROG)
	valgrind ./$(PROG);

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o *.t
	rm -f $(PROG)
