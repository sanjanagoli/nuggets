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

/* **************************************** */
int main() {

  char* path = "../maps/main.txt";
  int maxBytes = 65507;
  int goldTotal = 20;
  int minPiles = 5;
  int maxPiles = 10;
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
    }
    printf("\n");

    printf("\n");
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
