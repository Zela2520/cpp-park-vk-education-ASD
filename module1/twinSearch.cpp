// Дан отсортированный массив различных целых чисел A[0..n-1] и массив целых чисел B[0..m-1].
// Для каждого элемента массива B[i] найдите минимальный индекс элемента массива A[k], ближайшего по значению к B[i]. n ≤ 110000, m ≤ 1000.
// Время работы поиска для каждого элемента B[i]: O(log(k)).



// ЛОГИКА РЕШЕНИЯ:
// 1) Буду бежать по массиву source и передавать из него в метод find очередной элемент.
// 2) А в методе find будет искаться переданный элемент в массиве target(бинарный поиск)
// 3) Find вернёт индекс, который мы запишем в массив результатов

// 4) Написать функцию, который ищет диапазон возвращает границы и они передаются в бинарный поиск

#include <iostream>
#include <cstdint>
#include <cassert>

#define ZERO 0

template <typename T>
class Solution {
public:
    Solution();
    ~Solution();
    void fill(T* array, size_t size);
    size_t find_top(T target); // поиск верхней границы для осуществления бинарного поиска.
    size_t binary_search(size_t begin, size_t end, T target);
    void print_result();

private:
    T* target_sequence;
    T* source_sequence;
    size_t target_size;
    size_t source_size;
};

template <typename T>
Solution<T>::Solution() {

    std::cin >> target_size;
    assert(target_size != 0); // массивы не могут быть пустыми
    this->target_sequence = new T[target_size];
    this->fill(target_sequence, target_size);

    std::cin >> source_size;
    assert(source_size != 0); // массивы не могут быть пустыми
    this->source_sequence = new T[source_size];
    this->fill(source_sequence, source_size);
}

template <typename T>
Solution<T>::~Solution() {
    delete[] this->target_sequence;
    this->target_sequence = nullptr;

    delete[] this->source_sequence;
    this->source_sequence = nullptr;
}


template <typename T>
void Solution<T>::fill(T* array, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        std::cin >> array[i];
    }
}

template <typename T>
void Solution<T>::print_result() {

    for (size_t i = 0; i < source_size; ++i) {

        if (target_size != 1) {
            size_t end = find_top(source_sequence[i]);
            size_t begin = end>>1;
            std::cout << binary_search(begin, end, source_sequence[i]) << " ";
        } else {
            std::cout << ZERO << " ";
        }
    }
}


template <typename T>
size_t Solution<T>::find_top(T target) {
    size_t top_index = 1;
    for (; top_index <= target_size && target_sequence[top_index] < target; top_index = top_index << 1);

    if (top_index > target_size - 1) {
        return target_size - 1;
    } else {
        return top_index;
    }
}

template <typename T>
size_t Solution<T>::binary_search(size_t begin, size_t end, T target) {

    if (target <= this->target_sequence[0]) {
        return 0;
    }

    while(begin < end) {

        size_t middle = (begin + end) / 2;
        if (this->target_sequence[middle] < target) {
            begin = middle + 1;
        } else {
            end = middle;
        }
    }

    if (target - target_sequence[begin - 1] <= target_sequence[begin] - target) {
        return begin - 1;
    }

    return begin;
}


int main(int argc, char *argv[]) {
    Solution<int> answer;
    answer.print_result();
    return 0;
}