/* 
 * participant.c - 'participant' module for nuggets final project
 *
 * see participant.c for more information.
 * 
 * Sanjana Goli, May 2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../libcs50/set.h"
#include "map.h"
#include "point.h"
#include "../support/message.h"
#include "participant.h"

void point_checker(void *arg, const char *key, void *item);
void print_helper(FILE *fp, const char *key, void *item);
void delete_helper(void *item);
typedef struct pointBool pointBool_t;

/**************** typedef struct participant participant_t ****************/
/* see participant.h for description */
typedef struct participant {
    point_t *location;
    map_t *map;
    char id;
    bool player;
    set_t *visiblePoints;
    int purse;
    char * playerRealName;
    addr_t address;
} participant_t;

/* Helper struct to iterate through list of points visible to participant */
typedef struct pointBool {
    point_t *p;
    bool exists;
} pointBool_t;


/**************** participant_new() ****************/
/* see participant.h for description */
participant_t* participant_new(point_t* p, map_t* map, char id, bool player, char * playerRealName, addr_t address)
{
    participant_t *participant = malloc(sizeof(participant_t));
    if ((p != NULL) && (map != NULL)) {
        participant->location = p;
        participant->map = map;
        participant->id = id;
        participant->player = player;
        participant->purse = 0;
        participant->visiblePoints = map_getVisibility(map, point_getX(p), point_getY(p));
        participant->playerRealName = playerRealName;
        participant->address = address;
        return participant;
    } else {
        free(participant);
        return NULL;
    }    
}

/**************** participant_getAddress() ****************/
/* see participant.h for description */
addr_t* participant_getAddress(participant_t* part)
{
    if (part != NULL) {
        return &(part->address);
    } else {
        return NULL;
    }
    
}


/**************** participant_getRealName() ****************/
/* see participant.h for description */
char* participant_getRealName(participant_t* part)
{
    if (part != NULL) {
        return part->playerRealName;
    } else {
        return NULL;
    }
}

/**************** participant_getVisiblePoints() ****************/
/* see participant.h for description */
set_t* participant_getVisiblePoints(participant_t* part)
{
    if (part != NULL) {
        return part->visiblePoints;
    } else {
        return NULL;
    }
}

/**************** participant_getId() ****************/
/* see participant.h for description */
char participant_getId(participant_t* part)
{
    if (part != NULL) {
        return part->id;
    } else {
        return '\0';
    }
}

/**************** participant_getType() ****************/
/* see participant.h for description */
bool participant_getType(participant_t* part)
{
    if (part != NULL) {
        return part->player;
    } else {
        return false;
    }
}

/**************** participant_getLoc() ****************/
/* see participant.h for description */
point_t* participant_getLoc(participant_t* part)
{
    if (part != NULL) {
        return part->location;
    } else {
        return NULL;
    }
}

/**************** participant_setLoc() ****************/
/* see participant.h for description */
bool participant_setLoc(participant_t* part, point_t* p)
{
    if (part != NULL) {
        part->location = p;
        return true;
    } else {
        return false;
    }
}

/**************** participant_getPurse() ****************/
/* see participant.h for description */
int participant_getPurse(participant_t* part) 
{
    if (part != NULL) {
        return part->purse;
    } else {
       return -1; 
    }
}

/**************** participant_setPurse() ****************/
/* see participant.h for description */
void participant_setPurse(participant_t* part, int value)
{
    if (part != NULL && value >= 0) {
        part->purse = value;
    }
}

/**************** participant_incrementPurse() ****************/
/* see participant.h for description */
int participant_incrementPurse(participant_t* part, int value)
{
    if (part != NULL && value >= 0) {
        part->purse = part->purse + value;
    } 
    return part->purse;
}

/**************** participant_isVisible() ****************/
/* see participant.h for description */
bool participant_isVisible(participant_t* part, point_t* p)
{
    pointBool_t* pointBool = malloc(sizeof(pointBool_t));
    pointBool->p = p;
    pointBool->exists = false; 
    set_t *visibleSet = part->visiblePoints;
    set_iterate(visibleSet, pointBool, point_checker);
    bool exist = pointBool->exists;
    free(pointBool);
    return exist;
}

/*  Input: arg corresponds to pointBool struct, and item corresponds to point in the set
    Functionality: helper function to iterate through set of points visible to participant */
void point_checker(void *arg, const char *key, void *item)
{
    pointBool_t *pointBool = arg;
    point_t* point = item;
    if (item != NULL) {
        if (point_getX(pointBool->p) == point_getX(point) && point_getY(pointBool->p) == point_getY(point)) {
            pointBool->exists = true;
        } 
    }
}

/**************** participant_delete() ****************/
/* see participant.h for description */
void participant_delete(participant_t* part)
{
    if (part != NULL) {
        point_delete(part->location);
        set_delete(part->visiblePoints, delete_helper);
        free(part);
    }
}

/**************** participant_print() ****************/
/* see participant.h for description */
void participant_print(participant_t* part, FILE* fp)
{
    if (part != NULL) {
        fprintf(fp, "Participant %c: location (%d, %d), isPlayer (%d), purse (%d) \n", part->id, point_getX(part->location), point_getY(part->location), part->player, part->purse);
        fprintf(fp, "Set of Participant %c visible points\n", part->id);
        set_print(part->visiblePoints, fp, print_helper);
        fprintf(fp, "\n------------------------------- \n");
    }
}

/*  Input: takes in void item (that corresponds to point in list), key (corresponding to key in
    visible points set), and FILE* for clean logging
    helper function to print set of points visible to participant */
void print_helper(FILE *fp, const char *key, void *item)
{
    if (item != NULL) {
        point_t *p = item;
        point_print(p, fp);
        fprintf(fp, ", ");
    }
   
}

/*  Input: takes in void item (that corresponds to point in list)
    Functionality: helper function to free memory of points is set of points visible to participant */
void delete_helper(void* item)
{
    if (item != NULL) {
        point_t *p = item;
        point_delete(p);
    }
}
