/* 
 * point.h - header file for 'point' module for nuggets final project
 *
 * Sanjana Goli, May 2019
 */

#include <stdio.h>
#include <stdbool.h>
#include "set.h"

/* Structure holds int x and int y --> opaque to users of the module */
typedef struct point point_t;

/* Create and returns a new point with a given x and y value. Returns a pointer to the point. */
point_t* point_new(int x, int y);

/*  Retrieve and returns the x-coordinate of the point. 
*/
int point_getX(point_t* point);

/* Retrieve and returns the y-coordinate of the point. */
int point_getY(point_t* point);

/* Set the x-coordinate of a point, given an int x. */
void point_setX(point_t* point, int x);

/* Set the y-coordinate of a point, given an int y. */
void point_setY(point_t* point, int y);

/* Increment and returns the x-coordinate of point passed in by 1. */
int point_incrementX(point_t* point);

/* Increment and returns the y-coordinate of point passed in by 1. */
int point_incrementY(point_t* point);

/* Decrement and returns the x-coordinate of point passed in by 1. */
int point_decrementX(point_t* point);

/* Decrement and returns the y-coordinate of point passed in by 1. */
int point_decrementY(point_t* point);

/* Frees the memory allocated for point by point_new */
void point_delete(point_t* point);

/* Given a set of points, determine whether an item appears in the set at least once */
void point_setHasPoint(point_t* point, set_t *set);
