#include "structs.h"
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

// *** point_y_comparator *** //
// Comparator function for qsort() //
// Compare the y coordinate of the two compared  //
// points and decise their positions accordingly //
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

// *** distance_comparator *** //
// Comparator function for qsort() //
// Compare the distances of the two compared points to //
// given point and decise their positions accordingly  //
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

// *** point_hash_function *** //
// Simple Hash function //
unsigned long point_hash_function(char *name)
{
	int i;
	int length = strlen(name);  
	unsigned long result = 0;

	// For each character of the string //
	for (i = 0; i < length; i++)
	{
		// Add the character to the result //
		result += name[i];
	}
		
	return result;
}

// *** insert_point *** //
// Insert a point to the Hash Table //
void insert_point(char *name, double x, double y)
{
	unsigned long hash = 0;

	// If the Hash Table is empty //
	if (pointHTSize == 0)
	{
		// Create the Hash Table //
		pointHT = (struct pointHash *) calloc(primes[pointHTPrimeIndex], sizeof(struct pointHash));
		assert(pointHT != NULL);
		
		pointHTSize = primes[pointHTPrimeIndex];
		pointHTPrimeIndex++;
	}

	// Find the hash //
	hash = point_hash_function(name) % pointHTSize;
	
	// Add the point into the Hash Table //
	pointHT[hash].nodes = (struct pointHashNode *) realloc(pointHT[hash].nodes, ((pointHT[hash].depth + 1) * sizeof(struct pointHashNode)));
	assert(pointHT[hash].nodes != NULL);

	pointHT[hash].nodes[pointHT[hash].depth].name = name;
	pointHT[hash].nodes[pointHT[hash].depth].x = x;
	pointHT[hash].nodes[pointHT[hash].depth].y = y;

	// Icrease the depth of the bucket for the given hash //
	pointHT[hash].depth++;

	// If the depth of the bucket exceeds the maimum //
	if (pointHT[hash].depth == MAX_HASH_DEPTH)
	{
		// Rehash the Hash Table //
		rehash_point_hash();
	}
}

// *** rehash_point_hash *** //
// Rehashing of the Hash Table //
// Increase its size and redistribute the content of the buckets // 
void rehash_point_hash()
{
	struct pointHash *oldHash = pointHT;
	unsigned long oldHashSize = pointHTSize;
	unsigned long i;
	unsigned long hash = 0;
	int j;

	// Allocate a new hash table //
	// If the size of the old Hash Table has been changed more //
	// times than the maximum given number of prime numbers    //
	if (pointHTPrimeIndex == MAX_NOOF_PRIMES)
	{
		// Create a new one with double the size //
		pointHT = (struct pointHash *) calloc((2 * pointHTSize), sizeof(struct pointHash));
		assert(pointHT != NULL);

		pointHTSize *= 2;
	}
	// If it has not //
	else
	{
		// Create a new with its size being the next prime number //
		pointHT = (struct pointHash *) calloc(primes[pointHTPrimeIndex], sizeof(struct pointHash));
		assert(pointHT != NULL);

		pointHTSize = primes[pointHTPrimeIndex];
		pointHTPrimeIndex++;
	}
	
	// Transfer data from the old Hash Table to the new //
	// For each bucket of the old Hash Table
	for (i = 0; i < oldHashSize; i++)
	{
		// If the bucket is empty //
		if (oldHash[i].depth == 0)
		{
			// Continue to the next//
			continue;
		}

		// For each point into the bucket //
		for (j = 0; j < oldHash[i].depth; j++)
		{
			// Find a new hash based on the new Hash Table //
			hash = point_hash_function(oldHash[i].nodes[j].name) % pointHTSize;

			// Add the point into the corresponding bucket of the new Hash Table //
			pointHT[hash].nodes = (struct pointHashNode *) realloc(pointHT[hash].nodes, ((pointHT[hash].depth + 1) * sizeof(struct pointHashNode)));
			assert(pointHT[hash].nodes != NULL);

			pointHT[hash].nodes[pointHT[hash].depth].name = oldHash[i].nodes[j].name;
			pointHT[hash].nodes[pointHT[hash].depth].x = oldHash[i].nodes[j].x;
			pointHT[hash].nodes[pointHT[hash].depth].y = oldHash[i].nodes[j].y;

			pointHT[hash].depth++;
		}

		// Free the bucket of the old Hash table //
		free(oldHash[i].nodes);
	}

	// Free the old Hash Table //
	free(oldHash);
}

// *** get_point_hash_depth *** //
// Get the depth of a given point //
inline int get_point_hash_depth(unsigned long hash)
{
	return pointHT[hash].depth;
}

// *** get_point_x_coord *** //
// Get the x coordinate of a given point //
inline double get_point_x_coord(unsigned long hash, int depth)
{
	return pointHT[hash].nodes[depth].x;
}

// *** get_point_y_coord *** //
// Get the y coordinate of a given point //
inline double get_point_y_coord(unsigned long hash, int depth)
{
	return pointHT[hash].nodes[depth].y;
}

// *** get_point_name *** //
// Get the name of a given point //
inline char *get_point_name(unsigned long hash, int depth)
{
	return pointHT[hash].nodes[depth].name;
}

// *** free_point_hash *** //
// Free the point Hash Table //
void free_point_hash()
{
	unsigned long i;
	int j;

	// If the Hash Table is empty //
	if (pointHT == NULL)
	{
		// Do nothing //
		return;
	}

	// For each bucket of the Hash Table //
	for (i = 0; i < pointHTSize; i++)
	{
		// If the bucket is empty //
		if (pointHT[i].depth == 0)
		{
			// Continue to the next //
			continue;
		}

		// For each point into the bucket //
		for (j = 0; j < pointHT[i].depth; j++)
		{
			// Free its name //
			free(pointHT[i].nodes[j].name);
		}

		// Free the bucket
		free(pointHT[i].nodes);
	}

	// Free the Hash Table //
	free(pointHT);
	pointHT = NULL;
	pointHTSize = 0;
	pointHTPrimeIndex = 0;
}

// *** print_point_hash *** //
// Print the content of the point Hash Table //
void print_point_hash()
{
	unsigned long i;
	int j;

	// If the Hash Table is empty //
	if (pointHTSize == 0)
	{
		// Print a correspoinding message //
		printf(YEL"Point Hash Table is empty!\r\n"NRM);
		return;
	}

	// For each bucket of the Hash Table //
	for (i = 0; i < pointHTSize; i++)
	{
		// Print its hash // 
		printf(BLU"Hash:     "MAG"%lu\r\n"NRM, i);
		printf(BLU"Stored Points:\n"NRM);

		// If its empty //
		if (pointHT[i].depth == 0)
		{
			// Print a corresponding message //
			printf(MAG"\t(null)\r\n"NRM);
		}
		// If it is not //
		else
		{
			// For each point into the bucket //
			for (j = 0; j < pointHT[i].depth; j++)
			{
				// Print is name and coordinates //
				printf(MAG"\tPoint: "NRM"%s "MAG"("NRM"%.2lf"MAG","NRM"%.2lf"MAG")\r\n"NRM, pointHT[i].nodes[j].name, pointHT[i].nodes[j].x, pointHT[i].nodes[j].y);
			}
		}
		
	}
}

// *** print_point_hash_distances *** //
// Debug Function //
// Print the distance of the given coordinates to all the stored points //
void print_point_hash_distances(double x, double y)
{
	unsigned long i;
	int j;
	double distance;

	// If the Hash Table is empty //
	if (pointHTSize == 0)
	{
		// Print a cooresponding message //
		printf(YEL"Point Hash Table is empty!\r\n"NRM);
		return;
	}

	// For each bucket of the hash table //
	for (i = 0; i < pointHTSize; i++)
	{
		// If the Hash Table is empty //
		if (pointHT[i].depth == 0)
		{
			// Continue to the next //
			continue;
		}

		// For each point of the bucket //
		for (j = 0; j < pointHT[i].depth; j++)
		{
			// Find its distance to the given point and print it //
			distance = euclidean_distance(x, y, pointHT[i].nodes[j].x, pointHT[i].nodes[j].y);
			printf(RED"--->"NRM"Distance from %s: %lf\r\n", pointHT[i].nodes[j].name, distance);
		}
	}
}

// Sorting Array for KD Tree //

// *** create_sorting_array *** //
// Create a sorting array that points into the Points   //
// hash table. It is used to be sorted for the KD tree. //
void create_sorting_array()
{
	unsigned long i, k;
	int j;

	// First Pass - Find the size of the array by adding the number of points in each bucket//
	for (i = 0; i < pointHTSize; i++)
	{
		pointArraySize += pointHT[i].depth;
	}

	// Allocate memory for the array //
	pointArray = (struct pointHashNode **) calloc(pointArraySize, sizeof(struct pointHashNode *));
	assert(pointArray != NULL);

	// Second Pass - Fill the array //
	// For each bucket of the Hash Table //
	for (i = 0, k = 0; i < pointHTSize; i++)
	{
		// For each point into the bucket //
		for(j = 0; j < pointHT[i].depth; j++)
		{
			// Add a reference to the point into the array //
			pointArray[k] = &pointHT[i].nodes[j];
			k++;
		}
	}
}

// *** free_sorting_array *** //
// Free the sorting array //
void free_sorting_array()
{
 	// If the array is empty //
	if (pointArray == NULL)
	{
		// Do nothing //
		return;
	}

	// Free the array //
	free(pointArray);
	pointArray = NULL;
	pointArraySize = 0;
}

// *** print_sorting_array *** //
// Print the content of the sorting array //
void print_sorting_array()
{
	unsigned long i;

	// For each point referred into the array //
	for (i = 0; i < pointArraySize; i++)
	{
		// Print its name and coordinates //
		printf(BLU"Point: "NRM"%s\r\n", pointArray[i]->name);
		printf(MAG"\t("NRM"%.2lf"MAG","NRM"%.2lf"MAG")\r\n"NRM, pointArray[i]->x, pointArray[i]->y);
	}
}

// KD Tree Funcions //

// *** create_KD_tree *** //
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

// *** insert_KD_tree_node *** //
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

// *** free_KD_tree *** //
// Free the KD Tree //
void free_KD_tree()
{
	if (kdTree == NULL)
	{
		return;
	}

	free_KD_node(kdTree);
	kdTree = NULL;
}

// *** free_KD_node *** //
// Free a node of the KD tree //
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

// *** print_KD_tree *** //
// Print the KD tree and its content. //
void print_KD_tree()
{
	if (kdTree == NULL)
	{
		printf(YEL"KD Tree Empty!\r\n"NRM);
		return;
	}

	printf(MAG"Root:\r\n"NRM);
	print_KD_node(kdTree, 0);
}

// *** print_tabs *** //
// Supplementary function that prints a certain number of tabs. It //
// is used conjuction with print_KD_node() to print the KD tree.   //
inline void print_tabs(int depth)
{
	int i;

	for (i = 0; i < depth; i++)
	{
		putchar('\t');
	}
}

// *** print_KD_node *** //
// Print a given node of the KD tree in a certain depth. //
// For the node print its start & end indexes of its     //
// sub-array and its children, if its not a leaf node.   //
void print_KD_node(struct kdNode *node, int depth)
{
	// Pint the start index of the node's sub-array //
	print_tabs(depth);
	printf(BLU"Start Index: "NRM"%lu\r\n", node->startIndex);

	// Pint the end index of the node's sub-array //
	print_tabs(depth);
	printf(BLU"End Index:   "NRM"%lu\r\n", node->endIndex);

	// Check if the node is leaf //
	if (node->isLeaf == true)
	{
		// Print a corresponding message //
		print_tabs(depth);
		printf(MAG"\tLeaf node!\r\n");
	}
	// If it is not, print its children //
	else
	{
		// First the left child //
		print_tabs(depth);
		printf(MAG"\tLeft child:\r\n");
		print_KD_node(node->left, (depth + 1));

		// Then the right //
		print_tabs(depth);
		printf(MAG"\tRight child:\r\n");
		print_KD_node(node->right, (depth + 1));
	}
}

// *** find_nearest_neighbour *** //
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


// *** find_nearest_neighbours_within_radius *** //
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
	
	// Pass the number of indexes //
	*noofNeighbors = noofIndexes;

	// Return the array of the indexes //
	return indexes;
}

// *** find_n_nearest_neighbours *** //
unsigned long *find_n_nearest_neighbours(struct kdNode *node, double x, double y, unsigned long n, unsigned long *noofNeighbors)
{
	unsigned long *indexes = NULL;
	unsigned long *newIndexes = NULL;
	unsigned long noofIndexes = 0;
	unsigned long noofNewIndexes = 0;
	unsigned long i, j;
	double distance = 0;

	// Assign the given coordinates to the global variables used in sorting //
	tempX = x;
	tempY = y;

	// Check if the node is a leaf one //
	if (node->isLeaf == true)
	{
		// Create an array the size of all the points designated to the leaf //
		noofIndexes = node->endIndex - node->startIndex + 1;
		indexes = (unsigned long *) calloc(noofIndexes, sizeof(unsigned long));
		assert(indexes != NULL);

		// Run through the array //
		for (i = 0, j = node->startIndex; j <= node->endIndex; i++, j++)
		{
			// Fill it with the indexes of the points //
			indexes[i] = j;
		}

		// Sort the array of indexes based on their distance from the given coordinates //
		qsort(indexes, noofIndexes, sizeof(struct pointHashNode *), distance_comparator);

		// If the number of indexes in the leaf exceed the number of required neighbors //
		if (noofIndexes > n)
		{
			// Shrink the array to the required size //
			indexes = (unsigned long *) realloc(indexes, n * sizeof(unsigned long));
			noofIndexes = n;
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
				// Find the nearest points on this half //
				indexes = find_n_nearest_neighbours(node->right, x, y, n, &noofIndexes);
				
				// If the distance between the given x and the distance of the furthest point is less or equal with the radius //
				distance = euclidean_distance(x, y, pointArray[indexes[noofIndexes - 1]]->x, pointArray[indexes[noofIndexes - 1]]->y);
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
			// If the given x coordinate is before or on the split //
			else
			{
				// Find the nearest points on this half //
				indexes = find_n_nearest_neighbours(node->left, x, y, n, &noofIndexes);
				distance = euclidean_distance(x, y, pointArray[indexes[noofIndexes - 1]]->x, pointArray[indexes[noofIndexes - 1]]->y);
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
		// For the y axis //
		else
		{
			// If the given y coordinate is after the split //
			if (isgreater(y, node->splitIndex->y))
			{
				// Find the nearest points on this half //
				indexes = find_n_nearest_neighbours(node->right, x, y, n, &noofIndexes);
				distance = euclidean_distance(x, y, pointArray[indexes[noofIndexes - 1]]->x, pointArray[indexes[noofIndexes - 1]]->y);
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
			// If the given y coordinate is before or on the split //
			else
			{
				// Find the nearest points on this half //
				indexes = find_n_nearest_neighbours(node->left, x, y, n, &noofIndexes);
				distance = euclidean_distance(x, y, pointArray[indexes[noofIndexes - 1]]->x, pointArray[indexes[noofIndexes - 1]]->y);
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
	
	// Pass the number of indexes //
	*noofNeighbors = noofIndexes;
	
	// Return the array of the indexes //
	return indexes;
}

// *** print_nearest *** //
void print_nearest(double x, double y)
{
	create_KD_tree();
	printf("\r\nNearest Point: %s\r\n\n", pointArray[find_nearest_neighbour(kdTree, x, y)]->name);
}
