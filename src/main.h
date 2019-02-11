#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include <tcl.h> // The inclusion of the tcl library depends on your system //
#include <readline/history.h>

#include "definitions.h"

// External Values //
extern Tcl_Interp *interpreter;

// External Functions //
extern void init_tcl(char *argv[]);
extern void init_readline();
extern void start_gui();
