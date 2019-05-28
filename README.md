# Nuggets

This repository contains the code for the CS50 "Nuggets" game, in which players explore a set of rooms and passageways in search of gold nuggets.
The rooms and passages are defined by a *map* loaded by the server at the start of the game.
The gold nuggets are randomly distributed in *piles* within the rooms.
Up to 26 players, and one spectator, may play a given game.
Each player is randomly dropped into a room when joining the game.
Players move about, collecting nuggets when they move onto a pile.
When all gold nuggets are collected, the game ends and a summary is printed.

## Overview
* `gameComm.c` -> main program that runs the server side of the game, manages adding/removing clients, calls game logic functions, and end/starts overall game
* `support` -> subdirectory with useful modules provided by professor
* `maps` -> subdirectory containing maps for testing/playing with
* `lib` -> subdirectory containing essential modules used in running the internal game logic
* `libcs50` -> subdirectory contianing utility modules from class 
* `DESIGN.md` -> design specifications for the game
* `IMPLEMENTATION.md` -> implementation details for the game
* `TESTING.md` -> testing protocol for the game and its modules
* `REQUIREMENTS.md` -> requirements specifications for the game
* `Makefile` -> top-level make file that makes all subdirectories and `gameComm`

## Specs

See the [Requirements Spec](REQUIREMENTS.md) for details about the game and its network protocol.

See the [Implementation Spec](IMPLEMENTATION.md) for details about how modules were implemented to run the game.

See the [Desgin Spec](DESIGN.md) for details about the overall strucutre and flow of data in the game.

See the [Testing Spec](TESTING.md) for details about the testing done to ensure the game works.


## Subdirectories

See the [support library](support/README.md) for some useful modules.

See the [maps](maps/README.md) for some draft maps.

See the [lib](lib/README.md) for essential game modules.

See the [libcs50](libcs50/README.md) for useful utility modules provided from class.



## Updating your starter kit

To update your repo from the starter kit, *commit and push* your work, then

    # do this once:
    git remote add starter git@github.com:cs50spring2019/nuggets.git
    # do this every time you want to update:
    git pull starter master

much like in these [instructions](https://www.cs.dartmouth.edu/~cs50/Labs/updates.html).

## Usage

To make the main program (`gameComm`) and all subdirectories call:
```c
make
```
To clean,
```c
make clean
```

This approach allows the main program to be built (or cleaned) while automatically building (cleaning) the support libraries as needed.
