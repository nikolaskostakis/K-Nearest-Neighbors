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
inline double euclidean_distance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow(fabs(x2 - x1), 2) + pow(fabs(y2 - y1), 2));
}

inline void insert_nearest_point_to_nn_array(unsigned long *array[], unsigned long *arraySize, unsigned long n, unsigned long index, double x, double y)
{
	unsigned long i;
	unsigned long size = *arraySize;
	unsigned long temp, swapTemp;
	double distance;
	double minDist;

	distance = euclidean_distance(x, y, pointArray[index]->x, pointArray[index]->y);

	temp = index;
	for (i = 0; i < size; i++)
	{
		minDist = euclidean_distance(x, y, pointArray[*(*array + i)]->x, pointArray[*(*array + i)]->y);

		if (isless(distance, minDist) == 1)
		{
			swapTemp = *(*array + i);
			*(*array + i) = temp;
			temp = swapTemp;
		}
	}
	
	if (size < n)
	{
		*(*array + size) = temp;
		size += 1;
	}

	*arraySize = size;
}

inline void reorder_nn_array(unsigned long *array[], unsigned long arraySize, double x, double y)
{
	unsigned long i, j;
	unsigned long minIndex, temp;
	double distance = 0;
	double minDistance = 0;

	for (i = 0; i < arraySize; i++)
	{
		minIndex = i;
		minDistance = euclidean_distance(x, y, pointArray[*(*array + i)]->x, pointArray[*(*array + i)]->y);

		for (j = i + 1; j < arraySize; j++)
		{
			distance = euclidean_distance(x, y, pointArray[*(*array + j)]->x, pointArray[*(*array + j)]->y);

			if (isless(distance, minDistance) == 1)
			{
				minDistance = distance;
				minIndex = j;
			}
		}

		temp = *(*array + i);
		*(*array + i) = *(*array + minIndex);
		*(*array + minIndex) = temp;
	}
}

// Point Hash Table Functions //
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
		printf(YEL"Point Hash Table is empty!\n"NRM);
		return;
	}

	for (i = 0; i < pointHTSize; i++)
	{
		printf(BLU"Hash Code:     "MAG"%lu\n"NRM, i);
		printf(BLU"Stored Points:\n"NRM);

		if (pointHT[i].depth == 0)
		{
			printf(MAG"\t(null)\n"NRM);
		}
		else
		{
			for (j = 0; j < pointHT[i].depth; j++)
			{
				printf(MAG"\tPoint: "NRM"%s "MAG"("NRM"%.2lf"MAG","NRM"%.2lf"MAG")\n"NRM,pointHT[i].nodes[j].name, pointHT[i].nodes[j].x, pointHT[i].nodes[j].y);
			}
		}
		
	}
}

// Comparators //
// Compare the x axis values of two points on the array //
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

// Compare the y axis values of two points on the array //
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
		printf(BLU"Point: "NRM"%s\n", pointArray[i]->name);
		printf(MAG"\t("NRM"%.2lf"MAG","NRM"%.2lf"MAG")\n"NRM, pointArray[i]->x, pointArray[i]->y);
	}
}

// KD Tree Funcions //
/**
 * @brief Create the KD Tree
 * 
 */
void create_KD_tree()
{
	// Free the previous tree //
	free_KD_tree();
	
	free_sorting_array();
	create_sorting_array();

	// Create the head of the tree //
	kdTree = (struct kdNode *) calloc(1, sizeof(struct kdNode));
	assert(kdTree != NULL);

	kdTree->isLeaf = false;
	kdTree->axis = 0;
	kdTree->startIndex = 0;
	kdTree->endIndex = pointArraySize - 1;

	// Sort the array based on the x coordinate //
	qsort((void *)pointArray, pointArraySize, sizeof(struct pointHashNode *), point_x_comparator);
	
	// Cut the array in two halves //
	kdTree->splitIndex = pointArraySize / 2;
	kdTree->left = insert_KD_tree_node(kdTree, kdTree->startIndex, kdTree->splitIndex);
	kdTree->right = insert_KD_tree_node(kdTree, (kdTree->splitIndex + 1), kdTree->endIndex);
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
		node->isLeaf = false;
		node->splitIndex = startIndex + ((endIndex - startIndex) / 2);
		node->axis = ~parent->axis;
		if (node->axis == 0) // x axis //
		{
			qsort((void *)&pointArray[startIndex], noofElements, sizeof(struct pointHashNode *), point_x_comparator);
		}
		else // y axis //
		{
			qsort((void *)&pointArray[startIndex], noofElements, sizeof(struct pointHashNode *), point_y_comparator);
		}
		
		node->left = insert_KD_tree_node(node, startIndex, node->splitIndex);
		node->right = insert_KD_tree_node(node, (node->splitIndex + 1), endIndex);
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

// Free this node of the KD Tree//
/**
 * @brief Free this node
 * 
 * @param node 
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

// Printe the KD Tree //
// Actually a wrapper for print_KD_node //
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
	printf(BLU"Start Index: "NRM"%lu\n", node->startIndex);
	
	print_tabs(depth);
	printf(BLU"End Index:   "NRM"%lu\n", node->endIndex);

	if (node->isLeaf == true)
	{
		print_tabs(depth);
		printf(MAG"\tLeaf node!\n");
	}
	else
	{
		print_tabs(depth);
		printf(MAG"\tLeft child:\n");
		print_KD_node(node->left, (depth + 1));

		print_tabs(depth);
		printf(MAG"\tRight child:\n");
		print_KD_node(node->right, (depth + 1));
	}
	
}

/**
 * @brief Find the nearest neighbour from certain coordinnates
 * 
 * @param node The KD node on which the neigbour is searced
 * @param x The given x-coordinate
 * @param y The given y-coordinate
 * @return unsigned long Returns the index to the Reference Array
 */
unsigned long find_nearest_neighbour(struct kdNode *node, double x, double y)
{
	double minDistance = 0;
	double distance = 0;
	unsigned long minIndex = 0;
	unsigned long secondIndex = 0;
	unsigned long i;

	if (node->isLeaf == true)
	{
		minIndex = node->startIndex; 
		minDistance = euclidean_distance(x, y, pointArray[minIndex]->x, pointArray[minIndex]->y);
		for (i = (node->startIndex + 1); i <= node->endIndex; i++)
		{
			distance = euclidean_distance(x, y, pointArray[i]->x, pointArray[i]->y);
			if (isless(distance, minDistance) == 1)
			{
				minDistance = distance;
				minIndex = i;
			}
		}
	}
	else
	{
		if (node->axis == 0)
		{
			if (isgreater(x, pointArray[node->splitIndex]->x))
			{
				minIndex = find_nearest_neighbour(node->right, x, y);
				minDistance = euclidean_distance(x, y, pointArray[minIndex]->x, pointArray[minIndex]->y);
				if (isless(fabs(x - pointArray[node->splitIndex]->x), minDistance) == 1)
				{
					secondIndex = find_nearest_neighbour(node->left, x, y);
					distance = euclidean_distance(x, y, pointArray[secondIndex]->x, pointArray[secondIndex]->y);
					if (isless(distance, minDistance) == 1)
					{
						minIndex = secondIndex;
					}
				}
			}
			else
			{
				minIndex = find_nearest_neighbour(node->left, x, y);
				minDistance = euclidean_distance(x, y, pointArray[minIndex]->x, pointArray[minIndex]->y);
				if (isless(fabs(x - pointArray[node->splitIndex]->x), minDistance) == 1)
				{
					secondIndex = find_nearest_neighbour(node->right, x, y);
					distance = euclidean_distance(x, y, pointArray[secondIndex]->x, pointArray[secondIndex]->y);
					if (isless(distance, minDistance) == 1)
					{
						minIndex = secondIndex;
					}
				}
			}
		}
		else
		{
			if (isgreater(y, pointArray[node->splitIndex]->y))
			{
				minIndex = find_nearest_neighbour(node->right, x, y);
				minDistance = euclidean_distance(x, y, pointArray[minIndex]->x, pointArray[minIndex]->y);
				if (isless(fabs(y - pointArray[node->splitIndex]->y), minDistance) == 1)
				{
					secondIndex = find_nearest_neighbour(node->left, x, y);
					distance = euclidean_distance(x, y, pointArray[secondIndex]->x, pointArray[secondIndex]->y);
					if (isless(distance, minDistance) == 1)
					{
						minIndex = secondIndex;
					}
				}
			}
			else
			{
				minIndex = find_nearest_neighbour(node->left, x, y);
				minDistance = euclidean_distance(x, y, pointArray[minIndex]->x, pointArray[minIndex]->y);
				if (isless(fabs(y - pointArray[node->splitIndex]->y), minDistance) == 1)
				{
					secondIndex = find_nearest_neighbour(node->right, x, y);
					distance = euclidean_distance(x, y, pointArray[secondIndex]->x, pointArray[secondIndex]->y);
					if (isless(distance, minDistance) == 1)
					{
						minIndex = secondIndex;
					}
				}
			}
		}
	}
	
	return minIndex;
}

unsigned long *find_nearest_neighbours_within_radius(struct kdNode *node, double x, double y, double radius, unsigned long *noofNeighbors)
{
	double distance = 0;
	unsigned long *indexes = NULL;
	unsigned long *newIndexes = NULL;
	unsigned long noofIndexes = 0;
	unsigned long noofNewIndexes = 0;
	unsigned long i;

	if (node->isLeaf == true)
	{
		for (i = (node->startIndex + 1); i <= node->endIndex; i++)
		{
			distance = euclidean_distance(x, y, pointArray[i]->x, pointArray[i]->y);
			if (islessequal(distance, radius) == 1)
			{
				indexes = (unsigned long *) realloc(indexes, (noofIndexes + 1) * sizeof(unsigned long));
				assert(indexes != NULL);

				indexes[noofIndexes] = i;
				noofIndexes++;
			}
		}
	}
	else
	{
		if (node->axis == 0)
		{
			if (isgreater(x, pointArray[node->splitIndex]->x))
			{
				indexes = find_nearest_neighbours_within_radius(node->right, x, y, radius, &noofIndexes);
				distance = euclidean_distance(x, y, pointArray[node->splitIndex]->x, pointArray[node->splitIndex]->y);
				if (isless(fabs(x - pointArray[node->splitIndex]->x), distance) == 1)
				{
					newIndexes = find_nearest_neighbours_within_radius(node->left, x, y, radius, &noofNewIndexes);
					if (newIndexes != NULL)
					{
						indexes = (unsigned long *) realloc(indexes, (noofIndexes + noofNewIndexes) * sizeof(unsigned long));
						assert(indexes != NULL);
						memcpy(&indexes[noofIndexes], newIndexes, noofNewIndexes * sizeof(unsigned long));
						noofIndexes += noofNewIndexes;
					}
				}
			}
			else
			{
				indexes = find_nearest_neighbours_within_radius(node->left, x, y, radius, &noofIndexes);
				distance = euclidean_distance(x, y, pointArray[node->splitIndex]->x, pointArray[node->splitIndex]->y);
				if (isless(fabs(x - pointArray[node->splitIndex]->x), distance) == 1)
				{
					newIndexes = find_nearest_neighbours_within_radius(node->right, x, y, radius, &noofNewIndexes);
					if (newIndexes != NULL)
					{
						indexes = (unsigned long *) realloc(indexes, (noofIndexes + noofNewIndexes) * sizeof(unsigned long));
						assert(indexes != NULL);
						memcpy(&indexes[noofIndexes], newIndexes, noofNewIndexes * sizeof(unsigned long));
						noofIndexes += noofNewIndexes;
					}
				}
			}
		}
		else
		{
			if (isgreater(y, pointArray[node->splitIndex]->y))
			{
				indexes = find_nearest_neighbours_within_radius(node->right, x, y, radius, &noofIndexes);
				distance = euclidean_distance(x, y, pointArray[node->splitIndex]->x, pointArray[node->splitIndex]->y);
				if (isless(fabs(y - pointArray[node->splitIndex]->y), distance) == 1)
				{
					newIndexes = find_nearest_neighbours_within_radius(node->left, x, y, radius, &noofNewIndexes);
					if (newIndexes != NULL)
					{
						indexes = (unsigned long *) realloc(indexes, (noofIndexes + noofNewIndexes) * sizeof(unsigned long));
						assert(indexes != NULL);
						memcpy(&indexes[noofIndexes], newIndexes, noofNewIndexes * sizeof(unsigned long));
						noofIndexes += noofNewIndexes;
					}
				}
			}
			else
			{
				indexes = find_nearest_neighbours_within_radius(node->left, x, y, radius, &noofIndexes);
				distance = euclidean_distance(x, y, pointArray[node->splitIndex]->x, pointArray[node->splitIndex]->y);
				if (isless(fabs(y - pointArray[node->splitIndex]->y), distance) == 1)
				{
					newIndexes = find_nearest_neighbours_within_radius(node->right, x, y, radius, &noofNewIndexes);
					if (newIndexes != NULL)
					{
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

unsigned long *find_n_nearest_neighbours(struct kdNode *node, double x, double y, unsigned long n, unsigned long *noofNeighbors)
{
	unsigned long *indexes = NULL;
	unsigned long *newIndexes = NULL;
	unsigned long noofIndexes = 0;
	unsigned long noofNewIndexes = 0;
	unsigned long i;
	double distance = 0;

	if (node->isLeaf == true)
	{
		indexes = (unsigned long *) calloc(n, sizeof(unsigned long));
		assert(indexes != NULL);

		for (i = (node->startIndex + 1); i <= node->endIndex; i++)
		{
			insert_nearest_point_to_nn_array(&indexes, &noofIndexes, n, i, x, y);
		}

		if (noofIndexes < n)
		{
			indexes = (unsigned long *) realloc(indexes, noofIndexes * sizeof(unsigned long));
		}
	}
	else
	{
		if (node->axis == 0)
		{
			if (isgreater(x, pointArray[node->splitIndex]->x))
			{
				indexes = find_n_nearest_neighbours(node->right, x, y, n, &noofIndexes);
				distance = euclidean_distance(x, y, pointArray[node->splitIndex]->x, pointArray[node->splitIndex]->y);
				if (isless(fabs(x - pointArray[node->splitIndex]->x), distance) == 1)
				{
					newIndexes = find_n_nearest_neighbours(node->left, x, y, n, &noofNewIndexes);
					if (newIndexes != NULL)
					{
						indexes = (unsigned long *) realloc(indexes, (noofIndexes + noofNewIndexes) * sizeof(unsigned long));
						assert(indexes != NULL);
						memcpy(&indexes[noofIndexes], newIndexes, noofNewIndexes * sizeof(unsigned long));
						noofIndexes += noofNewIndexes;

						reorder_nn_array(&indexes, noofIndexes, x, y);
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
				distance = euclidean_distance(x, y, pointArray[node->splitIndex]->x, pointArray[node->splitIndex]->y);
				if (isless(fabs(x - pointArray[node->splitIndex]->x), distance) == 1)
				{
					newIndexes = find_n_nearest_neighbours(node->right, x, y, n, &noofNewIndexes);
					if (newIndexes != NULL)
					{
						indexes = (unsigned long *) realloc(indexes, (noofIndexes + noofNewIndexes) * sizeof(unsigned long));
						assert(indexes != NULL);
						memcpy(&indexes[noofIndexes], newIndexes, noofNewIndexes * sizeof(unsigned long));
						noofIndexes += noofNewIndexes;

						reorder_nn_array(&indexes, noofIndexes, x, y);
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
			if (isgreater(y, pointArray[node->splitIndex]->y))
			{
				indexes = find_n_nearest_neighbours(node->right, x, y, n, &noofIndexes);
				distance = euclidean_distance(x, y, pointArray[node->splitIndex]->x, pointArray[node->splitIndex]->y);
				if (isless(fabs(y - pointArray[node->splitIndex]->y), distance) == 1)
				{
					newIndexes = find_n_nearest_neighbours(node->left, x, y, n, &noofNewIndexes);
					if (newIndexes != NULL)
					{
						indexes = (unsigned long *) realloc(indexes, (noofIndexes + noofNewIndexes) * sizeof(unsigned long));
						assert(indexes != NULL);
						memcpy(&indexes[noofIndexes], newIndexes, noofNewIndexes * sizeof(unsigned long));
						noofIndexes += noofNewIndexes;

						reorder_nn_array(&indexes, noofIndexes, x, y);
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
				distance = euclidean_distance(x, y, pointArray[node->splitIndex]->x, pointArray[node->splitIndex]->y);
				if (isless(fabs(y - pointArray[node->splitIndex]->y), distance) == 1)
				{
					newIndexes = find_n_nearest_neighbours(node->right, x, y, n, &noofNewIndexes);
					if (newIndexes != NULL)
					{
						indexes = (unsigned long *) realloc(indexes, (noofIndexes + noofNewIndexes) * sizeof(unsigned long));
						assert(indexes != NULL);
						memcpy(&indexes[noofIndexes], newIndexes, noofNewIndexes * sizeof(unsigned long));
						noofIndexes += noofNewIndexes;

						reorder_nn_array(&indexes, noofIndexes, x, y);
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
	printf("\nNearest Point: %s\n\n", pointArray[find_nearest_neighbour(kdTree, x, y)]->name);
}