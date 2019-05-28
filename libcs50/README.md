# libcs50 utility library

This library contains a variety of support modules potentially useful in the CS50 final project.

## Usage
To build libcs50.a, run `make`.  

To bring  data-structure implementation, modify the Makefile to replace the rule for `$(LIB)`:
```c
$(LIB): libcs50-given.a
	cp libcs50-given.a $(LIB)
```
Notice that command just copies the relevant pre-compiled library to libcs50.a.

To clean up, run `make clean`.

## Overview
* `bag` - the bag data structure from Lab 3
* `counters` - the counters data structure from Lab 3
* `file` - functions to read files (includes readlinep)
* `hashtable` - the hashtable data structure from Lab 3
* `jhash` - the Jenkins Hash function used by hashtable
* `memory` - handy wrappers for malloc/free
* `set` - the set data structure from Lab 3
* `webpage` - functions to load and scan web pages