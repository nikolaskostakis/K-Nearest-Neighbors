#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "structures.h"


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
