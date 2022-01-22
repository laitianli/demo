static inline void swap(int *a, int *b)
{
    int tmp = 0;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

int bubble_sort(int arr[], int size)
{
    int i = 0, j = 0;
    for (i = 0; i <size; i++) {
        for(j = 0; j < size - 1 - i; j ++) {
            if (arr[j + 1] < arr[j]) {
                swap(&arr[j + 1], &arr[j]);
            }
        }
    }
    return 0;
}
