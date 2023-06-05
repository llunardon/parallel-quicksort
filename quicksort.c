#include "quicksort.h"
#include "utils.h"
#include <stdio.h>

// sequential quicksort
void quick_sort (int *arr, int low, int high) {
    if (low < high) {
        int q = partition(arr, low, high);
        quick_sort(arr, low, q - 1);
        quick_sort(arr, q + 1, high);
    }    
}

int partition (int *arr, int low, int high) {
    // pivot
    int x = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < x) {
            i = i + 1;
            swap(arr, i, j);
        }
    }
    swap(arr, i + 1, high);
    return i + 1;
}
