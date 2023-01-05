#include <iostream>
#include <string>
#include <vector>
#include <cassert>

using std::cout;
using std::cin;
using std::vector;
using std::string;

#define INIT_CAPACITY 8
#define KOEF_ALPHA 0.75
#define KOEF_DEL_CELLS 0.33
#define MAGIC_SIMPLE_1 137
#define MAGIC_SIMPLE_2 37
#define MAGIC_SIMPLE_3 11
#define DEL_NOT_FOUND -1

enum STATUS_CELL {
    FREE,
    BUSY,
    DELETED
};


struct StringHash {
    size_t operator() (const string& str) const {
        size_t hash = 0;
        for (size_t i = 0; i < str.size(); ++i) {
            hash = hash * MAGIC_SIMPLE_1 + str[i];
        }
        return hash;
    }
};

struct Node {
    std::string product_name;
    int color;
    int size;

    bool operator==(const Node other) const {return (this->product_name == other.product_name) &&
                                        (this->color == other.color) &&
                                        (this->size == other.size);}
};

struct NodeHash {
    size_t operator() (const Node& other) const {
        size_t hash = 0;
        for (size_t i = 0; i < other.product_name.size(); ++i) {
            hash = hash * MAGIC_SIMPLE_1 + other.product_name[i];
        }
        hash = hash + other.color * MAGIC_SIMPLE_2 + other.size * MAGIC_SIMPLE_3;
        return hash;
    }
};

template<typename T>
struct HashTableCell {
    T value;
    size_t absHash;
    int status;

    HashTableCell() : absHash(0), status(FREE) {}
    HashTableCell(const T& value, size_t hash, int status = FREE) :
        value(value),
        absHash(hash),
        status(status) {}

    HashTableCell(const HashTableCell& other) = default;
    HashTableCell& operator= (const HashTableCell& other) = default;
};

// Хеш-таблица
template<typename T, typename H>
class HashTable {
public:
    explicit HashTable(const H& _hasher);
    HashTable(const HashTable& other) = delete;
    ~HashTable() = default;

    HashTable& operator= (const HashTable& other) = delete;

    bool Has(const T& data) const;
    bool Add(const T& data);
    bool Delete(const T& data);

private:
    vector<HashTableCell<T>> table;
    H hasher;
    int keys_count;
    int deleted_count;

    void reHashTable();
    void growTable();
};


template<typename T, typename H>
HashTable<T,H>::HashTable(const H& _hasher) :
    table(INIT_CAPACITY),
    hasher(_hasher),
    keys_count(0),
    deleted_count(0) {}


template<typename T, typename H>
void HashTable<T,H>::reHashTable() {
    vector<HashTableCell<T>> newTable(table.size());
    for (size_t i = 0; i < table.size(); ++i) {
        if (table[i].status == BUSY) {
            size_t new_pos_insert = table[i].absHash % newTable.size();
            size_t new_pos_insert_prob = new_pos_insert;

            for (int j = 1; newTable[new_pos_insert_prob].status == BUSY; ++j) {
                new_pos_insert_prob = (new_pos_insert + j*(j+1)/2) % newTable.size();
            }

            newTable[new_pos_insert_prob] = table[i];
        }
    }
    deleted_count = 0;
    table = std::move(newTable);
}

template<typename T, typename H>
void HashTable<T,H>::growTable() {
    vector<HashTableCell<T>> newTable(table.size() * 2);
    for (size_t i = 0; i < table.size(); ++i) {
        if (table[i].status == BUSY) {
            size_t new_pos_insert = table[i].absHash % newTable.size();
            size_t new_pos_insert_prob = new_pos_insert;

            for (int j = 1; newTable[new_pos_insert_prob].status == BUSY; ++j) {
                new_pos_insert_prob = (new_pos_insert + j*(j+1)/2) % newTable.size();
            }

            newTable[new_pos_insert_prob] = table[i];
        }
    }
    table = std::move(newTable);
}

template<typename T, typename H>
bool HashTable<T,H>::Has(const T& data) const {
    size_t pos_insert = hasher(data) % table.size();
    for (size_t i = 0; i < table.size(); ++i) {
        size_t pos_insert_prob = (pos_insert + i*(i+1)/2) % table.size();
        switch (table[pos_insert_prob].status) {
        case BUSY:
            if (data == table[pos_insert_prob].value) {
                return true;
            }
            break;

        case FREE:
            return false;
            break;

        default:
            break;
        }
    }

    return false;
}

template<typename T, typename H>
bool HashTable<T,H>::Add(const T& data) {
    if (keys_count > KOEF_ALPHA * table.size()) {
        growTable();
    }

    size_t absHash = hasher(data);
    size_t pos_insert = absHash % table.size();
    ssize_t first_pos_del = DEL_NOT_FOUND;
    for (size_t i = 0; i < table.size(); ++i) {
        size_t pos_insert_prob = (pos_insert + i*(i+1)/2) % table.size();
        switch (table[pos_insert_prob].status) {
        case BUSY:
            if (data == table[pos_insert_prob].value) {
                return false;
            }
            break;

        case DELETED:
            first_pos_del = (first_pos_del == DEL_NOT_FOUND) ? pos_insert_prob : first_pos_del;
            break;

        case FREE:
            if (first_pos_del == DEL_NOT_FOUND) {
                table[pos_insert_prob].value = data;
                table[pos_insert_prob].absHash = absHash;
                table[pos_insert_prob].status = BUSY;
            } else {
                table[first_pos_del].value = data;
                table[first_pos_del].absHash = absHash;
                table[first_pos_del].status = BUSY;
                --deleted_count;
            }
            ++keys_count;
            return true;
            break;

        default:
            break;
        }
    }

    if (first_pos_del == DEL_NOT_FOUND) {
        return false;
    } else {
        table[first_pos_del].value = data;
        table[first_pos_del].absHash = absHash;
        table[first_pos_del].status = BUSY;
        ++keys_count;
        return true;
    }

}

template<typename T, typename H>
bool HashTable<T,H>::Delete(const T& data) {
    size_t pos_insert = hasher(data) % table.size();

    for (size_t i = 0; i < table.size(); ++i) {
        size_t pos_insert_prob = (pos_insert + i*(i+1)/2) % table.size();
        switch (table[pos_insert_prob].status) {
        case BUSY:
            if (data == table[pos_insert_prob].value) {
                table[pos_insert_prob].status = DELETED;
                --keys_count;
                ++deleted_count;
                if (deleted_count > KOEF_DEL_CELLS * table.size()) {
                    reHashTable();
                }
                return true;
            }
            break;

        case FREE:
            return false;
            break;

        default:
            break;
        }
    }

    return false;
}


int main() {
    NodeHash h;
    HashTable<Node, NodeHash> map(h);

    char operation = 0;
    Node data;

    while (cin >> operation >> data.product_name >> data.color >> data.size) {
        switch (operation) {
        case '+':
            cout << (map.Add(data) ? "OK" : "FAIL") << std::endl;
            break;
        case '-':
            cout << (map.Delete(data) ? "OK" : "FAIL") << std::endl;
            break;
        case '?':
            cout << (map.Has(data) ? "OK" : "FAIL") << std::endl;
            break;

        default:
            assert(false);
            break;
        }
    }

    return 0;
}
