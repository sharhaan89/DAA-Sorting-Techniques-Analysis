/* compare_quick.c */
#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_RUNS 10

// High-resolution timer function
#ifdef _WIN32
double get_time() {
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    return (double) counter.QuadPart / frequency.QuadPart;
}
#else
double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}
#endif

// Global comparisons counter for comparison-based algorithms
long long comparisons = 0;

/* QuickSort Variant 1: First Element as Pivot */
int partitionFirst(int arr[], int low, int high) {
    int pivot = arr[low];
    int i = low + 1, j = high;
    while (1) {
        while (i <= high && (comparisons++, arr[i] <= pivot)) i++;
        while (j >= low && (comparisons++, arr[j] > pivot)) j--;
        if (i < j) {
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        } else {
            break;
        }
    }
    int temp = arr[low];
    arr[low] = arr[j];
    arr[j] = temp;
    return j;
}

void quickSortFirst(int arr[], int low, int high) {
    if (low < high) {
        int p = partitionFirst(arr, low, high);
        quickSortFirst(arr, low, p - 1);
        quickSortFirst(arr, p + 1, high);
    }
}

void quickSortFirstWrapper(int arr[], int n) {
    quickSortFirst(arr, 0, n - 1);
}

/* QuickSort Variant 2: Random Pivot */
int partitionRandom(int arr[], int low, int high) {
    int randomIndex = low + rand() % (high - low + 1);
    int temp = arr[low];
    arr[low] = arr[randomIndex];
    arr[randomIndex] = temp;
    
    int pivot = arr[low];
    int i = low + 1, j = high;
    while (1) {
        while (i <= high && (comparisons++, arr[i] <= pivot)) i++;
        while (j >= low && (comparisons++, arr[j] > pivot)) j--;
        if (i < j) {
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        } else {
            break;
        }
    }
    temp = arr[low];
    arr[low] = arr[j];
    arr[j] = temp;
    return j;
}

void quickSortRandom(int arr[], int low, int high) {
    if (low < high) {
        int p = partitionRandom(arr, low, high);
        quickSortRandom(arr, low, p - 1);
        quickSortRandom(arr, p + 1, high);
    }
}

void quickSortRandomWrapper(int arr[], int n) {
    quickSortRandom(arr, 0, n - 1);
}

/* QuickSort Variant 3: Median-of-Three Pivot */
int medianOfThree(int arr[], int low, int high) {
    int mid = low + (high - low) / 2;
    if (arr[low] > arr[mid]) { comparisons++; int temp = arr[low]; arr[low] = arr[mid]; arr[mid] = temp; }
    if (arr[low] > arr[high]) { comparisons++; int temp = arr[low]; arr[low] = arr[high]; arr[high] = temp; }
    if (arr[mid] > arr[high]) { comparisons++; int temp = arr[mid]; arr[mid] = arr[high]; arr[high] = temp; }
    return mid;
}

int partitionMedian(int arr[], int low, int high) {
    int medianIndex = medianOfThree(arr, low, high);
    int temp = arr[low];
    arr[low] = arr[medianIndex];
    arr[medianIndex] = temp;
    
    int pivot = arr[low];
    int i = low + 1, j = high;
    while (1) {
        while (i <= high && (comparisons++, arr[i] <= pivot)) i++;
        while (j >= low && (comparisons++, arr[j] > pivot)) j--;
        if (i < j) {
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        } else {
            break;
        }
    }
    temp = arr[low];
    arr[low] = arr[j];
    arr[j] = temp;
    return j;
}

void quickSortMedian(int arr[], int low, int high) {
    if (low < high) {
        int p = partitionMedian(arr, low, high);
        quickSortMedian(arr, low, p - 1);
        quickSortMedian(arr, p + 1, high);
    }
}

void quickSortMedianWrapper(int arr[], int n) {
    quickSortMedian(arr, 0, n - 1);
}

/* Utility: Duplicate an array */
int* duplicateArray(int arr[], int n) {
    int *copy = (int*)malloc(n * sizeof(int));
    memcpy(copy, arr, n * sizeof(int));
    return copy;
}

/* Function pointer type for sorting functions: void f(int[], int) */
typedef void (*SortFunc)(int[], int);

/* Run a sort repeatedly, average its execution time and comparisons,
   and log the result to a CSV file along with the given case type */
void runSortAlgorithmRepeated(int arr[], int n, SortFunc sortFunc, const char *algoName, const char *caseStr, FILE *outputCSV) {
    double total_time = 0.0;
    long long total_comparisons = 0;
    for (int run = 0; run < NUM_RUNS; run++) {
        int *copy = duplicateArray(arr, n);
        comparisons = 0;
        double start = get_time();
        sortFunc(copy, n);
        double end = get_time();
        total_time += (end - start);
        total_comparisons += comparisons;
        free(copy);
    }
    double avg_time = total_time / NUM_RUNS;
    long long avg_comparisons = total_comparisons / NUM_RUNS;
    fprintf(outputCSV, "%d,%s,%.6f,%lld,%s\n", n, algoName, avg_time, avg_comparisons, caseStr);
}

/* Main Program
   Usage: ./compare_quick <input_file> <case_type>
   where:
     - <input_file> is the file containing test cases (first line: size, second line: elements)
     - <case_type> is one of: best, worst, average
*/
int main(int argc, char *argv[]) {
    const char *inputFilename = "random_input.txt";
    const char *caseType = "average";
    if (argc >= 2)
        inputFilename = argv[1];
    if (argc >= 3)
        caseType = argv[2];
    
    FILE *input = fopen(inputFilename, "r");
    if (!input) {
        printf("Error opening input file: %s\n", inputFilename);
        return 1;
    }
    
    FILE *outputCSV = fopen("quick_sort_comparison.csv", "w");
    if (!outputCSV) {
        printf("Error opening output CSV file!\n");
        fclose(input);
        return 1;
    }
    // Write CSV header: InputSize, Algorithm, Time, Comparisons, Case
    fprintf(outputCSV, "InputSize,Algorithm,Time,Comparisons,Case\n");
    
    int n;
    while (fscanf(input, "%d", &n) != EOF) {
        int *arr = (int*)malloc(n * sizeof(int));
        if (!arr) {
            perror("Memory allocation failed");
            break;
        }
        for (int i = 0; i < n; i++) {
            fscanf(input, "%d", &arr[i]);
        }
        
        // Run each QuickSort variant and log results:
        runSortAlgorithmRepeated(arr, n, quickSortFirstWrapper, "QuickSortFirst", caseType, outputCSV);
        runSortAlgorithmRepeated(arr, n, quickSortRandomWrapper, "QuickSortRandom", caseType, outputCSV);
        runSortAlgorithmRepeated(arr, n, quickSortMedianWrapper, "QuickSortMedian", caseType, outputCSV);
        
        free(arr);
    }
    
    fclose(input);
    fclose(outputCSV);
    printf("QuickSort comparison completed. Results saved to quick_sort_comparison.csv\n");
    return 0;
}
