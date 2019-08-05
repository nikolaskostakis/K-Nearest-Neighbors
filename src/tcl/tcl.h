#ifndef TCL
#define TCL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <tcl.h> // The inclusion of the tcl library depends on your system //
#include "../definitions.h"

void init_tcl(char *argv[]);

int less(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int cat(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);

int read_points(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int print_element_array(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int create_kdTree(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int print_kdTree(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int find_NN(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int clear_elements(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int find_nearest_neighbours(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int find_K_nearest_neighbours(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int find_neighbours_within_radius(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int print_array(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int read_drawbuffer_output(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);

// External Functions //

// From io //
extern void parse_points_file(FILE *fp);
extern void parse_drawbuffer_output_file(FILE *fp);

// From structs //
extern void dump_element_array();
extern void free_element_array();

extern void create_KD_tree();
extern void dump_KD_tree();
extern void free_KD_tree();

extern void dump_sorting_array();
extern void free_sorting_array();

extern void dump_nearest_neighbor(double x, double y);
extern void dump_nearest_neighbours_within_radius(double x, double y, double range);
extern void dump_k_nearest_neighbours(double x, double y, unsigned long k);

extern void dump_element_distances(double x, double y);


#endif