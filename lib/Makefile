# Makefile for lib.a
# make will generate the archive
# make sourcelist will provide items in the archive
# make clean will remove the archive

OBJS = point.o map.o participant.o
LIB = lib.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make

# Build the library by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)


# Dependencies: object files depend on header files
point.o: point.h
map.o: map.h
participant.o: participant.h

.PHONY: clean sourcelist

# list all the sources and docs in this directory.
sourcelist: Makefile *.md *.c *.h
	@echo $^

# clean up after our compilation
clean:
	rm -f $(LIB) *~ *.o