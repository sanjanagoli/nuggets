/*
 * mapTest.c - test program for the nuggets map module
 *
 * CS50, April 2019
 */

#include "../lib/point.h"
#include "../lib/map.h"
#include "../support/file.h"
#include <string.h>

void pointPrintHelper(FILE *fp, const char *key, void *item);
static void pointDeleteHelper(void *item);
static void nuggetsConsumeHelper(void *arg, const char *key, void *item);

/* **************************************** */
int main() {

  char* path = "../maps/hole.txt";
  int maxBytes = 65507;
  int goldTotal = 20;
  int minPiles = 8;
  int maxPiles = 15;
  int seed = 5;

  printf("\nCreating map with:\n");
  printf("\tMap data from: %s\n", path);
  printf("\tmaxBytes: %i\n", maxBytes);
  printf("\tgoldTotal: %i\n", goldTotal);
  printf("\tminPiles: %i\n", minPiles);
  printf("\tmaxPiles: %i\n", maxPiles);
  printf("\tseed: %i\n", seed);
  map_t* map = map_new(path, maxBytes, goldTotal, minPiles, maxPiles, seed);

  if (map == NULL) {
    printf("\nCould not create map\n");
  } else {

    printf("\nPrint mapData of generated map via map_getMapData:\n");
    char* returnedData = map_getMapData(map);
    if (returnedData == NULL) {
      printf("\nrip\n");
    } else {
      fprintf(stdout, "%s\n", returnedData);
      free(returnedData);
    }

    printf("\nRows of map obtained via map_getRows:\n");
    int rows = map_getRows(map);
    printf("%i\n", rows);

    printf("\nColumns of map obtained via map_getCols:\n");
    int cols = map_getCols(map);
    printf("%i\n", cols);

    printf("\nUse map_getChar to reconstruct the map:\n");
    for (int y = 0; y < rows; y++) {
      for (int x = 0; x < cols; x++) {
        char c = map_getChar(map, x, y);
        printf("%c", c);
      }
      printf("\n");
    }
    
    printf("\nTest of map_getEmptySpots:\n");
    set_t* emptySpots = map_getEmptySpots(map);
    for (int y = 0; y < rows; y++) {
      for (int x = 0; x < cols; x++) {
        point_t* p = point_new(x,y);
        if (point_setHasPoint(p, emptySpots)) {
          printf(".");
        } else {
          printf(" ");
        }
        point_delete(p);
      }
      printf("\n");
    }
    set_delete(emptySpots, pointDeleteHelper);
    
    printf("\nGold remaining: %i\n", map_nugsRemaining(map));
    printf("\nPiles remaining: %i\n", map_pilesRemaining(map));
    
    set_t* pileSet = map_getUnconsumedNugLocs(map);
    printf("\nPrinting Pile Locations:\n");
    set_print(pileSet, stdout, pointPrintHelper);
    printf("\n");
    
    printf("\nMap with piles (via map_getUnconsumedNugLocs):\n");
    for (int y = 0; y < rows; y++) {
      for (int x = 0; x < cols; x++) {
        point_t* p = point_new(x,y);
        if (point_setHasPoint(p, pileSet)) {
          printf("*");
        } else {
          char c = map_getChar(map, x, y);
          printf("%c", c);
        }
        point_delete(p);
      }
      printf("\n");
    }
    set_delete(pileSet, pointDeleteHelper);
        
    printf("\nVisibility Function (1: partial wall):\n");
    int xLoc = 8;
    int yLoc = 15;
    set_t* visibleSet = map_getVisibility(map, xLoc, yLoc);
    for (int y = 0; y < rows; y++) {
      for (int x = 0; x < cols; x++) {
        point_t* p = point_new(x,y);
        if (x == xLoc && y == yLoc) {
          printf("X");
        } 
        else if (point_setHasPoint(p, visibleSet)) {
          char c = map_getChar(map, x, y);
          printf("%c", c);
        } else {
          printf(" ");
        }
        point_delete(p);
      }
      printf("\n");
    }
    set_delete(visibleSet, pointDeleteHelper);
    
    printf("\nVisibility Function (2: around a corner):\n");
    xLoc = 12;
    yLoc = 13;
    visibleSet = map_getVisibility(map, xLoc, yLoc);
    for (int y = 0; y < rows; y++) {
      for (int x = 0; x < cols; x++) {
        point_t* p = point_new(x,y);
        if (x == xLoc && y == yLoc) {
          printf("X");
        } 
        else if (point_setHasPoint(p, visibleSet)) {
          char c = map_getChar(map, x, y);
          printf("%c", c);
        } else {
          printf(" ");
        }
        point_delete(p);
      }
      printf("\n");
    }
    set_delete(visibleSet, pointDeleteHelper);
    
    printf("\nVisibility Function (3: entire room and seeing into other room through tunnel):\n");
    xLoc = 62;
    yLoc = 13;
    visibleSet = map_getVisibility(map, xLoc, yLoc);
    for (int y = 0; y < rows; y++) {
      for (int x = 0; x < cols; x++) {
        point_t* p = point_new(x,y);
        if (x == xLoc && y == yLoc) {
          printf("X");
        } 
        else if (point_setHasPoint(p, visibleSet)) {
          char c = map_getChar(map, x, y);
          printf("%c", c);
        } else {
          printf(" ");
        }
        point_delete(p);
      }
      printf("\n");
    }
    set_delete(visibleSet, pointDeleteHelper);
    
    printf("\nVisibility Function (4: in a tunnel):\n");
    xLoc = 53;
    yLoc = 13;
    visibleSet = map_getVisibility(map, xLoc, yLoc);
    for (int y = 0; y < rows; y++) {
      for (int x = 0; x < cols; x++) {
        point_t* p = point_new(x,y);
        if (x == xLoc && y == yLoc) {
          printf("X");
        } 
        else if (point_setHasPoint(p, visibleSet)) {
          char c = map_getChar(map, x, y);
          printf("%c", c);
        } else {
          printf(" ");
        }
        point_delete(p);
      }
      printf("\n");
    }
    set_delete(visibleSet, pointDeleteHelper);
    
    printf("\nConsume each pile:\n");
    printf("\tNuggets remaining: %i\n", map_nugsRemaining(map));
    printf("\tPiles remaining: %i\n", map_pilesRemaining(map));
    set_t* nuggets = map_getNugLocs(map);
    set_iterate(nuggets, map, nuggetsConsumeHelper);
    
    printf("\nMap with piles (via map_getUnconsumedNugLocs; should be empty):\n");
    pileSet = map_getUnconsumedNugLocs(map);
    for (int y = 0; y < rows; y++) {
      for (int x = 0; x < cols; x++) {
        point_t* p = point_new(x,y);
        if (point_setHasPoint(p, pileSet)) {
          printf("*");
        } else {
          char c = map_getChar(map, x, y);
          printf("%c", c);
        }
        point_delete(p);
      }
      printf("\n");
    }
    set_delete(pileSet, pointDeleteHelper);
    
    printf("\nTesting map_getConsumedNugLocs:\n");
    set_t* consumedSet = map_getConsumedNugLocs(map);
    for (int y = 0; y < rows; y++) {
      for (int x = 0; x < cols; x++) {
        point_t* p = point_new(x, y);
        if (point_setHasPoint(p, consumedSet)) {
          printf("0");
        } else {
          char c = map_getChar(map, x, y);
          printf("%c", c);
        }
        point_delete(p);
      }
      printf("\n");
    }
    
    printf("\nDeleting map.\n\n");
    map_delete(map);
  }
  return 0;
}

void pointPrintHelper(FILE *fp, const char *key, void *item)
{
  point_t* p = item;
  if (item == NULL) {
    fprintf(fp, "(null)");
  }
  else {
    point_print(p, stdout);
  }
}

static void pointDeleteHelper(void *item) {
  if (item != NULL) {
    point_delete(item);
  }
}

static void nuggetsConsumeHelper(void *arg, const char *key, void *item) {
  map_t* map = arg;
  int value = map_consumeNug(map, point_getX(item), point_getY(item));
  printf("Consumed pile located at: (%i, %i)\n", point_getX(item), point_getY(item));
  printf("\tValue of pile: %i\n", value);
  printf("\tPiles remaining: %i\n", map_pilesRemaining(map));
  printf("\tNuggets remaining: %i\n", map_nugsRemaining(map));
}
