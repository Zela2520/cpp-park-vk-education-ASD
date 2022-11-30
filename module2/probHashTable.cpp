// Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из строчных латинских букв.
// Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера. Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
// Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству.

// Вариант 1. Для разрешения коллизий используйте квадратичное пробирование.
// i-ая проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.
// Вариант 2. Для разрешения коллизий используйте двойное хеширование.

// Формат ввода
// Каждая строка входных данных задает одну операцию над множеством. Запись операции состоит из типа операции и следующей за ним через пробел строки, над которой проводится операция. Тип операции – один из трех символов: + означает добавление данной строки в множество; - означает удаление строки из множества; ? означает проверку принадлежности данной строки множеству. При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует в этом множестве. При удалении элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он присутствует в этом множестве.

// Формат вывода
// Программа должна вывести для каждой операции одну из двух строк OK или FAIL.

// Для операции '?': OK, если элемент присутствует во множестве. FAIL иначе.

// Для операции '+': FAIL, если добавляемый элемент уже присутствует во множестве и потому не может быть добавлен. OK иначе.

// Для операции '-': OK, если элемент присутствовал во множестве и успешно удален. FAIL иначе.

// Пример
// Ввод	Вывод
// + hello
// + bye
// ? bye
// + bye
// - bye
// ? bye
// ? hello
// OK
// OK
// OK
// FAIL
// OK
// FAIL
// OK


#include <cassert>
#include <iostream>
#include <string>
#include <vector>


class Prob {
    public:
        int operator()(const int hash, int step) const {
            return (hash + step * step / 2 + step / 2);
        }
};

class Hash {
    public:
        long long unsigned int operator()(const std::string& value) const {
            long long unsigned int hash = 0;
            for (const auto& elem : value) {
                hash = hash * 127 + elem;
            }
            return hash;
        }
};

template<class T>
class Node {
    public:
        T value;
        explicit Node(T _value) : value(_value) {};
        Node() : value("0") {};
};

template <class T, class Hasher, class Probing>
class HashTable {
public:
    explicit HashTable(const Hasher& _hasher, const Probing& _probing) : m_hash(_hasher), m_prob(_probing), m_table(8), m_counter(0) {}
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;
	~HashTable() = default;

    bool Add(const T& value) {
        if (m_counter >= 3 * m_table.capacity() / 4) {
        updateHash();
    }
    
    int hash = m_hash(value);
    int index = hash % m_table.capacity();
    T curNode = m_table[index].value;

    for (size_t i = 0; i < m_table.capacity(); ++i ) {
        if (curNode == "0") break;

        if (curNode == value && curNode != "Not exist") {
            return false;
        }

        index = m_prob(hash, i + 2) % m_table.capacity();
        curNode = m_table[index].value;
    }

    m_table[index].value = value;
    ++m_counter;
    return true;
    }

    bool Delete(const T& value) {
        int hash = m_hash(value);
    int index = hash % m_table.capacity();
    T curNode = m_table[index].value;

    for ( size_t i = 0; i < m_table.capacity(); ++i ) {
        if (curNode == "0") return false;

        if (curNode == value && curNode != "Not exist") break;

        index = m_prob(hash, i + 2) % m_table.capacity();
        curNode = m_table[index].value;
    }

    m_table[index].value = "Not exist";
    --m_counter;
    return true;
    }

    bool Find(const T& value) {
        int hash = m_hash(value);
        int index = hash % m_table.capacity();

        T curNode = m_table[index].value;
        for (size_t i = 0; i < m_table.capacity(); ++i ) {
            if (curNode == "0") return false;
            if (curNode == value && curNode != "Not exist") return true;

            index = m_prob(hash, i + 2) % m_table.capacity();
            curNode = m_table[index].value;
        }
    }

private:
    std::vector<Node<T>> m_table;
    Probing m_prob;
    Hasher m_hash;
    int m_counter;

    // helper function
    void updateHash() {
        std::vector<Node<T>> otherTable(m_table.capacity() * 2);

    for (size_t i = 0; i < m_table.capacity(); ++i ) {
        T curNode = m_table[i].value;
        if (curNode != "0" && curNode != "Not exist") {
            int newHash = m_hash(curNode);
            int newIndex = newHash % otherTable.capacity();

            T newNode = otherTable[newIndex].value;
            int new_step = 2;

            while (newNode != "0" && newNode != curNode) {
                newIndex = m_prob(newHash, new_step++) % otherTable.capacity();
                newNode = otherTable[newIndex].value;
            }

            otherTable[newIndex].value = curNode;
            m_table[i].value = "0";
        }
    }

    m_table = std::move(otherTable);
    }
};

void run(std::istream &input, std::ostream &output) {
    Hash hasher;
    Prob prober;
    HashTable<std::string, Hash, Prob> m_table(hasher, prober);

    char operation;
	std::string word;
    
	while (input >> operation >> word) {
        switch (operation)
        {
            case '+': 
            {
                output << (m_table.Add(word) ? "OK" : "FAIL") << std::endl;
                break;
            }
            case '?':
            {
                output << (m_table.Find(word) ? "OK" : "FAIL") << std::endl;
                break;
            }
            case '-':
            {
                output << (m_table.Delete(word) ? "OK" : "FAIL") << std::endl;
                break;
            }
        }
	}
}

int main() {
   run(std::cin, std::cout);
   return 0;
}
