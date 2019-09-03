#include "structs.h"

void initialise_globals()
{
  // Initialize the max coordinates //
  elementmaxxcoordinate = -1;
  elementmaxycoordinate = -1;

  // Initialize the KD-Tree //
  kdtree = NULL;
  kdtreearraysize = 0;

  // Initialize the Sorting Array //
  sortingarray = NULL;
  sortingarraysize = 0;

  // Initialize the Elements' Array //
  elementarray = NULL;
  elementarraysize = 0;

  // Initialize the index to the Elements' Array //
  elementarrayindex = 0;
}

// *** euclidean_distance *** //
// Find the Euclidean distance between two points //
inline double euclidean_distance(double x1, double y1, double x2, double y2)
{ 
  return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

// *** point_x_comparator *** //
// Comparator function for qsort() //
// Compare the x coordinate of the two compared  //
// points and decise their positions accordingly //
int point_x_comparator(const void *p1, const void *p2)
{
  unsigned long index1 = *(unsigned long *)p1;
  unsigned long index2 = *(unsigned long *)p2;
  double x1, x2;

  // Take the representative x coordinate for the first element //
  // If the element is a point //
  if ((elementarray + index1)->type == point)
    {
      // Then its coordinate is the one given from the field x //
      x1 = (elementarray + index1)->x;
    }
  // If the element is not a point //
  else
    {
      // Then the coordinate used is the one from its center, since the x field is the coordinate of the top left corner //
      x1 = (elementarray + index1)->x + ((elementarray + index1)->width / 2);
    }

  // Take the representative x coordinate for the second element //
  // If the element is a point //
  if ((elementarray + index2)->type == point)
    {
      // Then its coordinate is the one given from the field x //
      x2 = (elementarray + index2)->x;
    }
  // If the element is not a point //
  else
    {
      // Then the coordinate used is the one from its center, since the x field is the coordinate of the top left corner //
      x2 = (elementarray + index2)->x + ((elementarray + index2)->width / 2);
    }

  // Check if the first element's coordinate is after the second's //
  if (isgreater(x1, x2) == 1)
    {
      return 1;
    }
  // If not check if the second's one has greater value //
  else if (isless(x1, x2) == 1)
    {
      return -1;
    }
  // If not either, then they have the same x coordinate //
  else 
    {
      return 0;
    }
}

// *** point_y_comparator *** //
// Comparator function for qsort() //
// Compare the y coordinate of the two compared  //
// points and decise their positions accordingly //
int point_y_comparator(const void *p1, const void *p2)
{
  unsigned long index1 = *(unsigned long *)p1;
  unsigned long index2 = *(unsigned long *)p2;
  double y1, y2;

  // Take the representative y coordinate for the first element //
  // If the element is a point //
  if ((elementarray + index1)->type == point)
    {
      // Then its coordinate is the one given from the field y //
      y1 = (elementarray + index1)->y;
    }
  // If the element is not a point //
  else
    {
      // Then the coordinate used is the one from its center, since the y field is the coordinates of the top left corner //
      y1 = (elementarray + index1)->y + ((elementarray + index1)->height / 2);
    }

  // Take the representative y coordinate for the second element //
  // If the element is a point //
  if ((elementarray + index2)->type == point)
    {
      // Then its coordinate is the one given from the field y //
      y2 = (elementarray + index2)->y;
    }
  // If the element is not a point //
  else
    {
      // Then the coordinate used is the one from its center, since the y field is the coordinate of the top left corner //
      y2 = (elementarray + index2)->y + ((elementarray + index2)->height / 2);
    }

  // Check if the first element's coordinate is after the second's //
  if (isgreater(y1, y2) == 1)
    {
      return 1;
    }
  // If not check if the second's one has greater value //
  else if (isless(y1, y2) == 1)
    {
      return -1;
    }
  // If not either, then they have the same x coordinate //
  else 
    {
      return 0;
    }
}

// *** distance_comparator *** //
// Comparator function for qsort() //
// Compare the distances of the two compared points to //
// given point and decise their positions accordingly  //
int distance_comparator(const void *p1, const void *p2)
{
  unsigned long index1 = *(sortingarray + *(unsigned long *)p1);
  unsigned long index2 = *(sortingarray + *(unsigned long *)p2);
  double distance1, distance2;
  double tempelementx, tempelementy;

  // Take the distance from the first element //
  // If the element is a point //
  if (elementarray[index1].type == point)
    {
      // Then its coordinates are the ones given from the fields x, y //
      tempelementx = (elementarray + index1)->x + ((elementarray + index1)->width / 2);
      tempelementy = (elementarray + index1)->y + ((elementarray + index1)->height / 2);
    }
  // If the element is not a point //
  else
    {
      // Then the coordinates used are the ones from its center, since the x, y fields are the coordinates of the top left corner //
      tempelementx = (elementarray + index1)->x;
      tempelementy = (elementarray + index1)->y;
    }
  distance1 = euclidean_distance(refpointx, refpointy, tempelementx, tempelementy);

  // Take the distance from the second element //
  // If the element is a point //
  if (elementarray[index2].type == point)
    {
      // Then its coordinates are the ones given from the fields x, y //
      tempelementx = (elementarray + index2)->x;
      tempelementy = (elementarray + index2)->y;
    }
  // If the element is not a point //
  else
    {
      // Then the coordinates used are the ones from its center, since the x, y fields are the coordinates of the top left corner //
      tempelementx = (elementarray + index2)->x + ((elementarray + index2)->width / 2);
      tempelementy = (elementarray + index2)->y + ((elementarray + index2)->height / 2);
    }
  distance2 = euclidean_distance(refpointx, refpointy, tempelementx, tempelementy);

  // Check if the distance of the first element is greater than the second's //
  if (isgreater(distance1, distance2) == 1)
    {
      return 1;
    }
  // If not check if the distance of the second element's distance is greater //
  else if (isless(distance1, distance2) == 1)
    {
      return -1;
    }
  // If not either, then they have the same distance //
  else
    {
      return 0;
    }
}

// *** init_element_array *** //
// Initialize the Element Array //
void init_element_array(unsigned long size)
{
  // If the Elements Array is not empty //
  if (elementarray != NULL)
  {
    // Free it //
    free_element_array();
  }

  // Initialize the array //
  elementarray = (struct knnelement *) calloc(size, sizeof(struct knnelement));
  assert(elementarray != NULL);

  elementarraysize = size;

  elementarrayindex = 0;
}

// *** insert_element *** //
// Insert an element into the Element Array //
void insert_element(char *name, double x, double y, int type, double width, double height)
{
  // Add the name and coordinates to the new element //
  (elementarray + elementarrayindex)->name = name;
  (elementarray + elementarrayindex)->x = x;
  (elementarray + elementarrayindex)->y = y;

  // Check the type for the new element and assign a corresponding type from those used //
  if (type == -1)
  {
    (elementarray + elementarrayindex)->type = point;
  }
  else if (type == 0)
  {
    (elementarray + elementarrayindex)->type = cell;
  }
  else if (type == 1)
  {
    (elementarray + elementarrayindex)->type = IO;
  }
  else if (type == 4)
  {
    (elementarray + elementarrayindex)->type = module;
  }
  else
  {
    (elementarray + elementarrayindex)->type = other;
  }

  // Add the width and hwight of the new element //
  (elementarray + elementarrayindex)->width = width;
  (elementarray + elementarrayindex)->height = height;

  // Increase the index for the Elements' Array //
  elementarrayindex++;
}

// *** void free_element_array *** //
// Free the element array and reset its variables //
void free_element_array()
{
  unsigned long i;

  // If the Array is empty //
  if (elementarraysize == 0)
    {
      // Do nothing //
      return;
    }

  // For each element of the array free its name //
  for (i = 0; i < elementarraysize; i++)
    {
      free((elementarray + i)->name);
    }

  free(elementarray);

  elementarray = NULL;
  elementarraysize = 0;
  elementarrayindex = 0;
}

// *** dump_point_hash *** //
// Print the content of the Array //
void dump_element_array()
{
  unsigned long i;

  // If the Array is empty //
  if (elementarraysize == 0)
    {
      // Print a correspoinding message //
      printf(YEL"Element Array is empty!\r\n"NRM);
      return;
    }

  // For each element of the Array //
  for (i = 0; i < elementarraysize; i++)
    {
      printf(BLU"Element:\r\n"NRM);

      printf(MAG"\tName:     "NRM"%s\r\n", (elementarray + i)->name);
      printf(MAG"\tCoords: ("NRM"%.4lf"MAG","NRM"%.4lf"MAG")\r\n"NRM, (elementarray + i)->x, (elementarray + i)->y);
      printf(MAG"\tType:     "NRM);

      if ((elementarray + i)->type == point)
        {
          printf("Point\r\n");
        }
      else if ((elementarray + i)->type == cell)
      {
        printf("Cell\r\n");
      }
      else if ((elementarray + i)->type == IO)
        {
          printf("IO\r\n");
        }
      else if ((elementarray + i)->type == module)
        {
          printf("Module\r\n");
        }
      else
        {
          printf("Other\r\n");
        }
    }
}

// *** void dump_element_distances *** //
// Print the distances of the elements //
// of the Array from the given point   //
// The function is used for debugging  //
void dump_element_distances(double x, double y)
{
  unsigned long i;
  double distance;
  double tempelementx, tempelementy;

  // For each element of the array //
  for (i = 0; i < elementarraysize; i++)
    {
      // Find its distance to the given point and print it //
      if ((elementarray + i)->type == point)
        {
          tempelementx = (elementarray + i)->x;
          tempelementy = (elementarray + i)->y;
        }
      else
        {
          tempelementx = (elementarray + i)->x + ((elementarray + i)->width / 2);
          tempelementy = (elementarray + i)->y + ((elementarray + i)->height / 2);
        }
      distance = euclidean_distance(x, y, tempelementx, tempelementy);
      
      printf(RED"DEBUG: "NRM"Distance from %s: %lf\r\n", (elementarray + i)->name, distance);
    }
}

// *** get_element_name *** //
// Return the name of the given element       //
// This function is used primarily to avoid   //
// exporting the element array to other files //
inline char *get_element_name(unsigned long index)
{
  return (elementarray + index)->name;
}

// *** get_element_x *** //
// Return the x coordinate of the given element //
// This function is used primarily to avoid     //
// exporting the element array to other files   //
inline double get_element_x(unsigned long index)
{
  return (elementarray + index)->x;
}

// *** get_element_y *** //
// Return the y coordinate of the given element //
// This function is used primarily to avoid     //
// exporting the element array to other files   //
inline double get_element_y(unsigned long index)
{
  return (elementarray + index)->y;
}

// *** get_element_width *** //
// Return the width of the given element      //
// This function is used primarily to avoid   //
// exporting the element array to other files //
inline double get_element_width(unsigned long index)
{
  return (elementarray + index)->width;
}

// *** get_element_height *** //
// Return the height of the given element     //
// This function is used primarily to avoid   //
// exporting the element array to other files //
inline double get_element_height(unsigned long index)
{
  return (elementarray + index)->height;
}

// *** get_element_type *** //
// Return the type of the given element       //
// This function is used primarily to avoid   //
// exporting the element array to other files //
int get_element_type(unsigned long index)
{
  return (elementarray + index)->type;
}

// *** create_sorting_array *** //
// Create an array that "points" into elements of the Elements' Array. This array    //
// is used by the KD tree, sorting it partially or fully depending on the axis used. //
void create_sorting_array()
{
  unsigned long i, j;

  // First Pass - Find the size of the array by finding the //
  // element that are going to be used by the NN Algoriths  //
  for (i = 0; i < elementarraysize; i++)
    {
      // In this stage of development, all elements described by // 
      // the type "other" are excluded from the NN Algorithms    //
      if ((elementarray + i)->type != other)
        {
          sortingarraysize++;
        }
    }

  // Allocate memory for the array //
  sortingarray = (unsigned long *) calloc(sortingarraysize, sizeof(unsigned long));
  assert(sortingarray != NULL);

  // Second Pass - Fill the array //
  // Run through the Elements' Array //
  for (i = 0, j = 0; i < elementarraysize; i++)
    {
      // If an element is of the types allowed by the  //
      // NN Algorithms, insert it to the Sorting Array //
      if ((elementarray + i)->type != other)
        {
          *(sortingarray + j) = i;
          j++;
        }
    }
}

// *** free_sorting_array *** //
// Free the sorting array and reset its variables //
void free_sorting_array()
{
  // If the Array is empty //
  if (sortingarray == NULL)
    {
      // Do nothing //
      return;
    }

  // Free the array //
  free(sortingarray);

  // Reset its variables //
  sortingarray = NULL;
  sortingarraysize = 0;
}

// *** dump_sorting_array *** //
// Print the content of the sorting array //
void dump_sorting_array()
{
  unsigned long i;
  unsigned long sortingarrayindex;

  // For each point referred into the array //
  for (i = 0; i < sortingarraysize; i++)
    {
      // Print its name and coordinates //
      sortingarrayindex = *(sortingarray + i);
      printf(BLU"Point: "NRM"%s\r\n", (elementarray + sortingarrayindex)->name);
      printf(MAG"\t("NRM"%.2lf"MAG","NRM"%.2lf"MAG")\r\n"NRM, (elementarray + sortingarrayindex)->x, (elementarray + sortingarrayindex)->y);
    }
}

// *** create_KD_tree *** //
// Cretes a KD Tree that will be used for the NN Algorithms.     //
// Create the Sorting Array that is used for the KD Tree. Then   //
// create the root node of the KD Tree. If the sorting array     //
// exceeds the maximum given size, sort the Sorting Array to its //
// predetermined axis, double the Tree's size and create two     //
// new nodes, each one having to use half of the Sorting Array.  //
void create_KD_tree()
{
  unsigned long noofelements = 0;
  unsigned long split = 0;

  // Free the existing tree //
  free_KD_tree();
  
  // Free the existing sorting array & create a new one //
  free_sorting_array();
  create_sorting_array();

  // Create the tree //
  // The first element of the array will remain empty and is  not used for easier traversal //
  kdtree = (struct kdtreenode *) calloc(2, sizeof(struct kdtreenode));
  assert(kdtree != NULL);
  kdtreearraysize = 2;

  // Initialise the fields of the root //
  (kdtree + 1)->axis = X_AXIS; // Chose the x axis as the first one to check //
  (kdtree + 1)->startindex = 0;
  (kdtree + 1)->endindex = sortingarraysize - 1;

  // If the elements in the sorting array are more than the defined max //
  noofelements = (kdtree + 1)->endindex - (kdtree + 1)->startindex + 1;
  if (noofelements > MAX_KDLEAF_ELEMENTS)
    {
      // Mark the node as a non leaf one //
      (kdtree + 1)->isleaf = 0;

      // Sort the array based on the x coordinate //
      qsort((void *)sortingarray, sortingarraysize, sizeof(struct pointHashElement *), point_x_comparator);
    
      // Save the split //
      split = (sortingarraysize - 1) / 2;
      (kdtree + 1)->splitindex = *(sortingarray + split);

      // Increase the tree size //
      kdtree = (struct kdtreenode *) realloc(kdtree, ((2 * kdtreearraysize) * sizeof(struct kdtreenode)));;
      assert(kdtree != NULL);
      kdtreearraysize *= 2; // The array that correspondes to the tree has to be doubled in size //

      // Fill the children nodes //
      // Create a sub tree for the left child //
      insert_KD_tree_node(2, (kdtree + 1)->startindex, split);
      // Create a sub tree for the right child //
      insert_KD_tree_node(3, (split + 1), (kdtree + 1)->endindex);
    }
  // If he elements in the sorting array are less than or equal the max value //
  else
    {
      // Mark the root node as a leaf one //
      (kdtree + 1)->isleaf = 1;
    }
}

// *** insert_KD_tree_node *** //
// Insert a new node to the KD tree. This node checks the given subarray if it //
// exceeds the maximum number of elements. If it does, the subarray sorted in  //
// the different of its parent axis, then it is split and the node calls two   //
// new nodes, each one handling a half of the subarray. If the subarray does   //
// not exceed the max value, then the node is characterised as a leaf one.     //
void insert_KD_tree_node(unsigned long index, unsigned long startindex, unsigned long endindex)
{
  unsigned long noofelements = 0;
  unsigned long split = 0;
  unsigned long parent = 0;

  // Fill the new node //
  (kdtree + index)->startindex = startindex;
  (kdtree + index)->endindex = endindex;

  // Find the number of elements that correspond to this node //
  noofelements = endindex - startindex + 1;

  // If the node has more elements than the defined max //
  if (noofelements > MAX_KDLEAF_ELEMENTS)
    {
      // Mark the node as a non leaf one //
      (kdtree + index)->isleaf = 0;
      
      // Define the axis as the one not used by the parent //
      parent = index / 2;
      if (kdtree[parent].axis == X_AXIS)
        {
          (kdtree + index)->axis = Y_AXIS;
        }
      else
        {
          (kdtree + index)->axis = X_AXIS;
        }

      // Sort the sub-array depending on the axis //
      if ((kdtree + index)->axis == Y_AXIS)
        {
          qsort((void *)&sortingarray[startindex], noofelements, sizeof(struct pointHashElement *), point_x_comparator);
        }
      else
        {
          qsort((void *)&sortingarray[startindex], noofelements, sizeof(struct pointHashElement *), point_y_comparator);
        }

      // Check if the tree has enough space for the new nodes. If the index of the left //
      // new node is greater or equal to the size of the tree, double the tree size.    //
      if ((2 * index) >= kdtreearraysize)
        {
          kdtree = (struct kdtreenode *) realloc(kdtree, ((2 * kdtreearraysize) * sizeof(struct kdtreenode)));
          assert(kdtree != NULL);
          kdtreearraysize *= 2;
        }

      // Find the index of the split //
      split = startindex + ((endindex - startindex) / 2);

      // Save the split //
      (kdtree + index)->splitindex = *(sortingarray + split);

      // Create a sub tree for the left child //
      insert_KD_tree_node((2 * index), startindex, split);
      // Create a sub tree for the right child //
      insert_KD_tree_node(((2 * index) + 1), (split + 1), endindex);
    }
  else
    {
      // Mark the node as a leaf one //
      (kdtree + index)->isleaf = 1;
    }
}

// *** free_KD_tree *** //
// Free the KD Tree //
void free_KD_tree()
{
  // If the kd tree is empty //
  if (kdtree == NULL)
    {
      // Do nothing //
      return;
    }

  // Free the kd tree //
  free(kdtree);
  kdtree = NULL;

  kdtreearraysize = 0;
}

// *** dump_KD_tree *** //
// Print the KD tree and its content. //
void dump_KD_tree()
{
  if (kdtree == NULL)
    {
      printf(YEL"KD Tree Empty!\r\n"NRM);
      return;
    }

  printf(MAG"Root:\r\n"NRM);
  dump_KD_node(1, 0);
}

// *** dump_tabs *** //
// Supplementary function that prints a certain number of tabs. It //
// is used conjuction with dump_KD_node() to print the KD tree.   //
inline void dump_tabs(int depth)
{
  int i;

  for (i = 0; i < depth; i++)
    {
      putchar('\t');
    }
}

// *** dump_KD_node *** //
// Print a given node of the KD tree in a certain depth. //
// It prints the node's start & end indexes of its       //
// subarray over the Sorting Array. If it not a leaf     //
// node, it is recursively called to print its childen.  //
void dump_KD_node(unsigned long index, int depth)
{
  // Pint the start index of the node's sub-array //
  dump_tabs(depth);
  printf(BLU"Start Index: "NRM"%lu\r\n", (kdtree + index)->startindex);

  // Pint the end index of the node's sub-array //
  dump_tabs(depth);
  printf(BLU"End Index:   "NRM"%lu\r\n", (kdtree + index)->endindex);

  // Check if the node is leaf //
  if ((kdtree + index)->isleaf == 1)
    {
      // Print a corresponding message //
      dump_tabs(depth);
      printf(MAG"\tLeaf node!\r\n");
    }
  // If it is not, print its children //
  else
    {
      // First the left child //
      dump_tabs(depth);
      printf(MAG"\tLeft child:\r\n");
      dump_KD_node((2 * index), (depth + 1));

      // Then the right //
      dump_tabs(depth);
      printf(MAG"\tRight child:\r\n");
      dump_KD_node((2 * index + 1), (depth + 1));
    }
}

// *** find_nearest_neighbor *** //
// Find the nearest neighbor element of the given set of coordinates.             //
// Traverses through the KD-Tree until it finds the leaf which has elements in    //
// an area where the given coordinates fit. On this leaf it checks the distances  //
// off all the elements from the given point and returns the nearest. While       //
// traversing backwards the tree towards the root, check on each node if the      //
// distance of the current nearest neighbor is less or equal the vertical         //
// distance between the split and the given point. If it is, the traverse the     //
// unexplored child of this node, until you find the nearest neighbor from that   //
// path. Then compare the distances of the two elements and keep the nearest one. //
unsigned long find_nearest_neighbor(unsigned long index, double x, double y)
{
  double mindistance = 0;              // The minimum distance found //
  double distance = 0;                 // Variable used for distance calculations //
  double splitelementx = 0;            // Temporary variable used to for the x-coordinate of the split element //
  double splitelementy = 0;            // Temporary variable used to for the y-coordinate of the split element //
  double tempelementx = 0;             // Temporary variable used to for the x-coordinate of an element //
  double tempelementy = 0;             // Temporary variable used to for the y-coordinate of an element //
  unsigned long minindex = 0;          // The index of the element with the minimum distance //
  unsigned long secondindex = 0;       // The index of the minimum distance from the node's other child //
  unsigned long splitindex;            // The index of the element used for the split //
  unsigned long sortingarrayindex = 0; // A field of the Sorting Array that is used as index to the Elements' Array //
  unsigned long i;

  // Check if the node is a leaf one //
  if ((kdtree + index)->isleaf == 1)
    {
      // Find the point with the smallest distance between the elements of the leaf //
      // Start by assuming the first element has the smallest distance //
      minindex = (kdtree + index)->startindex;
      sortingarrayindex = *(sortingarray + minindex);
      // If the element is a point //
      if ((elementarray + sortingarrayindex)->type == point)
        {
          // Then its coordinates are the ones given from the fields x, y //
          tempelementx = (elementarray + sortingarrayindex)->x;
          tempelementy = (elementarray + sortingarrayindex)->y;
        }
      // If the element is not a point //
      else
        {
          // Then the coordinates used are the ones from its center, since the x, y fields are the coordinates of the top left corner // 
          tempelementx = (elementarray + sortingarrayindex)->x + ((elementarray + sortingarrayindex)->width / 2);
          tempelementy = (elementarray + sortingarrayindex)->y + ((elementarray + sortingarrayindex)->height / 2);
        }
      mindistance = euclidean_distance(x, y, tempelementx, tempelementy);
      
      // Run through the rest of the elements of the subarray //
      for (i = ((kdtree + index)->startindex + 1); i <= (kdtree + index)->endindex; i++)
        {
          // Find the distance of the element and the given point //
          sortingarrayindex = *(sortingarray + i);
          // If the element is a point //
          if ((elementarray + sortingarrayindex)->type == point)
            {
              // Then its coordinates are the ones given from the fields x, y //
              tempelementx = (elementarray + sortingarrayindex)->x;
              tempelementy = (elementarray + sortingarrayindex)->y;
            }
          // If the element is not a point //
          else
            {
              // Then the coordinates used are the ones from its center, since the x, y fields are the coordinates of the top left corner // 
              tempelementx = (elementarray + sortingarrayindex)->x + ((elementarray + sortingarrayindex)->width / 2);
              tempelementy = (elementarray + sortingarrayindex)->y + ((elementarray + sortingarrayindex)->height / 2);
            }
          distance = euclidean_distance(x, y, tempelementx, tempelementy);

          // If the element's distance is less than the current minimum distance //
          if (isless(distance, mindistance) == 1)
            {
              // Change the minimum distance as well as the corresponding point //
              mindistance = distance;
              minindex = i;
            }
        }

      #ifdef DEBUGMODE
        {
          sortingarrayindex = *(sortingarray + minindex);
          printf(RED"DEBUG: "NRM"Nearest Neighbor on this leaf: %s\r\n", (elementarray + sortingarrayindex)->name);
        }
      #endif
    }
  // If the node is not a leaf one //
  else
    {
      // Find the split element //
      splitindex = (kdtree + index)->splitindex;

      // Check the axis of the node //
      // If the node's elements have been sorted based on the X axis //
      if ((kdtree + index)->axis == X_AXIS)
        {
          // Calculate the x-coordinate of the split element //
          // If the split element is a point //
          if ((elementarray + splitindex)->type == point)
            {
              // Its x-coordinate is the one given //
              splitelementx = (elementarray + splitindex)->x;
            }
          // If the split element is not a point //
          else
            {
              // Its x-coordinate is the one given plus walf the given width //
              splitelementx = (elementarray + splitindex)->x + ((elementarray + splitindex)->width / 2);
            }

          // If the given x coordinate is after the split element //
          if (isgreater(x, splitelementx))
            {
              // Find the nearest neighbor on this half //
              minindex = find_nearest_neighbor(((2 * index) + 1), x, y);
              
              // Find the distance of the nearest neighbor //
              sortingarrayindex = *(sortingarray + minindex);
              // If the nearest element is a point //
              if ((elementarray + sortingarrayindex)->type == point)
                {
                  // Its coordinates are the one given //
                  tempelementx = (elementarray + sortingarrayindex)->x;
                  tempelementy = (elementarray + sortingarrayindex)->y;
                }
              // If the nearest element is not a point //
              else
                {
                  // Its are on its center, since the given coordinate are those of the top left corner //
                  tempelementx = (elementarray + sortingarrayindex)->x + ((elementarray + sortingarrayindex)->width / 2);
                  tempelementy = (elementarray + sortingarrayindex)->y + ((elementarray + sortingarrayindex)->height / 2);
                }
              mindistance = euclidean_distance(x, y, tempelementx, tempelementy);

              // If the distance between the given x and the split is less than the minimum distance //
              if (islessequal(fabs(x - splitelementx), mindistance) == 1)
                {
                  // Find the nearest neighbor on the other half and compare it with the previous //
                  secondindex = find_nearest_neighbor((2 * index), x, y);
                  
                  sortingarrayindex = *(sortingarray + secondindex);
                  // If the nearest element on the second half is a point //
                  if ((elementarray + sortingarrayindex)->type == point)
                    {
                      // Its coordinates are the one given //
                      tempelementx = (elementarray + sortingarrayindex)->x;
                      tempelementy = (elementarray + sortingarrayindex)->y;
                    }
                  // If the nearest element on the second half is not a point //
                  else
                    {
                      // Its are on its center, since the given coordinate are those of the top left corner //
                      tempelementx = (elementarray + sortingarrayindex)->x + ((elementarray + sortingarrayindex)->width / 2);
                      tempelementy = (elementarray + sortingarrayindex)->y + ((elementarray + sortingarrayindex)->height / 2);
                    }
                  distance = euclidean_distance(x, y, tempelementx, tempelementy);

                  // If the new distance is less //
                  if (isless(distance, mindistance) == 1)
                    {
                      // Keep the new point as the nearest //
                      minindex = secondindex;
                    }
                }
            }
          // If the given x coordinate is before or on the split element //
          else
            {
              // Find the nearest neighbor on this half //
              minindex = find_nearest_neighbor((2 * index), x, y);
              
              // Find the distance of the nearest neighbor //
              sortingarrayindex = *(sortingarray + minindex);
              // If the nearest element is a point //
              if ((elementarray + sortingarrayindex)->type == point)
                {
                  // Its coordinates are the one given //
                  tempelementx = (elementarray + sortingarrayindex)->x;
                  tempelementy = (elementarray + sortingarrayindex)->y;
                }
              // If the nearest element is not a point //
              else
                {
                  // Its are on its center, since the given coordinate are those of the top left corner //
                  tempelementx = (elementarray + sortingarrayindex)->x + ((elementarray + sortingarrayindex)->width / 2);
                  tempelementy = (elementarray + sortingarrayindex)->y + ((elementarray + sortingarrayindex)->height / 2);
                }
              mindistance = euclidean_distance(x, y, tempelementx, tempelementy);

              // If the distance between the given x and the split is less than the minimum distance //
              if (islessequal(fabs(x - splitelementx), mindistance) == 1)
                {
                  // Find the nearest neighbor on the other half and compare it with the previous //
                  secondindex = find_nearest_neighbor(((2 * index) + 1), x, y);

                  sortingarrayindex = *(sortingarray + secondindex);
                  // If the nearest element on the second half is a point //
                  if ((elementarray + sortingarrayindex)->type == point)
                    {
                      // Its coordinates are the one given //
                      tempelementx = (elementarray + sortingarrayindex)->x;
                      tempelementy = (elementarray + sortingarrayindex)->y;
                    }
                  // If the nearest element on the second half is not a point //
                  else
                    {
                      // Its are on its center, since the given coordinate are those of the top left corner //
                      tempelementx = (elementarray + sortingarrayindex)->x + ((elementarray + sortingarrayindex)->width / 2);
                      tempelementy = (elementarray + sortingarrayindex)->y + ((elementarray + sortingarrayindex)->height / 2);
                    }
                  distance = euclidean_distance(x, y, tempelementx, tempelementy);

                  // If the new distance is less //
                  if (isless(distance, mindistance) == 1)
                    {
                      // Keep the new point as the nearest //
                      minindex = secondindex;
                    }
                }
            }
        }
      // If the node's elements have been sorted based on the Y axis //
      else
        {
          // Calculate the y-coordinate of the split element //
          // If the split element is a point //
          if ((elementarray + splitindex)->type == point)
            {
              // Its y-coordinate is the one given //
              splitelementy = (elementarray + splitindex)->y;
            }
          // If the split element is not a point //
          else
            {
              // Its y-coordinate is the one given plus walf the given height //
              splitelementy = (elementarray + splitindex)->y + ((elementarray + splitindex)->height / 2);
            }

          // If the given y coordinate is after the split //
          if (isgreater(y, splitelementy))
            {
              // Find the nearest neighbor on this half //
              minindex = find_nearest_neighbor(((2 * index) + 1), x, y);
              
              // Find the distance of the nearest neighbor //
              sortingarrayindex = *(sortingarray + minindex);
              // If the nearest element is a point //
              if ((elementarray + sortingarrayindex)->type == point)
                {
                  // Its coordinates are the one given //
                  tempelementx = (elementarray + sortingarrayindex)->x;
                  tempelementy = (elementarray + sortingarrayindex)->y;
                }
              // If the nearest element is not a point //
              else
                {
                  // Its are on its center, since the given coordinate are those of the top left corner //
                  tempelementx = (elementarray + sortingarrayindex)->x + ((elementarray + sortingarrayindex)->width / 2);
                  tempelementy = (elementarray + sortingarrayindex)->y + ((elementarray + sortingarrayindex)->height / 2);
                }
              mindistance = euclidean_distance(x, y, tempelementx, tempelementy);

              // If the distance between the given y and the split is less than the minimum distance //
              if (islessequal(fabs(y - splitelementy), mindistance) == 1)
                {
                  // Find the nearest neighbor on the other half and compare it with the previous //
                  secondindex = find_nearest_neighbor((2 * index), x, y);
                  
                  sortingarrayindex = *(sortingarray + secondindex);
                  // If the nearest element on the second half is a point //
                  if ((elementarray + sortingarrayindex)->type == point)
                    {
                      // Its coordinates are the one given //
                      tempelementx = (elementarray + sortingarrayindex)->x;
                      tempelementy = (elementarray + sortingarrayindex)->y;
                    }
                  // If the nearest element on the second half is not a point //
                  else
                    {
                      // Its are on its center, since the given coordinate are those of the top left corner //
                      tempelementx = (elementarray + sortingarrayindex)->x + ((elementarray + sortingarrayindex)->width / 2);
                      tempelementy = (elementarray + sortingarrayindex)->y + ((elementarray + sortingarrayindex)->height / 2);
                    }
                  distance = euclidean_distance(x, y, tempelementx, tempelementy);
                  
                  // If the new distance is less //
                  if (isless(distance, mindistance) == 1)
                    {
                      // Keep the new point as the nearest //
                      minindex = secondindex;
                    }
                }
            }
          // If the given y coordinate is before or on the split //
          else
            {
              // Find the nearest neighbor on this half //
              minindex = find_nearest_neighbor((2 * index), x, y);
              
              // Find the distance of the nearest neighbor //
              sortingarrayindex = *(sortingarray + minindex);
              // If the nearest element is a point //
              if ((elementarray + sortingarrayindex)->type == point)
                {
                  // Its coordinates are the one given //
                  tempelementx = (elementarray + sortingarrayindex)->x;
                  tempelementy = (elementarray + sortingarrayindex)->y;
                }
              // If the nearest element is not a point //
              else
                {
                  // Its are on its center, since the given coordinate are those of the top left corner //
                  tempelementx = (elementarray + sortingarrayindex)->x + ((elementarray + sortingarrayindex)->width / 2);
                  tempelementy = (elementarray + sortingarrayindex)->y + ((elementarray + sortingarrayindex)->height / 2);
                }
              mindistance = euclidean_distance(x, y, tempelementx, tempelementy);

               // If the distance between the given y and the split is less than the minimum distance //
              if (islessequal(fabs(y - splitelementy), mindistance) == 1)
                {
                  // Find the nearest neighbor on the other half and compare it with the previous //
                  secondindex = find_nearest_neighbor(((2 * index) + 1), x, y);
                  
                  sortingarrayindex = *(sortingarray + secondindex);
                  // If the nearest element on the second half is a point //
                  if ((elementarray + sortingarrayindex)->type == point)
                    {
                      // Its coordinates are the one given //
                      tempelementx = (elementarray + sortingarrayindex)->x;
                      tempelementy = (elementarray + sortingarrayindex)->y;
                    }
                  // If the nearest element on the second half is not a point //
                  else
                    {
                      // Its are on its center, since the given coordinate are those of the top left corner //
                      tempelementx = (elementarray + sortingarrayindex)->x + ((elementarray + sortingarrayindex)->width / 2);
                      tempelementy = (elementarray + sortingarrayindex)->y + ((elementarray + sortingarrayindex)->height / 2);
                    }
                  distance = euclidean_distance(x, y, tempelementx, tempelementy);
                  
                  // If the new distance is less //
                  if (isless(distance, mindistance) == 1)
                    {
                      // Keep the new point as the nearest //
                      minindex = secondindex;
                    }
                }
            }
        }

      #ifdef DEBUGMODE
        {
          sortingarrayindex = *(sortingarray + minindex);
          printf(RED"DEBUG: "NRM"Nearest Neighbor on this node: %s\r\n", (elementarray + sortingarrayindex)->name);
        }
      #endif
    }
  
  // Return the index of the nearest point //
  return minindex;
}

// *** dump_nearest_neighbor *** //
// Print the nearest neighbor element from a given set of coordinates.    //
// The function works as a wrapper for function find_nearest_neighbors(), //
// where its result is printed to the user.                               //
void dump_nearest_neighbor(double x, double y)
{
  unsigned long nearestneighborindex = 0; // The index of the nearest element //
  unsigned long sortingarrayindex = 0;    // A field of the Sorting Array that is used as index to the Elements' Array //

  // Find the nearest neighbor element //
  nearestneighborindex = find_nearest_neighbor(ROOT_NODE, x, y);
  
  // Print the nearest neighbor element //
  sortingarrayindex = *(sortingarray + nearestneighborindex);
  printf("\r\nNearest Neighbor: %s\r\n\n", (elementarray + sortingarrayindex)->name);
}

// *** find_nearest_neighbors_within_radius *** //
// Find the nearest neighbor elements within a radius from a given set of coordinates.    //
// Traverses through the KD-Tree until it finds the leaf which has elements in an area    //
// where the given coordinates fit. On this leaf it checks the distances off all the      //
// elements from the given point and returns all the neighbors whose distance is less or  //
// equal the given radius. If there are no neighbors within this radius, it returns NULL. //
// While traversing backwards the tree towards the root, check on each node if the given  //
// radius is less or equal the vertical distance between the split and the given point.   //
// If it is, the traverse the unexplored child of this node, and check if there are any   //
// neighbor elements within that radius. If there are, add them to the others.            //
unsigned long *find_nearest_neighbors_within_radius(unsigned long index, double x, double y, double radius, unsigned long *noofneighbors)
{
  double distance = 0;                 // Variable used for distance calculations //
  double splitelementx = 0;            // Temporary variable used to for the x-coordinate of the split element //
  double splitelementy = 0;            // Temporary variable used to for the y-coordinate of the split element //
  double tempelementx = 0;             // Temporary variable used to for the x-coordinate of an element //
  double tempelementy = 0 ;            // Temporary variable used to for the y-coordinate of an element //
  unsigned long *indexes = NULL;       // Array used for the indexes of the elements within radius //
  unsigned long *newindexes = NULL;    // Array used for the indexes of the elements within radius from the unexplored child of the node //
  unsigned long noofindexes = 0;       // Number of elements within radius //
  unsigned long noofnewindexes = 0;    // Number of elements within radius from the unexplored child of the node //
  unsigned long splitindex = 0;        // The index of the element used for the split //
  unsigned long sortingarrayindex = 0; // A field of the Sorting Array that is used as index to the Elements' Array //
  unsigned long i;

  // Check if the node is a leaf one //
  if ((kdtree + index)->isleaf == 1)
    {
      // Run through the elements of the subarray //
      for (i = (kdtree + index)->startindex; i <= (kdtree + index)->endindex; i++)
        {
          // Find the distance between point and given coordinates //
          sortingarrayindex = *(sortingarray + i);
          // If the element is a point //
          if ((elementarray + sortingarrayindex)->type == point)
            {
              // Then its coordinates are the ones given from the fields x, y //
              tempelementx = (elementarray + sortingarrayindex)->x;
              tempelementy = (elementarray + sortingarrayindex)->y;
            }
          // If the element is not a point /
          else
            {
              // Then the coordinates used are the ones from its center, since the x, y fields are the coordinates of the top left corner // 
              tempelementx = (elementarray + sortingarrayindex)->x + ((elementarray + sortingarrayindex)->width / 2);
              tempelementy = (elementarray + sortingarrayindex)->y + ((elementarray + sortingarrayindex)->height / 2);
            }
          distance = euclidean_distance(x, y, tempelementx, tempelementy);

          // If the distance is less or equal to the radius //
          if (islessequal(distance, radius) == 1)
            {
              // Add the element to the list //
              indexes = (unsigned long *) realloc(indexes, (noofindexes + 1) * sizeof(unsigned long));
              assert(indexes != NULL);

              *(indexes + noofindexes) = i;
              noofindexes++;
            }
        }

      #ifdef DEBUGMODE
        {
          // If there were neighbor elements within the radius //
          if (indexes != NULL)
            {
              printf(RED"DEBUG: "NRM"Nearest Neighbors within radius on this leaf:\r\n");

              // Print the neighbor elements //
              for (i = 0; i < noofindexes; i++)
                {
                  sortingarrayindex = *(sortingarray + *(indexes + i));
                  printf("\t%s\r\n", (elementarray + sortingarrayindex)->name);
                }
            }
          // If there were not any neighbor elements within the radius //
          else
            {
              // Print a corresponding message //
              printf(RED"DEBUG: "YEL"No neighbors within radius on this leaf!\r\n"NRM);
            }
        }
      #endif
    }
  // If the node is not a leaf one //
  else
    {
      // Find the split element //
      splitindex = (kdtree + index)->splitindex;

      // Check the axis of the node //
      // If the node's elements have been sorted based on the X axis //
      if ((kdtree + index)->axis == X_AXIS)
        {
          // Calculate the x-coordinate of the split element //
          // If the split element is a point //
          if ((elementarray + splitindex)->type == point)
            {
              // Its x-coordinate is the one given //
              splitelementx = (elementarray + splitindex)->x;
            }
          // If the split element is not a point //
          else
            {
              // Its x-coordinate is the one given plus walf the given width //
              splitelementx = (elementarray + splitindex)->x + ((elementarray + splitindex)->width / 2);
            }

          // If the given x coordinate is after the split element //
          if (isgreater(x, splitelementx))
            {
              // Find a list of elements that are within radius on this half //
              indexes = find_nearest_neighbors_within_radius(((2 * index) + 1), x, y, radius, &noofindexes);
              
              // If the distance between the given x and the split is less or equal with the radius //
              if (islessequal(fabs(x - splitelementx), radius) == 1)
                {
                  // Find if there are elements within the radius on the other half //
                  newindexes = find_nearest_neighbors_within_radius((2 * index), x, y, radius, &noofnewindexes);

                  // If there are elements within the radius on the other half //
                  if (newindexes != NULL)
                    {
                      // Add the element to the list //
                      indexes = (unsigned long *) realloc(indexes, (noofindexes + noofnewindexes) * sizeof(unsigned long));
                      assert(indexes != NULL);
                      memcpy((indexes + noofindexes), newindexes, noofnewindexes * sizeof(unsigned long));
                      noofindexes += noofnewindexes;
                      free(newindexes);
                    }
                }
            }
          // If the given x coordinate is before or on the split //
          else
            {
              // Find a list of elements that are within radius on this half //
              indexes = find_nearest_neighbors_within_radius((2 * index), x, y, radius, &noofindexes);
              
              // If the distance between the given x and the split is less or equal with the radius //
              if (islessequal(fabs(x - splitelementx), radius) == 1)
                {
                  // Find if there are elements within the range on the other half //
                  newindexes = find_nearest_neighbors_within_radius(((2 * index) + 1), x, y, radius, &noofnewindexes);

                  // If there are elements within the radius on the other half //
                  if (newindexes != NULL)
                    {
                      // Add the element to the list //
                      indexes = (unsigned long *) realloc(indexes, (noofindexes + noofnewindexes) * sizeof(unsigned long));
                      assert(indexes != NULL);
                      memcpy((indexes + noofindexes), newindexes, noofnewindexes * sizeof(unsigned long));
                      noofindexes += noofnewindexes;
                      free(newindexes);
                    }
                }
            }
        }
      // If the node's elements have been sorted based on the Y axis //
      else
        {
          // Calculate the y-coordinate of the split element //
          // If the split element is a point //
          if ((elementarray + splitindex)->type == point)
            {
              // Its y-coordinate is the one given //
              splitelementy = (elementarray + splitindex)->y;
            }
          // If the split element is not a point //
          else
            {
              // Its y-coordinate is the one given plus walf the given height //
              splitelementy = (elementarray + splitindex)->y + ((elementarray + splitindex)->height / 2);
            }

          // If the given y coordinate is after the split //
          if (isgreater(y, splitelementy))
            {
              // Find a list of elements that are within radius on this half //
              indexes = find_nearest_neighbors_within_radius(((2 * index) + 1), x, y, radius, &noofindexes);
              
              // If the distance between the given y and the split is less or equal with the radius //
              if (islessequal(fabs(y - splitelementy), radius) == 1)
                {
                  // Find if there are elements within the range on the other half //
                  newindexes = find_nearest_neighbors_within_radius((2 * index), x, y, radius, &noofnewindexes);
                  
                  // If there are elements within the radius on the other half //
                  if (newindexes != NULL)
                    {
                       // Add the element to the list //
                      indexes = (unsigned long *) realloc(indexes, (noofindexes + noofnewindexes) * sizeof(unsigned long));
                      assert(indexes != NULL);
                      memcpy((indexes + noofindexes), newindexes, noofnewindexes * sizeof(unsigned long));
                      noofindexes += noofnewindexes;
                      free(newindexes);
                    }
                }
            }
          // If the given y coordinate is before or on the split //
          else
            {
              // Find a list of elements that are within radius on this half //
              indexes = find_nearest_neighbors_within_radius((2 * index), x, y, radius, &noofindexes);
              
              // If the distance between the given x and the split is less or equal with the radius //
              if (islessequal(fabs(y - splitelementy), radius) == 1)
                {
                  // Find if there are elements within the range on the other half //
                  newindexes = find_nearest_neighbors_within_radius(((2 * index) + 1), x, y, radius, &noofnewindexes);
                  
                  // If there are elements within the radius on the other half //
                  if (newindexes != NULL)
                    {
                      // Add the element to the list //
                      indexes = (unsigned long *) realloc(indexes, (noofindexes + noofnewindexes) * sizeof(unsigned long));
                      assert(indexes != NULL);
                      memcpy((indexes + noofindexes), newindexes, noofnewindexes * sizeof(unsigned long));
                      noofindexes += noofnewindexes;
                      free(newindexes);
                    }
                }
            }
        }

      #ifdef DEBUGMODE
        {
          // If there were neighbor elements within the radius //
          if (indexes != NULL)
            {
              printf(RED"DEBUG: "NRM"Nearest Neighbors within radius on this node:\r\n");

              // Print the neighbor elements //
              for (i = 0; i < noofindexes; i++)
                {
                  sortingarrayindex = *(sortingarray + *(indexes + i));
                  printf("\t%s\r\n", (elementarray + sortingarrayindex)->name);
                }
            }
          // If there were not any neighbor elements within the radius //
          else
            {
              // Print a corresponding message //
              printf(RED"DEBUG: "YEL"No neighbors within radius on this node!\r\n"NRM);
            }
        }
      #endif
    }
  
  // Pass the number of indexes //
  *noofneighbors = noofindexes;

  // Return the array of the indexes //
  return indexes;
}

// *** dump_nearest_neighbors_within_radius *** //
// Print the nearest neighbor elements, if there are any,       //
// within a given radius from the given set of coordinates.     //
// The function works as a wrapper for function                 //
// find_nearest_neighbors_within_radius(), where its result is  //
// printed to the user, whether an element is found or not.     //
void dump_nearest_neighbors_within_radius(double x, double y, double radius)
{
  unsigned long *indexes = NULL;       // Array used for the indexes of nearest neighbors within radius //
  unsigned long noofindexes = 0;       // Number of elements within radius //
  unsigned long sortingarrayindex = 0; // A field of the Sorting Array that is used as index to the Elements' Array //
  unsigned long i = 0;

  // Search for neighbor element within the radius //
  indexes = find_nearest_neighbors_within_radius(ROOT_NODE, x, y, radius, &noofindexes);
  
  // If there were neighbor elements within the radius //
  if (indexes != NULL)
    {
      printf("\r\nNeighbors within radius:\r\n");

      // Print the neighbor elements //
      for (i = 0; i < noofindexes; i++)
        {
          sortingarrayindex = *(sortingarray + *(indexes + i));
          printf("\t%s\r\n", (elementarray + sortingarrayindex)->name);
        }
      putchar('\n');

      // Free the array with the neighbors //
      free(indexes);
    }
  // If there were not any neighbor elements within the radius //
  else
    {
      // Print a corresponding message //
      printf(YEL"\r\nNo neighbors within radius!\r\n\n"NRM);
    }
}

// *** find_k_nearest_neighbors *** //
// Find the k nearest neighbor elements from a given set of coordinates             //
// Traverses through the KD-Tree until it finds the leaf which has elements in an   //
// area where the given coordinates fit. On this leaf sort all the elements based   //
// on their distance from the given point. If the number of elements on the leaf    //
// exceed the required number, return only the k nearest. While traversing          //
// backwards the tree towards the root, check on each node if the distance of the   //
// furthest of the nearest neighbors is less or equal the vertical distance between //
// the split and the given point. If it is, the traverse the unexplored child of    //
// this node, until you find the k nearest neighbors from that path. After that,    //
// sort all the found neighbors based on their distances from the given point. If   //
// their number exceeds k, keep only the k nearest.                                 //
unsigned long *find_k_nearest_neighbors(unsigned long index, double x, double y, unsigned long k, long *noofneighbors)
{
  double distance = 0;                 // Variable used for distance calculations //
  double splitelementx = 0;            // Temporary variable used to for the x-coordinate of the split element //
  double splitelementy = 0;            // Temporary variable used to for the y-coordinate of the split element //
  double furthestelementx = 0;         // Temporary variable used to for the x-coordinate of the furthest nearest element //
  double furthestelementy = 0;         // Temporary variable used to for the y-coordinate of the furthest nearest element //
  long noofindexes = 0;                // Number of elements within radius //
  long noofnewindexes = 0;             // Number of elements within radius from the unexplored child of the node //
  unsigned long *indexes = NULL;       // Array used for the indexes for the elements //
  unsigned long *newindexes = NULL;    // Array used for the indexes for the elements from the unexplored child of the node //
  unsigned long splitindex = 0;        // The index of the element used for the split //
  unsigned long sortingarrayindex = 0; // A field of the Sorting Array that is used as index to the Elements' Array //
  unsigned long i, j;

  // Assign the given coordinates to the global variables used in sorting //
  refpointx = x;
  refpointy = y;

  // Check if the node is a leaf one //
  if ((kdtree + index)->isleaf == true)
    {
      // Create an array the size of all the elements designated to the leaf //
      noofindexes = (kdtree + index)->endindex - (kdtree + index)->startindex + 1;
      indexes = (unsigned long *) calloc(noofindexes, sizeof(unsigned long));
      assert(indexes != NULL);

      // Run through the array //
      for (i = 0, j = (kdtree + index)->startindex; j <= (kdtree + index)->endindex; i++, j++)
        {
          // Fill it with the indexes of the elements //
          indexes[i] = j;
        }

      // Sort the array of indexes based on the elements distances from the given coordinates //
      qsort(indexes, noofindexes, sizeof(struct pointHashElement *), distance_comparator);

      // If the number of indexes in the leaf exceed the number of required neighbors //
      if (noofindexes > k)
        {
          // Shrink the array to the required size //
          indexes = (unsigned long *) realloc(indexes, k * sizeof(unsigned long));
          noofindexes = k;
        }

      #ifdef DEBUGMODE
        {
          printf(RED"DEBUG: "NRM"Nearest Neighbors on this leaf:\r\n");

          // Print the neighbor elements //
          for (i = 0; i < noofindexes; i++)
            {
              sortingarrayindex = *(sortingarray + *(indexes + i));
              printf("\t%s\r\n", (elementarray + sortingarrayindex)->name);
            }
        }
      #endif
    }
  // If the node is not a leaf one //
  else
    {
      // Find the split element //
      splitindex = (kdtree + index)->splitindex;

      // Check the axis of the node //
      // If the node's elements have been sorted based on the X axis //
      if ((kdtree + index)->axis == X_AXIS)
        {
          // Calculate the x-coordinate of the split element //
          // If the split element is a point //
          if ((elementarray + splitindex)->type == point)
            {
              // Its x-coordinate is the one given //
              splitelementx = (elementarray + splitindex)->x;
            }
          // If the split element is not a point //
          else
            {
              // Its x-coordinate is the one given plus walf the given width //
              splitelementx = (elementarray + splitindex)->x + ((elementarray + splitindex)->width / 2);
            }

          // If the given x coordinate is after the split element //
          if (isgreater(x, splitelementx))
            {
              // Find the k nearest elements on this half //
              indexes = find_k_nearest_neighbors(((2 * index) + 1), x, y, k, &noofindexes);

              // Find the distance of the furthest neighbor element //
              // The elements are sorted based on their distances from the given //
              // point, therefore the last element of the array is the furthest  //
              sortingarrayindex = *(sortingarray + *(indexes + (noofindexes - 1)));
              // If the furthes element is a point //
              if ((elementarray + sortingarrayindex)->type == point)
                {
                  // Its coordinates are the one given //
                  furthestelementx = (elementarray + sortingarrayindex)->x;
                  furthestelementy = (elementarray + sortingarrayindex)->y;
                }
              // If the furtest element is not a point //
              else
                {
                  // Its coordinnates are given from its center, and not its top left corner //
                  furthestelementx = (elementarray + sortingarrayindex)->x + ((elementarray + sortingarrayindex)->width / 2);
                  furthestelementy = (elementarray + sortingarrayindex)->y + ((elementarray + sortingarrayindex)->height / 2);
                }
              distance = euclidean_distance(x, y, furthestelementx, furthestelementy);

              // If the distance between the given x and the split is less or equal the furthest distance //
              if (islessequal(fabs(x - splitelementx), distance) == 1)
                {
                  // Find the k nearest elements on the other half //
                  newindexes = find_k_nearest_neighbors((index * 2), x, y, k, &noofnewindexes);
                  assert(newindexes != NULL);

                  // Expand the current array to accomodate the new neighbors //
                  indexes = (unsigned long *) realloc(indexes, (noofindexes + noofnewindexes) * sizeof(unsigned long));
                  assert(indexes != NULL);

                  // Add all the found elements into the array //
                  memcpy(&indexes[noofindexes], newindexes, noofnewindexes * sizeof(unsigned long));
                  noofindexes += noofnewindexes;

                  // Free the array used for the other half's neighbor elements //
                  free(newindexes);

                  // Sort the expanded array //
                  qsort(indexes, noofindexes, sizeof(struct pointHashElement *), distance_comparator);

                  // If the number of found neighbors exceed the requested number //
                  if (noofindexes > k)
                    {
                      // Keep only the k nearest ones //
                      indexes = (unsigned long *) realloc(indexes, k * sizeof(unsigned long));
                      assert(indexes != NULL);
                      noofindexes = k;
                    }
                }
            }
          // If the given x coordinate is before or on the split //
          else
            {
              // Find the k nearest elements on this half //
              indexes = find_k_nearest_neighbors((index * 2), x, y, k, &noofindexes);

              // Find the distance of the furthest neighbor element //
              // The elements are sorted based on their distances from the given //
              // point, therefore the last element of the array is the furthest  //
              sortingarrayindex = *(sortingarray + *(indexes + (noofindexes - 1)));
              // If the furthes element is a point //
              if ((elementarray + sortingarrayindex)->type == point)
                {
                  // Its coordinates are the one given //
                  furthestelementx = (elementarray + sortingarrayindex)->x;
                  furthestelementy = (elementarray + sortingarrayindex)->y;
                }
              // If the furtest element is not a point //
              else
                {
                  // Its coordinnates are given from its center, and not its top left corner //
                  furthestelementx = (elementarray + sortingarrayindex)->x + ((elementarray + sortingarrayindex)->width / 2);
                  furthestelementy = (elementarray + sortingarrayindex)->y + ((elementarray + sortingarrayindex)->height / 2);
                }
              distance = euclidean_distance(x, y, furthestelementx, furthestelementy);

              // If the distance between the given x and the split is less or equal the furthest distance //
              if (islessequal(fabs(x - splitelementx), distance) == 1)
                {
                  // Find the k nearest elements on the other half //
                  newindexes = find_k_nearest_neighbors(((2 * index) + 1), x, y, k, &noofnewindexes);
                  assert(newindexes != NULL);

                  // Expand the current array to accomodate the new neighbors //
                  indexes = (unsigned long *) realloc(indexes, (noofindexes + noofnewindexes) * sizeof(unsigned long));
                  assert(indexes != NULL);

                  // Add all the found elements into the array //
                  memcpy(&indexes[noofindexes], newindexes, noofnewindexes * sizeof(unsigned long));
                  noofindexes += noofnewindexes;

                  // Free the array used for the other half's neighbor elements //
                  free(newindexes);

                  // Sort the expanded array //
                  qsort(indexes, noofindexes, sizeof(struct pointHashElement *), distance_comparator);

                  // If the number of found neighbors exceed the requested number //
                  if (noofindexes > k)
                    {
                      // Keep only the k nearest ones //
                      indexes = (unsigned long *) realloc(indexes, k * sizeof(unsigned long));
                      assert(indexes != NULL);
                      noofindexes = k;
                    }
                }
            }
        }
      // If the node's elements have been sorted based on the Y axis //
      else
        {
          // Calculate the y-coordinate of the split element //
          // If the split element is a point //
          if ((elementarray + splitindex)->type == point)
            {
              // Its y-coordinate is the one given //
              splitelementy = (elementarray + splitindex)->y;
            }
          // If the split element is not a point //
          else
            {
              // Its y-coordinate is the one given plus walf the given height //
              splitelementy = (elementarray + splitindex)->y + ((elementarray + splitindex)->height / 2);
            }

          // If the given y coordinate is after the split //
          if (isgreater(y, splitelementy))
            {
              // Find the k nearest elements on this half //
              indexes = find_k_nearest_neighbors(((2 * index) + 1), x, y, k, &noofindexes);

              // Find the distance of the furthest neighbor element //
              // The elements are sorted based on their distances from the given //
              // point, therefore the last element of the array is the furthest  //
              sortingarrayindex = *(sortingarray + *(indexes + (noofindexes - 1)));
              // If the furthes element is a point //
              if ((elementarray + sortingarrayindex)->type == point)
                {
                  // Its coordinates are the one given //
                  furthestelementx = (elementarray + sortingarrayindex)->x;
                  furthestelementy = (elementarray + sortingarrayindex)->y;
                }
              // If the furtest element is not a point //
              else
                {
                  // Its coordinnates are given from its center, and not its top left corner //
                  furthestelementx = (elementarray + sortingarrayindex)->x + ((elementarray + sortingarrayindex)->width / 2);
                  furthestelementy = (elementarray + sortingarrayindex)->y + ((elementarray + sortingarrayindex)->height / 2);
                }
              distance = euclidean_distance(x, y, furthestelementx, furthestelementy);

              // If the distance between the given y and the split is less or equal the furthest distance //
              if (islessequal(fabs(y - splitelementy), distance) == 1)
                {
                  // Find the k nearest elements on the other half //
                  newindexes = find_k_nearest_neighbors((index * 2), x, y, k, &noofnewindexes);
                  assert(newindexes != NULL);

                  // Expand the current array to accomodate the new neighbors //
                  indexes = (unsigned long *) realloc(indexes, (noofindexes + noofnewindexes) * sizeof(unsigned long));
                  assert(indexes != NULL);

                  // Add all the found elements into the array //
                  memcpy(&indexes[noofindexes], newindexes, noofnewindexes * sizeof(unsigned long));
                  noofindexes += noofnewindexes;

                  // Free the array used for the other half's neighbor elements //
                  free(newindexes);

                  // Sort the expanded array //
                  qsort(indexes, noofindexes, sizeof(struct pointHashElement *), distance_comparator);

                  // If the number of found neighbors exceed the requested number //
                  if (noofindexes > k)
                    {
                      // Keep only the n nearest ones //
                      indexes = (unsigned long *) realloc(indexes, k * sizeof(unsigned long));
                      assert(indexes != NULL);
                      noofindexes = k;
                    }
                }
            }
          // If the given y coordinate is before or on the split //
          else
            {
              // Find the k nearest elements on this half //
              indexes = find_k_nearest_neighbors((index * 2), x, y, k, &noofindexes);

              // Find the distance of the furthest neighbor element //
              // The elements are sorted based on their distances from the given //
              // point, therefore the last element of the array is the furthest  //
              sortingarrayindex = *(sortingarray + *(indexes + (noofindexes - 1)));
              // If the furthes element is a point //
              if ((elementarray + sortingarrayindex)->type == point)
                {
                  // Its coordinates are the one given //
                  furthestelementx = (elementarray + sortingarrayindex)->x;
                  furthestelementy = (elementarray + sortingarrayindex)->y;
                }
              // If the furtest element is not a point //
              else
                {
                  // Its coordinnates are given from its center, and not its top left corner //
                  furthestelementx = (elementarray + sortingarrayindex)->x + ((elementarray + sortingarrayindex)->width / 2);
                  furthestelementy = (elementarray + sortingarrayindex)->y + ((elementarray + sortingarrayindex)->height / 2);
                }
              distance = euclidean_distance(x, y, furthestelementx, furthestelementy);

              // If the distance between the given y and the split is less or equal the furthest distance //
              if (islessequal(fabs(y - splitelementy), distance) == 1)
                {
                  // Find the k nearest elements on the other half //
                  newindexes = find_k_nearest_neighbors(((2 * index) + 1), x, y, k, &noofnewindexes);
                  assert(newindexes != NULL);

                  // Expand the current array to accomodate the new neighbors //
                  indexes = (unsigned long *) realloc(indexes, (noofindexes + noofnewindexes) * sizeof(unsigned long));
                  assert(indexes != NULL);

                  // Add all the found elements into the array //
                  memcpy(&indexes[noofindexes], newindexes, noofnewindexes * sizeof(unsigned long));
                  noofindexes += noofnewindexes;

                  // Free the array used for the other half's neighbor elements //
                  free(newindexes);

                  // Sort the expanded array //
                  qsort(indexes, noofindexes, sizeof(struct pointHashElement *), distance_comparator);

                  // If the number of found neighbors exceed the requested number //
                  if (noofindexes > k)
                    {
                      // Keep only the n nearest ones //
                      indexes = (unsigned long *) realloc(indexes, k * sizeof(unsigned long));
                      assert(indexes != NULL);
                      noofindexes = k;
                    }
                }
            }
        }

      #ifdef DEBUGMODE
        {
          printf(RED"DEBUG: "NRM"Nearest Neighbors on this node:\r\n");

          // Print the neighbor elements //
          for (i = 0; i < noofindexes; i++)
            {
              sortingarrayindex = *(sortingarray + *(indexes + i));
              printf("\t%s\r\n", (elementarray + sortingarrayindex)->name);
            }
        }
      #endif
    }
  
  // Pass the number of indexes //
  *noofneighbors = noofindexes;
  
  // Return the array of the indexes //
  return indexes;
}

// *** dump_k_nearest_neighbors *** //
// Print the k nearest neighbor elements from a given set of coordinates.       //
// The function works as a wrapper for function find_k_nearest_neighbors(),     //
// where its result is printed to the user, whether the elements are k or less. //
void dump_k_nearest_neighbors(double x, double y, long k)
{
  unsigned long *indexes = NULL;       // Array used for the indexes of k neighbors //
  unsigned long sortingarrayindex = 0; // A field of the Sorting Array that is used as index to the Elements' Array //
  long noofindexes = 0;                // Number of elements within radius //
  unsigned long i = 0;

  // Search for neighbor element within the radius //
  indexes = find_k_nearest_neighbors(ROOT_NODE, x, y, k, &noofindexes);
  assert(indexes != NULL);

  printf("\r\nNearest Neighbors:\r\n");

  // Print the neighbor elements //
  for (i = 0; i < noofindexes; i++)
    {
      sortingarrayindex = *(sortingarray + *(indexes + i));
      printf("\t%s\r\n", (elementarray + sortingarrayindex)->name);
    }
  putchar('\n');

  // Free the array with the neighbors //
  free(indexes);
}
