#include <stdio.h>

void swap(float *a, float *b) {
    // swaps two variables in memory
    float tmp = *a;
    *a = *b;
    *b = tmp;
}

int partition(float nums[], int l, int r) {
    // finds pivot for number between range in array
    int j;
    int i = l;
    for (j = l; j < r; j++) {
        if (nums[j] <= nums[r]) {
            swap(&nums[j], &nums[i]);
            i++;
        }
    }
    swap(&nums[i], &nums[r]);
    return i;
}

void quicksort(float nums[], int l, int r) {
    // sorts provided array
    int pivot;
    if (l >= r) {
        return;
    }
    pivot = partition(nums, l, r);
    printf("Quicksort: %d\n", pivot);
    quicksort(nums, l, pivot-1);
    quicksort(nums, pivot+1, r);
}

int main() {
    int i = 0, j = 0;
    float nums[] = {91.0, 82.0, 73.0, 84.0};
    for (i = 0; i < 5; i++) {
        printf("%f ", nums[i]);
    }
    printf("\n");
    quicksort(nums, 0, 4);
    for (j = 0; j < 5; j++) {
        printf("%f ", nums[j]);
    }
    printf("\n");
}
