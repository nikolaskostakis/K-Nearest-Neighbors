#include "quicksort.h"

void swap (double *x, double *y) {
    double temp;
    temp = *x;
    *x = *y;
    *y = temp;
}


void quicksort (TREE array[], int start, int end, int coord) {

    int pivot, i, j;
    double store;

    if (start < end) {
        pivot = (start + end)/2;

        if (coord == 0) {

            swap(&array[start].x, &array[pivot].x);
            swap(&array[start].y, &array[pivot].y);
            store = array[start].x;
        }else {

            swap(&array[start].x, &array[pivot].x);
            swap(&array[start].y, &array[pivot].y);
            store = array[start].y;
        }

        i = start + 1;
        j = end;
        while(i <= j){
            if (coord == 0) {

                while((i <= end) && (array[i].x <= store)) {
                    i++;
                }
                while((j >= start) && (array[j].x > store)) {
                    j--;
                }
                if( i < j) {
                    swap(&array[i].x,&array[j].x);
                    swap(&array[i].y,&array[j].y);
                }
            } else {
                while((i <= end) && (array[i].y <= store)) {
                    i++;
                }
                while((j >= start) && (array[j].y > store)) {
                    j--;
                }
                if( i < j) {
                    swap(&array[i].x,&array[j].x);
                    swap(&array[i].y,&array[j].y);
                }
            }
        }
        
        if (coord == 0) {
            swap(&array[start].x,&array[j].x);
            swap(&array[start].y,&array[j].y);
        }else {
            swap(&array[start].x,&array[j].x);
            swap(&array[start].y,&array[j].y);
        }

        quicksort(array,start,j-1,coord);
        quicksort(array,j+1,end,coord);
    }
}
