#include "structs.h"
#include "prime_numbers.h"

// Max coordinates of points //
double pointMaxXCoordinate = -1;
double pointMaxYCoordinate = -1;

// KD-Tree //
struct kdTreeNode *kdTree = NULL;
unsigned long kdTreeArraySize = 0;

// Pointer Array for KD-Tree //
unsigned long *sortingArray = NULL;
unsigned long sortingArraySize = 0;

// Elements' Array //
struct knnElement *elementArray = NULL;
unsigned long elementArraySize = 0;
unsigned long elementArrayIndex = 0;

// Temporary Globals for coordinates //
// They are used for sorting using qsort() //
double tempX, tempY;

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
  if (elementArray[index1].type == point)
    {
      x1 = elementArray[index1].x;
    }
  else
    {
      x1 = elementArray[index1].x + elementArray[index1].width/2;
    }

  // Take the representative x coordinate for the second element //
  if (elementArray[index2].type == point)
    {
      x2 = elementArray[index2].x;
    }
  else
    {
      x2 = elementArray[index2].x + elementArray[index2].width/2;
    }

  // Check if the first point's coordinate is after the second's //
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
  if (elementArray[index1].type == point)
    {
      y1 = elementArray[index1].y;
    }
  else
    {
      y1 = elementArray[index1].y + elementArray[index1].height/2;
    }

  // Take the representative y coordinate for the second element //
  if (elementArray[index2].type == point)
    {
      y2 = elementArray[index2].y;
    }
  else
    {
      y2 = elementArray[index2].y + elementArray[index2].height/2;
    }

  // Check if the first point's coordinate is after the second's //
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
  unsigned long index1 = sortingArray[*(unsigned long *)p1];
  unsigned long index2 = sortingArray[*(unsigned long *)p2];
  double distance1;
  double distance2;

  if (elementArray[index1].type == point)
  {
    distance1 = euclidean_distance(tempX, tempY, elementArray[index1].x, elementArray[index1].y);
  }
  else
  {
    distance1 = euclidean_distance(tempX, tempY, elementArray[index1].x + elementArray[index1].width/2, elementArray[index1].y + elementArray[index1].height/2);
  }

  if (elementArray[index2].type == point)
  {
    distance2 = euclidean_distance(tempX, tempY, elementArray[index2].x, elementArray[index2].y);
  }
  else
  {
    distance2 = euclidean_distance(tempX, tempY, elementArray[index2].x + elementArray[index2].width/2, elementArray[index2].y + elementArray[index2].height/2);
  }
  // Check if the distance of the first point is greater than the second's //
  if (isgreater(distance1, distance2) == 1)
  {
    return 1;
  }
  // If not check if the distance of the second point's distance is greater //
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
  if (elementArray != NULL)
  {
  }

  elementArray = (struct knnElement *) calloc(size, sizeof(struct knnElement));
  assert(elementArray != NULL);

  elementArraySize = size;

  elementArrayIndex = 0;
}

// *** insert_element *** //
// Insert an element into the Element Array //
void insert_element(char *name, double x, double y, int type, double width, double height)
{
  elementArray[elementArrayIndex].name = name;
  elementArray[elementArrayIndex].x = x;
  elementArray[elementArrayIndex].y = y;

  if (type == -1)
  {
    elementArray[elementArrayIndex].type = point;
  }
  else if (type == 0)
  {
    elementArray[elementArrayIndex].type = cell;
  }
  else if (type == 1)
  {
    elementArray[elementArrayIndex].type = IO;
  }
  else if (type == 4)
  {
    elementArray[elementArrayIndex].type = module;
  }
  else
  {
    elementArray[elementArrayIndex].type = other;
  }

  elementArray[elementArrayIndex].width = width;
  elementArray[elementArrayIndex].height = height;

  elementArrayIndex++;
}

// *** insert_point *** //
// Insert a point into the Element Array //
void insert_point(char *name, double x, double y)
{
  insert_element(name, x, y, -1, 0, 0);
}

void free_element_array()
{
  unsigned long i;

  for (i = 0; i < elementArraySize; i++)
  {
    free(elementArray[i].name);
  }

  free(elementArray);
  elementArraySize = 0;
  elementArrayIndex = 0;
}

// *** dump_point_hash *** //
// Print the content of the Array //
void dump_element_array()
{
  unsigned long i;

  // If the Hash Table is empty //
  if (elementArraySize == 0)
    {
      // Print a correspoinding message //
      printf(YEL"Element Array is empty!\r\n"NRM);
      return;
    }

  // For each element of the Array //
  for (i = 0; i < elementArraySize; i++)
    {
      printf(BLU"Element:\r\n"NRM);

      printf(MAG"\tName:     "NRM"%s\r\n", elementArray[i].name);
      printf(MAG"\tCoords: ("NRM"%.4lf"MAG","NRM"%.4lf"MAG")\r\n"NRM, elementArray[i].x, elementArray[i].y);
      printf(MAG"\tType:     "NRM);

      if (elementArray[i].type == point)
        {
          printf("Point\r\n");
        }
      else if (elementArray[i].type == cell)
      {
        printf("Cell\r\n");
      }
      else if (elementArray[i].type == IO)
        {
          printf("IO\r\n");
        }
      else if (elementArray[i].type == module)
        {
          printf("Module\r\n");
        }
      else
        {
          printf("Other\r\n");
        }
    }
}

void dump_element_distances(double x, double y)
{
  unsigned long i;
  double distance;

  // If the Hash Table is empty //
  if (elementArraySize == 0)
    {
      // Print a cooresponding message //
      printf(YEL"Element Array is empty!\r\n"NRM);
      return;
    }

  // For each element of the array //
  for (i = 0; i < elementArraySize; i++)
    {
      // Find its distance to the given point and print it //
      if (elementArray[i].type == point)
        {
          distance = euclidean_distance(x, y, elementArray[i].x, elementArray[i].y);
        }
      else
        {
          distance = euclidean_distance(x, y, elementArray[i].x + elementArray[i].width/2, elementArray[i].y + elementArray[i].height/2);
        }
      printf(RED"DEBUG: "NRM"Distance from %s: %lf\r\n", elementArray[i].name, distance);
    }
}

char *get_element_name(unsigned long index)
{
  return elementArray[index].name;
}

double get_element_x(unsigned long index)
{
  return elementArray[index].x;
}

double get_element_y(unsigned long index)
{
  return elementArray[index].y;
}

double get_element_width(unsigned long index)
{
  return elementArray[index].width;
}

double get_element_height(unsigned long index)
{
  return elementArray[index].height;
}

int get_element_type(unsigned long index)
{
  return elementArray[index].type;
}

// Sorting Array for KD Tree //

// *** create_sorting_array *** //
// Create a sorting array that points into the Points   //
// hash table. It is used to be sorted for the KD tree. //
void create_sorting_array()
{
  unsigned long i, j;

  // First Pass - Find the size of the array by adding the number of points in each bucket//
  for (i = 0; i < elementArraySize; i++)
    {
      if (elementArray[i].type != other)
        {
          sortingArraySize++;
        }
    }

  // Allocate memory for the array //
  sortingArray = (unsigned long *) calloc(sortingArraySize, sizeof(unsigned long));
  assert(sortingArray != NULL);

  // Second Pass - Fill the array //
  // For each bucket of the Hash Table //
  for (i = 0, j = 0; i < elementArraySize; i++)
    {
      if (elementArray[i].type != other)
        {
          sortingArray[j] = i;
          j++;
        }
    }
}

// *** free_sorting_array *** //
// Free the sorting array //
void free_sorting_array()
{
  // If the array is empty //
  if (sortingArray == NULL)
    {
      // Do nothing //
      return;
    }

  // Free the array //
  free(sortingArray);
  sortingArray = NULL;
  sortingArraySize = 0;
}

// *** dump_sorting_array *** //
// Print the content of the sorting array //
void dump_sorting_array()
{
  unsigned long i;

  // For each point referred into the array //
  for (i = 0; i < sortingArraySize; i++)
    {
      // Print its name and coordinates //
      printf(BLU"Point: "NRM"%s\r\n", elementArray[sortingArray[i]].name);
      printf(MAG"\t("NRM"%.2lf"MAG","NRM"%.2lf"MAG")\r\n"NRM, elementArray[sortingArray[i]].x, elementArray[sortingArray[i]].y);
    }
}

// KD Tree Funcions //

// *** create_KD_tree *** //
void create_KD_tree()
{
  unsigned long noofElements = 0;
  unsigned long split = 0;

  // Free the existing tree //
  free_KD_tree();
  
  // Free the existing sorting array & create a new one //
  free_sorting_array();
  create_sorting_array();

  // Create the tree //
  // The first element of the array will remain empty and is allocated for easier traversal //
  kdTree = (struct kdTreeNode *) calloc(2, sizeof(struct kdTreeNode));
  assert(kdTree != NULL);
  kdTreeArraySize = 2;

  // kdTree->isLeaf = false;
  kdTree[1].axis = X_AXIS; // Chose the x axis as the first one to check //
  kdTree[1].startIndex = 0;
  kdTree[1].endIndex = sortingArraySize - 1;

  // If the elements in the sorting array are more than the defined max //
  noofElements = kdTree[1].endIndex - kdTree[1].startIndex + 1;
  if (noofElements > MAX_KDLEAF_ELEMENTS)
    {
      // Mark the node as a non leaf one //
      kdTree[1].isLeaf = 0;

      // Sort the array based on the x coordinate //
      qsort((void *)sortingArray, sortingArraySize, sizeof(struct pointHashElement *), point_x_comparator);
    
      // Save the split //
      split = (sortingArraySize - 1) / 2;
      kdTree[1].splitIndex = sortingArray[split];

      // Increase the tree size //
      kdTree = (struct kdTreeNode *) realloc(kdTree, ((2 * kdTreeArraySize) * sizeof(struct kdTreeNode)));;
      assert(kdTree != NULL);
      kdTreeArraySize *= 2; // The array that correspondes to the tree has to be doubled in size //

      // Fill the children nodes //
      // Left Child
      insert_KD_tree_node(2, kdTree[1].startIndex, split);
      // Right Child
      insert_KD_tree_node(3, (split + 1), kdTree[1].endIndex);
    }
  // If they are not //
  else
    {
      // Mark the node as a leaf one //
      kdTree[1].isLeaf = 1;
    }

  // kdTree->left = insert_KD_tree_node(kdTree, kdTree->startIndex, split);
  // kdTree->right = insert_KD_tree_node(kdTree, (split + 1), kdTree->endIndex);
}

// *** insert_KD_tree_node *** //
void insert_KD_tree_node(unsigned long index, unsigned long startIndex, unsigned long endIndex)
{
  unsigned long noofElements = 0;
  unsigned long split = 0;
  unsigned long parent = 0;

  // Fill the new node //
  kdTree[index].startIndex = startIndex;
  kdTree[index].endIndex = endIndex;

  // Find the number of elements that correspond to this node //
  noofElements = endIndex - startIndex + 1;

  // If the node has more elements than the defined max //
  if (noofElements > MAX_KDLEAF_ELEMENTS)
    {
      // Mark the node as a non leaf one //
      kdTree[index].isLeaf = 0;
      
      // Define the axis as the one not used by the parent //
      parent = index / 2;
      if (kdTree[parent].axis == X_AXIS)
        {
          kdTree[index].axis = Y_AXIS;
        }
      else
        {
          kdTree[index].axis = X_AXIS;
        }

      // Sort the sub-array depending on the axis //
      if (kdTree[index].axis == Y_AXIS)
        {
          qsort((void *)&sortingArray[startIndex], noofElements, sizeof(struct pointHashElement *), point_x_comparator);
        }
      else
        {
          qsort((void *)&sortingArray[startIndex], noofElements, sizeof(struct pointHashElement *), point_y_comparator);
        }

      // Check if the tree has enough space for the new nodes //
      // If the index of the left new node is greater or //
      // equal to the size of the tree, double its size  //
      if ((2 * index) >= kdTreeArraySize)
        {
          kdTree = (struct kdTreeNode *) realloc(kdTree, ((2 * kdTreeArraySize) * sizeof(struct kdTreeNode)));
          assert(kdTree != NULL);
          kdTreeArraySize *= 2;
        }

      // Find the index of the split //
      split = startIndex + ((endIndex - startIndex) / 2);

      // Save the split //
      kdTree[index].splitIndex = sortingArray[split];

      // Left Child
      insert_KD_tree_node((2 * index), startIndex, split);
      // Right Child
      insert_KD_tree_node(((2 * index) + 1), (split + 1), endIndex);
    }
  else
    {
      // Mark the node as a leaf one //
      kdTree[index].isLeaf = 1;
    }
}

// *** free_KD_tree *** //
// Free the KD Tree //
void free_KD_tree()
{
  // If the kd tree is empty //
  if (kdTree == NULL)
    {
      // Do nothing //
      return;
    }

  // Free the kd tree //
  free(kdTree);
  kdTree = NULL;

  kdTreeArraySize = 0;
}

// *** dump_KD_tree *** //
// Print the KD tree and its content. //
void dump_KD_tree()
{
  if (kdTree == NULL)
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
// For the node print its start & end indexes of its     //
// sub-array and its children, if its not a leaf node.   //
void dump_KD_node(unsigned long index, int depth)
{
  // Pint the start index of the node's sub-array //
  dump_tabs(depth);
  printf(BLU"Start Index: "NRM"%lu\r\n", kdTree[index].startIndex);

  // Pint the end index of the node's sub-array //
  dump_tabs(depth);
  printf(BLU"End Index:   "NRM"%lu\r\n", kdTree[index].endIndex);

  // Check if the node is leaf //
  if (kdTree[index].isLeaf == 1)
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

// *** find_nearest_neighbour *** //
unsigned long find_nearest_neighbour(unsigned long index, double x, double y)
{
  double minDistance = 0;        // The minimum distance found //
  double distance = 0;
  double splitElementX = 0;
  double splitElementY = 0;
  unsigned long minIndex = 0;    // The index of the element with the minimum distance //
  unsigned long secondIndex = 0;
  unsigned long i;

  // Check if the node is a leaf one //
  if (kdTree[index].isLeaf == 1)
    {
      // Find the point with the smallest distance between the points of the leaf //

      // Start by assuming the first node has the smallest distance //
      minIndex = kdTree[index].startIndex;
      if (elementArray[sortingArray[minIndex]].type == point)
      {
        minDistance = euclidean_distance(x, y, elementArray[sortingArray[minIndex]].x, elementArray[sortingArray[minIndex]].y);
      }
      else
        {
          minDistance = euclidean_distance(x, y, elementArray[sortingArray[minIndex]].x + elementArray[sortingArray[minIndex]].width/2, elementArray[sortingArray[minIndex]].y + elementArray[sortingArray[minIndex]].height/2);
        }
      
      // Run through the rest of the points //
      for (i = (kdTree[index].startIndex + 1); i <= kdTree[index].endIndex; i++)
        {
          // If a point's distance is less than the current minimum distance //
          if (elementArray[sortingArray[i]].type == point)
            {
              distance = euclidean_distance(x, y, elementArray[sortingArray[i]].x, elementArray[sortingArray[i]].y);
            }
          else
            {
              distance = euclidean_distance(x, y, elementArray[sortingArray[i]].x + elementArray[sortingArray[i]].width/2, elementArray[sortingArray[i]].y + elementArray[sortingArray[i]].height/2);
            }

          if (isless(distance, minDistance) == 1)
            {
              // Change the minimum distance as well as the corresponding point //
              minDistance = distance;
              minIndex = i;
            }
        }

      #ifdef DEBUG
        {
          printf(RED"DEBUG: "NRM"Nearest Neighbor on this leaf: %s\r\n", elementArray[sortingArray[minIndex]].name);
        }
      #endif
    }
  // If it is not //
  else
  {
    // Check the axis //
    // For the x axis //
    if (kdTree[index].axis == X_AXIS)
      {
        // If the given x coordinate is after the split //
        if (elementArray[kdTree[index].splitIndex].type == point)
          {
            splitElementX = elementArray[kdTree[index].splitIndex].x;
          }
        else
          {
            splitElementX = elementArray[kdTree[index].splitIndex].x + elementArray[kdTree[index].splitIndex].width/2;
          }

        if (isgreater(x, splitElementX))
          {
            // Find the nearest neighbour on this half //
            minIndex = find_nearest_neighbour(((2 * index) + 1), x, y);
            
            // If the distance between the given x and the split is less than the minimum distance //
            if (elementArray[sortingArray[minIndex]].type == point)
              {
                minDistance = euclidean_distance(x, y, elementArray[sortingArray[minIndex]].x, elementArray[sortingArray[minIndex]].y);
              }
            else
              {
                minDistance = euclidean_distance(x, y, elementArray[sortingArray[minIndex]].x + elementArray[sortingArray[minIndex]].width/2, elementArray[sortingArray[minIndex]].y + elementArray[sortingArray[minIndex]].height/2);
              }

            if (islessequal(fabs(x - splitElementX), minDistance) == 1)
              {
                // Find the nearest neighbour on the other half and compare it with the previous //
                secondIndex = find_nearest_neighbour((2 * index), x, y);
                if (elementArray[sortingArray[secondIndex]].type == point)
                  {
                    distance = euclidean_distance(x, y, elementArray[sortingArray[secondIndex]].x, elementArray[sortingArray[secondIndex]].y);
                  }
                else
                  {
                    distance = euclidean_distance(x, y, elementArray[sortingArray[secondIndex]].x + elementArray[sortingArray[secondIndex]].width/2, elementArray[sortingArray[secondIndex]].y + elementArray[sortingArray[secondIndex]].height/2);
                  }

                // If the new distance is less //
                if (isless(distance, minDistance) == 1)
                  {
                    // Keep the new point //
                    minIndex = secondIndex;
                  }
              }
          }
        // If the given x coordinate is before or on the split //
        else
          {
            // Find the nearest neighbour on this half //
            minIndex = find_nearest_neighbour((2 * index), x, y);
            
            // If the distance between the given x and the split is less than the minimum distance //
            if (elementArray[sortingArray[minIndex]].type == point)
              {
                minDistance = euclidean_distance(x, y, elementArray[sortingArray[minIndex]].x, elementArray[sortingArray[minIndex]].y);
              }
            else
              {
                minDistance = euclidean_distance(x, y, elementArray[sortingArray[minIndex]].x + elementArray[sortingArray[minIndex]].width/2, elementArray[sortingArray[minIndex]].y + elementArray[sortingArray[minIndex]].height/2);
              }

            if (islessequal(fabs(x - splitElementX), minDistance) == 1)
            {
              // Find the nearest neighbour on the other half and compare it with the previous //
              secondIndex = find_nearest_neighbour(((2 * index) + 1), x, y);
              if (elementArray[sortingArray[secondIndex]].type == point)
                {
                  distance = euclidean_distance(x, y, elementArray[sortingArray[secondIndex]].x, elementArray[sortingArray[secondIndex]].y);
                }
              else
                {
                  distance = euclidean_distance(x, y, elementArray[sortingArray[secondIndex]].x + elementArray[sortingArray[secondIndex]].width/2, elementArray[sortingArray[secondIndex]].y + elementArray[sortingArray[secondIndex]].height/2);
                }

              // If the new distance is less //
              if (isless(distance, minDistance) == 1)
                {
                  // Keep the new point //
                  minIndex = secondIndex;
                }
            }
        }
      }
    // For the y axis //
    else
      {
        // If the given y coordinate is after the split //
        if (elementArray[kdTree[index].splitIndex].type == point)
          {
            splitElementY = elementArray[kdTree[index].splitIndex].y;
          }
        else
          {
            splitElementY = elementArray[kdTree[index].splitIndex].y + elementArray[kdTree[index].splitIndex].height/2;
          }

        if (isgreater(y, splitElementY))
        {
          // Find the nearest neighbour on this half //
          minIndex = find_nearest_neighbour(((2 * index) + 1), x, y);
          
          // If the distance between the given y and the split is less than the minimum distance //
          if (elementArray[sortingArray[minIndex]].type == point)
            {
              minDistance = euclidean_distance(x, y, elementArray[sortingArray[minIndex]].x, elementArray[sortingArray[minIndex]].y);
            }
          else
            {
              minDistance = euclidean_distance(x, y, elementArray[sortingArray[minIndex]].x + elementArray[sortingArray[minIndex]].width/2, elementArray[sortingArray[minIndex]].y + elementArray[sortingArray[minIndex]].height/2);
            }

          if (islessequal(fabs(y - splitElementY), minDistance) == 1)
            {
              // Find the nearest neighbour on the other half and compare it with the previous //
              secondIndex = find_nearest_neighbour((2 * index), x, y);
              if (elementArray[sortingArray[secondIndex]].type == point)
                {
                  distance = euclidean_distance(x, y, elementArray[sortingArray[secondIndex]].x, elementArray[sortingArray[secondIndex]].y);
                }
              else
                {
                  distance = euclidean_distance(x, y, elementArray[sortingArray[secondIndex]].x + elementArray[sortingArray[secondIndex]].width/2, elementArray[sortingArray[secondIndex]].y + elementArray[sortingArray[secondIndex]].height/2);
                }
              
              // If the new distance is less //
              if (isless(distance, minDistance) == 1)
                {
                  // Keep the new point //
                  minIndex = secondIndex;
                }
            }
        }
        // If the given x coordinate is before or on the split //
        else
          {
            // Find the nearest neighbour on this half //
            minIndex = find_nearest_neighbour((2 * index), x, y);
            
            // If the distance between the given y and the split is less than the minimum distance //
            if (elementArray[sortingArray[minIndex]].type == point)
              {
                minDistance = euclidean_distance(x, y, elementArray[sortingArray[minIndex]].x, elementArray[sortingArray[minIndex]].y);
              }
            else
              {
                minDistance = euclidean_distance(x, y, elementArray[sortingArray[minIndex]].x + elementArray[sortingArray[minIndex]].width/2, elementArray[sortingArray[minIndex]].y + elementArray[sortingArray[minIndex]].height/2);
              }

            if (islessequal(fabs(y - splitElementY), minDistance) == 1)
              {
                // Find the nearest neighbour on the other half and compare it with the previous //
                secondIndex = find_nearest_neighbour(((2 * index) + 1), x, y);
                if (elementArray[sortingArray[secondIndex]].type == point)
                  {
                    distance = euclidean_distance(x, y, elementArray[sortingArray[secondIndex]].x, elementArray[sortingArray[secondIndex]].y);
                  }
                else
                  {
                    distance = euclidean_distance(x, y, elementArray[sortingArray[secondIndex]].x + elementArray[sortingArray[secondIndex]].width/2, elementArray[sortingArray[secondIndex]].y + elementArray[sortingArray[secondIndex]].height/2);
                  }
                
                // If the new distance is less //
                if (isless(distance, minDistance) == 1)
                  {
                    minIndex = secondIndex;
                  }
              }
          }
      }

    #ifdef DEBUG
    {
      printf(RED"DEBUG: "NRM"Nearest Neighbor on this node: %s\r\n", elementArray[sortingArray[minIndex]].name);
    }
    #endif
  }
  
  // Return the index of the nearest point //
  return minIndex;
}

// *** dump_nearest *** //
void dump_nearest_neighbor(double x, double y)
{
  printf("\r\nNearest Neighbor: %s\r\n\n", elementArray[sortingArray[find_nearest_neighbour(ROOT_NODE, x, y)]].name);
}

// *** find_nearest_neighbours_within_radius *** //
unsigned long *find_nearest_neighbours_within_radius(unsigned long index, double x, double y, double radius, unsigned long *noofNeighbors)
{
  double distance = 0;
  double splitElementX = 0;
  double splitElementY = 0;
  unsigned long *indexes = NULL;
  unsigned long *newIndexes = NULL;
  unsigned long noofIndexes = 0;
  unsigned long noofNewIndexes = 0;
  unsigned long i;

  // Check if the node is a leaf one //
  if (kdTree[index].isLeaf == 1)
    {
      // Run through the points //
      for (i = kdTree[index].startIndex; i <= kdTree[index].endIndex; i++)
        {
          // Find the distance between point and given coordinates //
          if (elementArray[sortingArray[i]].type == point)
            {
              distance = euclidean_distance(x, y, elementArray[sortingArray[i]].x, elementArray[sortingArray[i]].y);
            }
          else
            {
              distance = euclidean_distance(x, y, elementArray[sortingArray[i]].x + elementArray[sortingArray[i]].width/2, elementArray[sortingArray[i]].y + elementArray[sortingArray[i]].height/2);
            }

          // If the distance is less or equal to the radius //
          if (islessequal(distance, radius) == 1)
            {
              // Add the point to the list
              indexes = (unsigned long *) realloc(indexes, (noofIndexes + 1) * sizeof(unsigned long));
              assert(indexes != NULL);

              indexes[noofIndexes] = i;
              noofIndexes++;
            }
        }
    }
  // If it is not //
  else
    {
      // Check the axis //
      // For the x axis //
      if (kdTree[index].axis == X_AXIS)
        {
          // If the given x coordinate is after the split //
          if (elementArray[kdTree[index].splitIndex].type == point)
            {
              splitElementX = elementArray[kdTree[index].splitIndex].x;
            }
          else
            {
              splitElementX = elementArray[kdTree[index].splitIndex].x + elementArray[kdTree[index].splitIndex].width/2;
            }

          if (isgreater(x, splitElementX))
            {
              // Find a list of points that are within radius on this half //
              indexes = find_nearest_neighbours_within_radius(((2 * index) + 1), x, y, radius, &noofIndexes);
              
              // If the distance between the given x and the split is less or equal with the radius //
              if (islessequal(fabs(x - splitElementX), radius) == 1)
                {
                  // Find if there are points within the range on the other half //
                  newIndexes = find_nearest_neighbours_within_radius((2 * index), x, y, radius, &noofNewIndexes);

                  // If there are //
                  if (newIndexes != NULL)
                    {
                      // Add them to the list //
                      indexes = (unsigned long *) realloc(indexes, (noofIndexes + noofNewIndexes) * sizeof(unsigned long));
                      assert(indexes != NULL);
                      memcpy(&indexes[noofIndexes], newIndexes, noofNewIndexes * sizeof(unsigned long));
                      noofIndexes += noofNewIndexes;
                      free(newIndexes);
                    }
                }
            }
          // If the given x coordinate is before or on the split //
          else
            {
              // Find a list of points that are within radius on this half //
              indexes = find_nearest_neighbours_within_radius((2 * index), x, y, radius, &noofIndexes);
              
              // If the distance between the given x and the split is less or equal with the radius //
              if (islessequal(fabs(x - splitElementX), radius) == 1)
                {
                  // Find if there are points within the range on the other half //
                  newIndexes = find_nearest_neighbours_within_radius(((2 * index) + 1), x, y, radius, &noofNewIndexes);

                  // If there are //
                  if (newIndexes != NULL)
                    {
                      // Add them to the list //
                      indexes = (unsigned long *) realloc(indexes, (noofIndexes + noofNewIndexes) * sizeof(unsigned long));
                      assert(indexes != NULL);
                      memcpy(&indexes[noofIndexes], newIndexes, noofNewIndexes * sizeof(unsigned long));
                      noofIndexes += noofNewIndexes;
                      free(newIndexes);
                    }
                }
            }
        }
      // For the y axis //
      else
        {
          // If the given y coordinate is after the split //
          // If the given y coordinate is after the split //
          if (elementArray[kdTree[index].splitIndex].type == point)
            {
              splitElementX = elementArray[kdTree[index].splitIndex].y;
            }
          else
            {
              splitElementY = elementArray[kdTree[index].splitIndex].y + elementArray[kdTree[index].splitIndex].height/2;
            }

          if (isgreater(y, splitElementY))
            {
              // Find a list of points that are within radius on this half //
              indexes = find_nearest_neighbours_within_radius(((2 * index) + 1), x, y, radius, &noofIndexes);
              
              // If the distance between the given y and the split is less or equal with the radius //
              if (islessequal(fabs(y - splitElementY), radius) == 1)
                {
                  // Find if there are points within the range on the other half //
                  newIndexes = find_nearest_neighbours_within_radius((2 * index), x, y, radius, &noofNewIndexes);
                  
                  // If there are //
                  if (newIndexes != NULL)
                    {
                      // Add them to the list //
                      indexes = (unsigned long *) realloc(indexes, (noofIndexes + noofNewIndexes) * sizeof(unsigned long));
                      assert(indexes != NULL);
                      memcpy(&indexes[noofIndexes], newIndexes, noofNewIndexes * sizeof(unsigned long));
                      noofIndexes += noofNewIndexes;
                      free(newIndexes);
                    }
                }
            }
          // If the given y coordinate is before or on the split //
          else
            {
              // Find a list of points that are within radius on this half //
              indexes = find_nearest_neighbours_within_radius((2 * index), x, y, radius, &noofIndexes);
              
              // If the distance between the given x and the split is less or equal with the radius //
              if (islessequal(fabs(y - splitElementY), radius) == 1)
                {
                  // Find if there are points within the range on the other half //
                  newIndexes = find_nearest_neighbours_within_radius(((2 * index) + 1), x, y, radius, &noofNewIndexes);
                  
                  // If there are //
                  if (newIndexes != NULL)
                    {
                      // Add them to the list //
                      indexes = (unsigned long *) realloc(indexes, (noofIndexes + noofNewIndexes) * sizeof(unsigned long));
                      assert(indexes != NULL);
                      memcpy(&indexes[noofIndexes], newIndexes, noofNewIndexes * sizeof(unsigned long));
                      noofIndexes += noofNewIndexes;
                      free(newIndexes);
                    }
                }
            }
        }
    }
  
  // Pass the number of indexes //
  *noofNeighbors = noofIndexes;

  // Return the array of the indexes //
  return indexes;
}

// *** dump_nearest_neighbours_within_radius *** //
void dump_nearest_neighbours_within_radius(double x, double y, double radius)
{
  unsigned long *indexes = NULL;
  unsigned long i = 0;
  unsigned long noofIndexes = 0;

  indexes = find_nearest_neighbours_within_radius(ROOT_NODE, x, y, radius, &noofIndexes);
  
  if (indexes != NULL)
    {
      printf("\r\nNeighbors within radius:\r\n");

      for (i = 0; i < noofIndexes; i++)
        {
          printf("\t%s\r\n", elementArray[sortingArray[indexes[i]]].name);
        }

      putchar('\n');
      free(indexes);
    }
  else
    {
      printf(YEL"\r\nNo neighbors within radius!\r\n\n"NRM);
    }
}

// *** find_k_nearest_neighbours *** //
unsigned long *find_k_nearest_neighbours(unsigned long index, double x, double y, unsigned long k, unsigned long *noofNeighbors)
{
  unsigned long *indexes = NULL;
  unsigned long *newIndexes = NULL;
  unsigned long noofIndexes = 0;
  unsigned long noofNewIndexes = 0;
  unsigned long i, j;
  double distance = 0;
  double splitElementX = 0;
  double splitElementY = 0;

  // Assign the given coordinates to the global variables used in sorting //
  tempX = x;
  tempY = y;

  // Check if the node is a leaf one //
  if (kdTree[index].isLeaf == true)
    {
      // Create an array the size of all the points designated to the leaf //
      noofIndexes = kdTree[index].endIndex - kdTree[index].startIndex + 1;
      indexes = (unsigned long *) calloc(noofIndexes, sizeof(unsigned long));
      assert(indexes != NULL);

      // Run through the array //
      for (i = 0, j = kdTree[index].startIndex; j <= kdTree[index].endIndex; i++, j++)
        {
          // Fill it with the indexes of the points //
          indexes[i] = j;
        }

      // Sort the array of indexes based on their distance from the given coordinates //
      qsort(indexes, noofIndexes, sizeof(struct pointHashElement *), distance_comparator);

      // If the number of indexes in the leaf exceed the number of required neighbors //
      if (noofIndexes > k)
      {
        // Shrink the array to the required size //
        indexes = (unsigned long *) realloc(indexes, k * sizeof(unsigned long));
        noofIndexes = k;
      }
    }
  // If it is not //
  else
    {
      // Check the axis //
      // For the x axis //
      if (kdTree[index].axis == X_AXIS)
        {
          // If the given x coordinate is after the split //
          if (elementArray[kdTree[index].splitIndex].type == point)
            {
              splitElementX = elementArray[kdTree[index].splitIndex].x;
            }
          else
            {
              splitElementX = elementArray[kdTree[index].splitIndex].x + elementArray[kdTree[index].splitIndex].width/2;
            }

          if (isgreater(x, splitElementX))
            {
              // Find the nearest points on this half //
              indexes = find_k_nearest_neighbours(((2 * index) + 1), x, y, k, &noofIndexes);

              // If the distance between the given x and the furthest point is //
              // less or equal the distance between the given x and the split  //
              if (elementArray[sortingArray[indexes[noofIndexes - 1]]].type == point)
                {
                  distance = euclidean_distance(x, y, elementArray[sortingArray[indexes[noofIndexes - 1]]].x, elementArray[sortingArray[indexes[noofIndexes - 1]]].y);
                }
              else
                {
                  distance = euclidean_distance(x, y, elementArray[sortingArray[indexes[noofIndexes - 1]]].x + elementArray[sortingArray[indexes[noofIndexes - 1]]].width/2, elementArray[sortingArray[indexes[noofIndexes - 1]]].y + elementArray[sortingArray[indexes[noofIndexes - 1]]].height/2);
                }

              if (islessequal(fabs(x - splitElementX), distance) == 1)
                {
                  // Find the nearest points on the other half //
                  newIndexes = find_k_nearest_neighbours((index * 2), x, y, k, &noofNewIndexes);
                  assert(newIndexes != NULL);

                  // Expand the current array //
                  indexes = (unsigned long *) realloc(indexes, (noofIndexes + noofNewIndexes) * sizeof(unsigned long));
                  assert(indexes != NULL);

                  // Add all the found points into the array //
                  memcpy(&indexes[noofIndexes], newIndexes, noofNewIndexes * sizeof(unsigned long));
                  free(newIndexes);
                  noofIndexes += noofNewIndexes;

                  // Sort the array //
                  qsort(indexes, noofIndexes, sizeof(struct pointHashElement *), distance_comparator);

                  // If the number of fouund points exceed the requested number //
                  if (noofIndexes > k)
                    {
                      // Keep only the k nearest points //
                      indexes = (unsigned long *) realloc(indexes, k * sizeof(unsigned long));
                      assert(indexes != NULL);
                      noofIndexes = k;
                    }
                }
            }
          // If the given x coordinate is before or on the split //
          else
            {
              // Find the nearest points on this half //
              indexes = find_k_nearest_neighbours((index * 2), x, y, k, &noofIndexes);

              // If the distance between the given x and the furthest point is //
              // less or equal the distance between the given x and the split  //
              if (elementArray[sortingArray[indexes[noofIndexes - 1]]].type == point)
                {
                  distance = euclidean_distance(x, y, elementArray[sortingArray[indexes[noofIndexes - 1]]].x, elementArray[sortingArray[indexes[noofIndexes - 1]]].y);
                }
              else
                {
                  distance = euclidean_distance(x, y, elementArray[sortingArray[indexes[noofIndexes - 1]]].x + elementArray[sortingArray[indexes[noofIndexes - 1]]].width/2, elementArray[sortingArray[indexes[noofIndexes - 1]]].y + elementArray[sortingArray[indexes[noofIndexes - 1]]].height/2);
                }

              if (islessequal(fabs(x - splitElementX), distance) == 1)
                {
                  // Find the nearest points on the other half //
                  newIndexes = find_k_nearest_neighbours(((2 * index) + 1), x, y, k, &noofNewIndexes);
                  assert(newIndexes != NULL);

                  // Expand the current array //
                  indexes = (unsigned long *) realloc(indexes, (noofIndexes + noofNewIndexes) * sizeof(unsigned long));
                  assert(indexes != NULL);

                  // Add all the found points into the array //
                  memcpy(&indexes[noofIndexes], newIndexes, noofNewIndexes * sizeof(unsigned long));
                  free(newIndexes);
                  noofIndexes += noofNewIndexes;

                  // Sort the array //
                  qsort(indexes, noofIndexes, sizeof(struct pointHashElement *), distance_comparator);

                  // If the number of fouund points exceed the requested number //
                  if (noofIndexes > k)
                    {
                      // Keep only the k nearest points //
                      indexes = (unsigned long *) realloc(indexes, k * sizeof(unsigned long));
                      assert(indexes != NULL);
                      noofIndexes = k;
                    }
                }
            }
        }
      // For the y axis //
      else
        {
          // If the given y coordinate is after the split //
          if (elementArray[kdTree[index].splitIndex].type == point)
            {
              splitElementY = elementArray[kdTree[index].splitIndex].y;
            }
          else
            {
              splitElementY = elementArray[kdTree[index].splitIndex].y + elementArray[kdTree[index].splitIndex].height/2;
            }

          if (isgreater(y, splitElementY))
            {
              // Find the nearest points on this half //
              indexes = find_k_nearest_neighbours(((2 * index) + 1), x, y, k, &noofIndexes);

              // If the distance between the given y and the furthest point is //
              // less or equal the distance between the given y and the split  //
              if (elementArray[sortingArray[indexes[noofIndexes - 1]]].type == point)
                {
                  distance = euclidean_distance(x, y, elementArray[sortingArray[indexes[noofIndexes - 1]]].x, elementArray[sortingArray[indexes[noofIndexes - 1]]].y);
                }
              else
                {
                  distance = euclidean_distance(x, y, elementArray[sortingArray[indexes[noofIndexes - 1]]].x + elementArray[sortingArray[indexes[noofIndexes - 1]]].width/2, elementArray[sortingArray[indexes[noofIndexes - 1]]].y + elementArray[sortingArray[indexes[noofIndexes - 1]]].height/2);
                }

              if (islessequal(fabs(y - splitElementY), distance) == 1)
                {
                  // Find the nearest points on the other half //
                  newIndexes = find_k_nearest_neighbours((index * 2), x, y, k, &noofNewIndexes);
                  assert(newIndexes != NULL);

                  // Expand the current array //
                  indexes = (unsigned long *) realloc(indexes, (noofIndexes + noofNewIndexes) * sizeof(unsigned long));
                  assert(indexes != NULL);

                  // Add all the found points into the array //
                  memcpy(&indexes[noofIndexes], newIndexes, noofNewIndexes * sizeof(unsigned long));
                  free(newIndexes);
                  noofIndexes += noofNewIndexes;

                  // Sort the array //
                  qsort(indexes, noofIndexes, sizeof(struct pointHashElement *), distance_comparator);

                  // If the number of fouund points exceed the requested number //
                  if (noofIndexes > k)
                    {
                      // Keep only the n nearest points //
                      indexes = (unsigned long *) realloc(indexes, k * sizeof(unsigned long));
                      assert(indexes != NULL);
                      noofIndexes = k;
                    }
                }
            }
          // If the given y coordinate is before or on the split //
          else
            {
              // Find the nearest points on this half //
              indexes = find_k_nearest_neighbours((index * 2), x, y, k, &noofIndexes);

              // If the distance between the given y and the furthest point is //
              // less or equal the distance between the given y and the split  //
              if (elementArray[sortingArray[indexes[noofIndexes - 1]]].type == point)
                {
                  distance = euclidean_distance(x, y, elementArray[sortingArray[indexes[noofIndexes - 1]]].x, elementArray[sortingArray[indexes[noofIndexes - 1]]].y);
                }
              else
                {
                  distance = euclidean_distance(x, y, elementArray[sortingArray[indexes[noofIndexes - 1]]].x + elementArray[sortingArray[indexes[noofIndexes - 1]]].width/2, elementArray[sortingArray[indexes[noofIndexes - 1]]].y + elementArray[sortingArray[indexes[noofIndexes - 1]]].height/2);
                }

              if (islessequal(fabs(y - splitElementY), distance) == 1)
                {
                  // Find the nearest points on the other half //
                  newIndexes = find_k_nearest_neighbours(((2 * index) + 1), x, y, k, &noofNewIndexes);
                  assert(newIndexes != NULL);

                  // Expand the current array //
                  indexes = (unsigned long *) realloc(indexes, (noofIndexes + noofNewIndexes) * sizeof(unsigned long));
                  assert(indexes != NULL);

                  // Add all the found points into the array //
                  memcpy(&indexes[noofIndexes], newIndexes, noofNewIndexes * sizeof(unsigned long));
                  free(newIndexes);
                  noofIndexes += noofNewIndexes;

                  // Sort the array //
                  qsort(indexes, noofIndexes, sizeof(struct pointHashElement *), distance_comparator);

                  // If the number of fouund points exceed the requested number //
                  if (noofIndexes > k)
                    {
                      // Keep only the k nearest points //
                      indexes = (unsigned long *) realloc(indexes, k * sizeof(unsigned long));
                      assert(indexes != NULL);
                      noofIndexes = k;
                    }
                }
            }
        }
    }
  
  // Pass the number of indexes //
  *noofNeighbors = noofIndexes;
  
  // Return the array of the indexes //
  return indexes;
}

// *** dump_k_nearest_neighbours *** //
void dump_k_nearest_neighbours(double x, double y, unsigned long k)
{
  unsigned long *indexes = NULL;
  unsigned long i = 0;
  unsigned long noofIndexes = 0;

  indexes = find_k_nearest_neighbours(ROOT_NODE, x, y, k, &noofIndexes);
  assert(indexes != NULL);

  printf("\r\nNearest Neighbors:\r\n");

  for (i = 0; i < noofIndexes; i++)
    {
      printf("\t%s\r\n", elementArray[sortingArray[indexes[i]]].name);
    }

  putchar('\n');
  free(indexes);
}
