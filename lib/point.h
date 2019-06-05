/*
 * point.h - header file for 'point' module for nuggets final project
 *
 * Sanjana Goli, Daniel DiPietro May 2019
 */
/* 
  STYLE: this file has DOS line endings!
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../libcs50/set.h"

/* 
  STYLE: 'point' is one of those structs that might actually be better
  transparent than opaque; it is very simple, and there is less need
  for methods to new, delete, get, set, when it is transparent.  Most
  notably, a transparent struct can be simply declared as a local
  variable, passed to a function, returned from a function, and even
  compared in an if() statement... and thus you can eliminate all the
  malloc/free involving points.
 */
/* Structure holds int x and int y --> opaque to users of the module */
typedef struct point point_t;

/* Create and returns a new point with a given x and y value. Returns a pointer to the point. */
/**************** point_new ****************/
/* Create a new point.
 *
 * Caller provides:
 *   two ints representing x and y coordinates
 * We return:
 *   pointer to the new point; return NULL if error.
 * We guarantee:
 *   point is initialized with given x and y values.
 * Caller is responsible for:
 *   later calling point_delete.
 */
point_t* point_new(int x, int y);

/*****************point_getX ***************/
/* Caller provides:
*   a pointer to point
*  We return:
*   int - x-coordinate of point; -1 if NULL
*  We guarantee:
*   integer is returned
*/
int point_getX(point_t* point);

/*****************point_getY ***************/
/* Caller provides:
*   a pointer to point
*  We return:
*   int - y-coordinate of point; -1 if NULL
*  We guarantee:
*   integer is returned
*/
int point_getY(point_t* point);

/*****************point_getX ***************/
/* Caller provides:
*   a pointer to point, int for x-coordinate
*  We return:
*   void
*  We guarantee:
*   point->x is updated if x (passed in) is valid
*/
void point_setX(point_t* point, int x);

/*****************point_getY ***************/
/* Caller provides:
*   a pointer to point, int for y-coordinate
*  We return:
*   void (no return value)
*  We guarantee:
*   point->y is updated if y (passed in) is valid
*/
void point_setY(point_t* point, int y);

/*****************point_incrementX ***************/
/* Caller provides:
*   a pointer to point
*  We return:
*   point->x increments by 1
*   integer of updated x-coordinate if point is not null; -1 if it is
*  We guarantee:
*   integer is returned
*/
int point_incrementX(point_t* point);

/*****************point_incrementY ***************/
/* Caller provides:
*   a pointer to point
*  We return:
*   point->y increments by 1
*   integer of updated y-coordinate if point is not null; -1 if it is
*  We guarantee:
*   integer is returned
*/
int point_incrementY(point_t* point);

/***************** point_decrementX ***************/
/* Caller provides:
*   a pointer to point
*  We return:
*   point->x decrements by 1
*   integer of updated x-coordinate if point is not null; -1 if it is
*  We guarantee:
*   integer is returned
*/
int point_decrementX(point_t* point);

/*****************point_decrementY ***************/
/* Caller provides:
*   a pointer to point
*  We return:
*   point->y decrements by 1
*   integer of updated y-coordinate if point is not null; -1 if it is
*  We guarantee:
*   integer is returned
*/
int point_decrementY(point_t* point);

/*****************point_print ***************/
/* Caller provides:
*   a pointer to point and a file pointer
*  Output:
*   print x-coord and y-coord of point to file
*/
void point_print(point_t* point, FILE* fp);

/****************** point_delete ***************/
/* Caller provides:
*   a pointer to point
*  We guarantee:
*   if point is not null, the memory allocated for point struct is freed
*/
void point_delete(point_t* point);

/****************** point_setHasPoint ***************/
/* Caller provides:
*   a pointer to point and pointer to set
*  We return:
*   true if point is contained in set; false otherwise
*  We guarantee:
*   bool is returned
*/
bool point_setHasPoint(point_t* point, set_t *set);

/****************** point_setHasPoint ***************/
/* Caller provides:
*   a pointer to point
*  We return:
*   concatenated version of x and y values in following format: "x,y"
*  Caller is responsible for:
*   free the char* allocated 
*/
char* point_toString(point_t* point);
