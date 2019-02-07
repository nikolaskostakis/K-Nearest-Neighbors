/*structure for tree*/
typedef struct TREE {
	double x;
	double y;
	int level; // My addition //
	struct TREE *left;
	struct TREE *right;
}TREE;


/*functions of quicksort*/
void quicksort (TREE array[], int start, int end, int coord);

void swap (double *x, double *y);


/*structures for tree*/
TREE *CreateTree(TREE coordinates[], int start, int end, int coord);

TREE *SortLeft(TREE coordinates[], int start, int end, int coord, int level);

TREE *SortRight(TREE coordinates[],int start, int end, int coord, int level);

extern double xMax;
extern double yMax;
extern int treeSize;
extern TREE *tree;

void find_maxes(TREE tree[], int size);