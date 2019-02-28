#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*structure for tree*/
typedef struct TREE {
	double x;
	double y;
	int level; // My addition //
	struct TREE *left;
	struct TREE *right;
}TREE;


/*structures for tree*/
TREE *CreateTree(TREE coordinates[], int start, int end, int coord);

TREE *SortLeft(TREE coordinates[], int start, int end, int coord, int level);

TREE *SortRight(TREE coordinates[],int start, int end, int coord, int level);

extern double xMax;
extern double yMax;
extern int treeSize;
extern TREE *tree;

void find_maxes(TREE tree[], int size);