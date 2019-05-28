# Makefile for CS50 nuggets 
#   Builds and compiles every directory, as well as gameComm.
#
# Dan DiPietro

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50 -I../common
CC = gcc
MAKE = make
PROG = gameComm
OBJS = gameComm.o
LIBS = gameComm.c ./support/support.a ./lib/lib.a ./libcs50/libcs50.a

# recursively make in each subdirectory
all:
	$(MAKE) --directory=lib
	$(MAKE) --directory=support
	$(PROG): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(PROG)

# 'phony' targets are helpful but do not create any file by that name
.PHONY: clean

# to clean up all derived files
clean:
	rm -f *~ *.o
	rm -f $(PROG)
	make --directory=lib clean
	make --directory=support clean