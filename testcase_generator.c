/* generate_input.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Comparison functions for qsort
int compareAsc(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int compareDesc(const void *a, const void *b) {
    return (*(int *)b - *(int *)a);
}

// mode: 0 = random, 1 = increasing, 2 = decreasing
void generate_input_file(const char *filename, int mode) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        perror("Error opening file");
        return;
    }
    
    srand((unsigned int)time(NULL));

    // Generate 20 test cases with sizes ranging from 1000 to ~10000
    int sizes[20];
    for (int i = 0; i < 20; i++) {
        sizes[i] = 1000 + (i * 450);  // Adjust the range as needed
    }

    for (int t = 0; t < 20; t++) {
        int n = sizes[t];
        int *arr = (int *)malloc(n * sizeof(int));
        if (arr == NULL) {
            perror("Memory allocation failed");
            fclose(f);
            return;
        }
        // Fill with random numbers between 1 and 1,000,000
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % 1000000 + 1;
        }
        // Sort if required
        if (mode == 1) {             // increasing order
            qsort(arr, n, sizeof(int), compareAsc);
        } else if (mode == 2) {      // decreasing order
            qsort(arr, n, sizeof(int), compareDesc);
        }
        // Write the test case: first the size then the list of numbers
        fprintf(f, "%d\n", n);
        for (int i = 0; i < n; i++) {
            fprintf(f, "%d ", arr[i]);
        }
        fprintf(f, "\n");
        free(arr);
    }
    fclose(f);
}

int main(int argc, char *argv[]) {
    int mode = 0;               // default: random input
    const char *filename = "random_input.txt";

    if (argc >= 2) {
        if (strcmp(argv[1], "random") == 0) {
            mode = 0;
            filename = "random_input.txt";
        } else if (strcmp(argv[1], "increasing") == 0) {
            mode = 1;
            filename = "increasing_input.txt";
        } else if (strcmp(argv[1], "decreasing") == 0) {
            mode = 2;
            filename = "decreasing_input.txt";
        } else {
            printf("Invalid mode. Please use: random, increasing, or decreasing.\n");
            return 1;
        }
    }

    generate_input_file(filename, mode);
    printf("Generated test cases in '%s'\n", filename);
    return 0;
}
