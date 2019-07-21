#ifndef TCL
#define TCL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tcl.h> // The inclusion of the tcl library depends on your system //
#include "../definitions.h"

void init_tcl(char *argv[]);

int less(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int cat(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);

int read_points(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int print_hash(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int create_kdTree(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int find_NN(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int clear_points(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int find_nearest_neighbours(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int find_neighbours_within_radius(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int print_array(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);

// External Functions //

// From io //
extern void parse_points_file(FILE *fp);

// From structs //
extern void print_point_hash();
extern void free_point_hash();

extern void create_KD_tree();
extern void print_KD_tree();
extern void free_KD_tree();

extern void print_sorting_array();
extern void free_sorting_array();

extern void print_nearest_neighbor(double x, double y);
extern void print_nearest_neighbours_within_radius(double x, double y, double range);
extern void print_k_nearest_neighbours(double x, double y, unsigned long k);

// extern struct kdTreeNode *kdTree;
extern struct pointHashElement **pointArray;

extern void print_point_hash_distances(double x, double y);


#endif