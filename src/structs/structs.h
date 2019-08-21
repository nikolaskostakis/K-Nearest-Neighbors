#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../definitions.h"

// Definitions //
#define MAX_KDLEAF_ELEMENTS 5

#define ROOT_NODE 1

#define X_AXIS 0
#define Y_AXIS 1

// Structuress for Points //

typedef enum {point = -1, cell, IO, module, other} knnElementType;

struct knnElement
{
	char *name;          // The name of the element //
	double x;            // Its x coordinate //
	double y;            // Its y coordinate //
	knnElementType type; // The type of the element //
	double width;        // Its width //
	double height;       // Its height //
};

struct kdTreeNode
{
	int isLeaf;               // Flag used to determine if the node is a leaf //
	int axis;                 // Defines the axis checked by this node //
	unsigned long startIndex; // Starting Index of the node's sub-array //
	unsigned long endIndex;   // Ending Index of the node's sub-array //
	unsigned long splitIndex; // Index of the Element used to split the array //
	                          // The index is used to access the element and  // 
	                          // use its coordinates for the NN algorithms    //
};


// Functions for Points //

int point_x_comparator(const void *p1, const void *p2);
int point_y_comparator(const void *p1, const void *p2);

void init_element_array(unsigned long size);
void insert_element(char *name, double x, double y, int type, double width, double height);
void insert_point(char *name, double x, double y);
void free_element_array();
void dump_element_array();
void dump_element_distances(double x, double y);
char *get_element_name(unsigned long index);
double get_element_x(unsigned long index);
double get_element_y(unsigned long index);
double get_element_width(unsigned long index);
double get_element_height(unsigned long index);
int get_element_type(unsigned long index);

void create_sorting_array();
void free_sorting_array();
void dump_sorting_array();

void create_KD_tree();
void insert_KD_tree_node(unsigned long index, unsigned long startIndex, unsigned long endIndex);
void free_KD_tree();

void dump_KD_tree();
void dump_tabs(int depth);
void dump_KD_node(unsigned long index, int depth);

unsigned long find_nearest_neighbour(unsigned long index, double x, double y);
void dump_nearest_neighbor(double x, double y);

unsigned long *find_nearest_neighbours_within_radius(unsigned long index, double x, double y, double radius, unsigned long *noofNeighbors);
void dump_nearest_neighbours_within_radius(double x, double y, double range);

unsigned long *find_k_nearest_neighbours(unsigned long index, double x, double y, unsigned long n, unsigned long *noofNeighbors);
void dump_k_nearest_neighbours(double x, double y, unsigned long k);
