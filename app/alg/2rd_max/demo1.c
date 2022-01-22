#include <unistd.h>
#include <stdio.h>

void swap(int *a, int *b)
{
    *a = *a + *b;
    *b = *a - *b;
    *a = *a - *b;
}

int get_second_max(int arr[], int size)
{
    int max = 0, sec_max = 0;
    int i = 0;
    if (size <= 1) {
        printf("[Warning] the element of array mast greater two!\n");
        return -1;
    }
    max = arr[0];
    sec_max = arr[1];
    if (max < sec_max) {
        swap(&max, &sec_max);
    }
    for (i = 2; i <= size - 1; i++) {
        if (arr[i] <= sec_max)
            continue;
        if(arr[i] == max)
            continue;
        if (arr[i] > max) {
            sec_max = max;
            max = arr[i];
        } 
        else
            sec_max = arr[i];
    }
    return sec_max;
}

int main(int argv, char** argc)
{
    int arr[] = {1, -2, 23, 45, 100, 80, 90};
    int second_max = get_second_max(arr, sizeof(arr) / sizeof(int));
    printf("second_max = %d \n", second_max);
    return  0;
}
