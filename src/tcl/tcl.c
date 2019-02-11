#include "tcl.h"

// Tcl Interpreter //
Tcl_Interp *interpreter = NULL;

void init_tcl(char *argv[])
{
	Tcl_FindExecutable(argv[0]);

	// Initialize the interpreter //
	interpreter = Tcl_CreateInterp();
}