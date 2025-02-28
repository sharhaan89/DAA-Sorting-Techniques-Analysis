#include <iostream>
#include <cstdlib>  
using namespace std;

int partitionFirst(int arr[], int low, int high) {
    int pivot = arr[low];
    int i = low + 1, j = high;

    while(i <= j) {
        while(i <= high && arr[i] <= pivot) i++;
        while(arr[j] > pivot) j--;
        if(i < j) swap(arr[i], arr[j]);
    }
    swap(arr[low], arr[j]);
    return j;
}

int partitionRandom(int arr[], int low, int high) {
    int randomIndex = low + rand() % (high - low + 1);
    swap(arr[low], arr[randomIndex]);
    return partitionFirst(arr, low, high);
}

int partitionMedian(int arr[], int low, int high) {
    int mid = low + (high - low) / 2;
    if((arr[low] > arr[mid]) != (arr[low] > arr[high])) swap(arr[low], arr[low]);
    else if((arr[mid] > arr[low]) != (arr[mid] > arr[high])) swap(arr[low], arr[mid]);
    else swap(arr[low], arr[high]);
    return partitionFirst(arr, low, high);
}

void quickSort(int arr[], int low, int high, int pivotType) {
    if(low < high) {
        int pi;
        if(pivotType == 1) pi = partitionFirst(arr, low, high);
        else if(pivotType == 2) pi = partitionRandom(arr, low, high);
        else pi = partitionMedian(arr, low, high);

        quickSort(arr, low, pi - 1, pivotType);
        quickSort(arr, pi + 1, high, pivotType);
    }
}
