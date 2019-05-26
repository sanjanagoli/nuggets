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
PPPROG = participantTest
PPPPROG = masterGameTest
OBJS = pointTest.o
OOBJS = mapTest.o
OOOBJS = participantTest.o
OOOOBJS = masterGameTest.o
LIBS = $L/libcs50.a
LLIBS = $V/lib.a

.PHONY: all test valgrind clean

all: $(PROG) $(PPROG) $(PPPROG) $(PPPPROG)

# executable depends on object files
$(PROG): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o $(PROG)

# executable depends on object files
$(PPROG): $(OOBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OOBJS) $(LLIBS) $(LIBS) -o $(PPROG)

$(PPPROG): $(OOOBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OOOBJS) $(LLIBS) $(LIBS) -o $(PPPROG)

$(PPPPROG): $(OOOOBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OOOOBJS) $(LLIBS) $(LIBS) -o $(PPPPROG)

pointTest.o: pointTest.c
mapTest.o: mapTest.c
participantTest.o: participantTest.c
masterGameTest.o: masterGameTest.c

test: $(PROG) $(PPROG)
	bash -v testing.sh

valgrind: $(PROG)
	valgrind ./$(PROG); valgrind ./$(PPROG)

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o *.t
	rm -f $(PROG) $(PPROG)
