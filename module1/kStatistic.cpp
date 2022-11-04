#include <iostream>
#include <cassert>
#include <sstream>
#include <algorithm>

template <typename T>
bool min(const T& left, const T& right) {
    return left < right;
}

int getRandomMedian(int* sequence, int border_left, int border_right, bool(*comparator)(const int& l, const int& r) = min) {
    // std::cout << "Border left: " << border_left << std::endl;
    // std::cout << "Border right: " << border_right << std::endl;

    int first_index = (rand() % (border_right - border_left - 1)) + border_left;
    int second_index = (rand() % (border_right - border_left - 1)) + border_left;
    int third_index = (rand() % (border_right - border_left - 1)) + border_left;

    // std::cout << first_index << " " << second_index << " " << third_index << std::endl;

    double median = (sequence[first_index] + sequence[second_index] + sequence[third_index]) / 3.0;

    if (min(std::abs(median - sequence[second_index]), std::abs(median - sequence[first_index])))
        std::swap(second_index, first_index);
    if (min(std::abs(median - sequence[third_index]), std::abs(median - sequence[second_index])))
        std::swap(third_index, second_index);
    if (min(std::abs(median - sequence[second_index]), std::abs(median - sequence[first_index])))
        std::swap(second_index, first_index);

    return first_index;
}


int getMedianIndex(int* arr, int l, int middle, int r, bool(*comparator)(const int& l, const int& r) = min) {
    if (arr[l] == arr[middle] && arr[r] == arr[middle]) {
        return middle;
    }
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

int partition(int* arr, int l, int r, bool(*comparator)(const int& l, const int& r) = min) {

    if (r - l - 1 <= 1) return l;

    int pivotIndex = getRandomMedian(arr, l, r - 1, min);
    int pivotValue = arr[pivotIndex];

    std::swap(arr[pivotIndex], arr[r - 1]);
    
    int i = 0;
    int j = 0;

    if (pivotValue == 2) {
        std::cout << "j = " << j << std::endl;
        std::cout << "i = " << i << std::endl;
    }

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
        if (pivotValue == 2) {
        std::cout << "j = " << j << std::endl;
        std::cout << "i = " << i << std::endl;
    }
    }
    std::swap(arr[i], arr[r - 1]);
    return i;
}

void ktn_element(int* arr, int size, int k, bool(*comparator)(const int& l, const int& r) = min) {
    int l = 0;
    int r = size;

    while (true) {
        int m = partition(arr, l, r, comparator);
        // std::cout << "Search elem index: " << k << std::endl;
        // std::cout << "Ordered index: " << m << std::endl;
        // std::cout << "Left border index: " << l << std::endl;
        // std::cout << "Right border index: " << r << std::endl;
        if (m == k) return;
        if (k > m) {
            l = m + 1;
        } else {
            r = m;
        }
    }

}

void printResult(int* arr, int size, std::ostream& output, bool(*comparator)(const int& l, const int& r) = min) {
    ktn_element(arr, size, static_cast<int>(size * 0.1), min);
    output << arr[static_cast<int>(size * 0.1)] << std::endl;
    // std::cout << "*************Отладка*****************" << std::endl;
    // std::cout << "Values: ";
    // for (size_t i = 0; i < size; ++i) {
    //     std::cout << arr[i] << " ";
    // }
    // std::cout << std::endl;
    // std::cout << "Median index: " << static_cast<int>(size * 0.1) << std::endl;
    // std::cout << "Median values: " << arr[static_cast<int>(size * 0.1)] << std::endl;
    // std::cout << "*************Отладка конец*****************" << std::endl;
    ktn_element(arr, size, static_cast<int>(size * 0.5), min);
    output << arr[static_cast<int>(size * 0.5)] << std::endl;
    // std::cout << "*************Отладка*****************" << std::endl;
    // std::cout << "Values: ";
    // for (size_t i = 0; i < size; ++i) {
    //     std::cout << arr[i] << " ";
    // }
    // std::cout << std::endl;
    // std::cout << "Median index: " << static_cast<int>(size * 0.5) << std::endl;
    // std::cout << "Median values: " << arr[static_cast<int>(size * 0.5)] << std::endl;
    // std::cout << "*************Отладка конец*****************" << std::endl;
    ktn_element(arr, size, static_cast<int>(size * 0.9), min);
    output << arr[static_cast<int>(size * 0.9)] << std::endl;
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
            int* arr = new int[8]{2, 6, 1, 4, 3, 7, 5, 7};
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
    int arraySize;
    input >> arraySize;
    int* array = new int[arraySize]{0};

    for (int i = 0; i < arraySize; ++i) {
        input >> array[i];
    }

    printResult(array, arraySize, output, min);

    delete[] array;
    array = nullptr;
}

void testLogic() {
    std::cout << "***********TEST LOGIC***********";
    {
        std::stringstream sstr_input;
        sstr_input << "10" << std::endl;
        sstr_input << "1 2 3 4 5 6 7 8 9 10" << std::endl;
        std::stringstream sstr_output;

        run(sstr_input, sstr_output);
        assert(sstr_output.str() == "2 6 10");
    }
    std::cout << "OK\n";
}

int main() {
    // testLogic();
    // test(); 
    run(std::cin, std::cout);
}


// надо сделать вывод массива во всех функциях