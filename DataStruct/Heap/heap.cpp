#include <heap.hpp>

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
void Heap<T, Comparator>::buildHeap() {}