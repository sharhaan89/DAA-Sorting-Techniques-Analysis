#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

void generateRandomArray(int arr[], int n) {
    for(int i = 0; i < n; i++)
        arr[i] = rand() % 100000000;
}

void generateSortedArray(int arr[], int n) {
    generateRandomArray(arr, n);
    sort(arr, arr + n);
}

void generateReverseSortedArray(int arr[], int n) {
    generateSortedArray(arr, n);
    reverse(arr, arr + n);
}

void saveToFile(int arr[], int n, const string& filename) {
    ofstream file(filename);
    if(file.is_open()) {
        file << n << endl;
        for(int i = 0; i < n; i++)
            file << arr[i] << endl;
        file.close();
        cout << "Test case saved to " << filename << endl;
    } else {
        cout << "Error opening file!" << endl;
    }
}

int main() {
    srand(time(0));

    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    int* arr = new int[n];

    generateRandomArray(arr, n);
    saveToFile(arr, n, "random_input.txt");

    generateSortedArray(arr, n);
    saveToFile(arr, n, "sorted_input.txt");

    generateReverseSortedArray(arr, n);
    saveToFile(arr, n, "reverse_sorted_input.txt");

    delete[] arr;
    cout << "Test cases generated successfully!" << endl;
    return 0;
}
