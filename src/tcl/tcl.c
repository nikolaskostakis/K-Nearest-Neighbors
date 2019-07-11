#include "tcl.h"

// Tcl Interpreter //
Tcl_Interp *interpreter = NULL;

// *** find_flag_index *** //
int find_flag_index(int argc, Tcl_Obj *const argv[], const char flag[])
{
	int i;
	int result = -1;

	for (i = 0; i < argc; i++)
	{
		if (strcmp(flag, Tcl_GetString(argv[i])) == 0)
		{
			result = i;
			break;
		}
	}

	return result;
}

// *** init_tcl *** //
void init_tcl(char *argv[])
{
	Tcl_FindExecutable(argv[0]);

	// Initialize the interpreter //
	interpreter = Tcl_CreateInterp();

	// Create the commands //
	Tcl_CreateObjCommand(interpreter, "less", less, NULL, NULL);
	Tcl_CreateObjCommand(interpreter, "cat", cat, NULL, NULL);

	Tcl_CreateObjCommand(interpreter, "read_points", read_points, NULL, NULL);
	Tcl_CreateObjCommand(interpreter, "print_hash", print_hash, NULL, NULL);
	Tcl_CreateObjCommand(interpreter, "create_kdTree", create_kdTree, NULL, NULL);
	Tcl_CreateObjCommand(interpreter, "find_NN", find_NN, NULL, NULL);
	Tcl_CreateObjCommand(interpreter, "clear_points", clear_points, NULL, NULL);
	Tcl_CreateObjCommand(interpreter, "find_nearest_neighbours", find_nearest_neighbours, NULL, NULL);
	Tcl_CreateObjCommand(interpreter, "find_neighbours_within_radius", find_neighbours_within_radius, NULL, NULL);
	Tcl_CreateObjCommand(interpreter, "print_array", print_array, NULL, NULL);
}

// *** less *** //
int less(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{
	char syntax[] = "<filepath>";
	char command[] = "less";
	char *ex_command = NULL, *arg = NULL;
	int len = 0;

	if (argc != 2)
	{
		Tcl_WrongNumArgs(interp, 1, argv, syntax);
		return TCL_ERROR;
	}

	arg = Tcl_GetStringFromObj(argv[1], &len);
	if (arg == NULL)
	{
		printf("Wrong Encoding of Argument: 1\n");
		return TCL_ERROR;
	}

	ex_command = (char *) malloc((len + 4 + 2) * sizeof(char));
	if (ex_command == NULL)
	{
		printf("Unable to Allocate Memory! Exiting...\n");
		exit(1);
	}

	sprintf(ex_command, "%s %s", command, arg);

	system(ex_command);
	free(ex_command);

	return TCL_OK;
}

// *** cat *** //
int cat(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{
	char syntax[] = "<filepath>";
	char command[] = "cat";
	char *ex_command = NULL, *arg = NULL;
	int len = 0;

	if (argc != 2)
	{
		Tcl_WrongNumArgs(interp, 1, argv, syntax);
		return TCL_ERROR;
	}

	arg = Tcl_GetStringFromObj(argv[1], &len);
	if (arg == NULL)
	{
		printf("Wrong Encoding of Argument: 1\n");
		return TCL_ERROR;
	}

	ex_command = (char *) malloc((len + 4 + 2) * sizeof(char));
	if (ex_command == NULL)
	{
		printf("Unable to Allocate Memory! Exiting...\n");
		exit(1);
	}

	sprintf(ex_command, "%s %s", command, arg);

	system(ex_command);
	free(ex_command);

	return TCL_OK;
}

// *** read_points *** //
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

	printf(GRN"Points have been parsed and stored!"NRM"\r\n");
	
	return TCL_OK;
}

// *** print_hash *** //
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

// *** creat_kdTree *** //
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

int find_nearest_neighbours(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{
	const char syntax[] = "[-k <noof_neighbors> | -r <radius>] -point <x-coord> <y-coord>";
	unsigned long *indexes = NULL;
	unsigned long k = 0;
	unsigned long r = 0;
	unsigned long i = 0;
	unsigned long noofIndexes = 0;
	double x = 0;
	double y = 0;

	if ((argc != 4) && (argc != 6))
	{
		Tcl_WrongNumArgs(interp, 1, argv, syntax);
		return TCL_ERROR;
	}

	Tcl_GetLongFromObj(interp, argv[2], &k);
	Tcl_GetDouble(interp, Tcl_GetString(argv[4]), &x);
	Tcl_GetDouble(interp, Tcl_GetString(argv[5]), &y);

	if (k <= 0)
	{
		return TCL_ERROR;
	}
	else if (k == 1)
	{
		print_point_hash_distances(x, y);
		print_nearest(x, y);
	}
	else
	{
		create_KD_tree();
		print_point_hash_distances(x, y);
		indexes = find_n_nearest_neighbours(kdTree, x, y, k, &noofIndexes);
		assert(indexes != NULL);

		if (indexes != NULL)
		{
			for (i = 0; i < noofIndexes; i++)
			{
				printf("\t%s\n", pointArray[indexes[i]]->name);
			}

			free(indexes);
		}
		else
		{
			printf(YEL"No neighbors within radius!\n"NRM);
		}
	}

	return TCL_OK;
}

int find_neighbours_within_radius(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{
	const char syntax[] = "[-r <radius> -point <x-coord> <y-coord>";
	unsigned long *indexes = NULL;
	unsigned long r = 0;
	unsigned long i = 0;
	unsigned long noofIndexes = 0;
	double x = 0;
	double y = 0;

	if (argc != 6)
	{
		Tcl_WrongNumArgs(interp, 1, argv, syntax);
		return TCL_ERROR;
	}

	Tcl_GetLongFromObj(interp, argv[2], &r);
	Tcl_GetDouble(interp, Tcl_GetString(argv[4]), &x);
	Tcl_GetDouble(interp, Tcl_GetString(argv[5]), &y);

	if (r <= 0)
	{
		return TCL_ERROR;
	}
	else
	{
		create_KD_tree();

		print_point_hash_distances(x, y);

		indexes = find_nearest_neighbours_within_radius(kdTree, x, y, r, &noofIndexes);
		if (indexes != NULL)
		{
			for (i = 0; i < noofIndexes; i++)
			{
				printf("\t%s\n", pointArray[indexes[i]]->name);
			}

			free(indexes);
		}
		else
		{
			printf(YEL"No neighbors within radius!\n"NRM);
		}
	}

	return TCL_OK;
}

int print_array(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{
	const char syntax[] = "";

	if (argc != 1)
	{
		Tcl_WrongNumArgs(interp, 1, argv, syntax);
		return TCL_ERROR;
	}

	print_sorting_array();

	return TCL_OK;
}