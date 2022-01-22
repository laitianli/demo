#ifndef __BUBBLE_SORT__
#define __BUBBLE_SORT__
int bubble_sort(int arr[], int size);

static inline void dump_array(int arr[], int size)
{
    int i = 0;
    printf("dump array: \n");
    for(i = 0; i < size; i++) {
        printf("%4d ", arr[i]);
    }
    printf("\n");
}

#endif
