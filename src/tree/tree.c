#include "tree.h"

double xMax = -1;
double yMax = -1;


TREE *SortLeft(TREE coordinates[], int start, int end, int coord, int level){
    
    int median, flag=0;
	TREE *node=NULL;
    level++;

    //find the root of the subtree
    if (flag == 0) {
        quicksort (coordinates, start, end, coord);
        median = start + (end-start+1)/2;
         
        if (start <= end) {
            node=(TREE*)malloc(sizeof(TREE));
            node->x=coordinates[median].x;
            node->y=coordinates[median].y;
            coordinates[median].level = level+1;
            
            printf("left child: (%lf, %lf), depth %d\n", node->x, node->y, level);

            if (start != end){
                node->left=SortLeft(coordinates, start, median-1, !coord, level);
                node->right=SortRight(coordinates, median+1, end, !coord, level);
            } else {
                node->left = NULL;
                node->right = NULL;
            }
        }
        flag = 1;
    }
    return(node);
}


TREE *SortRight(TREE coordinates[],int start, int end, int coord, int level){
    
    int median, flag=0;
	TREE *node=NULL;   
    level++;

    //find the root of the subtree
    if (flag == 0) {
        quicksort (coordinates, start, end, coord);
        median = start + (end-start+1)/2;
        
        if (start <= end) {
            node=(TREE*)malloc(sizeof(TREE));
            node->x=coordinates[median].x;
            node->y=coordinates[median].y;
            coordinates[median].level = level+1;
            
            printf("right child: (%lf, %lf), depth %d\n", node->x, node->y, level);
            
            if (start != end){
                node->left=SortLeft(coordinates, start, median-1, !coord, level);
                node->right=SortRight(coordinates, median+1, end, !coord, level);
            } else {
                node->left = NULL;
                node->right = NULL;
            }
        }
        flag = 1;
    }
    return(node);
}


TREE *CreateTree(TREE coordinates[], int start, int end, int coord) {
    
	int median, level=0;
	TREE *root=NULL;   
    
    //find the root of the tree
    quicksort (coordinates, start, end, coord);
    median = start + (end-start+1)/2;
    
    root=(TREE*)malloc(sizeof(TREE));
    root->x=coordinates[median].x;
    root->y=coordinates[median].y;
    coordinates[median].level = 1;
    
    printf("root: (%lf, %lf)\n", root->x, root->y);
    
    if (end != start) {
        root->left=SortLeft(coordinates, start, median-1, 1, 0);
        root->right=SortRight(coordinates, median+1, end, 1, 0);
    }else {
        root->left=NULL;
        root->right=NULL;
    }
    
    return(root);
}


void find_maxes(TREE tree[], int size)
{
	int i = 0;

	for (i = 0; i < size; i++)
	{
		if (tree[i].x > xMax)
		{
			xMax = tree[i].x;
		}

		if (tree[i].y > yMax)
		{
			yMax = tree[i].y;
		}
	}
}