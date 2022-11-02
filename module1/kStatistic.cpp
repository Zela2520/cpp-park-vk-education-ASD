// Условие:
// Реализуйте стратегию выбора опорного элемента “медиана трёх”.
// Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу.





// Логика:
// Найти знчения элемента, который будет стоять в массиве на k-ом индексе, если массив будет отсортированн
// Отсортировать массив и взять индекс элемента, которого мы хотим найти

// Двигаем итератор i -> пока соблюдается условие, потом двигаем итератор j <- пока выполняется условие 
// В итоге в момент не соблюдения i и j итераторы будут указывать на элементы, которые должны будут лежать в разных частях массива
// Меняем элементы местами элементы, на которые они указывают
// В итоге i-ый элемент будет указывать на элемент, который больше pivat - меняем pivat с элементом, на который указывает i-ый итератор
 

#include <iostream>
#include <vector>


int partition(int* arr, int l, int r) {
    int pivotIndex = l + (r - l) / 2;
    int pivotVal = arr[pivotIndex];

    std::swap(arr[pivotIndex], arr[r - 1]);

    int i = 0;
    int j = r - 1;

    while (i < j) {
        for (; arr[i] < pivotVal; ++i);
        while(!(arr[i] < pivotVal)) --j;
    }
     
    if (i < j) {
        std::swap(arr[i], arr[j]);
        i++; 
        j--;
    }

    std::swap(arr[i], arr[r - 1]);
    return i;
}

// после выполнения этой функции, в массиве arr на позиции k будет стоять элемент такой если бы массив был остортирован  
void ktn_element(int* arr, size_t size, size_t k) {
    int l = 0;
    int r = size;

    while(l < r) {
        int m = partition(arr, l, r);
        if (m == k) {
            return;
        } else if (m < k) {
            l = m + 1; 
        } else {
            r = m;
        }
    }
}

void run(std::istream& input, std::ostream& output) {
    std::vector<int> arr = {5, 4, 3, 2, 1, 9, 5, 8, 6, 7};

    size_t k = 3;

    ktn_element(&arr[0], arr.size(), k);

    output << "ktn element: " << arr[k] << std::endl;
}

int main() {
    run(std::cin, std::cout);
    return 0;
}