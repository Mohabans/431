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

// The subarray to be sorted is in the index range [left..right]
void timSort(vector<long long>& vec, size_t left, size_t right, size_t k) {
    if (right - left + 1 <= k) {
        for (size_t i = left + 1; i <= right; ++i) {
            long long key = vec[i];
            size_t j = i;
            while (j > left && vec[j - 1] > key) {
                vec[j] = vec[j - 1];
                --j;
            }
            vec[j] = key;
        }
    } else if (left < right) {
        // Calculate the midpoint
        size_t mid = left + (right - left) / 2;

        // Sort first and second halves
        timSort(vec, left, mid, k);
        timSort(vec, mid + 1, right, k);

        // Merge the sorted halves
        merge(vec, left, mid, right);
    }
}

int main() {
    size_t numElements = 2;
    // Open CSV file for writing
    std::ofstream csvFile("timings.csv");
    csvFile << "NumElements,TimSortSeconds" << std::endl;

    while (numElements <= 1024) {
        std::vector<long long> timVec(numElements);

        // Fast random long long generation
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<long long> dist(LLONG_MIN, LLONG_MAX);
        for (size_t i = 0; i < numElements; ++i) {
            timVec[i] = dist(gen);
        }

        cout << "\n\nSorting " << numElements << " elements..." << endl;

        // Measure time for tim sort
        auto start = std::chrono::high_resolution_clock::now();
        timSort(timVec, 0, timVec.size() - 1, 16);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> timSortDuration = end - start;

        if (!is_sorted(timVec.begin(), timVec.end())) {
            cerr << "Error: Tim sort did not sort the array correctly!" << endl;
            return 1;
        }

        cout << "Tim Sort took " << timSortDuration.count() << " seconds." << endl;

        // Write results to CSV
        csvFile << numElements << "," << timSortDuration.count() << std::endl;

        numElements += 1;
    }
    csvFile.close();
}