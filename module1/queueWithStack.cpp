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
#include <sstream>

#define DEFAULT_QUEUE_SIZE 10

template <typename T>
class Buffer {
    public:
        explicit Buffer(size_t _capacity);
        Buffer(const Buffer& other) = delete;
        Buffer<T>& operator=(const Buffer<T>& other) = delete;
        T& operator[](size_t index) {return buffer[index];}
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
Buffer<T>::~Buffer()
{
    assert(this->buffer);
    delete[] this->buffer;
}

template <typename T>
T Buffer<T>::getAt(T index) {
//    assert(index >= 0 && index < this->size && this->buffer);
    assert(index >= 0 && this->buffer); // для тестов
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
    if (this->size == this->capacity) { // this->size + 1 ?
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

//////////////////////////////////Stack////////////////////////////////////

template <typename T>
class Stack {
public:
        Stack(size_t size);
        ~Stack();
        void push(T value);
        T pop();
        bool isEmpty() const {return m_top == -1;}
        Buffer<T> m_buffer; // public для тестов
private:
        T m_top;
};

// для тестов
template <typename T>
Stack<T>::Stack(size_t size) : m_buffer(size), m_top(-1)
{
}

//template <typename T>
//Stack<T>::Stack(size_t size) : m_top(-1), m_buffer(size)
//{
//}

template <typename T>
Stack<T>::~Stack()
{
}

// неправильно
template <typename T>
void Stack<T>::push(T value) {
    if (m_top + 1 == static_cast<int>(m_buffer.getCapacity())) {
        m_buffer.grow();
    }
    m_buffer[++m_top] = value;
}

template <typename T>
T Stack<T>::pop() {
    assert(m_top != -1);
    return m_buffer[m_top--];
}

//////////////////////////////////Queue////////////////////////////////////
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
    if (this->isEmpty()) {
        return -1;
    }

    if (m_stack2.isEmpty()) {
        while (!(m_stack1.isEmpty())) {
            m_stack2.push(m_stack1.pop());
        }
    }
    return m_stack2.pop();
}

void run(std::istream& input, std::ostream& output) {
    int numberOfOperation = 0;
    input >> numberOfOperation;

    Queue<int> queue(DEFAULT_QUEUE_SIZE);
    bool correct = true;

    for (int i = 0; i < numberOfOperation; ++i) {
        int operation = 0, value = 0;
        input >> operation >> value;
        switch (operation) {
            case 2: {
                // dequeue
                if (queue.dequeue() != value) {
                    correct = false;
                }
                break;
            }
            case 3: {
                // enqueue
                queue.enqueue(value);
                break;
            }
        }
    }

//   correct ? (output << "YES" << std::endl) : output << "NO" << std::endl;
    if (correct) {
        output << "YES" << std::endl;
    } else {
        output << "NO" << std::endl;
    }
}

void testLogic() {
    std::cout << "***********TEST LOGIC: ";
    {
        std::stringstream sstr_input;
        sstr_input << "3" << std::endl;
        sstr_input << "3 44" << std::endl;
        sstr_input << "3 50" << std::endl;
        sstr_input << "2 44" << std::endl;
        std::stringstream sstr_output;

        run(sstr_input, sstr_output);
        assert(sstr_output.str() == "YES\n");
    }
    {
        std::stringstream sstr_input;
        sstr_input << "2" << std::endl;
        sstr_input << "2 -1" << std::endl;
        sstr_input << "3 10" << std::endl;
        std::stringstream sstr_output;

        run(sstr_input, sstr_output);
        assert(sstr_output.str() == "YES\n");
    }
    {
        std::stringstream sstr_input;
        sstr_input << "2" << std::endl;
        sstr_input << "3 44" << std::endl;
        sstr_input << "2 66" << std::endl;
        std::stringstream sstr_output;

        run(sstr_input, sstr_output);
        assert(sstr_output.str() == "NO\n");
    }
    std::cout << "OK\n";
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
}

void stackTestCase() {
    std::cout << "**********Stack**********"<< std::endl;
    std::cout << "Push method test: ";
    {
        Stack<int> stack(3);
        stack.push(0);
        stack.push(1);
        stack.push(2);
        stack.push(3);
        stack.push(4);
        for (size_t i = 0; i < 5; ++i) {
            assert(stack.m_buffer.getAt(static_cast<int>(i)) == static_cast<int>(i));
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
        stack.push(3);
        stack.push(4);
        assert(stack.pop() == 4);
        assert(stack.pop() == 3);
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

int main() {
//    bufferTestCase();
//    stackTestCase();
//    queueTestCase();
//    testLogic();
    run(std::cin, std::cout);
    return 0;
}
