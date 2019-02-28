#include "structures.h"

// Point Hash Table //
struct pointHash *pointHT = NULL;
unsigned long pointHTSize = 0;
int pointHTPrimeIndex = 0;

// KD-Tree //
struct kdNode *kdTree = NULL;

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
		if (pointHT == NULL)
		{
			printf(RED"Error! Unable to allocate memory\n"NRM);
			exit(1);
		}

		pointHTSize = primes[pointHTPrimeIndex];
		pointHTPrimeIndex++;
	}
	
	hash = point_hash_function(name) % pointHTSize;
	
	pointHT[hash].nodes = (struct pointHashNode *) realloc(pointHT[hash].nodes, ((pointHT[hash].depth + 1) * sizeof(struct pointHashNode)));
	if (pointHT[hash].nodes == NULL)
	{
		printf(RED"Error! Unable to allocate memory\n"NRM);
		exit(1);
	}

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
		if (pointHT == NULL)
		{
			printf(RED"Error! Unable to allocate memory\n"NRM);
			exit(1);
		}

		pointHTSize *= 2;
	}
	else
	{
		pointHT = (struct pointHash *) calloc(primes[pointHTPrimeIndex], sizeof(struct pointHash));
		if (pointHT == NULL)
		{
			printf(RED"Error! Unable to allocate memory\n"NRM);
			exit(1);
		}

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
			if (pointHT[hash].nodes == NULL)
			{
				printf(RED"Error! Unable to allocate memory\n"NRM);
				exit(1);
			}

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
