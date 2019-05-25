/*
 * mapTest.c - test program for the nuggets map module
 *
 * CS50, April 2019
 */

#include "../libcs50/set.h"
#include "../lib/point.h"
#include "../lib/participant.h"
#include "../lib/map.h"
#include "../lib/masterGame.h"
#include "../support/file.h"
#include <string.h>

void pointPrintHelper(FILE *fp, const char *key, void *item);
static void pointDeleteHelper(void *item);
static void nuggetsConsumeHelper(void *arg, const char *key, void *item);
static void participantPrint(FILE *fp, const char *key, void *item);


/* **************************************** */
int main() {

  char* path = "../maps/hole.txt";
  int seed = 45;

  printf("\nCreating master game with:\n");
  printf("\tMap data from: %s\n", path);
  printf("\tseed: %i\n", seed);
  masterGame_t * mg = masterGame_new(path, seed);

  if (mg == NULL) {
    printf("\nCould not create master game\n");
  } 
  else {
    printf("\nPrint map data of generated map held in master game:\n");
    char* returnedData = map_getMapData(masterGame_getMap(mg));
    if (returnedData == NULL) {
      printf("\nfailed\n");
    } 
    else {
      fprintf(stdout, "%s\n", returnedData);
      free(returnedData);
    }

    printf("\nAdd a participant to a master game:\n");
    masterGame_addPart(mg, "dhaivat");
    masterGame_addPart(mg, "dog");
    masterGame_addPart(mg, "cat");
    masterGame_addPart(mg, "mouse");
    masterGame_addPart(mg, "turtle");
    masterGame_addPart(mg, "death");
    masterGame_addPart(mg, NULL);
    printf("\nCheck participant count after adding player (should be 1): %d\n", masterGame_getPlayerCount(mg) );
    masterGame_movePartLoc(mg, 'b', 2, 0);
    masterGame_movePartLoc(mg, 'b', 2, 0);
    participant_t * part = masterGame_getPart(mg, '@');

    printf("\nDisplay map for paticipant '%c':\n", participant_getId(part));
    printf("%s\n", masterGame_displayMap(mg, part));
  }
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

static void participantPrint(FILE *fp, const char *key, void *item)
{
  printf("key=%c item=%s", *key, participant_getRealName(item));
}