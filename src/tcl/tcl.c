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
  Tcl_CreateObjCommand(interpreter, "print_element_array", print_element_array, NULL, NULL);
  Tcl_CreateObjCommand(interpreter, "create_kdTree", create_kdTree, NULL, NULL);
  Tcl_CreateObjCommand(interpreter, "print_kdTree", print_kdTree, NULL, NULL);
  Tcl_CreateObjCommand(interpreter, "find_NN", find_NN, NULL, NULL);
  Tcl_CreateObjCommand(interpreter, "clear_elements", clear_elements, NULL, NULL);
  Tcl_CreateObjCommand(interpreter, "find_nearest_neighbours", find_nearest_neighbours, NULL, NULL);
  Tcl_CreateObjCommand(interpreter, "find_neighbours_within_radius", find_neighbours_within_radius, NULL, NULL);
  Tcl_CreateObjCommand(interpreter, "print_sorting_array", print_sorting_array, NULL, NULL);
  Tcl_CreateObjCommand(interpreter, "read_drawbuffer_output", read_drawbuffer_output, NULL, NULL);
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

  ex_command = (char *) malloc((len + 3 + 2) * sizeof(char));
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

// *** print_element_array *** //
int print_element_array(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{
  const char syntax[] = "";

  if (argc != 1)
    {
      Tcl_WrongNumArgs(interp, 1, argv, syntax);
      return TCL_ERROR;
    }

  dump_element_array();

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

  return TCL_OK;
}

// *** creat_kdTree *** //
int print_kdTree(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{
  const char syntax[] = "";

  if (argc != 1)
    {
      Tcl_WrongNumArgs(interp, 1, argv, syntax);
      return TCL_ERROR;
    }

  dump_KD_tree();

  return TCL_OK;
}

// *** find_NN *** //
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

  #ifdef DEBUG
    {
      dump_element_distances(x, y);
    }
  #endif

  dump_nearest_neighbor(x, y);

  return TCL_OK;
}

// *** clear_elements *** //
int clear_elements(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{
  const char syntax[] = "";

  if (argc != 1)
    {
      Tcl_WrongNumArgs(interp, 1, argv, syntax);
      return TCL_ERROR;
    }

  free_element_array();
  free_sorting_array();
  free_KD_tree();

  return TCL_OK;
}

// *** find_nearest_neighbours *** //
int find_nearest_neighbours(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{
  const char syntax[] = "[-k <noof_neighbors> | -r <radius>] -point <x-coord> <y-coord>";
  char *flag = NULL;
  unsigned long k = 0;
  unsigned long r = 0;
  double x = 0;
  double y = 0;
  clock_t start, end;

  if ((argc != 4) && (argc != 6))
    {
      Tcl_WrongNumArgs(interp, 1, argv, syntax);
      return TCL_ERROR;
    }

  
  flag = Tcl_GetString(argv[1]);
  Tcl_GetDouble(interp, Tcl_GetString(argv[4]), &x);
  Tcl_GetDouble(interp, Tcl_GetString(argv[5]), &y);

  // Check if the given flag is the one for knn //
  if (strcmp(flag, "-k") == 0)
    {
      Tcl_GetLongFromObj(interp, argv[2], (long *)&k);

      // If the number of neighbors is less than 1 //
      if (k <= 0)
        {
          // Print error //
          printf("Error: The number of neighbors must be greater than 0\r\n");
          return TCL_ERROR;
        }
      // If the number of neighbors is 1 //
      else if (k == 1)
        {
          // Call the nearest neighbor algorithm //
          #ifdef DEBUG
            {
              dump_element_distances(x, y);
            }
          #endif

          start = clock();
          dump_nearest_neighbor(x, y);
          end = clock();
          printf("\nTime == "RED"%ld %ld\n\r\t%lf"NRM"\r\n", end, start, (double)(end-start)/CLOCKS_PER_SEC);
        }
      // Else //
      else
        {
          // Call the algorithm forr the k nearest neighbors //
          #ifdef DEBUG
            {
              dump_element_distances(x, y);
            }
          #endif

          dump_k_nearest_neighbours(x, y, k);
        }
    }
  // Check if the given flag is the one for check within radius //
  else if (strcmp(flag, "-r") == 0)
  {
    Tcl_GetLongFromObj(interp, argv[2], (long *)&r);

    if (r <= 0)
    {
      return TCL_ERROR;
    }
    else
    {
      #ifdef DEBUG
      {
        dump_element_distances(x, y);
      }
      #endif

      dump_nearest_neighbours_within_radius(x, y, r);
    }
  }
  // If its neither flag //
  else
  {
    // Print Error //
    Tcl_WrongNumArgs(interp, 1, argv, syntax);
    return TCL_ERROR;
  }
  

  return TCL_OK;
}

// *** find_K_nearest_neighbours *** //
int find_K_nearest_neighbours(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{
  const char syntax[] = "-k <noof_neighbors> -point <x-coord> <y-coord>";
  unsigned long k = 0;
  double x = 0;
  double y = 0;
  clock_t start, end;

  if ((argc != 4) && (argc != 6))
  {
    Tcl_WrongNumArgs(interp, 1, argv, syntax);
    return TCL_ERROR;
  }

  Tcl_GetLongFromObj(interp, argv[2], (long *)&k);
  Tcl_GetDouble(interp, Tcl_GetString(argv[4]), &x);
  Tcl_GetDouble(interp, Tcl_GetString(argv[5]), &y);

  // If the number of neighbors is less than 1 // 
  if (k <= 0)
  {
    // Print error //
    printf("Error: The number of neighbors must be greater than 0\r\n");
    return TCL_ERROR;
  }
  // If the number of neighbors is 1 //
  else if (k == 1)
  {
    // Call the nearest neighbor algorithm //
    #ifdef DEBUG
    {
      dump_element_distances(x, y);
    }
    #endif
    start = clock();
    dump_nearest_neighbor(x, y);
    end = clock();
    printf("\nTime == "RED"%ld %ld\n\r\t%lf"NRM"\r\n", end, start, (double)(end-start)/CLOCKS_PER_SEC);
  }
  // Else //
  else
  {
    // Call the algorithm forr the k nearest neighbors //
    #ifdef DEBUG
    {
      dump_element_distances(x, y);
    }
    #endif

    dump_k_nearest_neighbours(x, y, k);
  }

  return TCL_OK;
}

// *** find_neighbours_within_radius *** //
int find_neighbours_within_radius(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{
  const char syntax[] = "[-r <radius> -point <x-coord> <y-coord>";
  double radius = 0;
  double x = 0;
  double y = 0;

  if (argc != 6)
  {
    Tcl_WrongNumArgs(interp, 1, argv, syntax);
    return TCL_ERROR;
  }

  Tcl_GetDouble(interp, Tcl_GetString(argv[2]), &radius);;
  Tcl_GetDouble(interp, Tcl_GetString(argv[4]), &x);
  Tcl_GetDouble(interp, Tcl_GetString(argv[5]), &y);

  if (radius <= 0)
  {
    return TCL_ERROR;
  }
  else
  {
    #ifdef DEBUG
    {
      dump_element_distances(x, y);
    }
    #endif

    dump_nearest_neighbours_within_radius(x, y, radius);
  }

  return TCL_OK;
}

// *** print_sorting_array *** //
int print_sorting_array(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{
  const char syntax[] = "";

  if (argc != 1)
  {
    Tcl_WrongNumArgs(interp, 1, argv, syntax);
    return TCL_ERROR;
  }

  dump_sorting_array();

  return TCL_OK;
}

// *** read_points *** //
int read_drawbuffer_output(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
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

  parse_drawbuffer_output_file(fp);
  fclose(fp);

  printf(GRN"Elements have been parsed and stored!"NRM"\r\n");
  
  return TCL_OK;
}