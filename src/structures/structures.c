#include "structures.h"
#include "prime_numbers.h"

// Point Hash Table //
struct pointHash *pointHT = NULL;
unsigned long pointHTSize = 0;
int pointHTPrimeIndex = 0;

// Max coordinates of points //
double pointMaxXCoordinate = -1;
double pointMaxYCoordinate = -1;

// KD-Tree //
struct kdNode *kdTree = NULL;

// Pointer Array for KD-Tree //
struct pointHashNode **pointArray = NULL;
unsigned long pointArraySize = 0;

// Temporary Globals for coordinates //
// They are used for sorting using qsort() //
double tempX, tempY;

/**
 * @brief Complementary function used only for printing tabs
 * 
 * This function is used as part of the printing functions.
 * 
 * @param depth The number of tabs to be used
 */
inline void print_tabs(int depth)
{
	int i;

	for (i = 0; i < depth; i++)
	{
		putchar('\t');
	}
}

/**
 * @brief The Eucledean distance of two points
 * 
 * The points should have their coordinates given as doubles.
 * 
 * @param x1 The x-coordinate of the first point
 * @param y1 The y-coordinate of the first point
 * @param x2 The x-coordinate of the second point
 * @param y2 The y-coordinate of the second point
 * @return double 
 */

/**
 * @brief      The Eucledean distance of two points
 *
 * @param      x1    The x-coordinate of the first point
 * @param      y1    The y-coordinate of the first point
 * @param      x2    The x-coordinate of the second point
 * @param      y2    The y-coordinate of the second point
 *
 * @return     ((x2 - x1)^2 + (y2 - y1)^2)^(1/2)
 */
inline double euclidean_distance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

/**
 * Comparators
 */

/**
 * @brief Compare the x axis values of two points on the array
 * 
 * @param p1 Pointer to the first point
 * @param p2 Pointer to the second point
 * 
 * @return int Returns 1 if p1 is greater than p2, -1 if p2 is greater than p1 & 0 if they have equal value
 */
int point_x_comparator(const void *p1, const void *p2)
{
	// Check if the first point's coordinate is after the second's //
	if (isgreater((double)(*(struct pointHashNode **)p1)->x, (double)(*(struct pointHashNode **)p2)->x) == 1)
	{
		return 1;
	}
	// If not check if the second's one has greater value //
	else if (isless((double)(*(struct pointHashNode **)p1)->x, (double)(*(struct pointHashNode **)p2)->x) == 1)
	{
		return -1;
	}
	// If not either, then they have the same x coordinate //
	else 
	{
		return 0;
	}
}

/**
 * @brief Compare the y axis values of two points on the array
 * 
 * @param p1 Pointer to the first point
 * @param p2 Pointer to the second point
 * 
 * @return int Returns 1 if p1 is greater than p2, -1 if p2 is greater than p1 & 0 if they have equal value
 */
int point_y_comparator(const void *p1, const void *p2)
{
	// Check if the first point's coordinate is after the second's //
	if (isgreater((double)(*(struct pointHashNode **)p1)->y, (double)(*(struct pointHashNode **)p2)->y) == 1)
	{
		return 1;
	}
	// If not check if the second's one has greater value //
	else if (isless((double)(*(struct pointHashNode **)p1)->y, (double)(*(struct pointHashNode **)p2)->y) == 1)
	{
		return -1;
	}
	// If not either, then they have the same y coordinate //
	else 
	{
		return 0;
	}
}

int distance_comparator(const void *p1, const void *p2)
{
	double distance1;
	double distance2;

	distance1 = euclidean_distance(tempX, tempY, pointArray[*(int *)p1]->x, pointArray[*(int *)p1]->y);
	distance2 = euclidean_distance(tempX, tempY, pointArray[*(int *)p2]->x, pointArray[*(int *)p2]->y);

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

// Point Hash Table Functions //

/**
 * @brief      Hash function for the Point Hash
 *
 * @param      name  The name of the point
 *
 * @return     The hash
 */
unsigned long point_hash_function(char *name)
{
	int i;
	int length = strlen(name);  
	unsigned long result = 0;

	for (i = 0; i < length; i++)
	{
		result += name[i];
	}
    
	return result;
}

/**
 * @brief Insert a point to the Hash Table
 * 
 * @param name The name of the point
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 */
void insert_point(char *name, double x, double y)
{
	unsigned long hash = 0;

	// If the Hash Table is empty //
	if (pointHTSize == 0)
	{
		pointHT = (struct pointHash *) calloc(primes[pointHTPrimeIndex], sizeof(struct pointHash));
		assert(pointHT != NULL);
		
		pointHTSize = primes[pointHTPrimeIndex];
		pointHTPrimeIndex++;
	}
	
	hash = point_hash_function(name) % pointHTSize;
	
	pointHT[hash].nodes = (struct pointHashNode *) realloc(pointHT[hash].nodes, ((pointHT[hash].depth + 1) * sizeof(struct pointHashNode)));
	assert(pointHT[hash].nodes != NULL);

	pointHT[hash].nodes[pointHT[hash].depth].name = name;
	pointHT[hash].nodes[pointHT[hash].depth].x = x;
	pointHT[hash].nodes[pointHT[hash].depth].y = y;


	pointHT[hash].depth++;
	if (pointHT[hash].depth == MAX_HASH_DEPTH)
	{
		rehash_point_hash();
	}
}

/**
 * @brief Rehash the Hash Table
 * 
 * Expand the length of the hash table. The new length is drawn by a static array of prime numbers.
 * If all the primes on the array we reused, the length is doubled.
 * 
 */
void rehash_point_hash()
{
	struct pointHash *oldHash = pointHT;
	unsigned long oldHashSize = pointHTSize;
	unsigned long i;
	unsigned long hash = 0;
	int j;

	// Allocate a new hash table //
	if (pointHTPrimeIndex == MAX_NOOF_PRIMES)
	{
		pointHT = (struct pointHash *) calloc((2 * pointHTSize), sizeof(struct pointHash));
		assert(pointHT != NULL);

		pointHTSize *= 2;
	}
	else
	{
		pointHT = (struct pointHash *) calloc(primes[pointHTPrimeIndex], sizeof(struct pointHash));
		assert(pointHT != NULL);

		pointHTSize = primes[pointHTPrimeIndex];
		pointHTPrimeIndex++;
	}
	
	// Transfer data from the old hash table to the new //
	for (i = 0; i < oldHashSize; i++)
	{
		if (oldHash[i].depth == 0)
		{
			continue;
		}

		for (j = 0; j < oldHash[i].depth; j++)
		{
			hash = point_hash_function(oldHash[i].nodes[j].name) % pointHTSize;

			pointHT[hash].nodes = (struct pointHashNode *) realloc(pointHT[hash].nodes, ((pointHT[hash].depth + 1) * sizeof(struct pointHashNode)));
			assert(pointHT[hash].nodes != NULL);

			pointHT[hash].nodes[pointHT[hash].depth].name = oldHash[i].nodes[j].name;
			pointHT[hash].nodes[pointHT[hash].depth].x = oldHash[i].nodes[j].x;
			pointHT[hash].nodes[pointHT[hash].depth].y = oldHash[i].nodes[j].y;

			pointHT[hash].depth++;
		}

		free(oldHash[i].nodes);
	}
	free(oldHash);
}

/**
 * @brief Get the depth of the hash table for a particular hash
 * 
 * @param hash The hash code
 * @return int The depth for this hash
 */
inline int get_point_hash_depth(unsigned long hash)
{
	return pointHT[hash].depth;
}

/**
 * @brief Get the x-coordinate of the point
 * 
 * @param hash The hash code of the point
 * @param depth THe depth on which the point is
 * @return double The x-coordinate of the point
 */
inline double get_point_x_coord(unsigned long hash, int depth)
{
	return pointHT[hash].nodes[depth].x;
}

/**
 * @brief Get the y-coordinate of the point
 * 
 * @param hash The hash code of the point
 * @param depth THe depth on which the point is
 * @return double The y-coordinate of the point
 */
inline double get_point_y_coord(unsigned long hash, int depth)
{
	return pointHT[hash].nodes[depth].y;
}

/**
 * @brief Get the name of the point
 * 
 * @param hash The hash code of the point
 * @param depth THe depth on which the point is
 * @return char* The name of the point
 */
inline char *get_point_name(unsigned long hash, int depth)
{
	return pointHT[hash].nodes[depth].name;
}

/**
 * @brief Free the Hash Table that the points are stored
 * 
 */
void free_point_hash()
{
	unsigned long i;
	int j;

	if (pointHT == NULL)
	{
		return;
	}

	for (i = 0; i < pointHTSize; i++)
	{
		if (pointHT[i].depth == 0)
		{
			continue;
		}

		for (j = 0; j < pointHT[i].depth; j++)
		{
			free(pointHT[i].nodes[j].name);
		}
		free(pointHT[i].nodes);
	}

	free(pointHT);
	pointHT = NULL;
	pointHTSize = 0;
	pointHTPrimeIndex = 0;
}

// Print the content of the point Hash Table //
/**
 * @brief Print the content of the Hash Table
 * 
 */
void print_point_hash()
{
	unsigned long i;
	int j;

	if (pointHTSize == 0)
	{
		printf(YEL"Point Hash Table is empty!\r\n"NRM);
		return;
	}

	for (i = 0; i < pointHTSize; i++)
	{
		printf(BLU"Hash Code:     "MAG"%lu\r\n"NRM, i);
		printf(BLU"Stored Points:\n"NRM);

		if (pointHT[i].depth == 0)
		{
			printf(MAG"\t(null)\r\n"NRM);
		}
		else
		{
			for (j = 0; j < pointHT[i].depth; j++)
			{
				printf(MAG"\tPoint: "NRM"%s "MAG"("NRM"%.2lf"MAG","NRM"%.2lf"MAG")\r\n"NRM, pointHT[i].nodes[j].name, pointHT[i].nodes[j].x, pointHT[i].nodes[j].y);
			}
		}
		
	}
}

void print_point_hash_distances(double x, double y)
{
	unsigned long i;
	int j;
	double distance;

	if (pointHTSize == 0)
	{
		printf(YEL"Point Hash Table is empty!\r\n"NRM);
		return;
	}

	for (i = 0; i < pointHTSize; i++)
	{
		if (pointHT[i].depth != 0)
		{
			for (j = 0; j < pointHT[i].depth; j++)
			{
				distance = euclidean_distance(x, y, pointHT[i].nodes[j].x, pointHT[i].nodes[j].y);
				printf(RED"--->"NRM"Distance from %s: %lf\r\n", pointHT[i].nodes[j].name, distance);
			}
		}
	}
}

// Sorting Array for KD Tree //
/**
 * @brief Create a sorting array of pointers to the Hash Tabls
 * 
 * This array is used as reference for the KD Tree
 * 
 */
void create_sorting_array()
{
	unsigned long i, k;
	int j;

	// First Pass - Find the size of the array //
	for (i = 0; i < pointHTSize; i++)
	{
		pointArraySize += pointHT[i].depth;
	}

	// Allocate memory for the array //
	pointArray = (struct pointHashNode **) calloc(pointArraySize, sizeof(struct pointHashNode *));
	assert(pointArray != NULL);

	// Second Pass - Fill the array //
	for (i = 0, k = 0; i < pointHTSize; i++)
	{
		for(j = 0; j < pointHT[i].depth; j++)
		{
			pointArray[k] = &pointHT[i].nodes[j];
			k++;
		}
	}
}

/**
 * @brief      { function_description }
 *
 * @return     { description_of_the_return_value }
 */
void free_sorting_array()
{
	if (pointArray == NULL)
	{
		return;
	}

	free(pointArray);
	pointArray = NULL;
	pointArraySize = 0;
}

void print_sorting_array()
{
	unsigned long i;

	for (i = 0; i < pointArraySize; i++)
	{
		printf(BLU"Point: "NRM"%s\r\n", pointArray[i]->name);
		printf(MAG"\t("NRM"%.2lf"MAG","NRM"%.2lf"MAG")\r\n"NRM, pointArray[i]->x, pointArray[i]->y);
	}
}

// KD Tree Funcions //
/**
 * @brief Create the KD Tree
 * 
 */
void create_KD_tree()
{
	unsigned long split = 0;
	// Free the previous tree //
	free_KD_tree();
	
	free_sorting_array();
	create_sorting_array();

	// Create the head of the tree //
	kdTree = (struct kdNode *) calloc(1, sizeof(struct kdNode));
	assert(kdTree != NULL);

	kdTree->isLeaf = false;
	kdTree->axis = true;
	kdTree->startIndex = 0;
	kdTree->endIndex = pointArraySize - 1;

	// Sort the array based on the x coordinate //
	qsort((void *)pointArray, pointArraySize, sizeof(struct pointHashNode *), point_x_comparator);
	
	// Cut the array in two halves //
	split = pointArraySize / 2;
	kdTree->splitIndex = pointArray[split];
	kdTree->left = insert_KD_tree_node(kdTree, kdTree->startIndex, split);
	kdTree->right = insert_KD_tree_node(kdTree, (split + 1), kdTree->endIndex);
}

/**
 * @brief Insert a KD node to the parent
 * 
 * @param parent Parent Node
 * @param startIndex Start index for the new node
 * @param endIndex End index for the new node
 * @return struct kdNode* Returns the new node
 */
struct kdNode *insert_KD_tree_node(struct kdNode *parent, unsigned long startIndex, unsigned long endIndex)
{
	unsigned long noofElements = 0;
	unsigned long split = 0;
	struct kdNode *node = NULL;

	// Create the new node //
	node = (struct kdNode *) calloc(1, sizeof(struct kdNode));
	assert(node != NULL);

	node->parent = parent;
	node->startIndex = startIndex;
	node->endIndex = endIndex;

	// Find the number of elements that correspond to this node //
	noofElements = endIndex - startIndex + 1;

	// If the node has more elements than the defined max //
	if (noofElements > MAX_KDLEAF_ELEMENTS)
	{
		// The node cannote be a leaf node and the elements are split in two to its children //
		split = startIndex + ((endIndex - startIndex) / 2);
		node->isLeaf = false;
		// node->splitIndex = pointArray[split];
		node->axis = !parent->axis;
		if (node->axis == true) // x axis //
		{
			qsort((void *)&pointArray[startIndex], noofElements, sizeof(struct pointHashNode *), point_x_comparator);
		}
		else // y axis //
		{
			qsort((void *)&pointArray[startIndex], noofElements, sizeof(struct pointHashNode *), point_y_comparator);
		}
		node->splitIndex = pointArray[split];

		node->left = insert_KD_tree_node(node, startIndex, split);
		node->right = insert_KD_tree_node(node, (split + 1), endIndex);
	}
	else
	{
		node->isLeaf = true;
	}
	
	return node;
}

// Free the KD Tree //
/**
 * @brief Free the KD tree
 * 
 * Actually a wrapper of the function free_KD_node for the whole KD Tree.
 * 
 */
void free_KD_tree()
{
	if (kdTree == NULL)
	{
		return;
	}

	free_KD_node(kdTree);
	kdTree = NULL;
}

/**
 * @brief      Free this KD Tree node
 *
 * @param      node  The node
 *
 * @return     { description_of_the_return_value }
 */
void free_KD_node(struct kdNode *node)
{
	// If the node is not a leaf one //
	if (node->isLeaf == false)
	{
		// First free its children //
		free_KD_node(node->left);
		free_KD_node(node->right);
	}

	// Free the node //
	free(node);
}

/**
 * @brief Print the KD tree
 * 
 * Actually a wrapper of the function print_KD_node for the whole KD Tree.
 * 
 */
void print_KD_tree()
{
	if (kdTree == NULL)
	{
		printf(YEL"KD Tree Empty!\n"NRM);
		return;
	}

	print_KD_node(kdTree, 0);
}

// Print this node of the KD Tree //
/**
 * @brief Print this specific node
 * 
 * The node is printed with the a number of tabs in front of it the same as its depth.
 * The tabs are printed with the function print_tabs().
 * 
 * @param node The node that needs to printed
 * @param depth The depth of the node
 */
void print_KD_node(struct kdNode *node, int depth)
{
	print_tabs(depth);
	printf(BLU"Start Index: "NRM"%lu\r\n", node->startIndex);
	
	print_tabs(depth);
	printf(BLU"End Index:   "NRM"%lu\r\n", node->endIndex);

	if (node->isLeaf == true)
	{
		print_tabs(depth);
		printf(MAG"\tLeaf node!\r\n");
	}
	else
	{
		print_tabs(depth);
		printf(MAG"\tLeft child:\r\n");
		print_KD_node(node->left, (depth + 1));

		print_tabs(depth);
		printf(MAG"\tRight child:\r\n");
		print_KD_node(node->right, (depth + 1));
	}
	
}

/**
 * @brief      Find the nearest neighbour from certain coordinnates
 *
 * @param      node  The KD node on which the neigbour is searced
 * @param      x     The given x-coordinate
 * @param      y     The given y-coordinate
 *
 * @return     The index of the nearest point
 */
unsigned long find_nearest_neighbour(struct kdNode *node, double x, double y)
{
	double minDistance = 0;
	double distance = 0;
	unsigned long minIndex = 0;
	unsigned long secondIndex = 0;
	unsigned long i;

	// Check if the node is a leaf one //
	if (node->isLeaf == true)
	{
		// Find the point with the smallest distance between the points of the leaf //

		// Start by assuming the first node has the smallest distance //
		minIndex = node->startIndex; 
		minDistance = euclidean_distance(x, y, pointArray[minIndex]->x, pointArray[minIndex]->y);
		
		// Run through the rest of the points //
		for (i = (node->startIndex + 1); i <= node->endIndex; i++)
		{
			// If a point's distance is less than the current minimum distance //
			distance = euclidean_distance(x, y, pointArray[i]->x, pointArray[i]->y);
			if (isless(distance, minDistance) == 1)
			{
				// Change the minimum distance as well as the corresponding point //
				minDistance = distance;
				minIndex = i;
			}
		}
	}
	// If it is not //
	else
	{
		// Check the axis //
		// For the x axis //
		if (node->axis == true)
		{
			// If the given x coordinate is after the split //
			if (isgreater(x, node->splitIndex->x))
			{
				// Find the nearest neighbour on this half //
				minIndex = find_nearest_neighbour(node->right, x, y);
				
				// If the distance between the given x and the split is less than the minimum distance //
				minDistance = euclidean_distance(x, y, pointArray[minIndex]->x, pointArray[minIndex]->y);
				if (isless(fabs(x - node->splitIndex->x), minDistance) == 1)
				{
					// Find the nearest neighbour on the other half and compare it with the previous //
					secondIndex = find_nearest_neighbour(node->left, x, y);
					distance = euclidean_distance(x, y, pointArray[secondIndex]->x, pointArray[secondIndex]->y);

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
				minIndex = find_nearest_neighbour(node->left, x, y);
				
				// If the distance between the given x and the split is less than the minimum distance //
				minDistance = euclidean_distance(x, y, pointArray[minIndex]->x, pointArray[minIndex]->y);
				if (isless(fabs(x - node->splitIndex->x), minDistance) == 1)
				{
					// Find the nearest neighbour on the other half and compare it with the previous //
					secondIndex = find_nearest_neighbour(node->right, x, y);
					distance = euclidean_distance(x, y, pointArray[secondIndex]->x, pointArray[secondIndex]->y);

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
			if (isgreater(y, node->splitIndex->y))
			{
				// Find the nearest neighbour on this half //
				minIndex = find_nearest_neighbour(node->right, x, y);
				
				// If the distance between the given y and the split is less than the minimum distance //
				minDistance = euclidean_distance(x, y, pointArray[minIndex]->x, pointArray[minIndex]->y);
				if (isless(fabs(y - node->splitIndex->y), minDistance) == 1)
				{
					// Find the nearest neighbour on the other half and compare it with the previous //
					secondIndex = find_nearest_neighbour(node->left, x, y);
					distance = euclidean_distance(x, y, pointArray[secondIndex]->x, pointArray[secondIndex]->y);
					
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
				minIndex = find_nearest_neighbour(node->left, x, y);
				
				// If the distance between the given y and the split is less than the minimum distance //
				minDistance = euclidean_distance(x, y, pointArray[minIndex]->x, pointArray[minIndex]->y);
				if (isless(fabs(y - node->splitIndex->y), minDistance) == 1)
				{
					// Find the nearest neighbour on the other half and compare it with the previous //
					secondIndex = find_nearest_neighbour(node->right, x, y);
					distance = euclidean_distance(x, y, pointArray[secondIndex]->x, pointArray[secondIndex]->y);
					
					// If the new distance is less //
					if (isless(distance, minDistance) == 1)
					{
						minIndex = secondIndex;
					}
				}
			}
		}
	}
	
	// Return the index of the nearest point //
	return minIndex;
}


/**
 * @brief      Find the nearest neighbours from certain coordinates within a certain range
 *
 * @param      node           The tree node in which the search is done
 * @param      x              The given x-coordinate
 * @param      y              The given y-coordinate
 * @param      radius         The radius of the search
 * @param      noofNeighbors  The number neighbors found
 *
 * @return     An array of the found neighbours
 */
unsigned long *find_nearest_neighbours_within_radius(struct kdNode *node, double x, double y, double radius, unsigned long *noofNeighbors)
{
	double distance = 0;
	unsigned long *indexes = NULL;
	unsigned long *newIndexes = NULL;
	unsigned long noofIndexes = 0;
	unsigned long noofNewIndexes = 0;
	unsigned long i;

	// Check if the node is a leaf one //
	if (node->isLeaf == true)
	{
		// Run through the points //
		for (i = node->startIndex; i <= node->endIndex; i++)
		{
			// Find the distance between point and given coordinates //
			distance = euclidean_distance(x, y, pointArray[i]->x, pointArray[i]->y);
			
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
		if (node->axis == true)
		{
			// If the given x coordinate is after the split //
			if (isgreater(x, node->splitIndex->x))
			{
				// Find a list of points that are within radius on this half //
				indexes = find_nearest_neighbours_within_radius(node->right, x, y, radius, &noofIndexes);
				
				// If the distance between the given x and the split is less or equal with the radius //
				if (islessequal(fabs(x - node->splitIndex->x), radius) == 1)
				{
					// Find if there are points within the range on the other half //
					newIndexes = find_nearest_neighbours_within_radius(node->left, x, y, radius, &noofNewIndexes);

					// If there are //
					if (newIndexes != NULL)
					{
						// Add them to the list //
						indexes = (unsigned long *) realloc(indexes, (noofIndexes + noofNewIndexes) * sizeof(unsigned long));
						assert(indexes != NULL);
						memcpy(&indexes[noofIndexes], newIndexes, noofNewIndexes * sizeof(unsigned long));
						noofIndexes += noofNewIndexes;
					}
				}
			}
			// If the given x coordinate is before or on the split //
			else
			{
				// Find a list of points that are within radius on this half //
				indexes = find_nearest_neighbours_within_radius(node->left, x, y, radius, &noofIndexes);
				
				// If the distance between the given x and the split is less or equal with the radius //
				if (islessequal(fabs(x - node->splitIndex->x), radius) == 1)
				{
					// Find if there are points within the range on the other half //
					newIndexes = find_nearest_neighbours_within_radius(node->right, x, y, radius, &noofNewIndexes);

					// If there are //
					if (newIndexes != NULL)
					{
						// Add them to the list //
						indexes = (unsigned long *) realloc(indexes, (noofIndexes + noofNewIndexes) * sizeof(unsigned long));
						assert(indexes != NULL);
						memcpy(&indexes[noofIndexes], newIndexes, noofNewIndexes * sizeof(unsigned long));
						noofIndexes += noofNewIndexes;
					}
				}
			}
		}
		// For the y axis //
		else
		{
			// If the given y coordinate is after the split //
			if (isgreater(y, node->splitIndex->y))
			{
				// Find a list of points that are within radius on this half //
				indexes = find_nearest_neighbours_within_radius(node->right, x, y, radius, &noofIndexes);
				
				// If the distance between the given y and the split is less or equal with the radius //
				if (islessequal(fabs(y - node->splitIndex->y), radius) == 1)
				{
					// Find if there are points within the range on the other half //
					newIndexes = find_nearest_neighbours_within_radius(node->left, x, y, radius, &noofNewIndexes);
					
					// If there are //
					if (newIndexes != NULL)
					{
						// Add them to the list //
						indexes = (unsigned long *) realloc(indexes, (noofIndexes + noofNewIndexes) * sizeof(unsigned long));
						assert(indexes != NULL);
						memcpy(&indexes[noofIndexes], newIndexes, noofNewIndexes * sizeof(unsigned long));
						noofIndexes += noofNewIndexes;
					}
				}
			}
			// If the given y coordinate is before or on the split //
			else
			{
				// Find a list of points that are within radius on this half //
				indexes = find_nearest_neighbours_within_radius(node->left, x, y, radius, &noofIndexes);
				
				// If the distance between the given x and the split is less or equal with the radius //
				if (islessequal(fabs(y - node->splitIndex->y), radius) == 1)
				{
					// Find if there are points within the range on the other half //
					newIndexes = find_nearest_neighbours_within_radius(node->right, x, y, radius, &noofNewIndexes);
					
					// If there are //
					if (newIndexes != NULL)
					{
						// Add them to the list //
						indexes = (unsigned long *) realloc(indexes, (noofIndexes + noofNewIndexes) * sizeof(unsigned long));
						assert(indexes != NULL);
						memcpy(&indexes[noofIndexes], newIndexes, noofNewIndexes * sizeof(unsigned long));
						noofIndexes += noofNewIndexes;
					}
				}
			}
		}
	}
	
	*noofNeighbors = noofIndexes;

	return indexes;
}

/**
 * @brief      { function_description }
 *
 * @param      node           The node
 * @param      x              { parameter_description }
 * @param      y              { parameter_description }
 * @param      n              { parameter_description }
 * @param      noofNeighbors  The noof neighbors
 *
 * @return     { description_of_the_return_value }
 */
unsigned long *find_n_nearest_neighbours(struct kdNode *node, double x, double y, unsigned long n, unsigned long *noofNeighbors)
{
	unsigned long *indexes = NULL;
	unsigned long *newIndexes = NULL;
	unsigned long noofIndexes = 0;
	unsigned long noofNewIndexes = 0;
	unsigned long i, j;
	double distance = 0;

	tempX = x;
	tempY = y;

	// Check if the node is a leaf one //
	if (node->isLeaf == true)
	{
		noofIndexes = node->endIndex - node->startIndex + 1;
		indexes = (unsigned long *) calloc(noofIndexes, sizeof(unsigned long));
		assert(indexes != NULL);

		for (i = 0, j = node->startIndex; j <= node->endIndex; i++, j++)
		{
			indexes[i] = j;
		}

		qsort(indexes, noofIndexes, sizeof(struct pointHashNode *), distance_comparator);

		if (noofIndexes > n)
		{
			indexes = (unsigned long *) realloc(indexes, n * sizeof(unsigned long));
			noofIndexes = n;
		}
	}
	else
	{
		if (node->axis == true)
		{
			if (isgreater(x, node->splitIndex->x))
			{
				indexes = find_n_nearest_neighbours(node->right, x, y, n, &noofIndexes);
				distance = euclidean_distance(x, y, node->splitIndex->x, node->splitIndex->y);
				if (isless(fabs(x - node->splitIndex->x), distance) == 1)
				{
					newIndexes = find_n_nearest_neighbours(node->left, x, y, n, &noofNewIndexes);
					if (newIndexes != NULL)
					{
						indexes = (unsigned long *) realloc(indexes, (noofIndexes + noofNewIndexes) * sizeof(unsigned long));
						assert(indexes != NULL);
						memcpy(&indexes[noofIndexes], newIndexes, noofNewIndexes * sizeof(unsigned long));
						noofIndexes += noofNewIndexes;

						qsort(indexes, noofIndexes, sizeof(struct pointHashNode *), distance_comparator);
						if (noofIndexes > n)
						{
							indexes = (unsigned long *) realloc(indexes, n * sizeof(unsigned long));
							assert(indexes != NULL);
							noofIndexes = n;
						}
					}
					free(newIndexes);
				}
			}
			else
			{
				indexes = find_n_nearest_neighbours(node->left, x, y, n, &noofIndexes);
				distance = euclidean_distance(x, y, node->splitIndex->x, node->splitIndex->y);
				if (isless(fabs(x - node->splitIndex->x), distance) == 1)
				{
					newIndexes = find_n_nearest_neighbours(node->right, x, y, n, &noofNewIndexes);
					if (newIndexes != NULL)
					{
						indexes = (unsigned long *) realloc(indexes, (noofIndexes + noofNewIndexes) * sizeof(unsigned long));
						assert(indexes != NULL);
						memcpy(&indexes[noofIndexes], newIndexes, noofNewIndexes * sizeof(unsigned long));
						noofIndexes += noofNewIndexes;

						qsort(indexes, noofIndexes, sizeof(struct pointHashNode *), distance_comparator);
						if (noofIndexes > n)
						{
							indexes = (unsigned long *) realloc(indexes, n * sizeof(unsigned long));
							assert(indexes != NULL);
							noofIndexes = n;
						}
					}
					free(newIndexes);
				}
			}
		}
		else
		{
			if (isgreater(y, node->splitIndex->y))
			{
				indexes = find_n_nearest_neighbours(node->right, x, y, n, &noofIndexes);
				distance = euclidean_distance(x, y, node->splitIndex->x, node->splitIndex->y);
				if (isless(fabs(y - node->splitIndex->y), distance) == 1)
				{
					newIndexes = find_n_nearest_neighbours(node->left, x, y, n, &noofNewIndexes);
					if (newIndexes != NULL)
					{
						indexes = (unsigned long *) realloc(indexes, (noofIndexes + noofNewIndexes) * sizeof(unsigned long));
						assert(indexes != NULL);
						memcpy(&indexes[noofIndexes], newIndexes, noofNewIndexes * sizeof(unsigned long));
						noofIndexes += noofNewIndexes;

						qsort(indexes, noofIndexes, sizeof(struct pointHashNode *), distance_comparator);
						if (noofIndexes > n)
						{
							indexes = (unsigned long *) realloc(indexes, n * sizeof(unsigned long));
							assert(indexes != NULL);
							noofIndexes = n;
						}
					}
					free(newIndexes);
				}
			}
			else
			{
				indexes = find_n_nearest_neighbours(node->left, x, y, n, &noofIndexes);
				distance = euclidean_distance(x, y, node->splitIndex->x, node->splitIndex->y);
				if (isless(fabs(y - node->splitIndex->y), distance) == 1)
				{
					newIndexes = find_n_nearest_neighbours(node->right, x, y, n, &noofNewIndexes);
					if (newIndexes != NULL)
					{
						indexes = (unsigned long *) realloc(indexes, (noofIndexes + noofNewIndexes) * sizeof(unsigned long));
						assert(indexes != NULL);
						memcpy(&indexes[noofIndexes], newIndexes, noofNewIndexes * sizeof(unsigned long));
						noofIndexes += noofNewIndexes;

						qsort(indexes, noofIndexes, sizeof(struct pointHashNode *), distance_comparator);
						if (noofIndexes > n)
						{
							indexes = (unsigned long *) realloc(indexes, n * sizeof(unsigned long));
							assert(indexes != NULL);
							noofIndexes = n;
						}
					}
					free(newIndexes);
				}
			}
		}
	}
	
	*noofNeighbors = noofIndexes;
	
	return indexes;
}

void print_nearest(double x, double y)
{
	create_KD_tree();
	printf("\r\nNearest Point: %s\r\n\n", pointArray[find_nearest_neighbour(kdTree, x, y)]->name);
}