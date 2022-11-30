#include <cassert>
#include <iostream>
#include <string>
#include <vector>

class Prob {
    public:
        int operator()(const int hash, int step) const {
            return (hash + step / 2 + step * step / 2);
        }
};

class Hash {
    public:
        long long unsigned int operator()(const std::string& str) {
            long long unsigned int hash = 0;
            for (const auto& elem : str) {
                hash = hash * 127 + elem;
            }
            return hash;
        }
};


template <typename T>
class Node {
    public:
        T value;
        explicit Node(T _value) : value(_value) {};
        Node() : value("0") {};
};

template <typename T, typename Hasher, typename Probing>
class HashTable {
    public:
        explicit HashTable(const Hasher& _hasher, const Probing& _probing) : m_hash(_hasher), m_prob(_probing), m_table(8), m_size(0) {}
        HashTable(const HashTable&) = delete;
        HashTable& operator=(const HashTable&) = delete;
        ~HashTable() = default;

        bool Add(const T& value) {
            if (m_size >= 3 * m_table.capacity() / 4) {
                updateHash();
            }

            int hash = m_hash(value);
            int index = hash % m_table.capacity();
            T curNode = m_table[index].value;

            for (size_t i = 0; i < m_table.capacity(); ++i) {
                if (curNode == "0") break;

                if (curNode == value && curNode != "DEL") {
                    return false;
                }

                index = m_prob(hash, i + 2) % m_table.capacity();
                curNode = m_table[index].value;
            }

            m_table[index].value = value;
            ++m_size;
            return true;
        }

        bool Delete(const T& value) {
            int hash = m_hash(value);
            int index = hash % m_table.capacity();
            T curNode = m_table[index].value;

            for (size_t i = 0; i < m_table.capacity(); ++i) {
                if (curNode == "0") return false;

                if (curNode == value && curNode != "DEL") break;

                index = m_prob(hash, i + 2) % m_table.capacity();
                curNode = m_table[index].value;
            }

            m_table[index].value = "DEL";
            --m_size;
            return true;
        }

        bool Find(const T& value) {
            int hash = m_hash(value);
            int index = hash % m_table.capacity();

            T curNode = m_table[index].value;
            for (size_t i = 0; i < m_table.capacity(); ++i) {
                if (curNode == "0") return false;
                if (curNode == value && curNode != "DEL") return true;

                index = m_prob(hash, i + 2) % m_table.capacity();
                curNode = m_table[index].value;
            }
        }

    private:
        std::vector<Node<T>> m_table;
        Probing m_prob;
        Hasher m_hash;
        int m_size;

        void updateHash() {
            std::vector<Node<T>> newTable(m_table.capacity() * 2);

            for (size_t i = 0; i < m_table.capacity(); ++i) {
                T curNode = m_table[i].value;
                if (curNode != "0" && curNode != "DEL") {
                    int newHash = m_hash(curNode);
                    int newIndex = newHash % newTable.capacity();

                    T newNode = newTable[newIndex].value;
                    int new_step = 2;

                    while (newNode != "0" && newNode != curNode) {
                        newIndex = m_prob(newHash, new_step++) % newTable.capacity();
                        newNode = newTable[newIndex].value;
                    }

                    newTable[newIndex].value = curNode;
                    m_table[i].value = "0";
                }
            }

            m_table = std::move(newTable);
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