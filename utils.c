#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

void swap (int *arr, int i, int j) {
    int tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}

void print_arr (int *arr, int size) {
    for( int i = 0; i < size; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int * concatenate (int *arr1, int *arr2, int size1, int size2) {
	int i = 0, j = 0, k;
    int ret_size = size1 + size2;
    int *ret_arr = (int *) malloc(sizeof(int) * (ret_size));

	for (k = 0; k < ret_size; k++) {
		if (i >= size1) {
			ret_arr[k] = arr2[j];
			j += 1;
		}
		else if (j >= size2) {
			ret_arr[k] = arr1[i];
			i += 1;
		}
		else if (arr1[i] < arr2[j]) {
			ret_arr[k] = arr1[i];
			i += 1;
		}
		else {
			ret_arr[k] = arr2[j];
			j += 1;
		}
	}
	return ret_arr;
}

unsigned int log_2 (unsigned int x) {
    unsigned int ans = 0;

    while (x >>= 1) ans++;

    return ans;
}
