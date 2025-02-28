#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generateRandomArray(int arr[], int n) {
    for(int i = 0; i < n; i++)
        arr[i] = rand() % 100000000;
}

int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

void generateSortedArray(int arr[], int n) {
    generateRandomArray(arr, n);
    qsort(arr, n, sizeof(int), compare);
}

void generateReverseSortedArray(int arr[], int n) {
    generateSortedArray(arr, n);
    for(int i = 0; i < n / 2; i++) {
        int temp = arr[i];
        arr[i] = arr[n - i - 1];
        arr[n - i - 1] = temp;
    }
}

void saveToFile(int arr[], int n, const char* filename) {
    FILE* file = fopen(filename, "w");
    if(file != NULL) {
        fprintf(file, "%d\n", n);
        for(int i = 0; i < n; i++)
            fprintf(file, "%d\n", arr[i]);
        fclose(file);
        printf("Test case saved to %s\n", filename);
    } else {
        printf("Error opening file!\n");
    }
}

int main() {
    srand(time(0));

    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);

    int* arr = (int*)malloc(n * sizeof(int));

    generateRandomArray(arr, n);
    saveToFile(arr, n, "random_input.txt");

    generateSortedArray(arr, n);
    saveToFile(arr, n, "sorted_input.txt");

    generateReverseSortedArray(arr, n);
    saveToFile(arr, n, "reverse_sorted_input.txt");

    free(arr);
    printf("Test cases generated successfully!\n");
    return 0;
}
