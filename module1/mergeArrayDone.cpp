// Напишите программу, которая использует кучу для слияния K отсортированных массивов суммарной длиной N.

// Требования:

// Время работы O(N * logK)
// Куча должна быть реализована в виде шаблонного класса.
// Решение должно поддерживать передачу функции сравнения снаружи.
// Куча должна быть динамической.
// Формат ввода
// Сначала вводится количество массивов K.
// . Затем по очереди размер каждого массива и элементы массива.
// Каждый массив упорядочен по возрастанию.

// Формат вывода
// Итоговый отсортированный массив.

/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cstring>

#define DEFAULT_CAPACITY 1

template <typename T>
struct ArrayIterator {
    T *buffer;
    size_t size;
    size_t cur_iterator;

    void push_back(const T &elem);
    void print_array();
};

template <typename T>
void ArrayIterator<T>::push_back(const T &elem) {
    buffer[size] = elem;
    ++size;
}

template <typename T>
void ArrayIterator<T>::print_array() { // для отладки 
    for (int i = cur_iterator; i < size; ++i) { // печатаем начиная с позиции итератора
        std::cout << buffer[i] << " ";
    }
    std::cout << std::endl;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
class DefaultHeap {
public:
    bool operator()(const T &l, const T &r) const {
        return l < r;
    }
};

template <typename T>
class MinHeap {
public:
    bool operator()(const ArrayIterator<T> &l, const ArrayIterator<T> &r) const {
        return l.buffer[l.cur_iterator] < r.buffer[r.cur_iterator];
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T, class Comparator = DefaultHeap<T>>
class Heap {
    public:
        Heap(size_t _capasity = DEFAULT_CAPACITY);
        ~Heap();
        
        Heap(const T &) = delete;
        Heap &operator=(const Heap &other) = delete;
        T &operator[](size_t index) const;

        void push_back(const T &elem);
        T extract_top();
        T &top() const;

        void insert(const T &new_elem);
        T reduce();
        void grow();

        size_t get_size();
        size_t get_tail();
        bool empty() const;

        void order_heap();
        
    private:
        size_t size;
        size_t capasity;
        T *heap;
        Comparator comparator;

        void sift_down(size_t index);
        void sift_up(size_t index);
};

template <typename T, class Comparator>
Heap<T, Comparator>::Heap(size_t _capasity) : size(0), capasity(_capasity) {
    heap = new T[_capasity];
}

template <typename T, class Comparator>
Heap<T, Comparator>::~Heap() {
    delete[] heap;
}

template <typename T, class Comparator>
T& Heap<T, Comparator>::operator[](size_t index) const {
    return heap[index];
}

template <typename T, class Comparator>
void Heap<T, Comparator>::push_back(const T &elem) {
    if (size == capasity) {
        grow();
    }
    heap[size] = elem;
    ++size;
}

template <typename T, class Comparator>
T Heap<T, Comparator>::extract_top() {
    assert(!empty());

    std::swap(heap[0], heap[get_tail()]);
    T top = reduce();
    sift_down(0);

    return top;
}

template <typename T, class Comparator>
T &Heap<T, Comparator>::top() const {
    return heap[0];
}

template <typename T, class Comparator>
void Heap<T, Comparator>::insert(const T &new_elem) {
    push_back(new_elem);
    sift_up(get_size() - 1);
}

template <typename T, class Comparator>
T Heap<T, Comparator>::reduce() {
    assert(!empty());
    --size;
    return heap[get_tail() + 1];
}

template <typename T, class Comparator> // изменить реализацию
void Heap<T, Comparator>::grow() {
    ++capasity;
    size_t new_capasity = (2 * capasity) - 1;
    T *tmp_heap = new T[new_capasity];

    memcpy(tmp_heap, heap, capasity * sizeof(T));

    capasity = new_capasity;
    delete[] heap;
    heap = tmp_heap;
}

template <typename T, class Comparator>
size_t Heap<T, Comparator>::get_size() {
    return size;
}

template <typename T, class Comparator>
size_t Heap<T, Comparator>::get_tail() {
    return size - 1;
}

template <typename T, class Comparator>
bool Heap<T, Comparator>::empty() const {
    return size == 0;
}

template <typename T, class Comparator>
void Heap<T, Comparator>::order_heap() {
    for (int i = get_size() / 2 - 1; i >= 0; --i) {
        sift_down(i);
    }
}

template <typename T, class Comparator>
void Heap<T, Comparator>::sift_down(size_t index) {
    size_t largest = index;

    size_t left_child = 2 * index + 1;
    size_t right_child = 2 * index + 2;

    if (left_child < get_size() && !(comparator(heap[largest], heap[left_child]))) {
        largest = left_child;
    }

    if (right_child < get_size() && !(comparator(heap[largest], heap[right_child]))) {
        largest = right_child;
    }

    if (largest != index) {
        std::swap(heap[index], heap[largest]);
        sift_down(largest);
    }
}

template <typename T, class Comparator>
void Heap<T, Comparator>::sift_up(size_t index) {
    while (index > 0) {
        size_t parent = (index - 1) / 2;
        if (comparator(heap[parent], heap[index])) {
            return;
        }
        std::swap(heap[index], heap[parent]);
        index = parent;
    }
}

void print_result(int *result, int sum_size) {
    for (int i = 0; i < sum_size; ++i) {
        std::cout << result[i] << " ";
    }
}

void mergeArrays(Heap<ArrayIterator<int>, MinHeap<int>> &heap, int *result, int size) {
    for (int i = 0; i < size; ++i) {
        auto temp_buffer = heap.extract_top();
        result[i] = temp_buffer.buffer[temp_buffer.cur_iterator];
        ++temp_buffer.cur_iterator;

        if (temp_buffer.cur_iterator < temp_buffer.size) {
            heap.insert(temp_buffer);
        }
    }
}


int main(int argc, char* argv[]) {
    int heap_size;
    std::cin >> heap_size;
    assert(heap_size > 0);

    Heap<ArrayIterator<int>, MinHeap<int>> heap;
    int sum_size = 0;
    for (int i = 0; i < heap_size; ++i) {
        int tmp_size;
        std::cin >> tmp_size;
        sum_size += tmp_size;

        ArrayIterator<int> array;
        array.cur_iterator = 0;
        array.size = 0;
        array.buffer = new int[tmp_size];
        
        for (int j = 0; j < tmp_size; ++j) {
            int tmp;
            std::cin >> tmp;
            array.push_back(tmp);
        }

        heap.push_back(array);
    }

    heap.order_heap();

    int result[sum_size];
    mergeArrays(heap, result, sum_size);

    for (int i = 0; i < heap_size; ++i) {
        delete[] heap[i].buffer;
    }

    print_result(result, sum_size);

    return 0;
}
