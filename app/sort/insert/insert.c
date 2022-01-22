#include "swap.h"
int insert_sort(int arr[], int size)
{
    int i = 0, j = 0;
    int max_index = 0;
    for (i = 0; i <size; i++) {
        max_index = i;
        for (j = i + 1; j < size ; j ++) {
            if (arr[j] > arr[max_index]) {
                max_index = j;
            }
        }
        if (max_index != i) {
            //swap(&arr[i], &arr[max_index]);
            //swap2(&arr[i], &arr[max_index]);
            swap3(&arr[i], &arr[max_index]);
        }
    }
    return 0;
}
