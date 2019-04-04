#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../definitions.h"

#define DELIMITERS " \t\r\n"

void parse_points_file(FILE *fp);

// External Variables //
extern double pointMaxXCoordinate;
extern double pointMaxYCoordinate;

// External Functions //
extern void insert_point(char *name, double x, double y);