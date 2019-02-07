#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <math.h>

#include "structures.h"
#include "ui-scrolled-canvas-skeleton.h"

extern void quicksort (TREE array[], int start, int end, int TREE);
extern TREE *CreateTree(TREE kd_tree[], int start, int end, int TREE);

int treeSize = 0;
TREE *tree;

int main (int argc, char *argv[]){
    
	int line_size, bytes, counter, i;
	FILE *fd;
	char *numbers=NULL, ch[1];
	TREE *kd_tree=NULL;
	char *token;
	double x, y;
	//double distance=0;
	double distance[counter];

	if (argc == 1)
	{
		printf("There is not a file.\n");
		return(1);
	}

	fd = fopen (argv[1], "r+");
	bytes = 1;
	counter = 0;
	while (bytes != 0) {

		line_size = 1;
		bytes = fread (ch, 1, 1, fd);

		if (strcmp(ch, "\n") == 0)
		{
			break;
		}

		numbers = (char *)calloc(1, sizeof(char));
		while (ch[0] != '\n')
		{
			numbers = (char *)realloc(numbers, line_size+1);
			numbers[strlen(numbers)] = ch[0];

			bytes = fread (ch, 1, 1, fd);
			if (bytes == 0)
			{
				break;
			}
			line_size++;
		}

		numbers[strlen(numbers)]='\0';

		kd_tree = (TREE *)realloc(kd_tree, (counter+1)*sizeof(TREE)+1);
		token = strtok(numbers, " ");
		sscanf(token, "%lf", &kd_tree[counter].x);
		while(token)
		{
			token = strtok(NULL, " ");
			if (token != NULL)
			{
			    sscanf(token, "%lf", &kd_tree[counter].y);
			}
		}

		counter++;
		free(numbers);
		numbers=NULL;
	}
	
	printf("The coordinates are:\n");
	for (i=0; i < counter; i++)
	{
		printf("(%.2lf, %.2lf)\n", kd_tree[i].x,kd_tree[i].y);
	}

	printf("\nThe coordinates after kd-tree are:\n");
	CreateTree(kd_tree, 0, counter-1, 0);

	find_maxes(kd_tree, counter);

	treeSize = counter;
	tree = kd_tree;
	start_gui();

	// This Part is just the Eucledean distance a point to the stored coordinates //

	printf("\nGive the coordinates of the query point\n");
	printf("x: ");
	scanf("%lf", &x);
	printf("y: ");
	scanf("%lf", &y);

	//printf("%lf\n", kd_tree->right->y);
	for (i=0; i < counter; i++)
	{
		distance[i] = sqrt(pow((fabs(x - kd_tree[i].x)), 2) +  pow((fabs(y - kd_tree[i].y)), 2));
    	printf("The euclidean distance is: %lf\n", distance[i]);
	}


	/*distance = sqrt(pow((abs(x - ), 2) +  pow(abs((y - ), 2));

	printf("The euclidean distance is: %lf\n", distance);*/



	return 0;
}
