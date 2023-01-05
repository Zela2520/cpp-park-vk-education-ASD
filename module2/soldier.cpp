// В одной военной части решили построить в одну шеренгу по росту. Т.к. часть была далеко не образцовая, то солдаты часто приходили не вовремя, а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги. Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены по росту – сначала самые высокие, а в конце – самые низкие. За расстановку солдат отвечал прапорщик, который заметил интересную особенность – все солдаты в части разного роста. Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять солдат, а именно для каждого приходящего солдата указывать, перед каким солдатом в строе он должен становится. Требуемая скорость выполнения команды - O(log n).

// Формат ввода
// Первая строка содержит число N – количество команд (1 ≤ N ≤ 90 000). В каждой следующей строке содержится описание команды: число 1 и X если солдат приходит в строй (X – рост солдата, натуральное число до 100 000 включительно) и число 2 и Y если солдата, стоящим в строе на месте Y надо удалить из строя. Солдаты в строе нумеруются с нуля.

// Формат вывода
// На каждую команду 1 (добавление в строй) вы должны выводить число K – номер позиции, на которую должен встать этот солдат (все стоящие за ним двигаются назад).

// Пример
// Ввод	Вывод
// 5
// 1 100
// 1 200
// 1 50
// 2 1
// 1 150
// 0
// 0
// 2
// 1


// по сути обратная задача ко второй надо найти номер к-ой порядковой статистике узла
// как это сделать ?
// мы должны искть спускаться к листу и суммировать к-ые статистики родителей ?

#include <iostream>
#include <sstream>
#include <cassert>
#include <random>
#include <algorithm>
#include <functional>
#include <stack>

template<class T>
struct Comparator {
    enum Operators {LESS, EQUAL, MORE};

    Operators operator()(const T &l, const T &r) {
        if (l < r) { return LESS; }
        if (l > r) { return MORE; }
        return EQUAL;
    };
};

template<class K, class D, class C = Comparator<K>>
class AVLTree {
protected:
    struct Node {
        D value;
        Node *left_child;
        Node *right_child;
        char height;

        K key;
        size_t elementCount;

        Node(const K &k, const D &d) : value(d), left_child(nullptr), right_child(nullptr), height(1), key(k), elementCount(1) {}
        Node(const Node &) = delete;
        Node &operator=(const Node &) = delete;
        Node(Node &&) = delete;
        Node &operator=(Node &&) = delete;
        ~Node() { delete left_child; delete right_child; }
    };

public:
    explicit AVLTree(const C &Comparator = C());
    ~AVLTree() { delete m_root; }
    AVLTree(const Node &) = delete;
    AVLTree &operator=(const AVLTree &) = delete;
    AVLTree(AVLTree &&) = delete;
    AVLTree &operator=(AVLTree &&) = delete;

    D *find(const K &);
    void insert(const K &, const D &);
    void erase(const K &);
    size_t size();
    bool isEmpty();
    void traverseInOrder(const std::function<void(const K &)> &callback);

protected:
    Node *balance(Node*);
    Node *rotateLeft(Node*);
    Node *rotateRight(Node*);
    int balanceFactor(Node*);
    char height(Node*);
    void fixHeight(Node*);
    Node *findAndRemoveMin(Node*, Node**);
    void fixElementCount(Node*);
    
    // helper functions
    Node *innerFind(const K &, Node*);
    Node *innerInsert(const K&, const D&, Node*);
    Node *innerErase(const K&, Node*);
    Node *innerDelete(Node*);
    
    Node *m_root;
    C m_comparator;
    size_t m_soldierCounter;
};

template<class K, class D, class C>
AVLTree<K, D, C>::AVLTree(const C & _comparator) : m_comparator(_comparator), m_root(nullptr), m_soldierCounter(0) {}


template<class K, class D, class C>
D *AVLTree<K, D, C>::find(const K &key) {
    auto curNode = innerFind(key, m_root);
    return curNode ? &curNode->value : nullptr;
}

template<class K, class D, class C>
void AVLTree<K, D, C>::insert(const K &key, const D &value) {
    m_root = innerInsert(key, value, m_root);
}

template<class K, class D, class C>
typename AVLTree<K, D, C>::Node *AVLTree<K, D, C>::innerInsert(const K &key, const D &value, AVLTree::Node *curNode) {
    if (!curNode) {
        m_soldierCounter++;
        return new Node(key, value);
    }

    auto res = m_comparator(key, curNode->key);

    switch (res) {
        case C::LESS:
            curNode->left_child = innerInsert(key, value, curNode->left_child);
            break;
        case C::EQUAL:
            break;
        case C::MORE:
            curNode->right_child = innerInsert(key, value, curNode->right_child);
            break;
    }
    return balance(curNode);
}

template<class K, class D, class C>
void AVLTree<K, D, C>::erase(const K &key) {
    m_root = innerErase(key, m_root);
}

template<class K, class D, class C>
typename AVLTree<K, D, C>::Node *AVLTree<K, D, C>::innerErase(const K &key, AVLTree::Node *curNode) {
    if (!curNode) { return nullptr; }

    auto res = m_comparator(key, curNode->key);

    switch (res) {
        case C::LESS:
            curNode->left_child = innerErase(key, curNode->left_child);
            break;
        case C::EQUAL: {
            return innerDelete(curNode);
        }
        case C::MORE:
            curNode->right_child = innerErase(key, curNode->right_child);
            break;
    }
    return balance(curNode);
}

template<class K, class D, class C>
typename AVLTree<K, D, C>::Node *AVLTree<K, D, C>::innerFind(const K &key, AVLTree::Node *curNode) {
    if (!curNode) { return nullptr; }

    auto res = m_comparator(key, curNode->key);

    switch (res) {
        case C::LESS:
            return innerFind(key, curNode->left_child);
        case C::EQUAL:
            return curNode;
        case C::MORE:
            return innerFind(key, curNode->right_child);
        default:
            assert(false);
    }
}

template<class K, class D, class C>
typename AVLTree<K, D, C>::Node *AVLTree<K, D, C>::findAndRemoveMin(AVLTree::Node *curNode, AVLTree::Node **minNode) {
    if (!curNode->left_child) {
        *minNode = curNode;
        return curNode->right_child;
    }
    curNode->left_child = findAndRemoveMin(curNode->left_child, minNode);
    return balance(curNode);
}

template<class K, class D, class C>
char AVLTree<K, D, C>::height(AVLTree::Node *curNode) {
    if (!curNode) { return 0; }
    return curNode->height;
}

template<class K, class D, class C>
void AVLTree<K, D, C>::fixHeight(AVLTree::Node *curNode) {
    if (!curNode) { return; }
    curNode->height = std::max(curNode->left_child ? curNode->left_child->height : 0, curNode->right_child ? curNode->right_child->height : 0) + 1;
}

template<class K, class D, class C>
void AVLTree<K, D, C>::fixElementCount(AVLTree::Node *curNode) {
    if (!curNode) { return; }
    curNode->elementCount = ((curNode->left_child ? curNode->left_child->elementCount : 0) + (curNode->right_child ? curNode->right_child->elementCount : 0) + 1);
}

template<class K, class D, class C>
typename AVLTree<K, D, C>::Node *AVLTree<K, D, C>::balance(AVLTree::Node *curNode) {
    fixHeight(curNode);
    fixElementCount(curNode);

    auto balance = balanceFactor(curNode);

    if (balance == 2) {
        if (balanceFactor(curNode->right_child) < 0) {
            curNode->right_child = rotateRight(curNode->right_child);
        }

        return rotateLeft(curNode);
    } else if (balance == -2) {
        if (balanceFactor(curNode->left_child) > 0) {
            curNode->left_child = rotateLeft(curNode->left_child);
        }

        return rotateRight(curNode);
    }

    return curNode;
}

template<class K, class D, class C>
int AVLTree<K, D, C>::balanceFactor(Node *curNode) {
    return height(curNode->right_child) - height(curNode->left_child);
}

template<class K, class D, class C>
size_t AVLTree<K, D, C>::size() {
    return m_soldierCounter;
}

template<class K, class D, class C>
bool AVLTree<K, D, C>::isEmpty() {
    return !m_soldierCounter;
}

template<class K, class D, class C>
typename AVLTree<K, D, C>::Node *AVLTree<K, D, C>::rotateLeft(AVLTree::Node *curNode) {
    auto right_child = curNode->right_child;

    curNode->right_child = right_child->left_child;
    right_child->left_child = curNode;

    fixHeight(curNode);
    fixHeight(right_child);

    fixElementCount(curNode);
    fixElementCount(right_child);

    return right_child;
}

template<class K, class D, class C>
typename AVLTree<K, D, C>::Node *AVLTree<K, D, C>::rotateRight(AVLTree::Node *curNode) {
    auto left_child = curNode->left_child;
    curNode->left_child = left_child->right_child;
    left_child->right_child = curNode;

    fixHeight(curNode);
    fixHeight(left_child);

    fixElementCount(curNode);
    fixElementCount(left_child);

    return left_child;
}

template<class K, class D, class C>
typename AVLTree<K, D, C>::Node *AVLTree<K, D, C>::innerDelete(Node *curNode) {
    m_soldierCounter--;
    auto left_child = curNode->left_child;
    auto right_child = curNode->right_child;

    curNode->left_child = nullptr;
    curNode->right_child = nullptr;

    delete curNode;

    if (!right_child) { return left_child; }

    Node *minNode = nullptr;
    auto treeWithoutMinNode = findAndRemoveMin(right_child, &minNode);

    minNode->right_child = treeWithoutMinNode;
    minNode->left_child = left_child;

    return balance(minNode);
}

template<class K, class D, class C>
void AVLTree<K, D, C>::traverseInOrder(const std::function<void(const K &)> &callback) {
    if (!m_root) { return; }

    std::stack<Node *> stack;
    stack.push(m_root);

    auto current = m_root;
    while (current->left_child) {
        current = current->left_child;
        stack.push(current);
    }

    while (!stack.empty()) {
        current = stack.top();
        callback(current->key);

        stack.pop();

        if (!current->right_child) { continue; }
        current = current->right_child;
        stack.push(current);

        while (current->left_child) {
            current = current->left_child;
            stack.push(current);
        }
    }
}


template<class K, class D, class C = Comparator<K>>
class SolderAVLTree : public AVLTree<K, D, C> {
public:
    using typename AVLTree<K, D, C>::Node;
    explicit SolderAVLTree(const C &Comparator = C());
    void deleteWithPos(size_t);
    size_t insertAndGetPos(const K &, const D &);

private:
    Node *addWithPosInner(const K &, const D &, Node *, size_t &);
    Node *deleteWithPosInner(Node *, size_t);

    size_t elementCountWithoutright_childTree(Node *);
    using AVLTree<K, D, C>::innerErase;
    using AVLTree<K, D, C>::innerInsert;
    using AVLTree<K, D, C>::balance;
    using AVLTree<K, D, C>::innerDelete;
    using AVLTree<K, D, C>::m_root;
    using AVLTree<K, D, C>::m_comparator;
    using AVLTree<K, D, C>::m_soldierCounter;
};


template<class K, class D, class C>
typename SolderAVLTree<K, D, C>::Node *
SolderAVLTree<K, D, C>::addWithPosInner(const K &key, const D &value, Node *curNode, size_t &index) {
    if (!curNode) {
        m_soldierCounter++;
        return new Node(key, value);
    }

    auto res = m_comparator(key, curNode->key);

    switch (res) {
        case C::LESS:
            curNode->left_child = addWithPosInner(key, value, curNode->left_child, index);
            break;
        case C::EQUAL:
            break;
        case C::MORE:
            index += elementCountWithoutright_childTree(curNode);
            curNode->right_child = addWithPosInner(key, value, curNode->right_child, index);
            break;
    }

    return balance(curNode);
}

template<class K, class D, class C>
size_t SolderAVLTree<K, D, C>::insertAndGetPos(const K &key, const D &value) {
    size_t pos = 0;
    m_root = addWithPosInner(key, value, m_root, pos);

    return m_soldierCounter - pos - 1;
}

template<class K, class D, class C>
SolderAVLTree<K, D, C>::SolderAVLTree(const C &_comparator) : AVLTree<K, D, C>(_comparator) {
}

template<class K, class D, class C>
void SolderAVLTree<K, D, C>::deleteWithPos(size_t index) {
    assert(index <= m_soldierCounter);

    index = m_soldierCounter - index - 1;
    m_root = deleteWithPosInner(m_root, index + 1);
}

template<class K, class D, class C>
size_t SolderAVLTree<K, D, C>::elementCountWithoutright_childTree(Node *curNode) {
    if (!curNode->left_child) { return 1; }
    return curNode->left_child->elementCount + 1;
}

template<class K, class D, class C>
typename SolderAVLTree<K, D, C>::Node *
SolderAVLTree<K, D, C>::deleteWithPosInner(Node *curNode, size_t notion) {
    if (!curNode) { return nullptr; }

    auto nodePos = elementCountWithoutright_childTree(curNode);

    if (nodePos < notion) {
        notion -= nodePos;
        curNode->right_child = deleteWithPosInner(curNode->right_child, notion);
    } else if(nodePos > notion) {
        curNode->left_child = deleteWithPosInner(curNode->left_child, notion);
    } else {
        return innerDelete(curNode);
    }

    return balance(curNode);
}

void run(std::istream &input, std::ostream &output) {
    size_t numbersOfCommand = 0;
    input >> numbersOfCommand;

    SolderAVLTree<size_t, char> tree;

    int command = 0;
    size_t index = 0;

    for (size_t i = 0; i < numbersOfCommand; ++i) {
        input >> command >> index;
        switch (command) {
            case 1:
                output << tree.insertAndGetPos(index, 0) << std::endl;
                break;
            case 2:
                tree.deleteWithPos(index);
                break;
            default:
                assert(false && "Не известная команда");
        }
    }
}

int main() {
    run(std::cin, std::cout);

    return 0;
}
