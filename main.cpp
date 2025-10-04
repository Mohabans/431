#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <random>
#include <chrono>
// For file output
#include <fstream>
using namespace std;

void merge(vector<long long>& vec, size_t left, size_t mid, size_t right) {
    size_t i, j, k;
    size_t n1 = mid - left + 1;
    size_t n2 = right - mid;

    // Create temporary vectors
    vector<long long> leftVec(n1), rightVec(n2);

    // Copy data to temporary vectors
    for (i = 0; i < n1; i++)
        leftVec[i] = vec[left + i];
    for (j = 0; j < n2; j++)
        rightVec[j] = vec[mid + 1 + j];

    // Merge the temporary vectors back into vec[left..right]
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (leftVec[i] <= rightVec[j]) {
            vec[k] = leftVec[i];
            i++;
        } else {
            vec[k] = rightVec[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of leftVec[], if any
    while (i < n1) {
        vec[k] = leftVec[i];
        i++;
        k++;
    }

    // Copy the remaining elements of rightVec[], if any
    while (j < n2) {
        vec[k] = rightVec[j];
        j++;
        k++;
    }
}

// The subarray to be sorted is in the index range [left..right]
void mergeSort(vector<long long>& vec, size_t left, size_t right) {
    if (left < right) {
        // Calculate the midpoint
        size_t mid = left + (right - left) / 2;

        // Sort first and second halves
        mergeSort(vec, left, mid);
        mergeSort(vec, mid + 1, right);

        // Merge the sorted halves
        merge(vec, left, mid, right);
    }
}

void insertionSort(vector<long long>& vec)
{
    for (size_t i = 1; i < vec.size(); ++i) {
        long long key = vec[i];
        size_t j = i;
        while (j > 0 && vec[j - 1] > key) {
            vec[j] = vec[j - 1];
            --j;
        }
        vec[j] = key;
    }
}

int main() {
    size_t numElements = 150;
    // Open CSV file for writing
    std::ofstream csvFile("timings.csv");
    csvFile << "NumElements,MergeSortSeconds,InsertionSortSeconds" << std::endl;

    while (numElements <= 350) {
        std::vector<long long> mergeVec(numElements);
        std::vector<long long> insertionVec(numElements);

        // Fast random long long generation
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<long long> dist(LLONG_MIN, LLONG_MAX);
        for (size_t i = 0; i < numElements; ++i) {
            mergeVec[i] = dist(gen);
            insertionVec[i] = mergeVec[i];
        }

        cout << "\n\nSorting " << numElements << " elements..." << endl;

        // Measure time for merge sort
        auto start = std::chrono::high_resolution_clock::now();
        mergeSort(mergeVec, 0, mergeVec.size() - 1);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> mergeSortDuration = end - start;

        if (!is_sorted(mergeVec.begin(), mergeVec.end())) {
            cerr << "Error: Merge sort did not sort the array correctly!" << endl;
            return 1;
        }

        cout << "Merge Sort took " << mergeSortDuration.count() << " seconds." << endl;

        // Measure time for insertion sort
        start = std::chrono::high_resolution_clock::now();
        insertionSort(insertionVec);
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> insertionSortDuration = end - start;

        if (!is_sorted(insertionVec.begin(), insertionVec.end())) {
            cerr << "Error: Insertion sort did not sort the array correctly!" << endl;
            return 1;
        }

        cout << "Insertion Sort took " << insertionSortDuration.count() << " seconds." << endl;

        // Write results to CSV
        csvFile << numElements << "," << mergeSortDuration.count() << "," << insertionSortDuration.count() << std::endl;

        numElements += 1;
    }
    csvFile.close();
}