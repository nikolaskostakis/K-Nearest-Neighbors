#ifndef STRUCTURES
#define STRUCTURES

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "prime_numbers.h"
#include "../definitions.h"

#define MAX_HASH_DEPTH 10
#define MAX_KDLEAF_ELEMENTS 5

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
	struct kdNode *parent;
};


unsigned long point_hash_function(char *name);
void insert_point(char *name, double x, double y);
void rehash_point_hash();
void free_point_hash();
void print_point_hash();

int point_x_comparator(const void *p1, const void *p2);
int point_y_comparator(const void *p1, const void *p2);

void create_sorting_array();
void free_sorting_array();
void print_sorting_array();

void create_KD_tree();
struct kdNode *insert_KD_tree_node(struct kdNode *parent, unsigned long startIndex, unsigned long endIndex);
void free_KD_tree();
void free_KD_node(struct kdNode *node);
void print_KD_tree();
void print_KD_node(struct kdNode *node, int depth);

inline double euclidean_distance(double x1, double y1, double x2, double y2);

#endif