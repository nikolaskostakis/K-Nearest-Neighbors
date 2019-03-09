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
	Tcl_CreateObjCommand(interpreter, "print_hash", print_hash, NULL, NULL);
	Tcl_CreateObjCommand(interpreter, "create_kdTree", create_kdTree, NULL, NULL);
	Tcl_CreateObjCommand(interpreter, "find_NN", find_NN, NULL, NULL);
	Tcl_CreateObjCommand(interpreter, "clear_points", clear_points, NULL, NULL);
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
	
	return TCL_OK;
}

int print_hash(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{
	const char syntax[] = "";

	if (argc != 1)
	{
		Tcl_WrongNumArgs(interp, 1, argv, syntax);
		return TCL_ERROR;
	}

	print_point_hash();

	return TCL_OK;
}

int create_kdTree(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{
	const char syntax[] = "";

	if (argc != 1)
	{
		Tcl_WrongNumArgs(interp, 1, argv, syntax);
		return TCL_ERROR;
	}

	create_KD_tree();
	print_KD_tree();

	return TCL_OK;
}

int find_NN(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{
	const char syntax[] = "<x-coord> <y-coord>";
	double x = 0;
	double y = 0;

	if (argc != 3)
	{
		Tcl_WrongNumArgs(interp, 1, argv, syntax);
		return TCL_ERROR;
	}

	Tcl_GetDouble(interp, Tcl_GetString(argv[1]), &x);
	Tcl_GetDouble(interp, Tcl_GetString(argv[2]), &x);

	print_nearest(x, y);

	return TCL_OK;
}

int clear_points(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{
	const char syntax[] = "";

	if (argc != 1)
	{
		Tcl_WrongNumArgs(interp, 1, argv, syntax);
		return TCL_ERROR;
	}

	free_point_hash();
	free_sorting_array();
	free_KD_tree();

	return TCL_OK;
}
