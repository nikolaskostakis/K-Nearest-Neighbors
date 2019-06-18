#ifndef TCL
#define TCL

#include <stdio.h>

#include <tcl.h> // The inclusion of the tcl library depends on your system //
#include "../definitions.h"

#include "../structures/structures.h"

void init_tcl(char *argv[]);

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
extern void print_point_hash();
extern void create_KD_tree();
extern void print_sorting_array();
extern void print_KD_tree();
extern void print_nearest(double x, double y);
extern void free_point_hash();
extern void free_sorting_array();
extern void free_KD_tree();
extern unsigned long *find_n_nearest_neighbours(struct kdNode *node, double x, double y, unsigned long n, unsigned long *noofNeighbors);
extern unsigned long *find_nearest_neighbours_within_radius(struct kdNode *node, double x, double y, double radius, unsigned long *noofNeighbors);

extern struct kdNode *kdTree;
extern struct pointHashNode **pointArray;

extern void print_point_hash_distances(double x, double y);


#endif