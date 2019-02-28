#ifndef STRUCTURES
#define STRUCTURES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "prime_numbers.h"
#include "../definitions.h"

#define MAX_HASH_DEPTH 10

struct pointHashNode
{
    char *name;
    double x;
	double y;
};

struct pointHash
{
    struct pointHashNode *nodes;
    unsigned int depth;
};

struct kdNode
{
	bool isLeaf;
	int axis; // 0 for x & 1 for y //
	unsigned long startIndex;
	unsigned long endIndex;
	unsigned long splitIndex;
	struct kdNode *left;
	struct kdNode *right;
};


unsigned long point_hash_function(char *name);
void insert_point(char *name, double x, double y);
void rehash_point_hash();
void free_point_hash();
void print_point_hash();


#endif