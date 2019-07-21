#ifndef STRUCTURES
#define STRUCTURES

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// #include "prime_numbers.h"
#include "../definitions.h"

// Definitions //
#define MAX_HASH_DEPTH 10
#define MAX_KDLEAF_ELEMENTS 5

#define ROOT_NODE 1

#define X_AXIS 0
#define Y_AXIS 1

#define CELL_WIDTH    1.260
#define CELL_HEIGHT   0.576

// Structuress for Points //

struct pointHashElement
{
    char *name; // Name of the point //
    double x;   // x coordinate //
	double y;   // y coordinate //
};

struct pointHash
{
    struct pointHashElement *bucket; // Hash Table Bucket //
    unsigned int depth;              // Number of points into the bucket //
};

struct kdTreeNode
{
	int isLeaf;               // Flag used to determine if the node is a leaf //
	int axis;                 // Defines the axis checked by this node //
	unsigned long startIndex; // Starting Index of the node's sub-array //
	unsigned long endIndex;   // Ending Index of the node's sub-array //
	struct pointHashElement *splitReference; // Pointer to the point used to split the index //
	                                         // The pointer is used to access the point and  // 
	                                         // use its coordinates for the algorithms       //
	// struct kdTreeNode *left;
	// struct kdTreeNode *right;
	// struct kdTreeNode *parent;
};

// End //

// Structs for Design //

enum struct_type {nothing = 0, io, component};

// Design Row //
struct row
{
	char *name;
	char *type;
	// Starting Point of the Row
	double location_x;
	double location_y;
	// Size
	double width;
	double height;
};

// Design I/O Pins //
struct io
{
	char *name;
	// Location
	double x;
	double y;
};

// Design Components/Cells //
struct component
{
	char *name;

	// Location (Top Left)
	double x;
	double y;
};

// P2P Net //
struct net
{
	enum struct_type edge_type_one;
	enum struct_type edge_type_two;
	unsigned int edge_pointer_one;
	unsigned int edge_pointer_two;
	char *edge_name_one;
	char *edge_name_two;
};

// Functions for Points //

unsigned long point_hash_function(char *name);
void insert_point(char *name, double x, double y);
void rehash_point_hash();
int get_point_hash_depth(unsigned long hash);
double get_point_x_coord(unsigned long hash, int depth);
double get_point_y_coord(unsigned long hash, int depth);
char *get_point_name(unsigned long hash, int depth);
void free_point_hash();
void print_point_hash();

int point_x_comparator(const void *p1, const void *p2);
int point_y_comparator(const void *p1, const void *p2);

void create_sorting_array();
void free_sorting_array();
void print_sorting_array();

void create_KD_tree();
void insert_KD_tree_node(unsigned long index, unsigned long startIndex, unsigned long endIndex);
void free_KD_tree();

void print_KD_tree();
void print_tabs(int depth);
void print_KD_node(unsigned long index, int depth);

unsigned long find_nearest_neighbour(unsigned long index, double x, double y);
void print_nearest_neighbor(double x, double y);

unsigned long *find_nearest_neighbours_within_radius(unsigned long index, double x, double y, double radius, unsigned long *noofNeighbors);
void print_nearest_neighbours_within_radius(double x, double y, double range);

unsigned long *find_k_nearest_neighbours(unsigned long index, double x, double y, unsigned long n, unsigned long *noofNeighbors);
void print_k_nearest_neighbours(double x, double y, unsigned long k);

#endif