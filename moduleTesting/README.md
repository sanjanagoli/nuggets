# moduleTesting

This library contains testing for four modules critical for the function of the CS50 final project.

## 'pointTest' module

This module contains testing for `pointTest` module in the `lib` directory


## 'participantTest' module

This module contains testing for `participantTest` module in the `lib` directory
 

## 'mapTest' module

This module contains testing for `mapTest` module in the `lib` directory


## 'masterGameTest' module

This module contains testing for `masterGame` module in the `lib` directory

## compiling

To compile,

	make

To clean,

	make clean

## using

In a typical use, assume this library is a subdirectory named `moduleTesting`, within a directory there is another folder called `lib` that contains the modules being tested by the files in this library.  
In order to see output of the testing one can look at the `.out` files for each testing file.

## testing

Each of these modules performs unit testing for each of the modules in the `moduleTesting` directory, enabling it to be compiled stand-alone for testing.
See the `Makefile`  in `moduleTesting` for the compilation.  

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