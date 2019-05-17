/* 
 * participant.h - 'participant' module for nuggets final project
 *
 * see participant.h for more information.
 * 
 * Sanjana Goli, May 2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*  Participant structure holds location, map, id, player type, visiblePoints, and purse. */ 
typedef struct participant participant_t; 

/*  Creates/allocates memory for and returns new participant that contains initialize data
    contained in struct.
*/
participant_t* participant_new(point_t* p, map_t* map, char id, bool player);

/*  Takes in participant pointer and returns true if the participant is a gamePlayer and
    false if spectator
*/
bool participant_getType(participant_t* part);

/*  Returns the point fo the current location of the participant */
point_t* participant_getLoc(participant_t* part);

/*  Returns true if the participant location was changed (if part is not null) */
bool participant_setLoc(participant_t* part, point_t* p);

/*  Returns integer value of how many nuggets are in specified participant's purse */
int participant_getPurse(participant_t* part);

/*  Allows caller to set the num of nuggets (value) in purse for part  */
void participant_setPurse(participant_t* part, int value);

/*  Allows caller to increment purse by specified values; returns new purse value of participant */
int participant_incrementPurse(participant_t* part, int value);

/*  Iterates through participant's list of visible points and determines if input point is
    in that list.    
*/
bool participant_isVisible(participant_t* part, point_t* p);

/*  Frees the memory allocated for participant from participant_new */
void participant_delete(participant_t* part);

/*  Prints the data encapsulated by participant to file */
void participant_print(participant_t *part, FILE *fp);
