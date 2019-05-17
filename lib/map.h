/*
 * map.h - header file for nuggets game map module
 *
 * Description here
 *
 * Author: Dan DiPietro
 * Last Updated: 5/16/2019
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
map_t* map_new(char* mapData);

/**************** map_getChar ****************/
/*
 */
char map_getChar(map_t* map, int x, int y);

/**************** map_getNugs ****************/
/*
 */
set_t* map_getNugs(map_t* map);

/**************** map_getPlayeLocs ****************/
/*
 */
set_t* map_getPlayerLocs(map_t* map);

/**************** map_nuggetPresent ****************/
/*
 */
bool map_nuggetPresent(map_t* map, int x, int y);

/**************** map_consumeNugs ****************/
/*
 */
void map_consumeNug(map_t* map, int x, int y);

/**************** map_setNugs ****************/
/*
*/
void map_setNugs(map_t* map, set_t* nugs);

/**************** map_playerPresent ****************/
/*
*/
bool map_playerPresent(map_t* map, int x, int y);

/**************** map_getVisibility ****************/
/*
*/
set_t* map_getVisibility(map_t* map, int x, int y);

#endif // __MAP_H
