/* process_all_sorts.c */
#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_RUNS 10

// High-resolution timer function:
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

// Global comparisons counter (for comparison-based sorts)
long long comparisons = 0;

/* Sorting algorithm prototypes */
void mergeSort(int arr[], int l, int r);
void bubbleSort(int arr[], int n);
void insertionSort(int arr[], int n);
void quickSortMedian(int arr[], int low, int high);
void heapSort(int arr[], int n);
void radixSort(int arr[], int n);

/* --- MergeSort --- */
// Wrapper so that our function pointer has the signature void f(int[], int)
void mergeSortWrapper(int arr[], int n) {
    mergeSort(arr, 0, n - 1);
}

void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    int *L = (int*)malloc(n1 * sizeof(int));
    int *R = (int*)malloc(n2 * sizeof(int));
    for (int i = 0; i < n1; i++) 
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) 
        R[j] = arr[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        comparisons++;
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    free(L);
    free(R);
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

/* --- Bubble Sort --- */
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            comparisons++;
            if (arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

/* --- Insertion Sort --- */
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i], j = i - 1;
        while (j >= 0) {
            comparisons++;
            if (arr[j] > key) {
                arr[j+1] = arr[j];
                j--;
            } else {
                break;
            }
        }
        arr[j+1] = key;
    }
}

/* --- QuickSort with Median-of-Three Pivot --- */
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
        } else break;
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

/* --- Heap Sort --- */
void heapify(int arr[], int n, int i) {
    int largest = i, left = 2*i + 1, right = 2*i + 2;
    if (left < n) {
        comparisons++;
        if (arr[left] > arr[largest])
            largest = left;
    }
    if (right < n) {
        comparisons++;
        if (arr[right] > arr[largest])
            largest = right;
    }
    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    for (int i = n/2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heapify(arr, i, 0);
    }
}

/* --- Radix Sort (non-comparison based) --- */
int getMax(int arr[], int n) {
    int mx = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] > mx)
            mx = arr[i];
    return mx;
}

void countSort(int arr[], int n, int exp) {
    int *output = (int*)malloc(n * sizeof(int));
    int count[10] = {0};
    for (int i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;
    for (int i = 1; i < 10; i++)
        count[i] += count[i-1];
    for (int i = n-1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }
    for (int i = 0; i < n; i++)
        arr[i] = output[i];
    free(output);
}

void radixSort(int arr[], int n) {
    int m = getMax(arr, n);
    for (int exp = 1; m/exp > 0; exp *= 10)
        countSort(arr, n, exp);
}

/* --- Utility: Duplicate an Array --- */
int* duplicateArray(int arr[], int n) {
    int *copy = (int*)malloc(n * sizeof(int));
    memcpy(copy, arr, n * sizeof(int));
    return copy;
}

/* Function pointer type for sorts that use signature: void f(int[], int) */
typedef void (*SortFunc)(int[], int);

/* Run a sort repeatedly and average the results (for comparison-based sorts) */
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

/* Run RadixSort repeatedly (non-comparison-based) */
void runRadixSortRepeated(int arr[], int n, const char *caseStr, FILE *outputCSV) {
    double total_time = 0.0;
    for (int run = 0; run < NUM_RUNS; run++) {
        int *copy = duplicateArray(arr, n);
        double start = get_time();
        radixSort(copy, n);
        double end = get_time();
        total_time += (end - start);
        free(copy);
    }
    double avg_time = total_time / NUM_RUNS;
    fprintf(outputCSV, "%d,RadixSort,%.6f,NA,%s\n", n, avg_time, caseStr);
}

/* Main program:
   Expects two command-line arguments:
   1. Input file name (which should follow the format: first line = n, second line = n integers)
   2. Case type: "best", "worst", or "average"
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
    
    FILE *outputCSV = fopen("output_all_sorts.csv", "w");
    if (!outputCSV) {
        printf("Error opening output file!\n");
        fclose(input);
        return 1;
    }
    // CSV header: InputSize, Algorithm, Time, Comparisons, Case
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
        
        // Run each sorting algorithm and log the averaged results:
        runSortAlgorithmRepeated(arr, n, mergeSortWrapper, "MergeSort", caseType, outputCSV);
        runSortAlgorithmRepeated(arr, n, bubbleSort, "BubbleSort", caseType, outputCSV);
        runSortAlgorithmRepeated(arr, n, insertionSort, "InsertionSort", caseType, outputCSV);
        runSortAlgorithmRepeated(arr, n, quickSortMedianWrapper, "QuickSortMedian", caseType, outputCSV);
        runSortAlgorithmRepeated(arr, n, heapSort, "HeapSort", caseType, outputCSV);
        runRadixSortRepeated(arr, n, caseType, outputCSV);
        
        free(arr);
    }
    
    fclose(input);
    fclose(outputCSV);
    printf("Sorting experiments completed. Results saved to output_all_sorts.csv\n");
    return 0;
}
