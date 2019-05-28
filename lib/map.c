// #############################################################################
// Overview
// #############################################################################

/*
* map.c - nuggets game map module
*
* Author: Daniel DiPietro
*
* Last Edited: 5/19/2019
*
* See map.h for more information
*/

// #############################################################################
// Imports
// #############################################################################

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include "../libcs50/file.h"
#include "../libcs50/set.h"
#include "point.h"
#include "map.h"

// #############################################################################
// Global Types
// #############################################################################

typedef struct map {
  char* mapData;
  int nrows;
  int ncols;
  set_t* nuggetLocs;
  set_t* consumedNugs;
  int nugsRemaining;
  int pilesRemaining;
  int avgValue;
} map_t;

// #############################################################################
// Local Types
// #############################################################################

struct ray {
  int xloc; // x-location of the ray
  int yloc; // y-location of the ray
  int xcomp; // x-component of the ray
  int ycomp; // y-component of the ray
  int xexpand; // Maximum value that ray can expand in its x component direction
  int yexpand; // Maximum value that ray can expand in its y component direction
  bool done; // Whether or not the ray can be expanded anymore.
};

struct nugLocHelper {
  map_t* map;
  set_t* unconsumedNugs;
};

// #############################################################################
// Local Functions
// #############################################################################

static void expandRay(map_t* map, struct ray* r, set_t* visPoints);
static bool setHasPointWrapper(set_t* set, int x, int y);
static void addAdjacentWalls(map_t* map, set_t* visPoints, int x, int y);
static bool isWallChar(char c);
static void pointDeleteHelper(void *item);
static void unconsumedNugLocsHelper(void *nugLocHelper, const char *key, void *item);
static void countHelper(void *arg, const char *key, void *item);

// #############################################################################
// Global Functions
// See map.h for comments about exported function
// #############################################################################

/**************** map_new ****************/ // fix gen nugs
/* see map.h for description */
map_t* map_new(char* mapData, int maxBytes, int goldTotal,
               int minPiles, int maxPiles, int seed) {
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
  int nrows = 1;
  char* cols;
  cols = freadlinep(dataPointer);
  int ncols = strlen(cols);
  free(cols);
  while ((cols = freadlinep(dataPointer)) != NULL) {
    nrows++;
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
  
  set_t* nuggetLocs = set_new();
  if (nuggetLocs == NULL) {
    return NULL;
  }

  // Map has passed checks; initialize it
  map_t *map = malloc(sizeof(map_t));
  if (map == NULL) {
    return NULL;
  }

  // Set seed for random generation
  if (seed != -1 && seed >= 0) {
    srand(seed);
  } else {
    srand(time(NULL));
  }

  map->nrows = nrows;
  map->ncols = ncols;
  dataPointer = fopen(mapData, "r");
  char* mapFromFile = freadfilep(dataPointer);
  map->mapData = mapFromFile;
  fclose(dataPointer);
  
  set_t* emptySpaces = map_getEmptySpots(map);
  int emptySpacesCount = 0;
  set_iterate(emptySpaces, &emptySpacesCount, countHelper);
  if (emptySpacesCount - 26 < maxPiles) {
    free(map->mapData);
    free(map);
    fprintf(stderr, "Map too small.");
    set_delete(consumedNugs, NULL);
    set_delete(nuggetLocs, NULL);
    set_delete(emptySpaces, pointDeleteHelper);
    return NULL;
  }
  set_delete(emptySpaces, pointDeleteHelper);
  
  map->consumedNugs = consumedNugs;
  map->nuggetLocs = nuggetLocs;
  map->nugsRemaining = goldTotal;
  map_genNugs(map, minPiles, maxPiles);
  
  if (map->avgValue < 1) { // Return null and delete
    map_delete(map);
    return NULL;
  }
  

  return map;
}

/**************** map_getChar ****************/
/* see map.h for description */
char map_getChar(map_t* map, int x, int y) {
  if (map == NULL) {
    return '\0';
  }
  if (x >= map->ncols || y >= map->nrows || x < 0 || y < 0) {
    return '\0';
  }
  char* mapData = map->mapData;
  int index = ((map->ncols)*y) + (x+y);
  char returnChar = mapData[(index)];
  return returnChar;
}

/**************** map_getRows ****************/
/* see map.h for description */
int map_getRows(map_t* map) {
  if (map == NULL || map-> mapData == NULL) {
    return 0;
  }
  return map->nrows;
}

/**************** map_getCols ****************/
/* see map.h for description */
int map_getCols(map_t* map) {
  if (map == NULL || map-> mapData == NULL) {
    return 0;
  }
  return map->ncols;
}

/**************** map_getMapData ****************/
/* see map.h for description */
char* map_getMapData(map_t* map) {
  if (map == NULL || map->mapData == NULL) {
    return NULL;
  }
  char* allocatedMap = malloc(strlen(map->mapData)+1);
  strcpy(allocatedMap, map->mapData);
  return allocatedMap;
}

/**************** map_getNugLocs ****************/
/* see map.h for description */
set_t* map_getNugLocs(map_t* map) {
  if (map == NULL) {
    return NULL;
  }
  return map->nuggetLocs;
}

/**************** map_getUnconsumedNugLocs ****************/
/* see map.h for description */
set_t* map_getUnconsumedNugLocs(map_t* map) {
  if (map == NULL || map->nuggetLocs == NULL || map->consumedNugs == NULL) {
    return NULL;
  }
  set_t* unconsumedNugs = set_new();
  struct nugLocHelper nLH = {map, unconsumedNugs};
  set_iterate(map->nuggetLocs, &nLH, unconsumedNugLocsHelper);
  return unconsumedNugs;
}

/**************** map_getConsumedNugLocs ****************/
/* see map.h for description */
set_t* map_getConsumedNugLocs(map_t* map) {
  if (map == NULL) {
    return NULL;
  }
  return map->consumedNugs;
}

/**************** map_consumeNug ****************/
/* see map.h for description */
int map_consumeNug(map_t* map, int x, int y) {
  if (map != NULL && map_nuggetPresent(map, x, y)) {

    point_t* p = point_new(x, y); // Make the point
    char* pointS = point_toString(p);
    if (set_insert(map->consumedNugs, pointS, p) == false) {
      free(p);
    }
    free(pointS);
    (map->pilesRemaining)--;
    
    int valueToReturn;
    if (map->pilesRemaining == 0) {
      valueToReturn = map->nugsRemaining;
      map->nugsRemaining = 0;
    } else {
      int extraNugs = map->nugsRemaining - map->pilesRemaining;
      if (extraNugs < map->avgValue*2) {
        valueToReturn = (rand() % (extraNugs) + 1);
      } else {
        valueToReturn = (rand() % (map->avgValue*2) + 1);
      }
      (map->nugsRemaining) -= valueToReturn;
    }
    return valueToReturn;
  }
  return 0;
}

/**************** map_nuggetPresent ****************/
/* see map.h for description */
bool map_nuggetPresent(map_t* map, int x, int y) {
  if (map == NULL) {
    return false;
  }
  if (map->nuggetLocs == NULL) {
    return false;
  }
  set_t* unconsumed = map_getUnconsumedNugLocs(map);
  bool result = setHasPointWrapper(unconsumed, x, y);
  set_delete(unconsumed, pointDeleteHelper);
  return result;
}

/**************** map_isEmptySpot ****************/
/* see map.h for description */
bool map_isEmptySpot(map_t* map, int x, int y) {
  if (map != NULL && map_getChar(map, x, y) == '.') {
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

/**************** map_pilesRemaining ****************/
/* see map.h for description */
int map_pilesRemaining(map_t* map) {
  if (map == NULL) {
    return 0;
  }
  return map->pilesRemaining;
}

/**************** map_genNugs ****************/
/* see map.h for description */
void map_genNugs(map_t* map, int minPiles, int maxPiles) {
  if (map != NULL) {
    int pilesToMake = (rand() % (maxPiles - minPiles + 1)) + minPiles;
    map->pilesRemaining = pilesToMake;
    map->avgValue = map->nugsRemaining / map->pilesRemaining;
    if (map->avgValue >= 1) {      
      while (pilesToMake > 0) {
        int randomX = rand() % map->ncols;
        int randomY = rand() % map->nrows;
        if (map_isEmptySpot(map, randomX, randomY) &&
        !map_nuggetPresent(map, randomX, randomY)) {
          
          point_t* p = point_new(randomX, randomY);
          char* pS = point_toString(p);
          if (set_insert(map->nuggetLocs, pS, p) == false) {
            point_delete(p);
          }
          free(pS);
          pilesToMake--;
        }
      }
    }
  }
}

/**************** map_getVisibility ****************/
/* see map.h for description */
set_t* map_getVisibility(map_t* map, int x, int y) {
  set_t* visiblePoints = set_new();
  point_t* startPoint = point_new(x, y);
  char* startPointP = point_toString(startPoint);
  set_insert(visiblePoints, startPointP, startPoint);
  free(startPointP);

  // There are only 4 diagonal rays at a given time
  struct ray* rayArray[4];
  
  // Make each of the starting diagonal rays
  struct ray ray1 = {x, y, 1, 1, INT_MAX, INT_MAX, false};
  struct ray* ray1P = &ray1;
  rayArray[0] = ray1P;
  struct ray ray2 = {x, y, 1, -1, INT_MAX, INT_MAX, false};
  struct ray* ray2P = &ray2;
  rayArray[1] = ray2P;
  struct ray ray3 = {x, y, -1, -1, INT_MAX, INT_MAX, false};
  struct ray* ray3P = &ray3;
  rayArray[2] = ray3P;
  struct ray ray4 = {x, y, -1, 1, INT_MAX, INT_MAX, false};
  struct ray* ray4P = &ray4;
  rayArray[3] = ray4P;

  int raysDone = 0;
  while (raysDone != 4) {
    raysDone = 0;
    for (int i = 0; i < 4; i++) {
      if (rayArray[i]->done == false) {
        expandRay(map, rayArray[i], visiblePoints);
      } else {
        raysDone++;
      }
    }
  }
  return visiblePoints;
}

/**************** map_getEmptySpots ****************/
/* see map.h for description */
set_t* map_getEmptySpots(map_t* map) {
  if (map == NULL || map->mapData == NULL) {
    return NULL;
  }
  
  set_t* visiblePoints = set_new();
  
  for (int y = 0; y < map->nrows; y++) {
    for (int x = 0; x < map->ncols; x++) {
      if (map_getChar(map, x, y) == '.') {
        point_t* p = point_new(x, y);
        char* pS = point_toString(p);
        if (set_insert(visiblePoints, pS, p) == false) {
          point_delete(p);
        }
        free(pS);
      }
    }
  }

  return visiblePoints;
}

/**************** map_delete ****************/
/* see map.h for description */
void map_delete(map_t* map) {
  if (map != NULL) {
    set_delete(map->nuggetLocs, pointDeleteHelper);
    set_delete(map->consumedNugs, pointDeleteHelper);
    free(map->mapData);
    free(map);
  }
}

// #############################################################################
// Local Functions
// #############################################################################

/**************** expandRay ****************/
// Expands a ray along its x and y components and sets new values for its
// descendent diagonal ray.
static void expandRay(map_t* map, struct ray* r, set_t* visPoints) {
  int xexpand = 0;
  int yexpand = 0;
  int xinc = r->xcomp;
  int yinc = r->ycomp;
  int xval = r->xloc;
  int yval = r->yloc;
  
  // Expand in x-direction
  while ((map_isEmptySpot(map, xval, yval) || map_getChar(map, xval, yval) == '#')
          && xexpand < r->xexpand) {
    if (map_getChar(map, xval, yval) != '#') {
      addAdjacentWalls(map, visPoints, xval, yval);
    }
    xexpand++;
    point_t* p1 = point_new(xval, yval);
    char* p1S = point_toString(p1);
    if (set_insert(visPoints, p1S, p1) == false) {
      point_delete(p1);
    }
    free(p1S);
    xval += xinc;
  }

  // Reset x val and expand in y-direction
  xval = r->xloc;
  while ((map_isEmptySpot(map, xval, yval) || map_getChar(map, xval, yval) == '#')
         && yexpand < r->yexpand) {
    if (map_getChar(map, xval, yval) != '#') {
      addAdjacentWalls(map, visPoints, xval, yval);
    }
    yexpand++;
    point_t* p2 = point_new(xval, yval);
    char* p2S = point_toString(p2);
    if (set_insert(visPoints, p2S, p2) == false) {
      point_delete(p2);
    }
    free(p2S);
    yval += yinc;
  }
  yval = r->yloc;
  
  r->xloc = xval + xinc;
  r->yloc = yval + yinc;
  r->xexpand = xexpand;
  r->yexpand = yexpand;
  // If it can no longer expand in either direction, it is done.
  if (xexpand == 0 && yexpand == 0) {
    r->done = true;
  }

}

/**************** setHasPointWrapper ****************/
// Wrapper for the point_setHasPoint function that takes an x and y
static bool setHasPointWrapper(set_t* set, int x, int y) {
  point_t* p = point_new(x, y);
  bool val = point_setHasPoint(p, set);
  point_delete(p);
  return val;
}

/**************** addAdjacentWalls ****************/
// allows us to add walls that are adjacent to rays to visiblePoints
static void addAdjacentWalls(map_t* map, set_t* visPoints, int x, int y) {
  // Characters to add: -+|#

  // Check diagonal corners around the point for '+'
  for (int xinc = -1; xinc <= 1; xinc+=2) {
    for (int yinc = -1; yinc <= 1; yinc+=2) {
      if (map_getChar(map, x+xinc, y+yinc) == '+') {
        point_t* p = point_new(x+xinc, y+yinc);
        char* pS = point_toString(p);
        if (set_insert(visPoints, pS, p) == false) {
          point_delete(p);
        }
        free(pS);
      }
    }
  }

  // Check a "plus" area around the point for -|#
  for (int xinc = -1; xinc <= 1; xinc++) {
      if (isWallChar(map_getChar(map, x+xinc, y))){
        point_t* p = point_new(x+xinc, y);
        char* pS = point_toString(p);
        if (set_insert(visPoints, pS, p) == false) {
          point_delete(p);
        }
        free(pS);
      }
    }
  for (int yinc = -1; yinc <= 1; yinc++) {
    if (isWallChar((map_getChar(map, x, y+yinc)))) {
      point_t* p = point_new(x, y+yinc);
      char* pS = point_toString(p);
      if (set_insert(visPoints, pS, p) == false) {
        point_delete(p);
      }
      free(pS);
    }
  }
}

/**************** isWallChar ****************/
// checks if a character is a corner, vertical wall, horizontal wall, or passage
static bool isWallChar(char c) {
  if (c == '-' || c == '+' || c == '|' || c == '#') {
    return true;
  }
  return false;
}

/**************** pointDeleteHelper ****************/
// point_delete that takes a void* so that it can be used for set_delete
static void pointDeleteHelper(void *item) {
  if (item != NULL) {
    point_delete(item);
  }
}

/**************** unconsumedNugLocsHelper ****************/
// If the nugget passed in by set_iterate hasn't been consumed, it's added to
// the set.
static void unconsumedNugLocsHelper(void *nugLocHelper, const char *key, void *item) {
  struct nugLocHelper *nLH = (struct nugLocHelper*)nugLocHelper;
  if (point_setHasPoint(item, nLH->map->consumedNugs) == false) {
    point_t* p = point_new(point_getX(item), point_getY(item));
    if (set_insert(nLH->unconsumedNugs, key, p) == false) {
      point_delete(p);
    }
  }
}

/**************** countHelper ****************/
// Determines the number of items in a set
static void countHelper(void *arg, const char *key, void *item) {
  int* count = arg;
  (*count)++;
}
