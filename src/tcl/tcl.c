#include "tcl.h"

// Tcl Interpreter //
Tcl_Interp *interpreter = NULL;

void init_tcl(char *argv[])
{
	Tcl_FindExecutable(argv[0]);

	// Initialize the interpreter //
	interpreter = Tcl_CreateInterp();

	// Create the commands //
	Tcl_CreateObjCommand(interpreter, "read_points", read_points, NULL, NULL);
}

int read_points(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{
	const char syntax[] = "<filepath>";

	int len = 0;
	FILE *fp = NULL;
	char *file = NULL;

	if (argc != 2)
	{
		Tcl_WrongNumArgs(interp, 1, argv, syntax);
		return TCL_ERROR;
	}

	file = Tcl_GetStringFromObj(argv[1], &len);

	fp = fopen(file, "r");

	if (fp == NULL)
	{
		printf("File does not exist!\n");
		return TCL_ERROR;
	}

	parse_points_file(fp);
	fclose(fp);
	print_point_hash();
	return TCL_OK;
}