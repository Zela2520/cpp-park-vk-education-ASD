// Дано множество целых чисел из [0..10^9] размера array_size.
// Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры множества:
// 10%  перцентиль медиана 90%  перцентил

// Требования: к дополнительной памяти: O(array_size).
// Среднее время работы: O(array_size)
// Должна быть отдельно выделенная функция partition.
// Рекурсия запрещена.
// Решение должно поддерживать передачу функции сравнения снаружи.


// Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении. Описание для случая прохода от начала массива к концу:
// Выбирается опорный элемент. Опорный элемент меняется с последним элементом массива.
// Во время работы Partition в начале массива содержатся элементы, не бОльшие опорного. Затем располагаются элементы, строго бОльшие опорного. В конце массива лежат нерассмотренные элементы. Последним элементом лежит опорный.
// Итератор (индекс) i указывает на начало группы элементов, строго бОльших опорного.
// Итератор j больше i, итератор j указывает на первый нерассмотренный элемент.
// Шаг алгоритма. Рассматривается элемент, на который указывает j. Если он больше опорного, то сдвигаем j.
// Если он не больше опорного, то меняем a[i] и a[j] местами, сдвигаем i и сдвигаем j.
// В конце работы алгоритма меняем опорный и элемент, на который указывает итератор i.
// Реализуйте стратегию выбора опорного элемента “медиана трёх”. Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу.


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cassert>
#include <cstring>


template<typename T>
class DefaultSequence {
public:
    bool operator() (const T& l, const T& r) const {
        return l < r;
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T, class Comparator = DefaultSequence<T>>
int get_median(T* sequence, int border_left, int border_right, Comparator min = DefaultSequence<T>()) {
    
    int first_index = (rand() % (border_right - border_left - 1)) + border_left;
    int second_index = (rand() % (border_right - border_left - 1)) + border_left;
    int third_index = (rand() % (border_right - border_left - 1)) + border_left;

    double median = (sequence[first_index] + sequence[second_index] + sequence[third_index]) / 3.0;

    if (min(std::abs(median - sequence[second_index]), std::abs(median - sequence[first_index])))
        std::swap(second_index, first_index);
    if (min(std::abs(median - sequence[third_index]), std::abs(median - sequence[second_index])))
        std::swap(third_index, second_index);
    if (min(std::abs(median - sequence[second_index]), std::abs(median - sequence[first_index])))
        std::swap(second_index, first_index);

    return first_index;
}

template<typename T, class Comparator = DefaultSequence<T>>
int partition(T* sequence, int border_left, int border_right, Comparator min = DefaultSequence<T>()) {

    if (border_right - border_left <= 1 ) {
        return border_left;
    }

    int pivot = get_median(sequence, border_left, border_right, min);
    std::swap(sequence[border_right - 1], sequence[pivot]);

    int pivot_data = sequence[border_right - 1];
    int insert_position = border_left;
    
	for (size_t i = border_left; i <= border_right - 2; ++i) {
        if (min(sequence[i], pivot_data)) {
        	std::swap(sequence[insert_position], sequence[i]);
        	++insert_position;
        }
    }
    std::swap(sequence[insert_position], sequence[border_right - 1]);

    return insert_position;
}


template<typename T, class Comparator = DefaultSequence<T>>
T find_k_statistic(T* sequence, int border_left, int border_right, int index, Comparator min = DefaultSequence<T>()) {

    int pivot_index;

    do {
        pivot_index = partition(sequence, border_left, border_right, min);
        
        if (index >= pivot_index) {
            border_left = pivot_index + 1;
        } else {
            border_right = pivot_index;
        }
    } while (pivot_index != index);

    return sequence[index];
}

template<typename T, class Comparator = DefaultSequence<T>>
void print_result(T* sequence, size_t seq_size, Comparator min = DefaultSequence<T>()) {

    std::cout << find_k_statistic(sequence, 0, seq_size, (seq_size * 0.1), min) << std::endl;
    std::cout << find_k_statistic(sequence, 0, seq_size, (seq_size * 0.5), min) << std::endl;
    std::cout << find_k_statistic(sequence, 0, seq_size, (seq_size * 0.9), min) << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
    size_t sequence_size;
    std::cin >> sequence_size;
    assert(sequence_size > 0);

    int* sequence = new int[sequence_size];
    assert(sequence != nullptr);
    
    for (size_t i = 0; i < sequence_size; ++i) {
        std::cin >> sequence[i];
    }

    print_result(sequence, sequence_size);

    delete[] sequence;
    return 0;
}