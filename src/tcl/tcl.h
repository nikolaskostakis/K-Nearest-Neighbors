#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <tcl.h> // The inclusion of the tcl library depends on your system //
#include "../definitions.h"

void init_tcl(char *argv[]);

int less(ClientData, Tcl_Interp *, int, Tcl_Obj *const*);
int cat(ClientData, Tcl_Interp *, int, Tcl_Obj *const*);

int read_points(ClientData, Tcl_Interp *, int, Tcl_Obj *const*);
int print_element_array(ClientData, Tcl_Interp *, int, Tcl_Obj *const*);
int create_kdTree(ClientData, Tcl_Interp *, int, Tcl_Obj *const*);
int print_kdTree(ClientData, Tcl_Interp *, int, Tcl_Obj *const*);
int find_NN(ClientData, Tcl_Interp *, int, Tcl_Obj *const*);
int clear_elements(ClientData, Tcl_Interp *, int, Tcl_Obj *const*);
int find_nearest_neighbors(ClientData, Tcl_Interp *, int, Tcl_Obj *const*);
int find_K_nearest_neighbors(ClientData, Tcl_Interp *, int, Tcl_Obj *const*);
int find_neighbors_within_radius(ClientData, Tcl_Interp *, int, Tcl_Obj *const*);
int print_sorting_array(ClientData, Tcl_Interp *, int, Tcl_Obj *const*);
int read_drawbuffer_output(ClientData, Tcl_Interp *, int, Tcl_Obj *const*);

// External Variables //

// From structs //
extern unsigned long elementarraysize;
extern unsigned long kdtreearraysize;

// External Functions //

// From io //
extern void parse_points_file(FILE *);
extern void parse_drawbuffer_output_file(FILE *);

// From structs //
extern void dump_element_array();
extern void free_element_array();
extern void create_KD_tree();
extern void dump_KD_tree();
extern void free_KD_tree();
extern void dump_sorting_array();
extern void free_sorting_array();
extern void dump_nearest_neighbor(double, double);
extern void dump_nearest_neighbors_within_radius(double, double, double);
extern void dump_k_nearest_neighbors(double, double, unsigned long);
extern void dump_element_distances(double, double);
