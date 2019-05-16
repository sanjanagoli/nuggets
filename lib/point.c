/* 
 * point.c - 'point' module for nuggets final project
 *
 * see point.h for more information.
 * 
 * Sanjana Goli, May 2019
 */

#include <stdio.h>
#include <stdbool.h>
#include "point.h"

typedef struct point {
    int x;
    int y;
} point_t;

/**************** point_new() ****************/
/* see point.h for description */
point_t* point_new(int x, int y)
{
    point_t *point = count_malloc(sizeof(point_t));

    if (point == NULL) {
        return NULL; // error allocating point
    } else {
        // initialize contents of point structure
        point->x = x;
        point->y = y;
    }
}

/**************** point_getX() ****************/
/* see point.h for description */
int point_getX(point_t* point)
{
    if (point != NULL) {
        return point->x;
    } else {
        return -1;
    }
}

/**************** point_getY() ****************/
/* see point.h for description */
int point_getY(point_t* point)
{
    if (point != NULL) {
        return point->y;
    } else {
        return -1;
    }
}

/**************** point_setX() ****************/
/* see point.h for description */
void point_setX(point_t* point, int x)
{
    if (point != NULL) {
        point->x = x;
    }
}

/**************** point_setY() ****************/
/* see point.h for description */
void point_setY(point_t* point, int y)
{
    if (point != NULL) {
        point->y = y;
    }
}

/**************** point_incrementX() ****************/
/* see point.h for description */
int point_incrementX(point_t* point)
{
    if (point != NULL) {
        point->x = point->x + 1;
    }
}

/**************** point_incrementY() ****************/
/* see point.h for description */
int point_incrementY(point_t* point)
{
    if (point != NULL) {
        point->y = point->y + 1;
    }
}

/**************** point_decrementX() ****************/
/* see point.h for description */
int point_decrementX(point_t* point)
{
    if (point != NULL) {
        point->x = point->x - 1;
    }
}

/**************** point_decrementY() ****************/
/* see point.h for description */
int point_decrementY(point_t* point)
{
    if (point != NULL) {
        point->y = point->y - 1;
    }
}

/**************** point_delete() ****************/
/* see point.h for description */
void point_delete(point_t* point)
{
    if (point != NULL) {
        free(point);
    }
}