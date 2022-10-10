#include <cassert>
#include <cstdlib>
#include <cstring>

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
        T getAt(T index) const;
        void grow();
        void add(T data);
        size_t getSize();
        void setSize(size_t _size) {size = _size;}
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