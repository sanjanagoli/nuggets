# lib library

This library contains four modules critical for the function of the CS50 final project.

## 'point' module

This module stores the struct `point` data structure used to hold 2D coordinates. `point` contains functions that easily allow for the point to move in any direction, as well as a function allowing point to be converted to a unique string for convenient use as a key  
See `point.h` for interface details and some usage examples

## 'participant' module

This module stores the struct `participant` data structure used to represent each player.  
See `participant.h` for interface details and some usage examples.   

## 'map' module

This module stores the struct `map` data structure used to hold the map. `map` deals with visibility and nugget-collecting.
See `map.h` for interface details and some usage examples.

## 'masterGame' module

This module contains thecontains the centralized version of the game, as well as functions to update player status (position, purse, etc) and display/modify map visibility for a given player.  
See `masterGame.h` for interface details and some usage examples.

## compiling

To compile,

	make

To clean,

	make clean

## using

In a typical use, assume this library is a subdirectory named `lib`, within a directory where some main program is located.
The Makefile of that main directory might then include content like this:

```make
S = support
CFLAGS = ... -I$S
LLIBS = $S/lib.a
...
program.o: ... $S/point.h $S/map.h $S/participant.h $S/masterGame.h
...
$S/support.a:
	make -C $S lib.a

clean:
	make -C $S clean
	...
```

This approach allows the main program to be built (or cleaned) while automatically building (cleaning) the lib library as needed.

## testing

Each of these modules has unit testing in the `moduleTesting` directory, enabling it to be compiled stand-alone for testing.
See the `Makefile`  in `moduleTesting` for the compilation.  
In order to test go into `moduleTesting` directory and use the Makefile.  

To compile,
```c
make
```

Make will create the test files for each of the four modules:
`pointTest`  
`participantTest`   
`mapTest`  
`masterGameTest`  

You can call each of these individually in `moduleTest` by calling them as:
```c
./pointTest
./participantTest
./mapTest
./masterGameTest
```

In order to run all these test files and save their output run `testing.sh` script  
This will create the folowing files:
```c
pointTest.out
participantTest.out
mapTest.out
masterGameTest.out
```
Each of these files contains the output of the unit test files.

To do this in a more simply way just run:
```c
make test
```
Which will simply run `testing.sh`