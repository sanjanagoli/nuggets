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
    participant_t *part = participant_new(loc, map, 'a', true, "Player1");

    if (part == NULL) {
        printf("\nCould not create map\n");
    } else {
        //Testing getters

        printf("Participant's realName: %s\n", participant_getRealName(part));
        printf("Participant's id: %c\n", participant_getId(part));
        printf("Participan's type (1 if player, 0 if spectator) %d\n", participant_getType(part));

        printf("\nParticipant's current location: (%d, %d)\n", point_getX(participant_getLoc(part)), point_getY(participant_getLoc(part)));
        point_t *point2 = point_new(3, 1);
        participant_setLoc(part, point2);
        printf("Participant's new current location (after participant_setLoc): (%d, %d)\n", point_getX(participant_getLoc(part)), point_getY(participant_getLoc(part)));


        printf("\nParticipant's initial purse value: %d\n", participant_getPurse(part));
        participant_setPurse(part, 3);
        printf("Participant's purse value after setLoc: %d\n", participant_getPurse(part));
        participant_incrementPurse(part, 2);
        printf("Participant's purse value after incrementPurse: %d\n", participant_getPurse(part));

        printf("Set of participant's visible points (and other information): \n\n");
        participant_print(part, stdout);
        
        point_t* point3 = point_new(6, 2);
        printf("Check if (6,2) is visible to participant from current location (should be true/1): %d\n", participant_isVisible(part, point3));
        point_setX(point3, 13);
        printf("Check if (13,2) is visible to participant from current location (should be true/1): %d\n", participant_isVisible(part, point3));

        point_t* point4 = point_new(16, 9);
        printf("Check if (16,9) is visible to participant from current location (should be false/0): %d\n", participant_isVisible(part, point4));


        map_delete(map);
        point_delete(loc);
        point_delete(point3);
        point_delete(point4);
        participant_delete(part);
    }
}
