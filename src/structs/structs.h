#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../definitions.h"

// Definitions //
#define MAX_KDLEAF_ELEMENTS 5

#define ROOT_NODE 1 // The index of the root node on the KD Tree     //
                    // The first element of the array is not used    //
                    // because the operations to find the chhildren  //
                    // of the node are smoother if they start from 1 //

#define X_AXIS 0  // Defined value used to describe the usage   //
                  // of the X axis form the node of the KD Tree //
#define Y_AXIS 1  // Defined value used to describe the usage   //
                  // of the Y axis form the node of the KD Tree //

// enum type: the type of knnelement //
typedef enum {point = -1, cell, IO, module, other} knnelementtype;

// knnelement - from input file //
// array of the stored elements //
struct knnelement
{
	char *name;          // The name of the element //
	double x;            // The x coordinate of the element //
	double y;            // The y coordinate of the element //
	knnelementtype type; // The type of the element //
	double width;        // The width of the element //
	double height;       // The height of the element //
};

// array reprsentation of the tree used for the NN Algorithms //
struct kdtreenode
{
	int isleaf;               // Flag used to determine if the node is a leaf //
	int axis;                 // Flag used to determine the axis used by this node //
	unsigned long startindex; // Starting Index of the node's sub-array //
	unsigned long endindex;   // Ending Index of the node's sub-array //
	unsigned long splitindex; // Index of the Element used to split the array //
	                          // The index is used to access the element and  // 
	                          // use its coordinates for the NN algorithms    //
};

// The Max coordinates of elemenents //
// They are used by UI for optimizing the design size //
double elementmaxxcoordinate; // The max X coordinate of elements //
double elementmaxycoordinate; // The max Y coordinate of elements //

// KD-Tree //
struct kdtreenode *kdtree;     // The KD Tree //
unsigned long kdtreearraysize; // The size of the array that represents the tree //

// Sorting Array used by KD-Tree //
unsigned long *sortingarray;    // The Sorting Array //
unsigned long sortingarraysize; // The size of the Sorting Array //

// Elements' Array //
struct knnelement *elementarray; // The Elements' Array //
unsigned long elementarraysize;  // The size of the Array //
unsigned long elementarrayindex; // An index to the Element's Array. It is  //
                                 // used to add a new element to the Array. //

// Temporary Globals for coordinates //
// They are used for sorting using qsort() //
double refpointx, refpointy;

void initialise_globals();
int point_x_comparator(const void *, const void *);
int point_y_comparator(const void *, const void *);
void init_element_array(unsigned long);
void insert_element(char *, double, double, int, double, double);
void free_element_array();
void dump_element_array();
void dump_element_distances(double, double);
char *get_element_name(unsigned long);
double get_element_x(unsigned long);
double get_element_y(unsigned long);
double get_element_width(unsigned long);
double get_element_height(unsigned long);
int get_element_type(unsigned long);
void create_sorting_array();
void free_sorting_array();
void dump_sorting_array();
void create_KD_tree();
void insert_KD_tree_node(unsigned long, unsigned long, unsigned long);
void free_KD_tree();
void dump_KD_tree();
void dump_tabs(int);
void dump_KD_node(unsigned long, int);
unsigned long find_nearest_neighbor(unsigned long, double, double);
void dump_nearest_neighbor(double, double);
unsigned long *find_nearest_neighbors_within_radius(unsigned long, double, double, double, unsigned long *);
void dump_nearest_neighbors_within_radius(double, double, double);
unsigned long *find_k_nearest_neighbors(unsigned long, double, double, unsigned long, long *);
void dump_k_nearest_neighbors(double, double, long);
