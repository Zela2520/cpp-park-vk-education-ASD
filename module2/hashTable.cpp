#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


struct Prob {
    int operator()(const int hash, int step) const {
        return (hash + step * step / 2 + step / 2);
    }
};

struct Hash {
	long long unsigned int operator()(const std::string& value) const {
		long long unsigned int hash = 0;
		for (const auto& elem : value)
			hash = hash * 127 + elem;
		return hash;
	}
};

template<class T>
struct HashTableNode {
	T value;
    explicit HashTableNode(T _value) : value(_value) {};
	HashTableNode() : value("0") {};
};

template <class T, class Hasher, class Probing>
class HashTable {
public:
    explicit HashTable(const Hasher& _hasher, const Probing& _probing) :
        hasher_(_hasher),
        prober_(_probing),
        table_(8),
        keys_counter_(0)
        {}
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;
	~HashTable() = default;

    bool Add(const T& value) {
        if (keys_counter_ >= 3 * table_.capacity() / 4) {
            ReHash();
        }
        int hash = hasher_(value);
        int index = hash % table_.capacity();
        T node_value = table_[index].value;

        for (size_t i = 0; i < table_.capacity(); ++i ) {
            if (node_value == "0") break;

            if (node_value == value && node_value != "DELETED") {
                return false;
            }

            index = prober_(hash, i + 2) % table_.capacity();
            node_value = table_[index].value;
        }

        table_[index].value = value;
        ++keys_counter_;
        return true;
    }

    bool Delete(const T& value) {
        int hash = hasher_(value);
        int index = hash % table_.capacity();
        T node_value = table_[index].value;

        for ( size_t i = 0; i < table_.capacity(); ++i ) {
            if (node_value == "0") return false;

            if (node_value == value && node_value != "DELETED") break;

            index = prober_(hash, i + 2) % table_.capacity();
            node_value = table_[index].value;
        }

        table_[index].value = "DELETED";
        --keys_counter_;
        return true;
    }

    bool Find(const T& value) {
        int hash = hasher_(value);
        int index = hash % table_.capacity();

        T node_value = table_[index].value;
        for (size_t i = 0; i < table_.capacity(); ++i ) {
            if (node_value == "0") return false;
            if (node_value == value && node_value != "DELETED") return true;

            index = prober_(hash, i + 2) % table_.capacity();
            node_value = table_[index].value;
        }
    }

private:
    void ReHash() {
        std::vector<HashTableNode<T> > new_table(table_.capacity() * 2);
        for (size_t i = 0; i < table_.capacity(); ++i ) {
            T node_value = table_[i].value;
            if ( node_value != "0" && node_value != "DELETED" ) {
                int new_hash = hasher_(node_value);
                int new_index = new_hash % new_table.capacity();
                T new_node_value = new_table[new_index].value;

                int new_step = 2;
                while ( new_node_value != "0" && new_node_value != node_value ) {
                    new_index = prober_(new_hash, new_step++) % new_table.capacity();
                    new_node_value = new_table[new_index].value;
                }

                new_table[new_index].value = node_value;
                table_[i].value = "0";
            }
        }
        table_ = std::move(new_table);
    }

    std::vector<HashTableNode<T> > table_;
    int keys_counter_;
    Hasher hasher_;
    Probing prober_;
};

void run(std::istream &input, std::ostream &output) {
    Hash hasher;
    Prob prober;
    HashTable<std::string, Hash, Prob> ht(hasher, prober);

    char operation;
	std::string word;
    
	while (input >> operation >> word) {
		if (operation == '+')
			output << (ht.Add(word) ? "OK" : "FAIL") << std::endl;
		else if (operation == '?')
			output << (ht.Find(word) ? "OK" : "FAIL") << std::endl;
		else if (operation == '-')
			output << (ht.Delete(word) ? "OK" : "FAIL") << std::endl;
	}
}

int main() {
   run(std::cin, std::cout);
   return 0;
}
