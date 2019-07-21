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
struct kdTreeNode *kdTree = NULL;
unsigned long kdTreeSize = 0;
unsigned long kdTreeArraySize = 0;

// Pointer Array for KD-Tree //
struct pointHashElement **pointArray = NULL;
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
	if (isgreater((double)(*(struct pointHashElement **)p1)->x, (double)(*(struct pointHashElement **)p2)->x) == 1)
	{
		return 1;
	}
	// If not check if the second's one has greater value //
	else if (isless((double)(*(struct pointHashElement **)p1)->x, (double)(*(struct pointHashElement **)p2)->x) == 1)
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
	if (isgreater((double)(*(struct pointHashElement **)p1)->y, (double)(*(struct pointHashElement **)p2)->y) == 1)
	{
		return 1;
	}
	// If not check if the second's one has greater value //
	else if (isless((double)(*(struct pointHashElement **)p1)->y, (double)(*(struct pointHashElement **)p2)->y) == 1)
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
	pointHT[hash].bucket = (struct pointHashElement *) realloc(pointHT[hash].bucket, ((pointHT[hash].depth + 1) * sizeof(struct pointHashElement)));
	assert(pointHT[hash].bucket != NULL);

	pointHT[hash].bucket[pointHT[hash].depth].name = name;
	pointHT[hash].bucket[pointHT[hash].depth].x = x;
	pointHT[hash].bucket[pointHT[hash].depth].y = y;

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
			hash = point_hash_function(oldHash[i].bucket[j].name) % pointHTSize;

			// Add the point into the corresponding bucket of the new Hash Table //
			pointHT[hash].bucket = (struct pointHashElement *) realloc(pointHT[hash].bucket, ((pointHT[hash].depth + 1) * sizeof(struct pointHashElement)));
			assert(pointHT[hash].bucket != NULL);

			pointHT[hash].bucket[pointHT[hash].depth].name = oldHash[i].bucket[j].name;
			pointHT[hash].bucket[pointHT[hash].depth].x = oldHash[i].bucket[j].x;
			pointHT[hash].bucket[pointHT[hash].depth].y = oldHash[i].bucket[j].y;

			pointHT[hash].depth++;
		}

		// Free the bucket of the old Hash table //
		free(oldHash[i].bucket);
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
	return pointHT[hash].bucket[depth].x;
}

// *** get_point_y_coord *** //
// Get the y coordinate of a given point //
inline double get_point_y_coord(unsigned long hash, int depth)
{
	return pointHT[hash].bucket[depth].y;
}

// *** get_point_name *** //
// Get the name of a given point //
inline char *get_point_name(unsigned long hash, int depth)
{
	return pointHT[hash].bucket[depth].name;
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
			free(pointHT[i].bucket[j].name);
		}

		// Free the bucket
		free(pointHT[i].bucket);
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
				printf(MAG"\tPoint: "NRM"%s "MAG"("NRM"%.2lf"MAG","NRM"%.2lf"MAG")\r\n"NRM, pointHT[i].bucket[j].name, pointHT[i].bucket[j].x, pointHT[i].bucket[j].y);
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
			distance = euclidean_distance(x, y, pointHT[i].bucket[j].x, pointHT[i].bucket[j].y);
			printf(RED"--->"NRM"Distance from %s: %lf\r\n", pointHT[i].bucket[j].name, distance);
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
	pointArray = (struct pointHashElement **) calloc(pointArraySize, sizeof(struct pointHashElement *));
	assert(pointArray != NULL);

	// Second Pass - Fill the array //
	// For each bucket of the Hash Table //
	for (i = 0, k = 0; i < pointHTSize; i++)
	{
		// For each point into the bucket //
		for(j = 0; j < pointHT[i].depth; j++)
		{
			// Add a reference to the point into the array //
			pointArray[k] = &pointHT[i].bucket[j];
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
	kdTreeSize = 2;
	kdTreeArraySize = 2;

	// kdTree->isLeaf = false;
	kdTree[1].axis = X_AXIS; // Chose the x axis as the first one to check //
	kdTree[1].startIndex = 0;
	kdTree[1].endIndex = pointArraySize - 1;

	// If the elements in the sorting array are more than the defined max //
	noofElements = kdTree[1].endIndex - kdTree[1].startIndex + 1;
	if (noofElements > MAX_KDLEAF_ELEMENTS)
	{
		// Mark the node as a non leaf one //
		kdTree[1].isLeaf = 0;

		// Sort the array based on the x coordinate //
		qsort((void *)pointArray, pointArraySize, sizeof(struct pointHashElement *), point_x_comparator);
	
		// Save the split //
		split = pointArraySize / 2;
		kdTree[1].splitReference = pointArray[split];

		// Increase the tree size //
		kdTree = (struct kdTreeNode *) realloc(kdTree, ((2 * kdTreeArraySize) * sizeof(struct kdTreeNode)));;
		assert(kdTree != NULL);
		kdTreeSize += 2; // The new tree will have two more nodes //
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
			qsort((void *)&pointArray[startIndex], noofElements, sizeof(struct pointHashElement *), point_x_comparator);
		}
		else

		{
			qsort((void *)&pointArray[startIndex], noofElements, sizeof(struct pointHashElement *), point_y_comparator);
		}

		// Check if the tree has enough space for the new nodes //
		// If the index of the left new node is greater or //
		// equal to the size of the tree, double its size  //
		if ((2 * index) >= kdTreeArraySize)
		{
			kdTree = (struct kdTreeNode *) realloc(kdTree, ((2 * kdTreeArraySize) * sizeof(struct kdTreeNode)));
			assert(kdTree != NULL);
			kdTreeSize *= 2;
		}

		// Increase the size of the tree by two //
		kdTreeSize += 2;

		// Find the index of the split //
		split = startIndex + ((endIndex - startIndex) / 2);

		// Save the split //
		kdTree[index].splitReference = pointArray[split];

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

	kdTreeSize = 0;
	kdTreeArraySize = 0;
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
	print_KD_node(1, 0);
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
void print_KD_node(unsigned long index, int depth)
{
	// Pint the start index of the node's sub-array //
	print_tabs(depth);
	printf(BLU"Start Index: "NRM"%lu\r\n", kdTree[index].startIndex);

	// Pint the end index of the node's sub-array //
	print_tabs(depth);
	printf(BLU"End Index:   "NRM"%lu\r\n", kdTree[index].endIndex);

	// Check if the node is leaf //
	if (kdTree[index].isLeaf == 1)
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
		print_KD_node((2 * index), (depth + 1));

		// Then the right //
		print_tabs(depth);
		printf(MAG"\tRight child:\r\n");
		print_KD_node((2 * index + 1), (depth + 1));
	}
}

// *** find_nearest_neighbour *** //
unsigned long find_nearest_neighbour(unsigned long index, double x, double y)
{
	double minDistance = 0;
	double distance = 0;
	unsigned long minIndex = 0;
	unsigned long secondIndex = 0;
	unsigned long i;

	// Check if the node is a leaf one //
	if (kdTree[index].isLeaf == 1)
	{
		// Find the point with the smallest distance between the points of the leaf //

		// Start by assuming the first node has the smallest distance //
		minIndex = kdTree[index].startIndex; 
		minDistance = euclidean_distance(x, y, pointArray[minIndex]->x, pointArray[minIndex]->y);
		
		// Run through the rest of the points //
		for (i = (kdTree[index].startIndex + 1); i <= kdTree[index].endIndex; i++)
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
		if (kdTree[index].axis == X_AXIS)
		{
			// If the given x coordinate is after the split //
			if (isgreater(x, kdTree[index].splitReference->x))
			{
				// Find the nearest neighbour on this half //
				minIndex = find_nearest_neighbour(((2 * index) + 1), x, y);
				
				// If the distance between the given x and the split is less than the minimum distance //
				minDistance = euclidean_distance(x, y, pointArray[minIndex]->x, pointArray[minIndex]->y);
				if (isless(fabs(x - kdTree[index].splitReference->x), minDistance) == 1)
				{
					// Find the nearest neighbour on the other half and compare it with the previous //
					secondIndex = find_nearest_neighbour((2 * index), x, y);
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
				minIndex = find_nearest_neighbour((2 * index), x, y);
				
				// If the distance between the given x and the split is less than the minimum distance //
				minDistance = euclidean_distance(x, y, pointArray[minIndex]->x, pointArray[minIndex]->y);
				if (isless(fabs(x - kdTree[index].splitReference->x), minDistance) == 1)
				{
					// Find the nearest neighbour on the other half and compare it with the previous //
					secondIndex = find_nearest_neighbour(((2 * index) + 1), x, y);
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
			if (isgreater(y, kdTree[index].splitReference->y))
			{
				// Find the nearest neighbour on this half //
				minIndex = find_nearest_neighbour(((2 * index) + 1), x, y);
				
				// If the distance between the given y and the split is less than the minimum distance //
				minDistance = euclidean_distance(x, y, pointArray[minIndex]->x, pointArray[minIndex]->y);
				if (isless(fabs(y - kdTree[index].splitReference->y), minDistance) == 1)
				{
					// Find the nearest neighbour on the other half and compare it with the previous //
					secondIndex = find_nearest_neighbour((2 * index), x, y);
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
				minIndex = find_nearest_neighbour((2 * index), x, y);
				
				// If the distance between the given y and the split is less than the minimum distance //
				minDistance = euclidean_distance(x, y, pointArray[minIndex]->x, pointArray[minIndex]->y);
				if (isless(fabs(y - kdTree[index].splitReference->y), minDistance) == 1)
				{
					// Find the nearest neighbour on the other half and compare it with the previous //
					secondIndex = find_nearest_neighbour(((2 * index) + 1), x, y);
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

// *** print_nearest *** //
void print_nearest_neighbor(double x, double y)
{
	printf("\r\nNearest Neighbor: %s\r\n\n", pointArray[find_nearest_neighbour(ROOT_NODE, x, y)]->name);
}

// *** find_nearest_neighbours_within_radius *** //
unsigned long *find_nearest_neighbours_within_radius(unsigned long index, double x, double y, double radius, unsigned long *noofNeighbors)
{
	double distance = 0;
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
		if (kdTree[index].axis == X_AXIS)
		{
			// If the given x coordinate is after the split //
			if (isgreater(x, kdTree[index].splitReference->x))
			{
				// Find a list of points that are within radius on this half //
				indexes = find_nearest_neighbours_within_radius(((2 * index) + 1), x, y, radius, &noofIndexes);
				
				// If the distance between the given x and the split is less or equal with the radius //
				if (islessequal(fabs(x - kdTree[index].splitReference->x), radius) == 1)
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
					}
				}
			}
			// If the given x coordinate is before or on the split //
			else
			{
				// Find a list of points that are within radius on this half //
				indexes = find_nearest_neighbours_within_radius((2 * index), x, y, radius, &noofIndexes);
				
				// If the distance between the given x and the split is less or equal with the radius //
				if (islessequal(fabs(x - kdTree[index].splitReference->x), radius) == 1)
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
					}
				}
			}
		}
		// For the y axis //
		else
		{
			// If the given y coordinate is after the split //
			if (isgreater(y, kdTree[index].splitReference->y))
			{
				// Find a list of points that are within radius on this half //
				indexes = find_nearest_neighbours_within_radius(((2 * index) + 1), x, y, radius, &noofIndexes);
				
				// If the distance between the given y and the split is less or equal with the radius //
				if (islessequal(fabs(y - kdTree[index].splitReference->y), radius) == 1)
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
					}
				}
			}
			// If the given y coordinate is before or on the split //
			else
			{
				// Find a list of points that are within radius on this half //
				indexes = find_nearest_neighbours_within_radius((2 * index), x, y, radius, &noofIndexes);
				
				// If the distance between the given x and the split is less or equal with the radius //
				if (islessequal(fabs(y - kdTree[index].splitReference->y), radius) == 1)
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

// *** print_nearest_neighbours_within_radius *** //
void print_nearest_neighbours_within_radius(double x, double y, double radius)
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
			printf("\t%s\r\n", pointArray[indexes[i]]->name);
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
			if (isgreater(x, kdTree[index].splitReference->x))
			{
				// Find the nearest points on this half //
				indexes = find_k_nearest_neighbours(((2 * index) + 1), x, y, k, &noofIndexes);

				// If the distance between the given x and the furthest point is //
				// less or equal the distance between the given x and the split  //
				distance = euclidean_distance(x, y, pointArray[indexes[noofIndexes - 1]]->x, pointArray[indexes[noofIndexes - 1]]->y);
				if (isless(fabs(x - kdTree[index].splitReference->x), distance) == 1)
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
				distance = euclidean_distance(x, y, pointArray[indexes[noofIndexes - 1]]->x, pointArray[indexes[noofIndexes - 1]]->y);
				if (isless(fabs(x - kdTree[index].splitReference->x), distance) == 1)
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
			if (isgreater(y, kdTree[index].splitReference->y))
			{
				// Find the nearest points on this half //
				indexes = find_k_nearest_neighbours(((2 * index) + 1), x, y, k, &noofIndexes);

				// If the distance between the given y and the furthest point is //
				// less or equal the distance between the given y and the split  //
				distance = euclidean_distance(x, y, pointArray[indexes[noofIndexes - 1]]->x, pointArray[indexes[noofIndexes - 1]]->y);
				if (isless(fabs(y - kdTree[index].splitReference->y), distance) == 1)
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
				distance = euclidean_distance(x, y, pointArray[indexes[noofIndexes - 1]]->x, pointArray[indexes[noofIndexes - 1]]->y);
				if (isless(fabs(y - kdTree[index].splitReference->y), distance) == 1)
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

// *** print_k_nearest_neighbours *** //
void print_k_nearest_neighbours(double x, double y, unsigned long k)
{
	unsigned long *indexes = NULL;
	unsigned long i = 0;
	unsigned long noofIndexes = 0;

	indexes = find_k_nearest_neighbours(ROOT_NODE, x, y, k, &noofIndexes);
	assert(indexes != NULL);

	printf("\r\nNearest Neighbors:\r\n");

	for (i = 0; i < noofIndexes; i++)
	{
		printf("\t%s\r\n", pointArray[indexes[i]]->name);
	}

	putchar('\n');
	free(indexes);
}
