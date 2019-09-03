#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../definitions.h"

#define DELIMITERS " \t\r\n"

void parse_points_file(FILE *);
void parse_drawbuffer_output_file(FILE *);

// External Variables //
extern double elementmaxxcoordinate;
extern double elementmaxycoordinate;

// External Functions //
extern void init_element_array(unsigned long);
extern void insert_element(char *, double, double, int, double, double);