/* 
 * masterGame.h - header file for Nuggets 'masterGame' module
 * 
 * A ‘masterGame’ is a struct holding the central data for the entire game
 *
 * Dhaivat Mehta, Dartmouth College, 19S, COSC 050, Final Project - Nuggets
 */

#ifndef __masterGame_H
#define __masterGame_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include <stdbool.h>
#include <dirent.h>
#include <limits.h>
#include "../libcs50/set.h"
#include "../libcs50/file.h"
#include "../lib/point.h"
#include "../lib/participant.h"
#include "../lib/map.h"

/**************** global types ****************/
typedef struct masterGame masterGame_t;  // opaque to users of the modules

map_t * masterGame_getMap(masterGame_t * mg); 
participant_t * masterGame_getPart(masterGame_t * mg, char id);
set_t * masterGame_getActiveParticipants(masterGame_t * mg);

/**************** masterGame_new ****************/
/* initializes masterGame structure
 * takes a struct map containing the offical overall game map
 * masterGame structure holds:
 * 		struct map
 *		struct set containing participants as items and their game symbols as keys
 * 		struct set containing removed players as items and their game symbols as keys
 *		boolean saying if there is a spectator participant yet or not
 *		count of current active players
 *		array with entire alphabet to serve as all potential game symbols
 *	
 * returns a masterGame data structure
 * returns NULL if there is an issue allocating memory for the masterGame structure
*/
masterGame_t * masterGame_new(char * pathname, int seed);

/**************** masterGame_addPart ****************/
/* takes masterGame data structure and string representing a new participant's real name
 * adds participant to masterGame
 * 
 * no major assumptions
 * 
 * intializes and creates a participant based on the real name string
 * if partcicipant is of type spectator and another spector already is in the game
 * then, removes current spectator and adds new spectator
 * inserts particpant to particpants set 
 *
 * returns char id for the intialized particpant
 */
char masterGame_addPart(masterGame_t * mg, char * playerRealName);

/**************** masterGame_removePart ****************/
/* takes masterGame data structure and struct participant
 * removes participant from masterGame
 * 
 * no major assumptions
 * 
 * checks that masterGame is not null and participant is not null
 * gets played id of participant
 * if player id isn't that of spectator then inserts participant into set of removed players
 * intializes a setUpdater struct that holds participant to be removed and empty sturct set
 * iterates over current set of participants 
 * copies all participants (besides one to be removed) into set in struct setUpdater
 * sets participant set in masterGame to be updated participants set hnow eld in setUpdater struct
 * deletes setUpdater struct
 *
 * returns true if adding the particpant was succesful
 * returns false if masterGame is null/ participant is null
 */
bool masterGame_removePart(masterGame_t* mg, participant_t* part);

/**************** masterGame_movePartLoc ****************/
/* takes masterGame data structure, character id, a change in x, and a change in y
 * moves character id from current location in map by given values for change in x and change in y
 * 
 * assumes that a valid updated verison of the map is stored in the master game
 * 
 * checks that masterGame is not null
 * checks that masterGame particpants set contains the given player id
 * gets the current location of player id as point struct from participants set
 * decomposes point representing current location into its x and y int 
 * adds change in x and y to location
 * creates new point representing updated location
 * moves player to updated lcation
 * checks if updated location is on a nugget
 * if location is on a nuggest consumes the nugget in the map and increments the player's purse
 * 
 * returns true if change in location process completes succesfully
 * returns false if masterGame is null, if palyer id isn't in current particpants list, or if location changing process fails
 */
bool masterGame_movePartLoc(masterGame_t* mg, char id, int dx, int dy);

/**************** masterGame_setPartLoc ****************/
/* takes masterGame data structure, character id, a x value for location, a y value for location
 * moves character id from current location to new location given by x and y
 *
 * assumes that a valid updated verison of the map is stored in the master game
 * 
 * checks that masterGame is not null
 * checks that masterGame particpants set contains the given player id
 * gets the current location of player id as point struct from participants set
 * creates new point representing new location from given x and y
 * moves player to new location
 * checks if updated location is on a nugget
 * if location is on a nuggest consumes the nugget in the map and increments the player's purse
 * 
 * returns true if change in location process completes succesfully
 * returns false if masterGame is null, if palyer id isn't in current particpants list, or if location updating process fails
 */
bool masterGame_setPartLoc(masterGame_t* mg, char id, int x, int y);

/**************** masterGame_getPlayerCount ****************/
/* getter function that returns number of active players
 * 
 * assumes master game is not null
 *
 * returns int that represents active players currently in game
 */
int masterGame_getPlayerCount(masterGame_t * mg);

/**************** masterGame_displayMap ****************/
/* takes masterGame data structure and participant
 * returns a string representng map data for given participant at current time
 *
 * assumes that a valid updated verison of the map is stored in the master game
 * 
 * gets point representing participants current location
 * decomposes point into integers representing x and y coordiantes
 * gets a set representing points that are now visible 
 * does this by calling `map_getVisibility` function 
 * this function returns as set where key = string representation of point and item = point
 * gets a set representing points that were previously made visible
 * does this by calling `participant_getVisiblePoints` function 
 * this function returns as set where key = string representation of point and item = point
 * merges the set of currently visible points with set of previously made visible points
 * gets a set representing points containing unconsumed nuggets
 * does this by calling `map_getUnconsumedNugLocs` function 
 * this function returns as set where key = string representation of point w/ a nugget and item = point
 * gets a set representing points containing current active participants via a helper function
 * this function returns as set where key = string representation of point w/ an active participant and item = point
 * loops over every point in map
 * if point is at end of row places a newline symbol there
 * if point is visible and location of nugget places asterisk there
 * if point is visible and location of paricipant then places given particiapnts id there
 * if point is visible and doesn't contain nugget or participant then places base map feature there (wall, hallway, etc.)
 * if point isn't visible places a blank space there
 * 
 * returns string representing map data for given participant at this point in time
 */
char * masterGame_displayMap(masterGame_t * mg, participant_t * part);


/**************** masterGame_endGame ****************/
/* returns a multi-line string summarizing game data
 * 
 * assumes that master game is valid and not null
 *
 * creates empty string to hold game summary data
 * iterates over list of active participants and adds them to game summary data
 * for each player displays their game id, their purse, and their real name 
 * then does the same for list of removed players
 * 
 * returns string with full game summary
 */
char * masterGame_endGame(masterGame_t * mg);

/**************** masterGame_delete ****************/
/* deletes masterGame data structure
 * frees allocated memory
 * 
 * assumes that no non-standard data was entered
 * assumes existance of set delete and map delete functions
 *
 * uses set delete function and map delete function
 * frees overall structure at end 
 */
void masterGame_delete(masterGame_t * mg);

#endif // __masterGame_H
