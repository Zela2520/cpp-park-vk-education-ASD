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

#include <iostream>
#include <vector>
#include <cstring>
#include <string.h>
#include <string>


#define MAX_ROWS_NUMBER 100000
#define MAX_SIZE_STRING 256


// заполнить строки нулями до максимальной строки в массиве
// написать цикл, который будет формировать массив из одинаковых разрядов
// написать функцию, которая будет сортировать
// отправлять массивы нв сортировку потом сливать 


void counterSort(std::string arr[], size_t size, size_t sortIndex, size_t alfavitSize, size_t maxStringSizeInArray) {
    int* counterArray = new int[alfavitSize];
    for (size_t i = 0; i < alfavitSize; ++i) counterArray[i] = 0;
    for (size_t i = 0; i < size; ++i) ++counterArray[static_cast<int>(arr[i][sortIndex])];

    // for (size_t i = 0; i < size; ++i) std::cout << arr[i][sortIndex] << " " << static_cast<int>(arr[i][sortIndex]) << std::endl; // отладка
    for (size_t i = 1; i < alfavitSize; ++i) {
        counterArray[i] += counterArray[i - 1];
    }

    // нужно по сути сделать строку, в которую будем записывать все символы.
    // возможно лучше все-таки создать массив массивов ???
    std::string result(size, '\0');
    
    for (int i = size - 1; i >= 0; --i) {
        result[--counterArray[arr[i][sortIndex]]] = arr[i][sortIndex];
    }

    // copy to dest array
    for (size_t i = 0; i < size; ++i) {
        arr[i][sortIndex] = result[i];
    }

}

void MSDSort(std::string* arr, size_t size, size_t alfavitSize, size_t maxStringSizeInArray) {

    for (size_t i = 0; i < maxStringSizeInArray; ++i) {
        counterSort(arr, size, i, alfavitSize, maxStringSizeInArray); // после выполнения этой строчки получим список массивов отсортированных по первому разряду
    }

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
    stringArray[arraySize] = buffer;


    // заполним строки отличающиеся по размеру от максимальной '\0' ?

    MSDSort(stringArray, arraySize, MAX_SIZE_STRING, maxStringSizeInArray);

    for (size_t i = 0; stringArray[i] != "\0"; ++i) {
        output << stringArray[i] << std::endl;
    }
}

void test() {
    std::cout << "***********TEST CASES************" << std::endl;
    {

    }
    std::cout << "OK\n";
}

int main() {
    run(std::cin, std::cout);
    return 0;
}
