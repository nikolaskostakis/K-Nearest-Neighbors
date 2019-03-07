#include "structures.h"

// Point Hash Table //
struct pointHash *pointHT = NULL;
unsigned long pointHTSize = 0;
int pointHTPrimeIndex = 0;

// KD-Tree //
struct kdNode *kdTree = NULL;

// Pointer Array for KD-Tree //
struct pointHashNode **pointArray = NULL;
unsigned long pointArraySize = 0;

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

void free_point_hash()
{
	unsigned long i;
	int j;

	for (i = 0; i < pointHTSize; i++)
	{
		if (pointHT[i].depth == 0)
		{
			continue;
		}

		for (j = 0; j < pointHT[i].depth; j++)
		{
			free(pointHT[i].nodes[i].name);
		}
		free(pointHT[i].nodes);
	}

	free(pointHT);
	pointHT = NULL;
	pointHTSize = 0;
	pointHTPrimeIndex = 0;
}

// Print the content of the point Hash Table //
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
void create_KD_tree()
{
	if (kdTree != NULL)
	{

	}
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

	if (noofElements > MAX_KDLEAF_ELEMENTS)
	{
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

void free_KD_tree()
{
	free_KD_node(kdTree);
	kdTree = NULL;
}

void free_KD_node(struct kdNode *node)
{
	if (node->isLeaf == false)
	{
		free_KD_node(node->left);
		free_KD_node(node->right);
	}

	free(node);
}

void print_KD_tree()
{
	if (kdTree == NULL)
	{
		printf(YEL"KD Tree Empty!\n"NRM);
		return;
	}

	print_KD_node(kdTree, 0);
}

void print_KD_node(struct kdNode *node, int depth)
{
	int i;

	for(i = 0; i < depth; i++)
	{
		printf("\t");
	}
	printf(BLU"Start Index: "NRM"%lu\n", node->startIndex);
	
	for(i = 0; i < depth; i++)
	{
		printf("\t");
	}
	printf(BLU"End Index:   "NRM"%lu\n", node->endIndex);

	if (node->isLeaf == true)
	{
		for(i = 0; i < depth; i++)
		{
			printf("\t");
		}
		printf(MAG"\tLeaf node!\n");
	}
	else
	{
		for(i = 0; i < depth; i++)
		{
			printf("\t");
		}
		printf(MAG"\tLeft child:\n");
		print_KD_node(node->left, (depth + 1));

		for(i = 0; i < depth; i++)
		{
			printf("\t");
		}
		printf(MAG"\tRight child:\n");
		print_KD_node(node->right, (depth + 1));
	}
	
}

inline double euclidean_distance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow(fabs(x2 - x1), 2) + pow(fabs(y2 - y1), 2));
}

unsigned long find_nearest_neighbour(struct kdNode *node, double x, double y)
{
	double minDistance = 0;
	double distance = 0;
	unsigned long minIndex = 0;
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
			}
			else
			{
				minIndex = find_nearest_neighbour(node->left, x, y);
			}
		}
		else
		{
			if (isgreater(y, pointArray[node->splitIndex]->y))
			{
				minIndex = find_nearest_neighbour(node->right, x, y);
			}
			else
			{
				minIndex = find_nearest_neighbour(node->left, x, y);
			}
		}
	}
	
	return minIndex;
}

void print_nearest(double x, double y)
{
	printf("\nNearest Point: %s\n\n", pointArray[find_nearest_neighbour(kdTree, x, y)]->name);
}