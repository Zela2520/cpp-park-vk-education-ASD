// В супермаркете решили оптимизировать показ рекламы. 
// Известно расписание прихода и ухода покупателей (два целых числа).
// Каждому покупателю необходимо показать минимум 2 рекламы.
// Рекламу можно транслировать только в целочисленные моменты времени.
// Покупатель может видеть рекламу от момента прихода до момента ухода из магазина.
// В каждый момент времени может показываться только одна реклама. 
// Считается, что реклама показывается мгновенно. Если реклама показывается в момент ухода или прихода,
// то считается, что посетитель успел её посмотреть.
// Требуется определить минимальное число показов рекламы.

// Пример
// Ввод	Вывод
// 5
// 1 10
// 10 12
// 1 10
// 1 10
// 23 24
// 5


// 5
// 1 4
// 2 6
// 7 8
// 20 21
// 5 9

// 6 - ответ


// Логика:
// 1) Сделать для mergeSort кастомный comparator (который сравнивает по времени ухода, если время ухода одинаковое, то сравнивает по времени прихода).
// по уходу сортируем по возрастанию, по приходу сортируем по убыванию.
// 2) Сделать mergeSort
// 3) Получаем список этих пар
// 4) Идем по этому отсортировнному массиву 
// 5) Если есть группа с одинаковым временем ухода, то выбираем группу для которой время прихода максимальное ? 
// 6) Дальше либо такую же пару, либо пару которая обособленно стоит


#include <iostream>
#include <cstring>
#include <cstdint>
#include <sstream>
#include <cassert>


struct Customer {
    int comingTime;
    int leavingTime;
    int showCounter = 0;

    bool hasTime(int showTime);
    bool needToShow() { return showCounter < 2; }
    void show () { ++showCounter; }
};

bool Customer::hasTime(int showTime) {
    return ((comingTime <= showTime) && (showTime <= leavingTime));
}

std::istream& operator>>(std::istream& input, Customer& customer) {
    input >> customer.comingTime >> customer.leavingTime;
    return input;
}

std::ostream& operator<<(std::ostream& output, const Customer& customer) {
    output << customer.comingTime << "\t" << customer.leavingTime << std::endl;
    return output;
}

bool min(Customer& interval_1, Customer& interval_2) {
    if (interval_1.leavingTime == interval_2.leavingTime) {
        return interval_1.comingTime > interval_2.comingTime;
    }
    return interval_1.leavingTime < interval_2.leavingTime;
}

bool max(Customer& interval_1, Customer& interval_2) {
    return interval_1.leavingTime > interval_2.leavingTime;
}

bool minTestComp(int& interval_1, int& interval_2) {
    return interval_1 < interval_2;
}

/////////////////////////////////////////////Main functions/////////////////////////////////////////////
template <typename T>
void merge(T* firstArray, size_t firstLen, T* secondArray, size_t secondLen, T* resultArr, bool (*comparator)(T& interval_1, T& interval_2)) {
    size_t i, j, k;
    i = j = k = 0;
    while (i < firstLen || j < secondLen) {
        if (j == secondLen || i < firstLen && comparator(firstArray[i], secondArray[j])) {
            resultArr[k++] = firstArray[i++];
        } else {
            resultArr[k++] = secondArray[j++];
        }
    }
}

template <typename T>
void mergeSort(T* customerArray, size_t size, bool (*comparator)(T& interval_1, T& interval_2)) {
    if (size <= 1) {
        return;
    }

    size_t firstLen = (size / 2);
    size_t secondLen = size - firstLen;

    mergeSort(customerArray, firstLen, comparator);
    mergeSort(customerArray + firstLen, secondLen, comparator);

    T* resultArr = new T[size];

    merge(customerArray, firstLen, customerArray + firstLen, secondLen, resultArr, comparator);

    memcpy(customerArray, resultArr, sizeof(T) * size);
    delete[] resultArr;
}

void test() {
    std::cout << "***********TEST CASES************" << std::endl;
    std::cout << "***********Merge case****: " << std::endl;
    {
        std::cout << "Merge case1: " << std::endl;
        {
            int* testArray1 = new int[3] {2, 3, 5};
            int* testArray2 = new int[3] {1, 4, 6};
            int* result = new int[6];

            merge(testArray1, 3, testArray2, 3, result, minTestComp);
            for(size_t i = 0; i < 6; ++i) {
                assert(result[i] == static_cast<int>(i + 1));
            }
            delete[] result;
            result = nullptr;
        }
        std::cout << "OK\n";
        std::cout << "Merge case2: " << std::endl;
        {
            int* testArray1 = new int[3] {1, 5, 8};
            int* testArray2 = new int[3] {3, 6, 7};
            int* result = new int[6];
            merge(testArray1, 3, testArray2, 3, result, minTestComp);

            assert(result[0] == 1);
            assert(result[1] == 3);
            assert(result[2] == 5);
            assert(result[3] == 6);
            assert(result[4] == 7);
            assert(result[5] == 8);

            delete[] result;
            result = nullptr;
        }
        std::cout << "OK\n";
        std::cout << "Merge case3 - different sizes: " << std::endl;
        {
            int* testArray1 = new int[3] {2, 3};
            int* testArray2 = new int[3] {1, 4, 6};
            int* result = new int[5];
            merge(testArray1, 2, testArray2, 3, result, minTestComp);
            
            assert(result[0] == 1);
            assert(result[1] == 2);
            assert(result[2] == 3);
            assert(result[3] == 4);
            assert(result[4] == 6);

            delete[] result;
            result = nullptr;
        }
        std::cout << "OK\n";
    }
    std::cout << "OK\n";

    std::cout << "***********MergeSort case***: " << std::endl;
    {
        size_t arraySize = 6;
        int* testArray = new int[arraySize] {2, 5, 1, 4, 3, 6};
        mergeSort(testArray, arraySize, minTestComp);

        assert(1 == testArray[0]);
        assert(2 == testArray[1]);
        assert(3 == testArray[2]);
        assert(4 == testArray[3]);
        assert(5 == testArray[4]);
        assert(6 == testArray[5]);
        
        delete[] testArray;
        testArray = nullptr;
    }
    std::cout << "OK\n";

    std::cout << "***********TEST CUTOMERS SORT: ";
    {
        {
            std::cout << "***TEST1:" << std::endl;
            size_t size = 5;
            Customer* CustomersArray = new Customer[size];
            
            CustomersArray[0].comingTime = 10;
            CustomersArray[0].leavingTime = 15;
        
            CustomersArray[1].comingTime = 5;
            CustomersArray[1].leavingTime = 10;

            CustomersArray[2].comingTime = 6;
            CustomersArray[2].leavingTime = 10;

            CustomersArray[3].comingTime = 1;
            CustomersArray[3].leavingTime = 5;

            CustomersArray[4].comingTime = 1;
            CustomersArray[4].leavingTime = 4;

            mergeSort(CustomersArray, size, min);
            
            assert(CustomersArray[0].comingTime == 1);
            assert(CustomersArray[0].leavingTime == 4);
        
            assert(CustomersArray[1].comingTime == 1);
            assert(CustomersArray[1].leavingTime == 5);

            assert(CustomersArray[2].comingTime = 6);
            assert(CustomersArray[2].leavingTime = 10);

            assert(CustomersArray[3].comingTime = 5);
            assert(CustomersArray[3].leavingTime = 10);

            assert(CustomersArray[4].comingTime = 10);
            assert(CustomersArray[4].leavingTime = 15);
            std::cout << "OK" << std::endl;
        }
        {
            std::cout << "***TEST2:" << std::endl;
            size_t size = 5;
            Customer* CustomersArray = new Customer[size];
            
            CustomersArray[0].comingTime = 1;
            CustomersArray[0].leavingTime = 10;
        
            CustomersArray[1].comingTime = 10;
            CustomersArray[1].leavingTime = 12;

            CustomersArray[2].comingTime = 1;
            CustomersArray[2].leavingTime = 10;

            CustomersArray[3].comingTime = 1;
            CustomersArray[3].leavingTime = 10;

            CustomersArray[4].comingTime = 23;
            CustomersArray[4].leavingTime = 24;

            mergeSort(CustomersArray, size, min);

            assert(CustomersArray[0].comingTime == 1);
            assert(CustomersArray[0].leavingTime == 10);
        
            assert(CustomersArray[1].comingTime == 1);
            assert(CustomersArray[1].leavingTime == 10);

            assert(CustomersArray[2].comingTime == 1);
            assert(CustomersArray[2].leavingTime == 10);

            assert(CustomersArray[3].comingTime == 10);
            assert(CustomersArray[3].leavingTime == 12);

            assert(CustomersArray[4].comingTime == 23);
            assert(CustomersArray[4].leavingTime == 24);

            std::cout << "OK" << std::endl;
        }
    }
    std::cout << "OK\n";

    std::cout << "All tests passed"<< std::endl;
}

/////////////////////////////////////////////////////////////////


void print(Customer* customerArray, size_t amount, std::ostream& output) {
    for (size_t iCustomer = 0; iCustomer < amount; ++iCustomer) {
        output << customerArray[iCustomer];
    }
}

void run(std::istream& input, std::ostream& output) {
    size_t numberOfCustomer;
    input >> numberOfCustomer;

    Customer* CustomersArray = new Customer[numberOfCustomer];
    for (size_t iCustomer = 0; iCustomer < numberOfCustomer; ++iCustomer) {
        input >> CustomersArray[iCustomer];
    }

    mergeSort(CustomersArray, numberOfCustomer, min);

    // решение 1

    // size_t sumShowCounter = 0;
    
    // for (size_t i = 0; i < numberOfCustomer; ++i) {
    //     if (CustomersArray[i].needToShow()) {
    //         size_t firstShow = CustomersArray[i].leavingTime;
    //         size_t secondShow = CustomersArray[i].leavingTime - 1;

    //         size_t j = i + 1;
    //         if (CustomersArray[i].needToShow()) {
    //             CustomersArray[i].show();
    //             ++sumShowCounter;
    //             while(CustomersArray[j].hasTime(firstShow) && j < numberOfCustomer) {
    //                 CustomersArray[j].show();
    //                 ++j;
    //             }
    //         }

    //         j = i + 1;
    //         if (CustomersArray[i].needToShow()) {
    //             CustomersArray[i].show();
    //             ++sumShowCounter;
    //             while(CustomersArray[j].hasTime(secondShow) && j < numberOfCustomer) {
    //                 CustomersArray[j].show();
    //                 ++j;
    //             }
    //         }
    //     } 
    // }

    // output << sumShowCounter << std::endl;

// решение 2
    size_t showCounter = 2;
    size_t firstShow = CustomersArray[0].leavingTime - 1;
    size_t secondShow = CustomersArray[0].leavingTime;

    for (size_t i = 1; i < numberOfCustomer; ++i) {       
        if (CustomersArray[i].comingTime > secondShow) {
            showCounter += 2;
            firstShow = CustomersArray[i].leavingTime - 1;
            secondShow = CustomersArray[i].leavingTime;
        } else if (CustomersArray[i].comingTime == secondShow) {
            showCounter++;
            firstShow = secondShow;
            secondShow = CustomersArray[i].leavingTime;
        } else if (CustomersArray[i].comingTime > firstShow && CustomersArray[i].leavingTime > secondShow) {
            showCounter++;
            firstShow = secondShow;
            secondShow = CustomersArray[i].leavingTime;
        }
    }

    output << showCounter << std::endl;

    delete[] CustomersArray;
    CustomersArray = nullptr;
}

int main(int argc, char* argv[]) {
    // test();
    run(std::cin, std::cout);
    return 0;
};
