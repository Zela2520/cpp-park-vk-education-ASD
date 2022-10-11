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
//Затем по очереди размер каждого массива и элементы массива.
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

///////////////////////////NOT__DIN__BUFF//////////////////////////////// 

template <typename T>
struct ArrayIterator {
    T *buffer;
    size_t size;
    size_t capacity;
    size_t curPosition;

    ArrayIterator() : size(0), capacity(0), curPosition(0) {
        this->buffer = nullptr;
    }
    ArrayIterator(size_t _size) : size(0), capacity(_size), curPosition(0) {
        buffer = new T[capacity];
    }
    ~ArrayIterator() {
        if (buffer) {
            delete[] buffer;
            buffer = nullptr;
        }
    }
    ArrayIterator<T>& operator=(const ArrayIterator& other) {
        this->size = other.size;
        this->capacity = other.capacity;
        // if (this->buffer) {
        //     delete[] this->buffer;
        // }

        this->buffer = new T[this->capacity];

        for (size_t i = 0; i < this->size; ++i) {
            this->buffer[i] = other.buffer[i];
        }

        for (size_t i = this->size; i < this->capacity; ++i) {
            this->buffer[i] = 0;
        }

        return *this;
    }
    T& operator[](const size_t i) {return this->buffer[i];}

    void pushBack(const T &value);
    T getAt(size_t index) const;
};

template <typename T>
void ArrayIterator<T>::pushBack(const T &value) {
    assert(this->size != this->capacity);
    buffer[size++] = value;
}

template <typename T>
T ArrayIterator<T>::getAt(size_t index) const {
    assert(this->buffer);
    return this->buffer[index];
}

template <typename T>
class ArrayComparator {
    public:
        bool operator()(const ArrayIterator<T>& child, const ArrayIterator<T>& parrent) const {
            return child.buffer[child.curPosition] < parrent.buffer[parrent.curPosition];
        }
};

























///////////////////////////DIN__BUFFER////////////////////////////////

template <typename T>
class Buffer {
    public:
        Buffer();
        explicit Buffer(size_t _capacity);
        Buffer(const Buffer& other);
        Buffer<T>& operator=(const Buffer<T>& other);
        Buffer<T>& operator=(const int& value) {this->buffer[this->size] = value; return *this;};
        T& operator[](size_t index) const {return buffer[index];}
        ~Buffer();
        T getAt(size_t index) const;
        void grow();
        void add(T data);
        size_t getSize();
        void setSize(size_t _size) {size = _size;}
        size_t decreaseSize() {return --this->size;}
        size_t getCapacity();
        size_t getCurPosition() const {return curPosition;}
        void increaseCurPositioin() {++this->curPosition;}
        T& getCurPosElement() const {return this->buffer[this->curPosition];}
    private:
        size_t size;
        size_t capacity;
        size_t curPosition;
        T* buffer;
};

template <typename T>
Buffer<T>::Buffer() : size(0), capacity(DEFAULT_BUFFER_SIZE), curPosition(0) {
    this->buffer = new T[this->capacity];
    for (size_t i = 0; i < this->capacity; ++i) {
        this->buffer[i] = 0;
    }
}

template <typename T>
Buffer<T>::Buffer(size_t _capacity) {
    this->size = 0;
    this->capacity = _capacity;
    this->curPosition = 0;
    this->buffer = new T[this->capacity];
}

template <typename T>
Buffer<T>::Buffer(const Buffer<T>& other) {
    this->size = other.size;
    this->capacity = other.capacity;
    this->curPosition = other.curPosition;
    // if (this->buffer) {
    //     delete[] this->buffer;
    // }

    this->buffer = new T[this->capacity];

    for (size_t i = 0; i < this->size; ++i) {
        this->buffer[i] = other.buffer[i];
    }

    for (size_t i = this->size; i < this->capacity; ++i) {
        this->buffer[i] = 0;
    }
}

template <typename T>
Buffer<T>& Buffer<T>::operator=(const Buffer<T>& other) {
    this->size = other.size;
    this->capacity = other.capacity;
    this->curPosition = other.curPosition;
    // if (this->buffer) {
    //     delete[] this->buffer;
    // }

    this->buffer = new T[this->capacity];

    for (size_t i = 0; i < this->size; ++i) {
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
T Buffer<T>::getAt(size_t index) const {
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

template <typename T>
class BufferComparator {
    public:
        bool operator()(const Buffer<T>& child, const Buffer<T>& parrent) const {
            return child.getAt(child.getCurPosition()) < parrent.getAt(parrent.getCurPosition());
        }
};




















///////////////////////////HEAP//////////////////////////////// 

template <typename T, class Comparator = BufferComparator<T>>
class Heap {
public:
    explicit Heap(size_t _size);
    Heap(const Heap& other) = delete;
    Heap<T>& operator=(const Heap<T>& other) = delete;
    // int& operator[](size_t index) const {return this->buffer[index].getAt(this->buffer.getCurPosition());}
    T& operator[](size_t index) const {return this->buffer[index];}
    ~Heap();

    void pushBack(const T &value);
    T removeMin();
    bool empty() const {return this->buffer.getSize() == 0;}

private:
    Buffer<T> buffer;
    Comparator comparator;

    void siftUp(size_t parrentIndex);
    void siftDown(size_t parrentIndex);
};

template <typename T, class Comparator>
Heap<T, Comparator>::Heap(size_t _size) : buffer(_size) {}

template <typename T, class Comparator>
Heap<T, Comparator>::~Heap() {}


template <typename T, class Comparator>
void Heap<T, Comparator>::pushBack(const T &value) {
    this->buffer.add(value);
    siftUp(this->buffer.getSize() - 1);
}

template <typename T, class Comparator>
void Heap<T, Comparator>::siftUp(size_t childIndex)  {
    while (childIndex > 0) {

        size_t parent = (childIndex - 1) / 2;
        if (this->comparator(this->buffer[parent], this->buffer[childIndex])) {
            return;
        }

        std::swap(this->buffer[childIndex], this->buffer[parent]);
        childIndex = parent;
    }
}

template <typename T, class Comparator>
T Heap<T, Comparator>::removeMin() {
    assert(this->buffer.getSize() != 0);
    T top = this->buffer[0];
    this->buffer[0] = this->buffer[this->buffer.decreaseSize()];
    siftDown(0);
    return top;    
}

template <typename T, class Comparator>
void Heap<T, Comparator>::siftDown(size_t parrentIndex) {
    size_t curParrentIndex = parrentIndex;
    while (true) {
        size_t maxNodeIndex = curParrentIndex;

        for (size_t i = 1; i < 3; ++i) {
            if (2 * curParrentIndex + i < this->buffer.getSize() && (this->comparator(this->buffer[2 * curParrentIndex + i], this->buffer[maxNodeIndex]))) {
                maxNodeIndex = 2 * curParrentIndex + i;
            }
        }

        if (maxNodeIndex == curParrentIndex) break;
        std::swap(this->buffer[curParrentIndex], this->buffer[maxNodeIndex]);

        curParrentIndex = maxNodeIndex;
    }
}

///////////////////////////MAIN//////////////////////////////// 
void printResult(int* result, int sumSize) {
    for (int i = 0; i < sumSize; ++i) {
        std::cout << result[i] << " ";
    }
}

void fillHeap(std::istream& input, Heap<Buffer<int>, BufferComparator<int>> &heap, size_t heapSize, size_t &sumHeapSize) {
    for (size_t i = 0; i < heapSize; ++i) {
        size_t curArraySize = 0;
        input >> curArraySize;
        sumHeapSize += curArraySize;

        Buffer<int> buffer(curArraySize);
        for (size_t i = 0; i < curArraySize; ++i) {
            int tmp;
            input >> tmp;
            buffer.add(tmp);
        }

        heap.pushBack(buffer);
    }
}

void mergeArrays(Heap<Buffer<int>, BufferComparator<int>>& heap, int* mergeSortArray, size_t arraySize) {
    for (size_t i = 0; i < arraySize; ++i) {
        Buffer<int> curItemOfMergeSortArray = heap.removeMin();
        mergeSortArray[i] = curItemOfMergeSortArray[curItemOfMergeSortArray.getCurPosition()];
        curItemOfMergeSortArray.increaseCurPositioin();
    }
}

void run(std::istream& input, std::ostream& output) {
    size_t heapSize;
    input >> heapSize;
    assert(heapSize > 0);

    Heap<Buffer<int>, BufferComparator<int>> heap(heapSize);
    size_t sumHeapSize = 0;

    fillHeap(input, heap, heapSize, sumHeapSize);

    int outputArray[sumHeapSize];

    mergeArrays(heap, outputArray, sumHeapSize);

    printResult(outputArray, sumHeapSize);
}

















///////////////////////////TEST//////////////////////////////// 

void bufferTestCase() {
    std::cout << "**********Buffer**********"<< std::endl;
    std::cout << "Single element adding test: ";
    {
        Buffer<int> m_buffer(3);
        m_buffer.add(1);
        m_buffer.add(8);
        assert(m_buffer.getAt(0) == 1);
        assert(m_buffer.getAt(1) == 8);
    }
    std::cout << "OK\n";

    std::cout << "Add method test: ";
    {
        Buffer<int> m_buffer(3);
        for (size_t i = 0; i < m_buffer.getCapacity(); ++i) {
            m_buffer.add(static_cast<int>(i));
        }
        assert(m_buffer.getSize() == 3);
    }
    std::cout << "OK\n";

    std::cout << "Grow and getAt methods tests: ";
    {
        Buffer<int> m_buffer(1);
        for (int i = 0; i < 1000; ++i) {
            m_buffer.add(i);
        }
        for(size_t i = 0; i < m_buffer.getSize(); ++i) {
            assert(m_buffer.getAt(static_cast<int>(i)) == static_cast<int>(i));
        }
    }
    std::cout << "OK\n";
    std::cout << "Comparator test: ";
    {
        BufferComparator<int> cmp;
        Buffer<int> first(3);
        Buffer<int> second(3);
        for (size_t i = 0; i < 6; ++i) {
            if (i < 3) {
                first.add(i);
            } else {
                second.add(i);
            }
        }

        for (size_t i = 0; i < first.getSize(); ++i) {
            assert(cmp(first, second) == true);
            first.increaseCurPositioin();
            second.increaseCurPositioin();
        }
    }
    std::cout << "OK\n";
}

void testArrayIteretor() {
    std::cout << "**********ArrayIterator Tests***********" << std::endl;
    std::cout << "PushBack method test: ";
    {
        ArrayIterator<int> arr(3);
        for(size_t i = 0; i < 3; ++i) {
            arr.pushBack(i);
        }

        for (size_t i = 0; i < 3; ++i) {
            assert(arr[i] == static_cast<int>(i));
        }
    }
    std::cout << "OK\n";
    std::cout << "Comparator test: ";
    {
        ArrayComparator<int> cmp;
        ArrayIterator<int> first(3);
        ArrayIterator<int> second(3);
        for (size_t i = 0; i < 6; ++i) {
            if (i < 3) {
                first.pushBack(i);
            } else {
                second.pushBack(i);
            }
        }

        for (size_t i = 0; i < first.size; ++i) {
            assert(cmp(first, second) == true);
        }
    }
    std::cout << "OK\n";
    std::cout << "GetAt method test: ";
    {
        ArrayIterator<int> arr(3);
        for(size_t i = 0; i < 3; ++i) {
            arr.pushBack(i);
        }

        for (size_t i = 0; i < 3; ++i) {
            assert(arr.getAt(i) == static_cast<int>(i));
        }
    }
    std::cout << "OK\n";
    std::cout << "Equal operator test: ";
    {
        ArrayIterator<int> first(3);
        ArrayIterator<int> second(3);
        for(size_t i = 0; i < 3; ++i) {
            first.pushBack(i);
        }

        second = first;
        second[1] = 8;
        assert(first.getAt(0) == static_cast<int>(0));
        assert(first.getAt(1) == static_cast<int>(1));
        assert(first.getAt(2) == static_cast<int>(2));
        assert(second.getAt(0) == static_cast<int>(0));
        assert(second.getAt(1) == static_cast<int>(8));
        assert(second.getAt(2) == static_cast<int>(2));
    }
    std::cout << "OK\n";
    std::cout << "*************All tests passed***********\n";
}

void testingHeapBaseStructure() {
    std::cout << "******************Vector vectors component tests******************" << std::endl;
    std::cout << "Buffer consist of ArrayIterator test: ";
    {
        ArrayIterator<int>* array = new ArrayIterator<int>[3]{ArrayIterator<int>(3)};
        ArrayIterator<int> first(3);
        for(size_t i = 0; i < 3; ++i) {
            first.pushBack(i);
        }
        ArrayIterator<int> second(3);
        for(size_t i = 3; i < 6; ++i) {
            second.pushBack(i);
        }
        ArrayIterator<int> third(3);
        for(size_t i = 6; i < 9; ++i) {
            third.pushBack(i);
        }

        array[0] = first;
        array[1] = second;
        array[2] = third;

        for (size_t i = 0; i < 3; ++i) {
            assert(array[0].getAt(i) == static_cast<int>(i));
        }

        delete[] array;
    }
    std::cout << "OK\n";
    std::cout << "Buffer consist of Buffer test: ";
    {
        Buffer<Buffer<int>> array(3);
        assert(array.getCapacity() == 3);

        Buffer<int> first(3);
        for(size_t i = 0; i < 3; ++i) {
            first.add(i);
        }
        Buffer<int> second(3);
        for(size_t i = 3; i < 6; ++i) {
            second.add(i);
        }
        Buffer<int> third(3);
        for(size_t i = 6; i < 9; ++i) {
            third.add(i);
        }

        array[0] = first;
        array[1] = second;
        array[2] = third;

        // Access to array elements
        assert(array[0].getAt(0) == static_cast<int>(0));
        assert(array[0].getAt(1) == static_cast<int>(1));
        assert(array[0].getAt(2) == static_cast<int>(2));

        assert(array[1].getAt(0) == static_cast<int>(3));
        assert(array[1].getAt(1) == static_cast<int>(4));
        assert(array[1].getAt(2) == static_cast<int>(5));

        assert(array[2].getAt(0) == static_cast<int>(6));
        assert(array[2].getAt(1) == static_cast<int>(7));
        assert(array[2].getAt(2) == static_cast<int>(8));

        assert(array[2].getAt(array[2].getCurPosition()) == static_cast<int>(6));
    }
    std::cout << "OK\n";
    std::cout << "*************All tests passed***********\n";
}

void testHeap() {
    std::cout << "******************Heap tests******************" << std::endl;
    std::cout << "Create Heap test: ";
    {
        size_t heapSize = 3;
        Heap<Buffer<int>, BufferComparator<int>> heap(heapSize);
    }
    std::cout << "OK\n";
    std::cout << "PushBack test: ";
    {
        size_t heapSize = 3;
        Heap<Buffer<int>, BufferComparator<int>> heap(heapSize);

        Buffer<int> buffer1(5);
        Buffer<int> buffer2(3);
        Buffer<int> buffer3(4);

        buffer1.add(4);
        buffer1.add(2);
        buffer1.add(3);
        buffer2.add(1);
        buffer2.add(5);
        buffer2.add(6);
        buffer3.add(7);
        buffer3.add(8);
        buffer3.add(9);

        heap.pushBack(buffer1);
        heap.pushBack(buffer2);
        heap.pushBack(buffer3);

        assert(heap[0].getAt(0) == 1);
        assert(heap[0].getAt(1) == 5);
        assert(heap[0].getAt(2) == 6);

        assert(heap[1].getAt(0) == 4);
        assert(heap[1].getAt(1) == 2);
        assert(heap[1].getAt(2) == 3);

        assert(heap[2].getAt(0) == 7);
        assert(heap[2].getAt(1) == 8);
        assert(heap[2].getAt(2) == 9);

        // GetCurPosition function test
        assert(heap[0].getCurPosElement() == 1);
        heap[0].increaseCurPositioin();
        assert(heap[0].getCurPosElement() == 5);
        heap[0].increaseCurPositioin();
        assert(heap[0].getCurPosElement() == 6);

        assert(heap[1].getCurPosElement() == 4);
        heap[1].increaseCurPositioin();
        assert(heap[1].getCurPosElement() == 2);
        heap[1].increaseCurPositioin();
        assert(heap[1].getCurPosElement() == 3);


        assert(heap[2].getCurPosElement() == 7);
        heap[2].increaseCurPositioin();
        assert(heap[2].getCurPosElement() == 8);
        heap[2].increaseCurPositioin();
        assert(heap[2].getCurPosElement() == 9);
    }
    std::cout << "OK\n";
    std::cout << "PushBack testCase from contest: ";
    {
        size_t heapSize = 3;
        Heap<Buffer<int>, BufferComparator<int>> heap(heapSize);

        Buffer<int> buffer1(1);
        Buffer<int> buffer2(2);
        Buffer<int> buffer3(3);
        Buffer<int> buffer4(1);

        buffer1.add(6);
        buffer2.add(50);
        buffer2.add(90);
        buffer3.add(1);
        buffer3.add(10);
        buffer3.add(70);
        buffer4.add(7);

        heap.pushBack(buffer1);
        heap.pushBack(buffer2);
        heap.pushBack(buffer3);
        heap.pushBack(buffer4);

        assert(heap[0].getAt(0) == 1);
        assert(heap[1].getAt(0) == 7);
        assert(heap[2].getAt(0) == 6);
        assert(heap[3].getAt(0) == 50);
    }
    std::cout << "OK\n";
    std::cout << "removeMin testCase from contest: ";
    {
        size_t heapSize = 3;
        Heap<Buffer<int>, BufferComparator<int>> heap(heapSize);

        Buffer<int> buffer1(1);
        Buffer<int> buffer2(2);
        Buffer<int> buffer3(3);
        Buffer<int> buffer4(1);

        buffer1.add(6);
        buffer2.add(50);
        buffer2.add(90);
        buffer3.add(1);
        buffer3.add(10);
        buffer3.add(70);
        buffer4.add(7);

        heap.pushBack(buffer1);
        heap.pushBack(buffer2);
        heap.pushBack(buffer3);
        heap.pushBack(buffer4);

        assert(heap.removeMin().getCurPosElement() == 1);
        assert(heap.removeMin().getCurPosElement() == 6);
        assert(heap.removeMin().getCurPosElement() == 7);
        assert(heap.removeMin().getCurPosElement() == 50);
    }
    std::cout << "OK\n";
    std::cout << "*************All tests passed***********\n";
}

int main() {
    // bufferTestCase();
    // testArrayIteretor();
    // testingHeapBaseStructure();
    testHeap();
    // run(std::cin, std::cout);
    return 0;
}
