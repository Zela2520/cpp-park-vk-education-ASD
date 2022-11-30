#include <iostream>
#include <vector>
#include <string>

const size_t DEFAULT_CAPACITY = 8;
const size_t MAX_ALPHA = 2;


class StringHasher {
    public:
        StringHasher(unsigned int prime = 71) : m_prime(prime) {}
        
        // Метод Горнера
        size_t operator()(const std::string &str) {
            size_t hash = 0;
            for (int i = 0; i < str.size(); ++i) {
                hash = hash * m_prime + str[i];
            }

            return hash; 
        } 
    private:
        unsigned int m_prime;
};


template <typename T>
struct HashNode {
    HashNode() : next(nullptr) {}  
    HashNode(const T& key, HashNode<T>* _next) : data(key), next(_next) {} 
    T data; 
    HashNode<T>* next;
};


template <typename T, typename Hasher>
class HashTable {
public:
    HashTable(size_t initialCapacity = DEFAULT_CAPACITY) :  m_table(initialCapacity, nullptr), m_size(0) {} 
    ~HashTable() {
        for (size_t i = 0; i < m_table.size(); ++i) {
            HashNode<T>* node = m_table[i];

            while (node) {
                HashNode<T>* next = node->next;
                delete node;
                node = next;
            }
        }
    }

    bool Add(const T& key) {
        
        if (!m_size || (m_size > m_table.size() * MAX_ALPHA) ) {
            grow();
        }

        size_t hash = m_hasher(key) % m_table.size();

        HashNode<T>* node = m_table[hash];

        while(node) {
            if (node->data == key) {
                return false;
            }
            node = node->next;
        }

        m_table[hash] = new HashNode<T>(key, m_table[hash]);
        ++m_size;

        return true;
        
    }

    bool Delete(const T& key) {
        size_t hash = m_hasher(key) % m_table.size();

        HashNode<T>* node = m_table[hash];
        HashNode<T>* prev = nullptr;

        while(node) {
            if (node->data == key) {
                break;
            }

            prev = node;
            node = node->next;
        }

        if (!node) {
            return false;
        }

        if (!prev) {
            m_table[hash] = node->next;
        } else {
            prev->next = node->next;
        }

        delete node;
        --m_size;
        return true;
    }

    bool Has(const T& key) {
        size_t hash = m_hasher(key) % m_table.size();
        HashNode<T>* node = m_table[hash];

        while (node) {
            if (node->data == key) {
                return true;
            }

            node = node->next;
        }

        return false;
    }
private:
    std::vector<HashNode<T>*> m_table;
    size_t m_size;
    Hasher m_hasher;

    void grow() {
        std::vector<HashNode<T>*> newTable(m_table.size() * 2, nullptr);

        for (size_t i = 0; i < m_table.size(); ++i) {
            // запоминаем головву
            HashNode<T>* node = m_table[i];

            while (node) {
                HashNode<T>* next = node->next; // запоминаем next у головы, так как он обновится, а нам надо будет перенести следующий узел
                size_t newHash = m_hasher(node->data) % newTable.size(); // вычисляем новый хеш
                node->next = newTable[newHash]; // говорим, что next у головы будет указывать на ячейку в новой таблице
                newTable[newHash] = node; // переносим голову из одной таблицы в другую
                node = next; // переходим к следующему узлу по данному хешу в старой таблице  
            }
        } 

        // переносим newTable в Table
        m_table = std::move(newTable);
    }
};


int main() {
    HashTable<std::string, StringHasher> hashtable;

    char op;
    std::string key;

    while (std::cin >> op >> key) {
        switch (op) {
            case '+': 
            {
                std::cout << (hashtable.Add(key) ? "OK" : "FAIL") << std::endl;
                break;
            }
            case '-':
            {
                std::cout << (hashtable.Delete(key) ? "OK" : "FAIL") << std::endl;
                break;
            }

            case '?':
            {
                std::cout << (hashtable.Has(key) ? "OK" : "FAIL") << std::endl;
                break;
            }
        }
    }

    return 0;
}