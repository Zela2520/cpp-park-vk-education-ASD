#include "myVector.hpp"

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
}

template <typename T>
Buffer<T>& Buffer<T>::operator=(const Buffer<T>& other) {
    this->size = other.size;
    this->capacity = other.capacity;
    this->curPosition = other.curPosition;
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
Buffer<T>::~Buffer()
{
    assert(this->buffer);
    delete[] this->buffer;
}

template <typename T>
T Buffer<T>::getAt(T index) const {
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