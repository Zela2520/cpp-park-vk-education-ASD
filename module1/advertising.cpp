// В супермаркете решили оптимизировать показ рекламы. Известно расписание прихода и ухода покупателей (два целых числа). Каждому покупателю необходимо показать минимум 2 рекламы. Рекламу можно транслировать только в целочисленные моменты времени. Покупатель может видеть рекламу от момента прихода до момента ухода из магазина. В каждый момент времени может показываться только одна реклама. Считается, что реклама показывается мгновенно. Если реклама показывается в момент ухода или прихода, то считается, что посетитель успел её посмотреть.
// Требуется определить минимальное число показов рекламы.
// Пример
// Ввод	Вывод
// 5
// 1 10
// 10 12
// 1 10
// 1 10
// 23 24

#include <iostream>
#include <cassert>
#include <sstream>


bool min(int left, int right) {
    return left > right;
}

bool max(int left, int right){
    return left < right;
}

struct Customer {
    size_t list;
    size_t contribution;
};

bool minContributionList(Customer* first, Customer* second) {
    if (first->contribution < second->contribution) {
        return true;
    }
    else if (first->contribution == second->contribution) {
        return first->list > second->list;
    }
    return false;
}

void merge(Customer *list, int head, int center, int tail, bool (*comparator)(Customer *first, Customer *second)) {
    size_t size_left  = center - head + 1;
    size_t size_right = tail - center;

    Customer *left  = new Customer[size_left];
    Customer *right = new Customer[size_right];

    for (size_t i = 0; i < size_left; i++) {
        left[i] = list[head + i];
    }
    for (size_t i = 0; i < size_right; i++) {
        right[i] = list[center + i + 1];
    }
    
    int z = head;
    size_t i = 0;
    size_t j = 0;
    for(; i < size_left && j < size_right; z++) {
        if(minContributionList(&left[i], &right[j]))
            list[z] = left[i++];
        else
            list[z] = right[j++];
    }
    while (i < size_left)
        list[z++] = left[i++];
    while (j < size_right)
        list[z++] = right[j++];
    
    delete[] left;
    delete[] right;
}

void mergeSort(Customer *list, int head, int tail, bool (*comparator)(Customer *first, Customer *second) ) {
    if (tail - head == 0)
        return;
    int center = (head + tail) / 2;
    mergeSort(list, head, center, comparator);
    mergeSort(list, center + 1, tail, comparator);
    merge(list, head, center, tail, comparator);
}

void run(std::istream &input, std::ostream &output) {

    size_t size = 0;
    input >> size;
    if (size == 0) {
        output << size << std::endl;
        return;
    }
    Customer *customer = new Customer[size];
    for (size_t i = 0; i < size; i++)
        input >> customer[i].list >> customer[i].contribution;

    mergeSort(customer, 0, size-1, minContributionList);

    ////////////////////Main////////////////////
    size_t outputRes = 2;
    size_t posToAdd = customer[0].contribution - 1;
    size_t pos2ToAdd = customer[0].contribution;

    for (size_t i = 1; i < size; i++) {
        Customer cust = customer[i];

        if (cust.list > pos2ToAdd) {
            outputRes += 2;
            posToAdd = cust.contribution - 1;
            pos2ToAdd = cust.contribution;

        } else if (cust.list == pos2ToAdd) {
            outputRes++;
            posToAdd = pos2ToAdd;
            pos2ToAdd = cust.contribution;
            
        } else if (cust.list > posToAdd && cust.contribution > pos2ToAdd) {
            outputRes++;
            posToAdd = pos2ToAdd;
            pos2ToAdd = cust.contribution;
        }
    }

    output << outputRes << std::endl;
    delete[] customer;
}

int main() {
    run(std::cin, std::cout);
    return 0;
}