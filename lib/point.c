/*
 * point.c - 'point' module for nuggets final project
 *
 * see point.h for more information.
 *
 * Sanjana Goli, Daniel DiPietro May 2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../libcs50/set.h"
#include "point.h"

typedef struct boolPoint boolPoint_t;
boolPoint_t* boolPoint_new(bool containsValue, point_t* point);
void boolPoint_delete(boolPoint_t* boolPoint);
void set_iterateHelper(void *arg, const char *key, void *item);

typedef struct point {
    int x;
    int y;
} point_t;

typedef struct boolPoint {
    point_t* point;
    bool containsValue;
} boolPoint_t;

/**************** point_new() ****************/
/* see point.h for description */
point_t* point_new(int x, int y)
{
    point_t *point = malloc(sizeof(point_t));

    if (point == NULL) {
        return NULL; // error allocating point
    } else {
        // initialize contents of point structure
        point->x = x;
        point->y = y;
    }
    return point;
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
        return point->x;
    }
    return 0;
}

/**************** point_incrementY() ****************/
/* see point.h for description */
int point_incrementY(point_t* point)
{
    if (point != NULL) {
        point->y = point->y + 1;
        return point->y;
    }
    return 0;
}

/**************** point_decrementX() ****************/
/* see point.h for description */
int point_decrementX(point_t* point)
{
    if (point != NULL) {
        point->x = point->x - 1;
        return point->x;
    }
    return 0;
}

/**************** point_decrementY() ****************/
/* see point.h for description */
int point_decrementY(point_t* point)
{
    if (point != NULL) {
        point->y = point->y - 1;
        return point->y;
    }
    return 0;
}

/**************** point_delete() ****************/
/* see point.h for description */
void point_print(point_t* point, FILE* fp)
{
  if (point != NULL && fp != NULL) {
    fprintf(fp, "(%i,%i)", point->x, point->y);
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

/**************** point_setHasPoint() ****************/
/* see point.h for description */
bool point_setHasPoint(point_t* point, set_t* set)
{
    bool val = false;
    boolPoint_t* boolPoint = boolPoint_new(val, point);
    set_iterate(set, boolPoint, set_iterateHelper);
    val = boolPoint->containsValue;
    boolPoint_delete(boolPoint);
    return val;
}

/* takes in boolPoint struct in order to determine if value exists in set*/
void set_iterateHelper(void *arg, const char *key, void *item)
{
    boolPoint_t* boolPoint = arg;
    point_t* p = item;

    if ((boolPoint->point->x == p->x) && (boolPoint->point->y == p->y)){
        boolPoint->containsValue = true;
    }
}

boolPoint_t* boolPoint_new(bool containsValue, point_t* point)
{
    boolPoint_t* boolPoint = malloc(sizeof(boolPoint_t));
    if (boolPoint == NULL) {
        return NULL;
    } else {
        boolPoint->containsValue = containsValue;
        boolPoint->point = point;
    }

    return boolPoint;
}

void boolPoint_delete(boolPoint_t* boolPoint)
{
    if (boolPoint != NULL) {
        free(boolPoint);
    }
}
