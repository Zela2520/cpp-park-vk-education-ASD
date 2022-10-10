#include <cassert>
#include <cstdlib>
#include <cstring>

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
    ArrayIterator<T>& operator=(const ArrayIterator& other);
    T& operator[](const size_t i) {return this->buffer[i];}

    void pushBack(const T &value);
    T getAt(size_t index) const;
};
