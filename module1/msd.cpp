// Дан массив строк. Количество строк не больше 100000. Отсортировать массив методом поразрядной сортировки MSD по символам. Размер алфавита - 256 символов. Последний символ строки = ‘
// 0’.

// Пример
// Ввод	Вывод
// ab
// a
// aaa
// aa
// a
// aa
// aaa
// ab

// Замечания:
// нужно по сути сделать строку, в которую будем записывать все символы. // std::string s(size, '\0');
// возможно лучше все-таки создать массив массивов, чтобы 
// хочу менять элементы по указателю и менять указатели

#include <iostream>
#include <vector>
#include <cstring>
#include <string.h>
#include <string>


#define MAX_ROWS_NUMBER 100000
#define ALPHABET_SIZE 256


// заполнить строки нулями до максимальной строки в массиве
// написать цикл, который будет формировать массив из одинаковых разрядов
// написать функцию, которая будет сортировать
// отправлять массивы нв сортировку потом сливать 

// после выполнения этой функции получим массив строк, в котором будут отсортированы буквы по индексу sortIndex
// void counterSort(std::string *arr, size_t size, size_t sortIndex, size_t maxStringSizeInArray) {
//     int* counterArray = new int[ALPHABET_SIZE];
//     for (size_t i = 0; i < ALPHABET_SIZE; ++i) counterArray[i] = 0;
//     for (size_t i = 0; i < size; ++i) ++counterArray[static_cast<int>(arr[i][sortIndex])]; // массив для определенного разряда - строка размером равным количеству строк

//     // for (size_t i = 0; i < size; ++i) std::cout << arr[i][sortIndex] << " " << static_cast<int>(arr[i][sortIndex]) << std::endl; // отладка

//     for (size_t i = 1; i < ALPHABET_SIZE; ++i) {
//         counterArray[i] += counterArray[i - 1];
//     }

//     std::string result[size];
    
//     for (int i = size - 1; i >= 0; --i) {
//         result[--counterArray[arr[i][sortIndex]]] = arr[i];
//     }

//     for (size_t i = 0; i < size; ++i) {
//         arr[i] = result[i];
//     }

//     delete[] counterArray;
//     counterArray =  nullptr;
// }

// void MSDSort(std::string* arr, size_t size, size_t alfavitSize, size_t maxStringSizeInArray) {
//     for (size_t i = 0; i < maxStringSizeInArray; ++i) {
//         counterSort(arr, size, i, maxStringSizeInArray);
//     }
// }

// void run(std::istream& input, std::ostream& output) {
//     std::string stringArray[MAX_ROWS_NUMBER];
//     size_t arraySize = 0;
//     size_t maxStringSizeInArray = 0; 

//     std::string buffer = "";

//     while (getline(input, buffer) && buffer != "\0") {
//         stringArray[arraySize] = buffer;

//         if (stringArray[arraySize].size() > maxStringSizeInArray) {
//             maxStringSizeInArray = stringArray[arraySize].size();
//         }
//         ++arraySize;
//     }

//     MSDSort(stringArray, arraySize, ALPHABET_SIZE, maxStringSizeInArray);

//     for (size_t i = 0; i < arraySize; ++i) {
//         output << stringArray[i] << std::endl;
//     }
// }

void counterSort(std::string *arr, size_t size, size_t sort_index, size_t maxStringSizeInArray) {
    // if (size <= 1) return;
    int* counterArray = new int[ALPHABET_SIZE];
    for (size_t i = 0; i < ALPHABET_SIZE; ++i) counterArray[i] = 0;
    for (size_t i = 0; i < size; ++i) ++counterArray[static_cast<int>(arr[i][sort_index])]; // массив для определенного разряда - строка размером равным количеству строк

    for (size_t i = 1; i < ALPHABET_SIZE; ++i) {
        counterArray[i] += counterArray[i - 1];
    }

    // for (size_t i = 0; i < size; ++i) {
    //     counterSort(arr + counterArray[i], counterArray[i + 1] - counterArray[i], sort_index + 1, maxStringSizeInArray - 1);
    // }
    
    
    std::string result[size];
    
    for (int i = size - 1; i >= 0; --i) {
        result[--counterArray[arr[i][sort_index]]] = arr[i];
    }

    for (size_t i = 0; i < size; ++i) {
        arr[i] = result[i];
    }

    delete[] counterArray;
    counterArray =  nullptr;
}

void test() {
    std::cout << "***********TEST CASES************" << std::endl;
    std::cout << "*****CounterSort case" << std::endl;
    {
        std::string s[4] = {"cc", "cb", "ca", "ba"};
        counterSort(s, 4, 0, 2);
        for (size_t i = 0; i < 4; ++i) {
            std::cout << s[i] << std::endl;
        }
    }
    std::cout << "OK\n";
    std::cout << "***********All tests passed***********\n";
}

int main() {assdcds
    test();
    // run(std::cin, std::cout);
    return 0;
}
