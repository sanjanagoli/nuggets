/*
 * map.h - header file for nuggets game map module
 *
 * Description here
 *
 * Author: Dan DiPietro
 * Last Updated: 5/19/2019
 */

#ifndef __MAP_H
#define __MAP_H

#include <stdio.h>
#include <stdbool.h>

/**************** global types ****************/
typedef struct map map_t;  // opaque to users of the module

/**************** functions ****************/

/**************** map_new ****************/
/*
 */
map_t* map_new(const char* mapData, int maxBytes, int goldTotal,
               int minPiles, int maxPiles);

/**************** map_getChar ****************/
/*
*/
char* map_getMapData(map_t* map);

/**************** map_getChar ****************/
/*
 */
char map_getChar(map_t* map, int x, int y);

/**************** map_isEmptySpot ****************/
/*
*/
bool map_isEmptySpot(map_t* map, int x, int y);

/**************** map_getEmptySpots ****************/
/*
*/
set_t* map_getEmptySpots(map_t* map);


/**************** map_getNugLocs ****************/
/*
*/
set_t* map_getNugLocs(map_t* map);

/**************** map_consumeNug ****************/
/*
*/
int map_consumeNug(map_t* map, int x, int y);

/**************** map_nuggetPresent ****************/
/*
*/
bool map_nuggetPresent(map_t* map, int x, int y);

/**************** map_nugsRemaining ****************/
/*
*/
int map_nugsRemaining(map_t* map);

/**************** map_pilesRemaining ****************/
/*
*/
int map_pilesRemaining(map_t* map);

/**************** map_setNugs ****************/
/*
*/
bool map_setNugs(map_t* map, set_t* nugs);

/**************** map_genNugs ****************/
/*
*/
void map_genNugs(map_t* map, int minPiles, int maxPiles);

/**************** map_getVisibility ****************/
/*
*/
set_t* map_getVisibility(map_t* map, int x, int y);

/**************** map_getEmptySpots ****************/
/*
*/
set_t* map_getEmptySpots(map_t* map);

/**************** map_delete ****************/
/*
*/
void map_delete(map_t* map);



#endif // __MAP_H
