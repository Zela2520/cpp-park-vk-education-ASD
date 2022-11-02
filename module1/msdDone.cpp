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
// counterArray - массив, индексы которого являются все возможными значениями словаря. В ячейках содержится количество раз, которое встретился тот или иной элемент.

// for (size_t i = 0; i < size; ++i) ++counter[arr[i]]; // говорит сколько раз встретился элемент в словаре.

// for (size_t i = 1; i < avaliableValues; ++i) counter[i] += counter[i - 1]; // формируем группы элементов путем суммирования элементов, которые встречались  в словаре. Таким образом понимаем на какой позиции находится та или иная группа одинаковых элементов. Чтобы извлекать элементы группы достаточно сделать следующее обращение: --counter[arr[i]];


// Как трактовать такую запись ? res[--counter[arr[i]]] = arr[i];
// Ответ:  извлекаем i-ый элемент на последнюю позицию в его группе. (следующее извлечение на предпоследную позицию и т.д.)

#include <iostream>
#include <vector>
#include <cstring>
#include <string.h>
#include <string>
#include <cassert>


#define MAX_ROWS_NUMBER 100000
#define ALPHABET_SIZE 256

struct CounterHelper {
    size_t stringNumber;
    char curCharacter;
};

// Что мы должны передавать в функцию counterSort ?
// 1 аргумент - начало группы
// 2 аргумент - конец группы
// 3 аргумент - индекс, по которому производится сортировка в группе


// counterSort - работает: сортирует строки по символу
// void counterSort(std::string *arr , size_t size, int* counterArray, size_t maxStringSizeInArray) 
// void counterSort(std::string *arr , size_t size, size_t maxStringSizeInArray)

void counterSort(std::string *arr , size_t size, int* counterArray, size_t sortIndex) {
    // int* counterArray = new int[ALPHABET_SIZE];
    for (size_t i = 0; i < ALPHABET_SIZE; ++i) counterArray[i] = 0;
    for (size_t i = 0; i < size; ++i) ++counterArray[arr[i][sortIndex]]; // заполняем словарь первыми буквами группы

    for (size_t i = 1; i < ALPHABET_SIZE; ++i) {
        counterArray[i] += counterArray[i - 1];
    }

    std::string result[size];
    
    for (int i = size - 1; i >= 0; --i) {
        result[--counterArray[arr[i][sortIndex]]] = arr[i];
    }

    for (size_t i = 0; i < size; ++i) {
        arr[i] = result[i];
    }
}

void MSDSort(std::string* arr, size_t size, size_t sortIndex, size_t maxStringSizeInArray) {
    if (size <= 1) return;
    std::cout << "SortIndex " << sortIndex << " Array size is: " <<  size << std::endl;
    for (size_t i = 0; i < size; ++i) {
        std::cout << arr[i] << ' ';
    }
    std::cout << std::endl;
    int* counterArray = new int[ALPHABET_SIZE];
    counterSort(arr, size, counterArray, sortIndex++); // после этой строки получаем отсортированный по первому разряду массив строк
    if (maxStringSizeInArray > sortIndex) {
        for (int i = 1; i < ALPHABET_SIZE; ++i) {
            // std::cout << *(arr + counterArray[i - 1]) << "\t" << counterArray[i] - counterArray[i - 1] << std::endl;
            MSDSort(arr + counterArray[i - 1], counterArray[i] - counterArray[i - 1], sortIndex, maxStringSizeInArray);
        }
    }
    delete[] counterArray;
}

void test() {
    std::cout << "***********TEST CASES************" << std::endl;
    std::cout << "*****MSDSort case" << std::endl;
    {
        {   
            constexpr auto arraySize = 4;
            size_t maxStringSize = 3;
            std::string s[arraySize] = {"bb", "a", "daa", "ba"};
            MSDSort(s, arraySize, 0, maxStringSize);
            for (size_t i = 0; i < 4; ++i) {
                std::cout << s[i] << std::endl;
            }
        }
    }
    std::cout << "*****OK\n";
    std::cout << "***********All tests passed***********\n";
}

void run(std::istream& input, std::ostream& output) {
    std::string stringArray[MAX_ROWS_NUMBER];
    size_t arraySize = 0;
    size_t maxStringSizeInArray = 0; 

    std::string buffer = "";

    while (getline(input, buffer) && buffer != "\0") {
        stringArray[arraySize] = buffer;

        if (stringArray[arraySize].size() > maxStringSizeInArray) {
            maxStringSizeInArray = stringArray[arraySize].size();
        }
        ++arraySize;
    }

    MSDSort(stringArray, arraySize, 0, maxStringSizeInArray);

    for (size_t i = 0; i < arraySize; ++i) {
        output << stringArray[i] << std::endl;
    }
}

int main() {
    // test();
    run(std::cin, std::cout);
    return 0;
}
