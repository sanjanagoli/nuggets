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

/*static void pointDeleteHelper(void *item);
static void participantPrint(FILE *fp, const char *key, void *item);
*/

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
    
    printf("\nCheck participant count after adding player (should be 6): %d\n", masterGame_getPlayerCount(mg) );

    masterGame_movePartLoc(mg, 'b', 3, 0);
    masterGame_movePartLoc(mg, 'b', 1, 0);
    
    printf("\nRemove participant currently with id 'b' from game: %d\n", masterGame_getPlayerCount(mg) );
    participant_t * partRemove = masterGame_getPart(mg, 'b');
    if(masterGame_removePart(mg, partRemove)){
      printf("Removed 'b'.\n");
    }
    
    printf("\nAdd new participant to game with real name 'rat': %d\n", masterGame_getPlayerCount(mg) );
    char newEntrantId = masterGame_addPart(mg, "rat");
    printf("The new entrant's ID is(should be 'b'): %c\n", newEntrantId);
    
    masterGame_setPartLoc(mg, 'c', 4, 3);
    participant_t * part = masterGame_getPart(mg, 'c');
    printf("\nDisplay map for paticipant '%c':\n", participant_getId(part));
    printf("%s\n", masterGame_displayMap(mg, part));
    masterGame_setPartLoc(mg, 'c', 60, 15);
    participant_t * part2 = masterGame_getPart(mg, 'c');
    printf("\nDisplay map for paticipant '%c':\n", participant_getId(part));
    printf("%s\n", masterGame_displayMap(mg, part2));
    printf("%s\n", masterGame_endGame(mg));
    
    masterGame_delete(mg);
  }
}

/*static void pointDeleteHelper(void *item) {
  if (item != NULL) {
    point_delete(item);
  }
}

static void participantPrint(FILE *fp, const char *key, void *item)
{
  printf("key=%c item=%s", *key, participant_getRealName(item));
}*/