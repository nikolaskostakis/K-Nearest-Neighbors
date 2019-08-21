#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <math.h>

#include <tcl.h> // The inclusion of the tcl library depends on your system //
#include <readline/readline.h>
#include <readline/history.h>

#include "definitions.h"

// External Values //
extern Tcl_Interp *interpreter;

// External Functions //

// From tcl //
extern void init_tcl(char *argv[]);

// From readline //
extern void init_readline();

// From ui //
extern void start_gui();
