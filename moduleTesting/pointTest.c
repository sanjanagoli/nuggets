/*
 * pointTest.c - test program for the nuggets point module
 *
 * usage: read lines from stdin
 *
 * CS50, April 2019
 */

#include "../lib/point.h"

void pointPrintHelper(FILE *fp, const char *key, void *item);

/* **************************************** */
int main() {
  int x = 5;
  int y = 5;

  // Make a point
  printf("\nMake a point with values x=5 and y=5.\n");
  point_t* p = point_new(x, y);
  printf("Point: ");
  point_print(p, stdout);
  printf("\n");

  //Testing point_toString
  printf("\nTesting point to string\n");
  char* pointString = point_toString(p);
  printf("%s\n", pointString);
  free(pointString);

  // Increment x the coordinate
  printf("\nIncrement the x coordinate.\n");
  point_incrementX(p);
  printf("Point: ");
  point_print(p, stdout);
  printf("\n");

  // Increment y the coordinate
  printf("\nIncrement the y coordinate.\n");
  point_incrementY(p);
  printf("Point: ");
  point_print(p, stdout);
  printf("\n");

  // Decrement x the coordinate
  printf("\nDecrement the x coordinate.\n");
  point_decrementX(p);
  printf("Point: ");
  point_print(p, stdout);
  printf("\n");

  // Decrement y the coordinate
  printf("\nDecrement the y coordinate.\n");
  point_decrementY(p);
  printf("Point: ");
  point_print(p, stdout);
  printf("\n");

  // Set the x coordinate
  printf("\nSet the x-coordinate to 10.\n");
  point_setX(p, 10);
  printf("Point: ");
  point_print(p, stdout);
  printf("\n");

  // Set the x coordinate
  printf("\nSet the y-coordinate to 15.\n");
  point_setY(p, 15);
  printf("Point: ");
  point_print(p, stdout);
  printf("\n");

  // Get the x-coordinate and print it
  printf("\nPrint the x-coordinate using point_getX.\n");
  x = point_getX(p);
  printf("x-value: %i\n", x);

  // Get the y-coordinate and print it
  printf("\nPrint the y-coordinate using point_getY.\n");
  y = point_getY(p);
  printf("y-value: %i\n", y);

  printf("\nMake a set of points.\n");
  set_t* pointSet = set_new();
  point_t* p1 = point_new(1, 1);
  point_t* p2 = point_new(2, 4);
  point_t* p3 = point_new(4, 5);
  point_t* p4 = point_new(3, 1);
  point_t* p5 = point_new(0, 0);

  set_insert(pointSet, "1", p1);
  set_insert(pointSet, "2", p2);
  set_insert(pointSet, "3", p3);
  set_insert(pointSet, "4", p4);
  set_insert(pointSet, "5", p5);

  set_print(pointSet, stdout, pointPrintHelper);
  printf("\n");

  printf("\nMake a new point of (0,0) and check if it's in the set.\n");
  point_t* p6 = point_new(0, 0);
  if (point_setHasPoint(p6, pointSet)) {
    printf("Point is in set.\n");
  } else {
    printf("Point is not in set.\n");
  }

  printf("\nCheck if the point (150, 12) is in the set.\n");
  point_setX(p6, 150);
  point_setY(p6, 12);
  if (point_setHasPoint(p6, pointSet)) {
    printf("Point is in set.\n");
  } else {
    printf("Point is not in set.\n");
  }

  printf("\nDelete the set and all points.\n");

  point_delete(p6);
  point_delete(p5);
  point_delete(p4);
  point_delete(p3);
  point_delete(p2);
  point_delete(p1);
  point_delete(p);
  set_delete(pointSet, NULL);

  return 0;
}

void pointPrintHelper(FILE *fp, const char *key, void *item)
{
  point_t* p = item;
  if (item == NULL) {
    fprintf(fp, "(null)");
  }
  else {
    point_print(p, stdout);
  }
}
