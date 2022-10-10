#include "../ArrayIterator/arrayIterator.hpp"
#include "../Vector/myVector.hpp"

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

    void buildHeap();

private:
    Buffer<T> buffer;
    Comparator comparator;

    void siftUp(size_t parrentIndex);
    void siftDown(size_t parrentIndex);
};