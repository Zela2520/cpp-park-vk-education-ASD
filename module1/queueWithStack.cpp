// Реализовать очередь с помощью двух стеков. Использовать стек, реализованный с помощью динамического буфера.

// Обрабатывать команды push back и pop front.

// Формат ввода
// В первой строке количество команд n. n ≤ 1000000.

// Каждая команда задаётся как 2 целых числа: a b.

// a = 2 - pop front
// a = 3 - push back

// Если дана команда pop front, то число b - ожидаемое значение. Если команда pop front вызвана для пустой структуры данных, то ожидается “-1”.

// Формат вывода
// Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.

#include <iostream>
#include <cassert>
#include <cstdlib>


// class Stack() {
// public:
// 	Stack(int size);
// 	~Stack();
	
// 	void push(int value);
// 	int pop();

// 	bool isEmpty() const {return top = -1;}
// private:
// 	int* buffer;
// 	int bufferSize;
// 	int top;
// };

// Stack::Stack(int size) :
// 	bufferSize(size);
// 	top(-1)
// {
//     buffer = new int(bufferSize);
// }

// Stack::~Stack() {
// 	delete[] buffer;
// }

// void Stack::push(int value) {
// 	assert(top + 1 < bufferSize);
// 	buffer[++top] = value;
// }

// int Stack::pop() {
// 	assert(top != -1);
// 	return buffer[top--];
// }


// 1) Реализовать динамический буффер
// 2) Реализовать стек на динамическом буффере
// 3) Реализовать очередь с помощью двух стеков


template <typename T>
class Buffer {
    public:
        explicit Buffer(size_t _size);
        explicit Buffer(const Buffer& other);
        Buffer<T>& operator=(const Buffer<T>& other);
        ~Buffer();
        T getAt(size_t index);
        void grow();
        void add(T data);
        size_t getSize();
        size_t getCapacity();
    private:
        T* buffer;
        size_t size;
        size_t capacity; 
};

template <typename T>
T Buffer<T>::getAt(size_t index) {
    assert(index >= 0 && index < this->size && this->buffer);
    return this->buffer[index];
}

template <typename T>
void Buffer<T>::grow() {
    size_t newCapacity = this->capacity * 2;
    size_t newSize = this->size;
    T* newBuffer = (T*) calloc(newCapacity, sizeof(T));

    for (size_t i = 0; i < this->size; ++i) {
        newBuffer[i] = buffer[i];
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
Buffer<T>::Buffer(size_t _capacity) {
    this->size = 0;
    this->capacity = _capacity;
    this->buffer = new T(this->capacity);
}

template <typename T>
Buffer<T>::Buffer(const Buffer& other) {
    this->size = other.size;
    this->capacity = other.capacity;
    this->buffer = new T(other.capacity);

    for (size_t i = 0; i < other.size; ++i) {
        this->buffer[i] = other.buffer[i];
    }
}

template <typename T>
Buffer<T>& Buffer<T>::operator=(const Buffer<T>& other) {
    this->size = other.size;
    this->capacity = other.capacity;
    if (this->data) {
        delete[] this->data;
    }
    
    this->data = new T(other.capacity);
    for (size_t i = 0; i < other.size; ++i) {
        this->buffer[i] = other->buffer[i];
    }

    return *this;
}

template <typename T>
Buffer<T>::~Buffer()
{
    assert(this->buffer);
    delete[] this->buffer;
}


void bufferTestCase() {
    std::cout << "Single element adding test : ";
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
            m_buffer.add(i);
        }
        assert(m_buffer.getSize() == 3);
    }
    std::cout << "OK\n";

    std::cout << "Grow and getAt methods tests: ";
    {
        Buffer<int> m_buffer(1);
        for (size_t i = 0; i < 20; ++i) {
            m_buffer.add(i);
        }
        for(size_t i = 0; i < m_buffer.getSize(); ++i) {
            assert(m_buffer.getAt(i) == i);
        }
    }
    std::cout << "OK\n";
}

int main(int argc, char* argv[]) {
    bufferTestCase();
//    size_t capacity = 0;
//    std::cin >> capacity;
//    assert(capacity != 0);
    return 0;
}
