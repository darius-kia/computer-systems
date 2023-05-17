/*
Name: Darius Kianersi
UID: 118799810
Username: radius
*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#define MAX_ASSIGNMENTS 50

/*
    max_int returns the greater of two integers.
*/
int max_int(int x, int y) {
    if (x > y) {
        return x;
    } else {
        return y;
    }
}

/*
    swap exchanges the values of two integer pointers in memory.
*/
void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

/*
    partition is used for the quicksort algorithm to determine a pivot position.
*/
int partition(int scores[], int weights[], int lates[], int l, int r) {
    int j;
    int i = l;
    for (j = l; j < r; j++) {
        if (scores[j] * weights[j] <= scores[r] * weights[r]) {
            swap(&scores[j], &scores[i]);
            swap(&weights[j], &weights[i]);
            swap(&lates[j], &lates[i]);
            i++;
        }
    }
    swap(&scores[i], &scores[r]);
    swap(&weights[i], &weights[r]);
    swap(&lates[i], &lates[r]);
    return i;
}

/*
    quicksort sorts a given array of integers in ascending order. 
*/
void quicksort(int scores[], int weights[], int lates[], int l, int r) {
    int pivot;
    if (l >= r) {
        return;
    }
    pivot = partition(scores, weights, lates, l, r);
    quicksort(scores, weights, lates, l, pivot-1);
    quicksort(scores, weights, lates, pivot+1, r);
}

/*
    duplicate performs a deep copy of an array into another array.
*/
void duplicate(int n, int arr1[], int arr2[]) {
    int i;
    for (i = 0; i < n; i++) {
        arr2[i] = arr1[i];
    }
}

/*
    The algorithm reads each of the assignment values and sorts them by 
    their value (score * weight). Then, it skips the first k values to drop
    to compute the numeric score.
*/
int main() {
    int point_penalty, num_to_drop, num_assignments;
    int i, j, k, l;
    int total = 0, weights_total = 0;
    double numeric_score = 0.0, mean, std_dev, dev = 0, weights_dropped = 0.0;
    char stats;
    int scores[MAX_ASSIGNMENTS], weights[MAX_ASSIGNMENTS], lates[MAX_ASSIGNMENTS];
    int scores_out[MAX_ASSIGNMENTS], weights_out[MAX_ASSIGNMENTS], lates_out[MAX_ASSIGNMENTS];

    scanf(" %d %d %c %d", &point_penalty, &num_to_drop, &stats, &num_assignments);
    for (i = 0; i < num_assignments; i++) {
        int number, score, weight, days_late;
        scanf("%d, %d, %d, %d", &number, &score, &weight, &days_late);
        scores[number-1] = score;
        weights[number-1] = weight;
        lates[number-1] = days_late;
        total += score - days_late * point_penalty;
        weights_total += weight;
    }

    if (weights_total != 100) {
        printf("ERROR: Invalid values provided\n");
        return 1;
    }

    mean = (double)total/num_assignments;
    for (j = 0; j < num_assignments; j++) {
        dev += pow((scores[j] - lates[j] * point_penalty) - mean, 2);
    }

    duplicate(MAX_ASSIGNMENTS, scores, scores_out);
    duplicate(MAX_ASSIGNMENTS, weights, weights_out);
    duplicate(MAX_ASSIGNMENTS, lates, lates_out);

    quicksort(scores, weights, lates, 0, num_assignments - 1);

    for (l = num_to_drop; l < num_assignments; l++) {
        numeric_score += max_int(0, scores[l] - lates[l] * point_penalty) * (weights[l] / 100.0);
        weights_dropped += weights[l] / 100.0;
    }

    numeric_score = numeric_score / weights_dropped;

    std_dev = sqrt(dev/num_assignments);
    printf("Numeric Score: %5.4f\nPoints Penalty Per Day Late: %d\nNumber of Assignments Dropped: %d\n", numeric_score, point_penalty, num_to_drop);
    printf("Values Provided:\nAssignment, Score, Weight, Days Late\n");
    for (k = 0; k < num_assignments; k++) {
        printf("%d, %d, %d, %d\n", k+1, scores_out[k], weights_out[k], lates_out[k]);
    }
    if (stats == 'Y' || stats == 'y') {
        printf("Mean: %5.4f, Standard Deviation: %5.4f\n", mean, std_dev);
    }
    return 0;
}
