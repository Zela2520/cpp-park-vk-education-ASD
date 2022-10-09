//Напишите программу, которая использует кучу для слияния K отсортированных массивов суммарной длиной N.
//
//Требования:
//
//Время работы O(N * logK)
//Куча должна быть реализована в виде шаблонного класса.
//Решение должно поддерживать передачу функции сравнения снаружи.
//Куча должна быть динамической.
//Формат ввода
//Сначала вводится количество массивов K.
//. Затем по очереди размер каждого массива и элементы массива.
//Каждый массив упорядочен по возрастанию.
//
//Формат вывода
//Итоговый отсортированный массив.

#include "iostream"
#include <cassert>
#include <cstdlib>
#include <sstream>
#include <cstring>

#define DEFAULT_BUFFER_SIZE 10

// 1) Залить все массивы в один массив
// 2) Для этого массива выполнить сортировку кучей


///////////////////////////DIN_BUFFER//////////////////////////////// 

template <typename T>
class Buffer {
    public:
        explicit Buffer(size_t _capacity);
        Buffer(const Buffer& other);
        Buffer<T>& operator=(const Buffer<T>& other);
        T& operator[](T index) {return buffer[index];}
        ~Buffer();
        T getAt(T index);
        void grow();
        void add(T data);
        size_t getSize();
        void setSize(size_t _size) {size = _size;}
        size_t getCapacity();
    private:
        size_t size;
        size_t capacity;
        T* buffer;
};

template <typename T>
Buffer<T>::Buffer(size_t _capacity) {
    this->size = 0;
    this->capacity = _capacity;
    this->buffer = new T[this->capacity];
    for (size_t i = 0; i < this->capacity; ++i) {
        this->buffer[i] = 0;
    }
}

template <typename T>
Buffer<T>::Buffer(const Buffer<T>& other) {
    this->size = other.size;
    this->capacity = other.capacity;
    this->buffer = new T[this->capacity];
    for(size_t i = 0; i < this->capacity; ++i) {
        this->buffer[i] = 0;
    }
    for(size_t i = 0; i < this->size; ++i) {
        this->buffer[i] = other.buffer[i];
    }
}

template <typename T>
Buffer<T>& Buffer<T>::operator=(const Buffer<T>& other) {
    this->size = other.size;
    this->capacity = other.capacity;
    if (this->buffer) {
        delete[] this->buffer;
    }

    this->buffer = new T[other.capacity];
    for (size_t i = 0; i < other.size; ++i) {
        this->buffer[i] = other.buffer[i];
    }

    for (size_t i = this->size; i < this->capacity; ++i) {
        this->buffer[i] = 0;
    }

    return *this;
}

template <typename T>
Buffer<T>::~Buffer()
{
    assert(this->buffer);
    delete[] this->buffer;
}

template <typename T>
T Buffer<T>::getAt(T index) {
    assert(index >= 0 && index < this->size && this->buffer);
    return this->buffer[index];
}

template <typename T>
void Buffer<T>::grow() {
    size_t newSize = this->capacity;
   size_t newCapacity = this->capacity * 2;

   T* newBuffer = new T[newCapacity];
   for (size_t i = 0; i < newSize; ++i) {
       newBuffer[i] = buffer[i];
   }
   for (size_t i = newSize; i < this->capacity; ++i) {
       newBuffer[i] = 0;
   }

   delete[] this->buffer;
   this->buffer = newBuffer;
   this->capacity = newCapacity;
   this->size = newSize;
}

template <typename T>
void Buffer<T>::add(T data) {
    if (this->size == this->capacity) {
        grow();
    }
    assert(this->size < this->capacity && this->buffer);
    this->buffer[this->size++] = data;
}

template <typename T>
size_t Buffer<T>::getSize() {
    return this->size;
}

template <typename T>
size_t Buffer<T>::getCapacity() {
    return this->capacity;
}

///////////////////////////Comparator//////////////////////////////// 

template <typename T>
class MaxHeap {
    public:
        bool operator()(const T& child, const T& parrent) const {
            return child > parrent;
        }
};

template <typename T>
class MinHeap {
    public:
        bool operator()(const T& child, const T& parrent) const {
            return child < parrent;
        }
};

///////////////////////////HEAP__HELPER//////////////////////////////// 

template <typename T>
class BufferIterator {
    public:
        Buffer<T> buffer;
        size_t curPosition;
        BufferIterator() : buffer(DEFAULT_BUFFER_SIZE), curPosition(0) {}
        BufferIterator(size_t _size) : buffer(_size), curPosition(0) {}
        BufferIterator<T>& operator=(const BufferIterator<T>& other) {
            this->curPosition = other.curPosition;
            this->buffer->size = other.buffer.size;
            this->buffer->capacity = other.buffer.capacity;
            if (this->buffer->buffer) {
                delete[] this->buffer->buffer;
            }

            this->buffer->buffer = new T[other.buffer.capacity];
            for (size_t i = 0; i < other.buffer.size; ++i) {
                this->buffer->buffer[i] = other.buffer.buffer[i];
            }

            for (size_t i = this->size; i < this->capacity; ++i) {
                this->buffer->buffer[i] = 0;
            }

            return *this;
        };
        T& operator[](size_t index) {return buffer.buffer[index];}
        void pushBack(const T& value);
};

template <typename T>
void BufferIterator<T>::pushBack(const T& value) {
    buffer.add(value);
}

template <typename T>
class MaxHeapIterator {
    public:
        bool operator()(const BufferIterator<T>& child, const BufferIterator<T>& parrent) const {
            return child.buffer[child.curPosition] < parrent.buffer[parrent.curPosition];
        }
};

///////////////////////////HEAP//////////////////////////////// 

template <typename T, class Comparator = MaxHeapIterator<T>>
class Heap {
public:
    explicit Heap(size_t _size);
    Heap(const Heap& other) = delete;
    ~Heap();
    Heap<T>& operator=(const Heap<T>& other) = delete;
    T& operator[](T index) const {return this->buffer[index];}

    void pushBack(const T &value);
    T removeMin();

    void buildHeap();
    
private:
    Buffer<T> buffer;
    Comparator comparator;
    size_t m_size;

    void siftUp(size_t parrentIndex);
    void siftDown(size_t parrentIndex);
};

template <typename T, class Comparator>
Heap<T, Comparator>::Heap(size_t _size) : buffer(_size), m_size(_size) {
}

template <typename T, class Comparator>
Heap<T, Comparator>::~Heap() {
}


template <typename T, class Comparator>
void Heap<T, Comparator>::pushBack(const T &value) {
    this->buffer.add(value);
    siftUp(this->buffer.getSize() - 1);
}

template <typename T, class Comparator>
void Heap<T, Comparator>::siftUp(size_t parrentIndex)  {
    size_t curParrentIndex = parrentIndex;
    while (curParrentIndex > 0 && this->comparator(this->buffer[curParrentIndex], this->buffer[(curParrentIndex - 1) / 2])) {
        std::swap(this->buffer[curParrentIndex], this->buffer[(curParrentIndex - 1 / 2)]);
        curParrentIndex = (curParrentIndex - 1) / 2;
    }
}

template <typename T, class Comparator>
T Heap<T, Comparator>::removeMin() {
    assert(buffer.getSize() != 0);

    std::swap(buffer[0], buffer[buffer.getSize() - 1]);
    auto top = buffer[0];
    siftDown(0);

    return top;
}

template <typename T, class Comparator>
void Heap<T, Comparator>::siftDown(size_t parrentIndex) {
    size_t curParrentIndex = parrentIndex;
    while (true) {
        size_t maxNodeIndex = curParrentIndex;

        for (size_t i = 1; i < 3; ++i) {
            if (2 * curParrentIndex + i < this->buffer.getSize() && !(this->comparator(this->buffer[2 * curParrentIndex + i], this->buffer[maxNodeIndex]))) {
                maxNodeIndex = 2 * curParrentIndex + i;
            }
        }

        if (maxNodeIndex == curParrentIndex) break;
        std::swap(this->buffer[curParrentIndex], this->buffer[maxNodeIndex]);

        curParrentIndex = maxNodeIndex;
    }
}

template <typename T, class Comparator>
void Heap<T, Comparator>::buildHeap() {

}
///////////////////////////MAIN//////////////////////////////// 

void printResult(int* result, int sumSize) {
    for (int i = 0; i < sumSize; ++i) {
        std::cout << result[i] << " ";
    }
}

void fillHeap(std::istream& input, Heap<BufferIterator<int>, MaxHeapIterator<int>> &heap, size_t heapSize, size_t &sumHeapSize) {
    for (size_t i = 0; i < heapSize; ++i) {
        size_t curArraySize = 0;
        input >> curArraySize;
        sumHeapSize += curArraySize;

        BufferIterator<int> buffer(curArraySize);
        for (size_t i = 0; i < curArraySize; ++i) {
            int tmp;
            input >> tmp;
            buffer.pushBack(tmp);
        }

        heap.pushBack(buffer);
    }
}

void mergeArrays(Heap<BufferIterator<int>, MaxHeapIterator<int>>& heap, int* mergeSortArray, size_t arraySize) {
    for (size_t i = 0; i < arraySize; ++i) {
        BufferIterator<int> curItemOfMergeSortArray = heap.removeMin();
        mergeSortArray[i] = curItemOfMergeSortArray.buffer[static_cast<size_t>(curItemOfMergeSortArray.curPosition++)];
    }
}

void run(std::istream& input, std::ostream& output) {
    size_t heapSize;
    input >> heapSize;
    assert(heapSize > 0);

    Heap<BufferIterator<int>, MaxHeapIterator<int>> heap(heapSize);
    size_t sumHeapSize = 0;

    fillHeap(input, heap, heapSize, sumHeapSize);

    int outputArray[sumHeapSize];

    mergeArrays(heap, outputArray, sumHeapSize);

    printResult(outputArray, sumHeapSize);
}

int main() {
   run(std::cin, std::cout);
    // Buffer<BufferIterator<int>> buffer(3);
   return 0;
}