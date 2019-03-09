#ifndef TCL
#define TCL

#include <stdio.h>

#include <tcl.h> // The inclusion of the tcl library depends on your system //
#include "../definitions.h"

void init_tcl(char *argv[]);

int read_points(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int print_hash(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int create_kdTree(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int find_NN(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);
int clear_points(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[]);

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

#endif