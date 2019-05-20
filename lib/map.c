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
#include "../support/file.h"
#include "../support/set.h"
#include "point.h"

// #############################################################################
// Global Types
// #############################################################################

typedef struct map {
  const char* mapData;
  int nrows;
  int ncols;
  set_t* nuggetLocs;
  set_t* consumedNugs;
  int nugsRemaining;
  int pilesRemaining;
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

static struct ray expandRay(struct ray* r, int* count, set_t* visPoints);
static bool setHasPointWrapper(set_t* set, int x, int y);
static void addAdjacentWalls(map_t* map, int* count, set_t* visPoints, int x, int y);
static bool isWallChar(char c);
static void pointDeleteHelper(void *item);

// #############################################################################
// Global Functions
// See map.h for comments about exported function
// #############################################################################

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

/**************** map_getMapData ****************/
/* see map.h for description */
char* map_getMapData(map_t* map) {
  if (map == NULL) {
    return NULL;
  }
  return map->mapData;
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
/* see map.h for description */ // Do this
set_t* map_getUnconsumedNugLocs(map_t* map) {
  if (map == NULL || map->nuggetLocs == NULL || map->consumedNugs == NULL) {
    return NULL;
  }
  set_t* unconsumedNugs = set_new();
  struct nugLocHelper nLH = {map, unconsumedNugs};
  set_iterate(map->nuggetLocs, &nLH, unconsumedNugLocsHelper);
  return unconsumedNugs;
}

/**************** map_consumeNug ****************/
/* see map.h for description */
int map_consumeNug(map_t* map, int x, int y) {
  if (map != NULL && map_nuggetPresent(map, x, y)) {
    char consumedKey[12]; // Make a key for the point
    char* consumedKeyPointer = consumedKey;
    sprintf(consumedKey, "%i,%i", x, y);

    point_t* p = point_new(x, y); // Make the point
    set_insert(map->consumedNugs, p);
    point_delete(p);

    pilesRemaining--;
    if (pilesRemaining == 1) {
      int valueToReturn = map->nugsRemaining;
      map->nugsRemaining = 0;
    } else {
      int extraNugs = map->pilesRemaining - map->nugsRemaining;
      int valueToReturn = (rand() % (extraNugs) + 1);
      map->nugsRemaining -= valueToReturn;
    }
    return valueToReturn;
  }
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
  bool nuggetPresent = setHasPointWrapper(map->nuggetLocs, x, y);
  bool nuggetConsumed = setHasPointWrapper(map->nuggetLocs, x, y);
  if (nuggetPresent && !nuggetConsumed) {
    return true;
  }
  return false;
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

/**************** map_pilesRemaining ****************/
/* see map.h for description */
int map_pilesRemaining(map_t* map) {
  if (map == NULL) {
    return 0;
  }
  return map->pilesRemaining;
}

/**************** map_genNugs ****************/ // Do seed thing
/* see map.h for description */
static void map_genNugs(map_t* map, int minPiles, int maxPiles) {
  if (map != NULL) {
    int pilesToMake = (rand() % (maxPiles - minPiles + 1)) + minPiles;
    int pilesRemaining = pilesToMake;
    char pilesString[4]; // We will be using this as a key so we need a string
    char* pilesPointer = pilesString;

    set_t* nuggetLocs = new_set();
    if (nuggetLocs != NULL  consumedNugs != NULL) {
      set_t* consumedNugs = new_set();
      if (consumedNugs != NULL) {
        map->nuggetLocs = nuggetLocs;
        map->consumedNugs = consumedNugs;

        while (numPiles > 0) {
          randomX = rand() % map->ncols;
          randomY = rand() % map->nrows;
          if (map_isEmptySpot(map, randomX, randomY) &&
              !map_nuggetPresent(map, randomX, randomY)) {

                sprintf(pilesString, "%i", pilesToMake);
                point_t* p = new_point(randomX, randomY);
                set_insert(nuggetLocs, pilesPointer, p);
                numPiles--;
          }
        }
      }
    }
  }
}

/**************** map_getVisibility ****************/ // Do this
/* see map.h for description */
set_t* map_getVisibility(map_t* map, int x, int y) {
  // Create set for visible points and int for count
  set_t* visiblePoints = set_new();
  point_t* startPoint = point_new(x, y);
  set_insert(visiblePoints, "0", startPoint);
  int count = 0;
  int* countP = &count;

  // There are only 4 diagonal rays at a given time
  struct ray* rayArray[4];
  // Make each of the starting diagonal rays
  struct ray ray1 = {x, y, 1, 1, int_max, int_max, false};
  struct ray* ray1P = &ray1;
  rayArray[0] = ray1P;
  struct ray ray2 = {x, y, 1, -1, int_max, int_max, false};
  struct ray* ray2P = &ray2;
  rayArray[1] = ray2P;
  struct ray ray3 = {x, y, -1, -1, int_max, int_max, false};
  struct ray* ray3P = &ray3;
  rayArray[2] = ray3P;
  struct ray ray4 = {x, y, -1, 1, int_max, int_max, false};
  struct ray* ray4P = &ray4;
  rayArray[3] = ray4P;

  int raysDone = 0;
  while (raysDone != 4) {
    raysDone = 0;
    for (int i = 0; i < 4; i++) {
      if (!rayArray[i].done) {
        struct ray* newRay = expandRay(map, rayArray[i], countP, visiblePoints);
        rayArray[i] = newRay;
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
  if (map == NULL) {
    return NULL;
  }
  FILE* mapData;
  mapData = fopen(map->mapData, "r");
  if (mapData == NULL) {
    return NULL;
  }
  // Used to generate keys for set, so we need strings
  int count = 0;
  char countString[4];
  char* countPointer = countString;

  int x = 0;
  int y = 0;
  set_t* visiblePoints = set_new();

  char* line;
  while ((line = freadlinep(mapData)) != NULL) {
    x=0;
    while (*line) {
      if (map_isEmptySpot(map, x, y)) {
        point_t* p = new_point(x, y);
        sprintf(countString, "%i", count);
        set_insert(visiblePoints, countPointer, p);
        count++;
      }
      line++;
      x++;
    }
    free(line);
    y++;
  }
  return visiblePoints;
}

/**************** map_delete ****************/
/* see map.h for description */
void map_delete(map_t* map) {
  set_delete(nuggetLocs, pointDeleteHelper);
  set_delete(consumedNugs, pointDeleteHelper);
  free(mapData);
  free(map);
}

// #############################################################################
// Local Functions
// #############################################################################

/**************** expandRay ****************/
// Expands a ray along its x and y components,
static struct ray* expandRay(map_t* map, struct ray* r, int* count, set_t* visPoints) {
  char countString[5];
  char* countPointer = countString;

  int xexpand = 0;
  int yexpand = 0;
  int xinc = r->xcomp;
  int yinc = r->ycomp;
  int xval = r->xloc + xinc;
  int yval = r->yloc + yinc;

  // Make the ray
  struct ray returnRay = {xval, yval, xinc, yinc, 0, 0, false};
  struct ray* returnRayP = &returnRay;

  // Expand in x-direction
  while (map_isEmptySpot(map, xval, yval)) {
    if (map_isEmptySpot(map, xval, yval) && !setHasPointWrapper(visPoints, xval, yval)) {
      addAdjacentWalls(map, count, visPoints, xval, yval);
      xexpand++;
      count++;
      sprintf(countString, "%i", *count);
      point_t* p1 = point_new(xval, yval);
      set_insert(visPoints, countPointer, p2);
    }
    xval += xinc;
  }

  // Expand in y-direction
  while (map_isEmptySpot(map, xval, yval)) {
    if (map_isEmptySpot(map, xval, yval) && !setHasPointWrapper(visPoints, xval, yval)) {
      addAdjacentWalls(map, visPoints, xval, yval);
      yexpand++;
      count++;
      sprintf(countString, "%i", *count);
      point_t* p2 = point_new(xval, yval);
      set_insert(visPoints, countPointer, p2);
    }
    yval += yinc;
  }

  returnRay.xexpand = xexpand;
  returnRay.yexpand = yexpand;
  // If it can no longer expand in either direction, it is done.
  if (returnRay.xexpand == 0 && returnRay.yexpand == 0) {
    returnRay.done = true;
  }
  return returnRayP;

}

/**************** setHasPointWrapper ****************/
// wrapper for the point_setHasPoint function that takes an x and y
static bool setHasPointWrapper(set_t* set, int x, int y) {
  point_t* p = point_new(x, y);
  bool val = point_setHasPoint(p, set);
  point_delete(p);
  return val;
}

/**************** addAdjacentWalls ****************/
// allows us to add walls that are adjacent to rays to visiblePoints
static void addAdjacentWalls(map_t* map, int* count, set_t* visPoints, int x, int y) {
  // Characters to add: -+|#
  char countString[4];
  char* countPointer = countString;

  // Check diagonal corners around the point for '+'
  for (int xinc = -1; xinc <= 1; xinc+=2) {
    for (int yinc = -1; yinc <= 1; yinc+=2) {
      if (map_getChar(map, x+xinc, y+yinc) == '+') &&
          setHasPointWrapper(visPoints, x+xinc, y+yinc)) {
        count++;
        sprintf(countString, "%i", *count);
        point_t* p = point_new(x+xinc, y+yinc);
        set_insert(visPoints, countPointer, p);
      }
    }
  }

  // Check a "plus" area around the point for -|#
  for (int xinc = -1; xinc <= 1; xinc++) {
      if (isWallChar((map_getChar(map, x+xinc, y))) &&
          setHasPointWrapper(visPoints, x+xinc, y)) {
        count++;
        sprintf(countString, "%i", *count);
        point_t* p = point_new(x+xinc, y);
        set_insert(visPoints, countPointer, p);
      }
    }
  for (int yinc = -1; yinc <= 1; yinc++) {
    if (isWallChar((map_getChar(map, x, y+yinc))) &&
        setHasPointWrapper(visPoints, x, y+yinc)) {
      count++;
      sprintf(countString, "%i", *count);
      point_t* p = point_new(x, y+yinc);
      set_insert(visPoints, countPointer, p);
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
  if (point_setHasPoint(nLH->map->consumedNugs, item) == false) {
    set_insert(nugLocHelper->unconsumedNugs, key, item);
  }
}
