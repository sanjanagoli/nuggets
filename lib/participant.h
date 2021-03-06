/* 
 * participant.h - 'participant' module for nuggets final project
 *
 * see participant.h for more information.
 * 
 * Sanjana Goli, May 2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/set.h"
#include "map.h"
#include "point.h"

/*  Participant structure holds location, map, id, player type, visiblePoints, and purse. */ 
typedef struct participant participant_t; 

/*  
    Input: takes in list of data encapsulated by participant struct
    Functionality: Creates/allocates memory new participant that contains initialize data
    contained in struct.
    Output: returns pointer to participant

    Caller is responsible for calling participant_delete in order to free allocated memory
*/
participant_t* participant_new(point_t* p, map_t* map, char id, bool player, char * playerRealName);

/*
*   Input: takes in pointer to participant
*   Output: returns the name of participant if type gamePlayer, else returns NULL
*   
*   Caller is not responsible for freeing playerRealName - it is handled by participant_delete
*/
char* participant_getRealName(participant_t* part);

/*
*   Input: takes in pointer to participant
*   Output: returns the pointer to set of the points that are visible to participant
*
*   Caller is not responsible for freeing set - it is handled by participant_delete   
*/
set_t* participant_getVisiblePoints(participant_t* part);

/*  Input: pointer to participant
    Output: returns true if the participant is a gamePlayer and false if spectator
*/
bool participant_getType(participant_t* part);

/*  Input: pointer to participant (part)
    Output: char corresponding to its id in the game (ex: 'B'); if the participant is NULL,
            return null character ('\0')
*/
char participant_getId(participant_t* part);

/*  Input: pointer to participant (part)
*   Output: the point to the current location of the participant 
*   
*   Caller is not responsible for for freeing the participant's current location -- handled by participant_delete
*/
point_t* participant_getLoc(participant_t* part);

/*  Input: pointer to participant (part) and pointer to point (p)
*   Returns true if the participant location was changed (if part is not null), else returns false 
*   
*   Caller is not responsible for freeing the previous location (handled by this method)
*/
bool participant_setLoc(participant_t* part, point_t* p);

/*  Input: pointer to participant (part)
    Output: integer value of how many nuggets are in specified participant's purse 
*/
int participant_getPurse(participant_t* part);

/*  Input: pointer to participant (part) and value to set purse value
    Functionality: Allows caller to set the num of nuggets (value) in purse for part  
*/
void participant_setPurse(participant_t* part, int value);

/*  Input: pointer to participant (part) and value to increment purse data
    Functionality: Allows caller to increment purse by specified values
    Output: returns new purse value of participant 
*/
int participant_incrementPurse(participant_t* part, int value);

/*  Input: pointer to participant (part) and point
    Functionality: Iterates through participant's list of visible points and determines if input point is
    in that list.    
    Output: returns true if point given is in participant's list of visible points
*/
bool participant_isVisible(participant_t* part, point_t* p);

/*  Input: pointer to participant (part)
    Functionality: Frees the memory allocated for participant from participant_new
 */
void participant_delete(participant_t* part);

/*  Input: pointer to participant (part) and file pointer
    Functionality/output: Prints the data encapsulated by participant to file 
*/
void participant_print(participant_t *part, FILE *fp);

/*  Input: pointer to participant (part) and set of points that are visible to said participant (visiblePoints)
    Functionality/output: frees the current visible point set in participant and replaces it with given visible points set
*/
void participant_setVisibility(participant_t * part, set_t * visiblePoints);

