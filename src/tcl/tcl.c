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
  Tcl_CreateObjCommand(interpreter, "find_nearest_neighbors", find_nearest_neighbors, NULL, NULL);
  Tcl_CreateObjCommand(interpreter, "find_K_nearest_neighbors", find_K_nearest_neighbors, NULL, NULL);
  Tcl_CreateObjCommand(interpreter, "find_neighbors_within_radius", find_neighbors_within_radius, NULL, NULL);
  Tcl_CreateObjCommand(interpreter, "print_sorting_array", print_sorting_array, NULL, NULL);
  Tcl_CreateObjCommand(interpreter, "read_drawbuffer_output", read_drawbuffer_output, NULL, NULL);
}

// *** less *** //
// command: less //
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
// command: cat //
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
// command: read_points //
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
// command: print_element_array //
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
// command: create_kdTree //
int create_kdTree(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{
  const char syntax[] = "";

  if (argc != 1)
    {
      Tcl_WrongNumArgs(interp, 1, argv, syntax);
      return TCL_ERROR;
    }
  
  #ifdef DEBUGMODE
    {
      clock_t starttime, endtime;
      
      starttime = clock();
      create_KD_tree();
      endtime = clock();
      printf(RED"DEBUG: "NRM"Execution time: %lf\r\n", (double)(endtime-starttime)/CLOCKS_PER_SEC);
    }
  #else
    {
      create_KD_tree();
    }
  #endif

  return TCL_OK;
}

// *** creat_kdTree *** //
// command: print_kdTree //
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
// command: find_NN //
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

  Tcl_GetDoubleFromObj(interp, argv[1], &x);
  Tcl_GetDoubleFromObj(interp, argv[2], &x);

  // If the Element Array is empty //
  if (elementarraysize == 0)
    {
      // Print a cooresponding message //
      printf(YEL"Element Array is empty!\r\n"NRM);
      return TCL_OK;
    }

  // If the KD Tree is empty //
  if (kdtreearraysize == 0)
    {
      // Print a cooresponding message //
      printf(YEL"The KD-Tree is missing for the execution of the algorithm!\r\n"NRM);
      return TCL_OK;
    }

  #ifdef DEBUGMODE
    {
      clock_t starttime, endtime;

      dump_element_distances(x, y);
    
      starttime = clock();
      dump_nearest_neighbor(x, y);
      endtime = clock();
      printf(RED"DEBUG: "NRM"Execution time: %lf\r\n", (double)(endtime-starttime)/CLOCKS_PER_SEC);
    }
  #else
    {
      dump_nearest_neighbor(x, y);
    }
  #endif

  return TCL_OK;
}

// *** clear_elements *** //
// command: clear_elements //
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

// *** find_nearest_neighbors *** //
// command: find_nearest_neighbors //
int find_nearest_neighbors(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{
  const char syntax[] = "<-noofneighbors <noof_neighbors> | -radius <radius>> -point <x-coord> <y-coord>";
  char *flag = NULL;
  long noofneighbors = 0;
  double radius = 0;
  double x = 0;
  double y = 0;

  if (argc != 6)
    {
      Tcl_WrongNumArgs(interp, 1, argv, syntax);
      return TCL_ERROR;
    }

  // If the Element Array is empty //
  if (elementarraysize == 0)
    {
      // Print a cooresponding message //
      printf(YEL"Element Array is empty!\r\n"NRM);
      return TCL_OK;
    }
  
  // If the KD tree is empty //
  if (kdtreearraysize == 0)
    {
      // Print a cooresponding message //
      printf(YEL"The KD-Tree is missing for the execution of the algorithm!\r\n"NRM);
      return TCL_OK;
    }

  flag = Tcl_GetString(argv[1]);
  Tcl_GetDoubleFromObj(interp, argv[4], &x);
  Tcl_GetDoubleFromObj(interp, argv[5], &y);

  // Check if the given flag is the one for knn //
  if (strcmp(flag, "-noofneighbors") == 0)
    {
      Tcl_GetLongFromObj(interp, argv[2], &noofneighbors);

      // If the number of neighbors is less than 1 //
      if (noofneighbors <= 0)
        {
          // Print error //
          printf("Error: The number of neighbors must be greater than 0!\r\n");
          return TCL_ERROR;
        }
      // If the number of neighbors is 1 //
      else if (noofneighbors == 1)
        {
          // Call the nearest neighbor algorithm //
          #ifdef DEBUGMODE
            {
              clock_t starttime, endtime;

              dump_element_distances(x, y);
           
              starttime = clock();
              dump_nearest_neighbor(x, y);
              endtime = clock();
              printf(RED"DEBUG: "NRM"Execution time: %lf\r\n", (double)(endtime-starttime)/CLOCKS_PER_SEC);
            }
          #else
            {
              dump_nearest_neighbor(x, y);
            }
          #endif
        }
      // If the number of neighbors is greater than 1 //
      else
        {
          // Call the algorithm forr the k nearest neighbors //
          #ifdef DEBUGMODE
            {
              clock_t starttime, endtime;

              dump_element_distances(x, y);

              starttime = clock();
              dump_k_nearest_neighbors(x, y, noofneighbors);
              endtime = clock();
              printf(RED"DEBUG: "NRM"Execution time: %lf\r\n", (double)(endtime-starttime)/CLOCKS_PER_SEC);
            }
          #else
            {
              dump_k_nearest_neighbors(x, y, noofneighbors);
            }
          #endif
        }
    }
  // Check if the given flag is the one for check within radius //
  else if (strcmp(flag, "-radius") == 0)
    {
      Tcl_GetDoubleFromObj(interp, argv[2], &radius);

      if (radius <= 0)
        {
          printf("Error: The radius must be greater than 0!\r\n");
          return TCL_ERROR;
        }
      else
        {
          #ifdef DEBUGMODE
            {
              clock_t starttime, endtime;

              dump_element_distances(x, y);
    
              starttime = clock();
              dump_nearest_neighbors_within_radius(x, y, radius);
              endtime = clock();
              printf(RED"DEBUG: "NRM"Execution time: %lf\r\n", (double)(endtime-starttime)/CLOCKS_PER_SEC);
            }
          #else
            {
              dump_nearest_neighbors_within_radius(x, y, radius);
            }
          #endif
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

// *** find_K_nearest_neighbors *** //
// command: find_K_nearest_neighbors //
int find_K_nearest_neighbors(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{
  const char syntax[] = "-noofneighbors <noof_neighbors> -point <x-coord> <y-coord>";
  long noofneighbors = 0;
  double x = 0;
  double y = 0;

  if (argc != 6)
    {
      Tcl_WrongNumArgs(interp, 1, argv, syntax);
      return TCL_ERROR;
    }

  Tcl_GetLongFromObj(interp, argv[2], &noofneighbors);
  Tcl_GetDoubleFromObj(interp, argv[4], &x);
  Tcl_GetDoubleFromObj(interp, argv[5], &y);

  // If the Element Array is empty //
  if (elementarraysize == 0)
    {
      // Print a cooresponding message //
      printf(YEL"Element Array is empty!\r\n"NRM);
      return TCL_OK;
    }
  
  // If the KD tree is empty //
  if (kdtreearraysize == 0)
    {
      // Print a cooresponding message //
      printf(YEL"The KD-Tree is missing for the execution of the algorithm!\r\n"NRM);
      return TCL_OK;
    }

  // If the number of neighbors is less than 1 // 
  if (noofneighbors <= 0)
    {
      // Print error //
      printf("Error: The number of neighbors must be greater than 0\r\n");
      return TCL_ERROR;
    }
  // If the number of neighbors is 1 //
  else if (noofneighbors == 1)
    {
      // Call the nearest neighbor algorithm //
      #ifdef DEBUGMODE
        {
          clock_t starttime, endtime;

          dump_element_distances(x, y);
          
          starttime = clock();
          dump_nearest_neighbor(x, y);
          endtime = clock();
          printf(RED"DEBUG: "NRM"Execution time: %lf\r\n", (double)(endtime-starttime)/CLOCKS_PER_SEC);
        }
      #else
        {
          dump_nearest_neighbor(x, y);
        }
      #endif
    }
  // Else //
  else
    {
      // Call the algorithm forr the k nearest neighbors //
      #ifdef DEBUGMODE
        {
          clock_t starttime, endtime;

          dump_element_distances(x, y);

          starttime = clock();
          dump_k_nearest_neighbors(x, y, noofneighbors);
          endtime = clock();
          printf(RED"DEBUG: "NRM"Execution time: %lf\r\n", (double)(endtime-starttime)/CLOCKS_PER_SEC);
        }
      #else
        {
          dump_k_nearest_neighbors(x, y, noofneighbors);
        }
      #endif
    }

  return TCL_OK;
}

// *** find_neighbors_within_radius *** //
// command: find_neighbors_within_radius //
int find_neighbors_within_radius(ClientData clientdata, Tcl_Interp *interp, int argc, Tcl_Obj *const argv[])
{
  const char syntax[] = "[-radius <radius> -point <x-coord> <y-coord>";
  double radius = 0;
  double x = 0;
  double y = 0;

  if (argc != 6)
    {
      Tcl_WrongNumArgs(interp, 1, argv, syntax);
      return TCL_ERROR;
    }

  // If the Element Array is empty //
  if (elementarraysize == 0)
    {
      // Print a cooresponding message //
      printf(YEL"Element Array is empty!\r\n"NRM);
      return TCL_OK;
    }
  
  // If the KD tree is empty //
  if (kdtreearraysize == 0)
    {
      // Print a cooresponding message //
      printf(YEL"The KD-Tree is missing for the execution of the algorithm!\r\n"NRM);
      return TCL_OK;
    }

  Tcl_GetDoubleFromObj(interp, argv[2], &radius);
  Tcl_GetDoubleFromObj(interp, argv[4], &x);
  Tcl_GetDoubleFromObj(interp, argv[5], &y);

  // If the Element Array is empty //
  if (elementarraysize == 0)
    {
      // Print a cooresponding message //
      printf(YEL"Element Array is empty!\r\n"NRM);
      return TCL_OK;
    }

  if (radius <= 0)
    {
      return TCL_ERROR;
    }
  else
    {
      #ifdef DEBUGMODE
        {
          clock_t starttime, endtime;

          dump_element_distances(x, y);

          starttime = clock();
          dump_nearest_neighbors_within_radius(x, y, radius);
          endtime = clock();
          printf(RED"DEBUG: "NRM"Execution time: %lf\r\n", (double)(endtime-starttime)/CLOCKS_PER_SEC);
        }
      #else
        {
          dump_nearest_neighbors_within_radius(x, y, radius);
        }
      #endif
    }

  return TCL_OK;
}

// *** print_sorting_array *** //
// command: print_sorting_array //
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

// *** read_drawbuffer_output *** //
// command: read_drawbuffer_output //
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