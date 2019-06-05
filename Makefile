# Makefile for CS50 nuggets 
#   Builds and compiles every directory, as well as gameComm.
#
# Dan DiPietro

## STYLE: define shorthand variables for ./support, ./lib, ./libcs50

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make
## GRADER: the spec requires name 'server' for this program.
PROG = gameComm
OBJS = gameComm.o
LIBS = ./support/support.a ./libcs50/libcs50.a
LLIBS = ./lib/lib.a

# 'phony' targets are helpful but do not create any file by that name
.PHONY: all clean

# recursively make in each subdirectory
all: libraries comm

libraries:
	$(MAKE) --directory=lib
	$(MAKE) --directory=support

## GRADER: target name is 'comm' but what it builds is gameComm
comm: $(OBJS) $(LIBS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o $(PROG)

gameComm.o: gameComm.c

# to clean up all derived files
clean:
	rm -f *~ *.o
	rm -f $(PROG)
	make --directory=lib clean
	make --directory=support clean
