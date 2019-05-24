/*
 * participant.c - test program for the nuggets participant module
 *
 * CS50, April 2019
 */

#include "../lib/point.h"
#include "../lib/map.h"
#include "../lib/participant.h"
#include "../support/file.h"
#include <string.h>

int main() {
    char* path = "../maps/hole.txt";
    int maxBytes = 65507;
    int goldTotal = 20;
    int minPiles = 8;
    int maxPiles = 15;
    int seed = 5;

    map_t* map = map_new(path, maxBytes, goldTotal, minPiles, maxPiles, seed);
    point_t* loc = point_new(4, 3);
    printf("calling participant_new...\n");
    printf("Initializing new participant with location (4,3), map: hole.txt, char id: 'a', realPlayerName: 'Player1'\n");
    participant_t *part = participant_new(loc, map, 'a', true, "Player1");

    if (part == NULL) {
        printf("\nCould not create map\n");
    } else {
        //Testing getters
        
        printf("participant_getRealName: Participant's realName: %s\n", participant_getRealName(part));
        printf("participant_getId: Participant's id: %c\n", participant_getId(part));
        printf("participant_getType: Participant's type (1 if player, 0 if spectator) %d\n", participant_getType(part));

        printf("\n participant_getLoc: Participant's current location: (%d, %d)\n", point_getX(participant_getLoc(part)), point_getY(participant_getLoc(part)));
        point_t *point2 = point_new(3, 1);
        participant_setLoc(part, point2);
        printf("participant_setLoc: Participant's new current location: (%d, %d)\n", point_getX(participant_getLoc(part)), point_getY(participant_getLoc(part)));
        printf("old participant location was freed...\n");

        printf("\nparticipant_getPurse: Participant's initial purse value: %d\n", participant_getPurse(part));
        participant_setPurse(part, 3);
        printf("participant_setPurse: Participant's purse value: %d\n", participant_getPurse(part));
        participant_incrementPurse(part, 2);
        printf("participant_incrementPurse: Participant's purse value: %d\n", participant_getPurse(part));

        printf("participant_print: Set of participant's visible points (and other information): \n\n");
        participant_print(part, stdout);
        
        point_t* point3 = point_new(6, 2);
        printf("point_isVisible: Check if (6,2) is visible to participant from current location (should be true/1): %d\n", participant_isVisible(part, point3));
        point_setX(point3, 13);
        printf("point_isVisible: Check if (13,2) is visible to participant from current location (should be true/1): %d\n", participant_isVisible(part, point3));

        point_t* point4 = point_new(16, 9);
        printf("point_isVisible: Check if (16,9) is visible to participant from current location (should be false/0): %d\n", participant_isVisible(part, point4));

        point_delete(point3);
        point_delete(point4);
        map_delete(map);
        participant_delete(part);
    }
}
