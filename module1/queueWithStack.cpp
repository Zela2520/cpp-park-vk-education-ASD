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

#define DEFAULT_ARRAY_SIZE 10

// 1) Реализовать динамический буффер +
// 2) Реализовать стек на динамическом буффере
// 3) Реализовать очередь с помощью двух стеков


template <typename T>
class Buffer {
    public:
        explicit Buffer(size_t _size);
        explicit Buffer(const Buffer& other);
        Buffer<T>& operator=(const Buffer<T>& other);
        T& operator[](size_t index) {return buffer[index];}
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

template <typename T>
class Stack {
public:
        Stack(size_t size);
        ~Stack();
        void push(T value);
        T pop();
        bool isEmpty() const {return m_top == -1;}

        Buffer<T> m_buffer;
private:
        T m_top;
};

template <typename T>
Stack<T>::Stack(size_t size) : m_buffer(size), m_top(-1)
{
}

template <typename T>
Stack<T>::~Stack()
{
}

template <typename T>
void Stack<T>::push(T value) {
//    assert(m_top + 1 < m_buffer.getCapacity());
    m_buffer[++m_top] = value;
}

template <typename T>
T Stack<T>::pop() {
    assert(m_top != -1);
    return m_buffer[m_top--];
}

template <typename T>
class Queue {
public:
    Queue(size_t size);
    ~Queue();
    void enqueue(T value);
    T dequeue();
    bool isEmpty() const {
        if (m_stack1.isEmpty() && m_stack2.isEmpty()) {
            return true;
        }
        return false;
    }

private:
    Stack<T> m_stack1;
    Stack<T> m_stack2;
};

template <typename T>
Queue<T>::Queue(size_t size) : m_stack1(size), m_stack2(size)
{
}

template <typename T>
Queue<T>::~Queue()
{
}

template <typename T>
void Queue<T>::enqueue(T value) {
    m_stack1.push(value);
}

template <typename T>
T Queue<T>::dequeue() {
    if (m_stack2.isEmpty()) {
        while (!(m_stack1.isEmpty())) {
            m_stack2.push(m_stack1.pop());
        }
    }
    return m_stack2.pop();
}

void bufferTestCase() {
    std::cout << "**********Buffer**********"<< std::endl;
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

void stackTestCase() {
    std::cout << "**********Stack**********"<< std::endl;
    std::cout << "Push method test: ";
    {
        Stack<int> stack(3);
        stack.push(0);
        stack.push(1);
        stack.push(2);
        for(size_t i = 0; i < stack.m_buffer.getSize(); ++i) {
            assert(stack.m_buffer.getAt(i) == i);
        }
        assert(stack.isEmpty() == false);
    }
    std::cout << "OK\n";
    std::cout << "isEmpty method test: ";
    {
        Stack<int> stack(3);
        assert(stack.isEmpty() == true);
    }
    std::cout << "OK\n";
    std::cout << "Pop method test: ";
    {
        Stack<int> stack(3);
        stack.push(0);
        stack.push(1);
        stack.push(2);
        assert(stack.pop() == 2);
        assert(stack.pop() == 1);
        assert(stack.pop() == 0);
        assert(stack.isEmpty() == true);
    }
    std::cout << "OK\n";
}

void queueTestCase() {
    std::cout << "**********Queue**********"<< std::endl;
    std::cout << "Base queue test: ";
    {
        Queue<int> queue(3);
        queue.enqueue(1);
        queue.enqueue(2);
        queue.enqueue(3);

        assert(queue.dequeue() == 1);
        assert(queue.dequeue() == 2);
        assert(queue.dequeue() == 3);

        assert(queue.isEmpty() == true);
    }
    std::cout << "OK\n";
}

int main(int argc, char* argv[]) {
//    bufferTestCase();
//    stackTestCase();
//    queueTestCase();
    size_t capacity = 0;
    std::cin >> capacity;
    assert(capacity != 0);


    return 0;
}
