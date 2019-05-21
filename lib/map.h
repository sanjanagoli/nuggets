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
/* Create a new map struct
 *
 * Caller provides:
 *   Valid map data, a max amount of bytes per message, the amount of gold on the
 *   map, the minimum number of gold piles, the maximum number of gold piles,
 *   and an unsigned integer to serve as a seed for random number generation. If
 *   they wish to use a completely random seed, they can pass in -1.
 * We return:
 *   A pointer to new map, which will have nuggets initialized via map_genNugs.
 * Caller is responsible for:
 *   Freeing the map when they are done.
 */
map_t* map_new(char* mapData, int maxBytes, int goldTotal,
               int minPiles, int maxPiles, int seed);

/**************** map_getMapData ****************/
/* Retrieve a pointer to mapData
 *
 * Caller provides:
 *   A valid map struct with mapData
 * We return:
 *   A pointer to the mapData contained within the passed map
 * Caller is responsible for:
 *   Not modifying map in a way that would render it invalid
 */
char* map_getMapData(map_t* map);

/**************** map_getChar ****************/
/* Retrieve the character located at a point on the map
 *
 * Caller provides:
 *   A valid map struct with mapData and valid x and y positions
 * We return:
 *   The character located at the given coordinates
 */
char map_getChar(map_t* map, int x, int y);

/**************** map_getRows ****************/
/* Retrieve the number of rows in the map (the maximum y-coordinate)
*
* Caller provides:
*   A valid map struct with mapData and valid x and y positions
* We return:
*   The character located at the given coordinates
*/
int map_getRows(map_t* map);

/**************** map_getCols ****************/
/* Retrieve the number of columns in the map (the maximum x-coordinate)
*
* Caller provides:
*   A valid map struct with mapData and valid x and y positions
* We return:
*   The character located at the given coordinates
*/
int map_getCols(map_t* map);

/**************** map_isEmptySpot ****************/
/* Returns true if a '.' is located at the given point. Does not take into
 * account player or nugget locations when determing an "empty" spot.
 *
 * Caller provides:
 *   A valid map struct with mapData and valid x and y positions
 * We return:
 *   A boolean representing whether the point specifies a '.' or not
 */
bool map_isEmptySpot(map_t* map, int x, int y);

/**************** map_getEmptySpots ****************/
/* Retrieve a set containing all empty spots (defined as '.') on the map.
 *
 * Caller provides:
 *   A valid map struct with mapData
 * We return:
 *   A pointer to a set that contains all empty points (defined as '.') located
 *   on the map. The keys of the set are trivial integers representing the order
 *   that each point was discovered. The items are point structs.
 * We guarantee:
 *   All empty spots on the map are contained within this set
 * Caller is responsible for:
 *   Freeing the set that is returned
 */
set_t* map_getEmptySpots(map_t* map);


/**************** map_getNugLocs ****************/
/* Retrieve a set containing all nugget locations, even those that have been
 * consumed.
 *
 * Caller provides:
 *   A valid map struct with mapData
 * We return:
 *   A pointer to a set that contains all nugget locations, even if consumed.
 *   The keys of the set are trivial integers representing the order that each
 *   point was discovered. The items are point structs.
 * We guarantee:
 *   All nuggets on the map are contained within this set, even if they have
 *   been consumed.
 * Caller is responsible for:
 *   Not freeing this set before the map is deleted, since the pointer returned
 *   refers to the nugget locations held by the map itself.
 */
set_t* map_getNugLocs(map_t* map);

/**************** map_getUnconsumedNugLocs ****************/
/* Retrieve a set containing all nugget locations that have not yet been
 * consumed.
 *
 * Caller provides:
 *   A valid map struct with mapData
 * We return:
 *   A pointer to a set that contains all unconsumed nugget locations.
 *   The keys of the set are meaningless; the items are point structs.
 * We guarantee:
 *   All nuggets contained within this set are unconsumed.
 * Caller is responsible for:
 *   Freeing the set that is returned.
 */
set_t* map_getUnconsumedNugLocs(map_t* map);

/**************** map_getConsumedNugLocs ****************/
/* Retrieve a set containing all nugget locations that have been consumed.
 *
 * Caller provides:
 *   A valid map struct with mapData
 * We return:
 *   A pointer to a set that contains all consumed nugget locations.
 *   The keys of the set are meaningless; the items are point structs.
 * We guarantee:
 *   All nuggets contained within this set are unconsumed.
 * Caller is responsible for:
 *   Not freeing this set before the map is deleted, since the pointer returned
 *   refers to the nugget locations held by the map itself.
 */
set_t* map_getConsumedNugLocs(map_t* map);

/**************** map_consumeNug ****************/
/* Attempt to consume a nugget at a specified location.
 *
 * Caller provides:
 *   A valid map struct with mapData, proper nuggets, and valid x and y
 *   coordinates.
 * We return:
 *   A reasonable randomly generated value for the pile located at the specified
 *   coordinates. If there is not a pile, 0 is returned.
 * We guarantee:
 *   Once all piles are gone, there will be no leftover nuggets.
 */
int map_consumeNug(map_t* map, int x, int y);

/**************** map_nuggetPresent ****************/
/* Returns whether or not a nugget, even if consumed, is located at specified
 * coordinates.
 *
 * Caller provides:
 *   A valid map struct with mapData, proper nuggets, and valid x and y
 *   coordinates.
 * We return:
 *   A boolean representing whether a nugget is located at the point.
 */
bool map_nuggetPresent(map_t* map, int x, int y);

/**************** map_nugsRemaining ****************/
/* Returns the number of nuggets left.
 *
 * Caller provides:
 *   A valid map struct with mapData and properly initialized nuggets
 * We return:
 *   The number of nuggets remaining.
 */
int map_nugsRemaining(map_t* map);

/**************** map_pilesRemaining ****************/
/* Returns the number of piles left.
 *
 * Caller provides:
 *   A valid map struct with mapData and properly initialized nuggets
 * We return:
 *   The number of piles remaining.
 */
int map_pilesRemaining(map_t* map);

/**************** map_genNugs ****************/
/* Generate the nuggets for a given map.
 *
 * Caller provides:
 *   A valid map struct with mapData and a specified amount of nuggets remaining.
 *   Parameters such that minPiles < maxPiles.
 * We return:
 *   Nothing; the generated nuggets are inserted directly into the map pointer.
 * We guarantee:
 *   Nuggets will be generated at random on valid location between the specified
 *   number of minimum and maximum piles.
 */
void map_genNugs(map_t* map, int minPiles, int maxPiles);

/**************** map_getVisibility ****************/
/* Return the visibility for a given point on the map via a ray cast algorithm
 *
 * Caller provides:
 *   A valid map struct with mapData and valid x and y coordinates for an
 *   empty space.
 * We return:
 *   A set containing all points visible at the specified point as determined by
 *   the ray cast algorithm outlined in the implementation specifications.
 * Caller is responsible for:
 *  Freeing the set that is returned eventually
 */
set_t* map_getVisibility(map_t* map, int x, int y);

/**************** map_delete ****************/
/* Free all memory associated with a map struct
 *
 * Caller provides:
 *   A valid map struct
 * We guarantee:
 *   All memory associated with the passed map will be freed.
 */
void map_delete(map_t* map);

#endif // __MAP_H
