#include "arrayIterator.hpp"

template <typename T>
ArrayIterator<T>& ArrayIterator<T>::operator=(const ArrayIterator& other) {
        this->size = other.size;
        this->capacity = other.capacity;
        if (this->buffer) {
            delete[] this->buffer;
        }

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