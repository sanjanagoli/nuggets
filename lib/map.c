/*
* map.c - nuggets game map module
*
* Author: Daniel DiPietro
*
* Last Edited: 5/16/2019
*
* See hashtable.h for more information
*/

#include <stdio.h>
#include <stdbool.h>
#include "../support/file.h"
#include "point.h"

/**************** global types ****************/
typedef struct map {
  const char* mapData;
  int nrows;
  int ncols;
  set_t* nuggetLocs;
  set_t* consumedNugs;
  int nugsRemaining;
  int pilesRemaining;
} map_t;

/**************** local functions *****************/

/**************** global functions ****************/
/* that is, visible outside this file */
/* see map.h for comments about exported functions */

/**************** map_new ****************/
/* see map.h for description */
map_t* map_new(const char* mapData, int maxBytes, int goldTotal,
               int minPiles, int maxPiles) {
  if (mapData == NULL) {
    return NULL;
  }

  // Verify validity of mapData
  FILE* dataPointer;
  dataPointer = fopen(mapData, "r");
  if (dataPointer == NULL) {
    return NULL;
  }

  // Get number of rows and verify that each row has same number of columns
  int nrows = lines_in_file(dataPointer);
  char* cols;
  cols = freadlinep(dataPointer);
  int ncols = strlen(cols);
  free(cols);
  while ((cols = freadlinep(dataPointer)) != NULL) {
    if (strlen(cols) != ncols) {
      return NULL;
    }
    free(cols);
  }
  fclose(dataPointer);

  // Check that map isn't above max number of bytes
  if ((nrows*ncols + 10) > maxBytes) {
    return NULL;
  }

  set_t* consumedNugs = set_new();
  if (consumedNugs == NULL) {
    return NULL;
  }

  // Map has passed checks; initialize it
  map_t *map = malloc(sizeof(map_t));
  if (map == NULL) {
    return NULL;
  }

  map->nrows = nrows;
  map->ncols = ncols;
  map->mapData = mapData;
  map->consumedNugs = consumedNugs;
  map->nugsRemaining = totalNuggets;
  map_genNugs(map, minPiles, maxPiles);

  return map;
}

/**************** map_getChar ****************/
/* see map.h for description */
char map_getChar(map_t* map, int x, int y) {
  if (map != NULL) {
    return '\0';
  }
  if (x > map->ncols || y > map->nrows || x < 0 || y < 0) {
    return '\0';
  }
  char* mapData = map->mapData;
  char returnChar = index[map->ncols*y + x];
  return returnChar;
}

/**************** map_getNugs ****************/
/* see map.h for description */
set_t* map_getNugLocs(map_t* map) {
  if (map == NULL) {
    return NULL;
  }
  return map->nuggetLocs;
}

/**************** map_nuggetPresent ****************/
/* see map.h for description */
int map_consumeNug(map_t* map, int x, int y) {
  if (map == NULL) {
    return 0;
  }
  if (map->nuggetLocs == NULL) {
    return 0;
  }
  point_t* p = point_new(int x, int y);

  // See if a nugget at this point has already been consumed
  // Faster iteration than checking if the point is a nugget; might as well do
  // it first.
  bool flag;
  set_iterate(map->nuggetLocs, flag, nuggetConsume_helper);
  if (!bool) {
    point_delete(p);
    return 0;
  }

  // See if there is a nugget at this point
  set_iterate(map->nuggetLocs, flag, nuggetConsume_helper);
  point_delete(p);
}


/**************** map_isEmptySpot ****************/
/* see map.h for description */
bool map_isEmptySpot(map_t* map, int x, int y) {
  if (map != NULL && map_getchar(map, x, y) == '.') {
    return true;
  }
  return false;
}

/**************** map_nugsRemaining ****************/
/* see map.h for description */
int map_nugsRemaining(map_t* map) {
  if (map == NULL) {
    return 0;
  }
  return map->nugsRemaining;
}

/**************** map_setNugs ****************/
/* see map.h for description */
bool map_setNugs(map_t* map, set_t* nugs) {
  if (map != NULL && nugs != NULL) {
    if (map->nuggetLocs != NULL) {
      return false;
    }
    map->nuggetLocs = nugs;
    return true;
  }
}

/**************** map_genNugs ****************/ do this
/* see map.h for description */
void map_genNugs(map_t* map, int minPiles, int maxPiles) {
  if (map != NULL) {
    int pilesToMake =
    int numPiles;
    while (numPiles < pilesToMake) {
      randomX = rand() % map->ncols;
      randomY = rand() % map->nrows;
      map_getChar(map, randomX, randomY);
    }
  }
}

/**************** map_getVisiblePoints ****************/
/* see map.h for description */
set_t* map_getVisibility(map_t* map, int x, int y) {
  set_t* visiblePoints = set_new();
  int numPoints = 0;
}

/**************** map_getVisibility ****************/
/* see map.h for description */
set_t* map_getVisibility(map_t* map, int x, int y) {

}

/**************** map_delete ****************/
/* see map.h for description */
void map_delete(map_t* map) {
  set_delete(nuggetLocs, point_delete);
  set_delete(consumedNugs, point_delete);
  free(mapData);
  free(map);
}

/**************** nuggetPresent_helper ****************/
// Function used by set_iterate called in map_nuggetPresent()
static void nuggetConsume_helper(void* arg, const char *key, void* item) {

}
