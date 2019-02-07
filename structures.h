/*structure for tree*/
typedef struct TREE {
	double x;
	double y;
	struct TREE *left;
	struct TREE *right;
}TREE;


/*functions of quicksort*/
void quicksort (TREE array[], int start, int end, int coord);

void swap (double *x, double *y);


/*structures for tree*/
#define count 10
TREE *CreateTree(TREE coordinates[], int start, int end, int coord);

TREE *SortLeft(TREE coordinates[], int start, int end, int coord, int level);

TREE *SortRight(TREE coordinates[],int start, int end, int coord, int level);

