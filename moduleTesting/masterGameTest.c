/* 
 * masterGameTest.c - Nuggets 'masterGame' module test program
 *
 * Dhaivat Mehta, Dartmouth College, 19S, COSC 050, Final Project - Nuggets
 */

#include "../libcs50/set.h"
#include "../lib/point.h"
#include "../lib/participant.h"
#include "../lib/map.h"
#include "../lib/masterGame.h"
#include "../support/file.h"
#include <string.h>

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

    printf("\nAdd 25 participants (24 players + 1 spectator) to a master game\n");
    masterGame_addPart(mg, "apple");
    masterGame_addPart(mg, "bat");
    masterGame_addPart(mg, "cat");
    masterGame_addPart(mg, "dog");
    masterGame_addPart(mg, "elephant");
    masterGame_addPart(mg, "fish");
    masterGame_addPart(mg, "gnat");
    masterGame_addPart(mg, "hat");
    masterGame_addPart(mg, "indian");
    masterGame_addPart(mg, "john");
    masterGame_addPart(mg, "kale");
    masterGame_addPart(mg, "lima");
    masterGame_addPart(mg, "money");
    masterGame_addPart(mg, "nap");
    masterGame_addPart(mg, "office");
    masterGame_addPart(mg, "phone");
    masterGame_addPart(mg, "quiet");
    masterGame_addPart(mg, "rat");
    masterGame_addPart(mg, "sound");
    masterGame_addPart(mg, "tv");
    masterGame_addPart(mg, "under");
    masterGame_addPart(mg, "vent");
    masterGame_addPart(mg, "water");
    masterGame_addPart(mg, "xylophone");
    masterGame_addPart(mg, NULL);
    
    printf("\nCheck player count after adding players (should be 24): %d\n", masterGame_getPlayerCount(mg) );

    printf("\nDisplay map for spectator:\n");
    char * map = masterGame_displayMap(mg, masterGame_getPart(mg, '$'));
    printf("%s\n", map);
    free(map);

    printf("\nMove participant with id 'b' so that it picks up a nugget: %d\n", masterGame_getPlayerCount(mg) );
    masterGame_movePartLoc(mg, 'b', 3, 0);
    masterGame_movePartLoc(mg, 'b', 1, 0);
    printf("\nDisplay map for spectator after moving 'b':\n");
    char * map2 = masterGame_displayMap(mg, masterGame_getPart(mg, '$'));
    printf("%s\n", map2);
    free(map2);
    
    printf("\nRemove participant currently with id 'b' from game: %d\n", masterGame_getPlayerCount(mg) );
    participant_t * partRemove = masterGame_getPart(mg, 'b');
    if(masterGame_removePart(mg, partRemove)){
      printf("Removed 'b'.\n");
    }
    printf("\nDisplay map after removing 'b':\n");
    char * map3 = masterGame_displayMap(mg, masterGame_getPart(mg, '$'));
    printf("%s\n", map3);
    free(map3);
    
    printf("\nAdd new participant to game with real name 'young'.\n");
    char newEntrantId = masterGame_addPart(mg, "young");
    printf("The new entrant's ID is (should be 'y'): %c\n", newEntrantId);
    printf("\nDisplay map after adding in new 'y':\n");
    char * map4 = masterGame_displayMap(mg, masterGame_getPart(mg, '$'));
    printf("%s\n", map4);
    free(map4);

    printf("\nAdd new participant to game with real name that is same as other real name in use ('apple')\n");
    char newEntrantId2 = masterGame_addPart(mg, "apple");
    printf("The new entrant's ID is (should be 'z'): %c\n", newEntrantId2);
    printf("\nDisplay map after adding in new 'z':\n");
    char * map5 = masterGame_displayMap(mg, masterGame_getPart(mg, '$'));
    printf("%s\n", map5);
    free(map5);

    printf("\nCurrent player count: %d\n", masterGame_getPlayerCount(mg) );
    printf("\nAdd new player to game beyond capacity\n");
    masterGame_addPart(mg, "rat");
    printf("\nDisplay map after attempting to add in new participant. (No participant should have been added):\n");
    char * map6 = masterGame_displayMap(mg, masterGame_getPart(mg, '$'));
    printf("%s\n", map6);
    free(map6);

    printf("\nAdd spectator to replace current spectator\n");
    masterGame_addPart(mg, NULL);
    printf("\nDisplay map from new spectator's viewpoint:\n");
    char * map7 = masterGame_displayMap(mg, masterGame_getPart(mg, '$'));
    printf("%s\n", map7);
    free(map7);
    
    printf("\nMove player 'c' to space occupied by player 'd':\n");
    masterGame_movePartLoc(mg, 'c', 0, -1);
    printf("\nDisplay map for spectator after 'c' and 'd' switch places:\n");
    char * map8 = masterGame_displayMap(mg, masterGame_getPart(mg, '$'));
    printf("%s\n", map8);
    free(map8);

    printf("\nMove player 'c' to top corner room:\n");
    masterGame_setPartLoc(mg, 'c', 4, 3);
    printf("\nDisplay map for player '%c':\n", participant_getId(masterGame_getPart(mg, 'c')));
    char * map9 = masterGame_displayMap(mg, masterGame_getPart(mg, 'c'));
    printf("%s\n", map9);
    free(map9);
    
    printf("\nMove player 'c' to middle room:\n");
    masterGame_setPartLoc(mg, 'c', 60, 15);
    printf("\nDisplay map for player '%c':\n", participant_getId(masterGame_getPart(mg, 'c')));
    char * map10 = masterGame_displayMap(mg, masterGame_getPart(mg, 'c'));
    printf("%s\n", map10);
    free(map10);

    printf("\nAttempt to move player 'c' to invalid space:\n");
    masterGame_movePartLoc(mg, 'c', 0, 1);
    printf("\nDisplay map for player '%c':\n", participant_getId(masterGame_getPart(mg, 'c')));
    char * map11 = masterGame_displayMap(mg, masterGame_getPart(mg, 'c'));
    printf("%s\n", map11);
    free(map11);

    printf("\nDisplay map for spectator after all moves:\n");
    char * map12 = masterGame_displayMap(mg, masterGame_getPart(mg, '$'));
    printf("%s\n", map12);
    free(map12);
        
    printf("\nDisplay endgame summary:\n");
    char * gameSum = masterGame_endGame(mg);
    printf("%s\n", gameSum);
    free(gameSum);
    
    masterGame_delete(mg);
  }
}