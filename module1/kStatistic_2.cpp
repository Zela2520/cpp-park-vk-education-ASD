#include <iostream>
#include <cassert>


bool min(const int& left, const int& right) {
    return left < right;
}

int getMedianIndex(int* arr, int l, int middle, int r) {
    if (arr[l] > arr[middle]) {
        if (arr[r] > arr[l]) {
            return l;
        }
        return (arr[middle] > arr[r]) ? middle : r;
    }
    if (arr[r] > arr[middle]) {
        return middle;
    }
    return (arr[l] > arr[r]) ? l : r;
}

int partition(int* arr, int l, int r, bool(*comparator)(const int& l, const int& r)) {
    int pivotIndex = getMedianIndex(arr, l, (l + r) / 2, r - 1);
    // int pivotIndex = l + (r - l) / 2;
    int pivotValue = arr[pivotIndex];

    std::swap(arr[pivotIndex], arr[r - 1]);
    
    int i = 0;
    int j = 0;

    while (j < r - 1) {
        while (comparator(arr[i],pivotValue)) {
            ++i;
            ++j;
        }
        for (; !comparator(arr[j], pivotValue); ++j);
        if (j < r - 1) {
            std::swap(arr[i], arr[j]);
            ++i;
            ++j;
        }
    }
    std::swap(arr[i], arr[r - 1]);
    return i;
}

void ktn_element(int* arr, int size, int k, bool(*comparator)(const int& l, const int& r)) {
    int l = 0;
    int r = size;

    while (comparator(l, r)) {
        int m = partition(arr, l, r, comparator);
        if (m == k) return;
        else if (comparator(m, k)) {
            l = m + 1;
        } else {
            r = m;
        }
    }

}

void printResult(int* arr, int size, bool(*comparator)(const int& l, const int& r)) {
    ktn_element(arr, size, size * 0.1, min);
    std::cout << arr[static_cast<int>(size * 0.1)] << std::endl;
    ktn_element(arr, size, size * 0.5, min);
    std::cout << arr[static_cast<int>(size * 0.5)] << std::endl;
    ktn_element(arr, size, size * 0.9, min);
    std::cout << arr[static_cast<int>(size * 0.9)] << std::endl;
}


void test() {
    std::cout << "***********TEST CASES************" << std::endl;
    std::cout << "***********ktnElement function test******** " << std::endl;
    {   std::cout << "Case 1: ";
        {
            int* arr = new int[8]{2, 6, 1, 4, 3, 8, 5, 7};
            ktn_element(arr, 8, 0, min);
            assert(arr[0] == 1);
            delete [] arr;
            arr = nullptr;
        }
        std::cout << "OK" << std::endl;
        std::cout << "Case 2: ";
        {
            int* arr = new int[8]{2, 6, 1, 4, 3, 8, 5, 7};
            ktn_element(arr, 8, 7, min);
            assert(arr[7] == 8);
            delete[] arr;
            arr = nullptr;
        }
        std::cout << "OK" << std::endl;
        std::cout << "Case 3: ";
        {
            int* arr = new int[8]{2, 6, 1, 4, 3, 8, 5, 7};
            ktn_element(arr, 8, 5, min);
            assert(arr[5] == 6);
            delete[] arr;
            arr = nullptr;
        }
        std::cout << "OK" << std::endl;
    }
    std::cout << "**********OK**********" << std::endl;
    std::cout << std::endl;
    std::cout << "***********All tests passed***********\n";
}

void run(std::istream& input, std::ostream& output) {
    size_t arraySize = -1;
    input >> arraySize;
    
    int* array = new int[arraySize]{0};
    for (size_t i = 0; i < arraySize; ++i) {
        input >> array[i];
    }

    printResult(array, arraySize, min);
}

int main() {
    // test();
    run(std::cin, std::cout);
}