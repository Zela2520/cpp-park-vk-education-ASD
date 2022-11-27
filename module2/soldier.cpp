#include <iostream>
#include <string>

template <typename T>
class DefaultComparator {
    public:
        bool operator()(const T& left, const T& right) {
            return left > right;
        }
};

template<class T, class Comparator = DefaultComparator<T>> class AvlTree;

template <typename T1>
class Node {
    private:
        T1 m_data;
        Node* left;
        Node* right;
        size_t height;
        size_t count;
    public:
        Node(const T1& _data = T1{}) : m_data(_data), left(nullptr), right(nullptr), height(1) {}
        template<class, class> friend class AvlTree; 
};

template <typename T, typename Comparator>
class AvlTree {
    Node<T>* root;
    size_t size;
    Comparator comp;

    size_t getHeight(Node<T>* curNode);
    void fixHeight(Node<T>* curNode);

    // повороты
    Node<T>* rotateLeft(Node<T>* curNode);
    Node<T>* rotateRight(Node<T>* curNode);

    // балансировка
    Node<T>* doBalance(Node<T>* curNode);
    int getBalance(Node<T>* curNode);

    // вспомогательные методы
    Node<T>* addInternal(Node<T>* curNode, const T& data);
    Node<T>* deleteInternal(Node<T>* curNode, const T& data);
    Node<T>* findMin(Node<T>* curNode);
    Node<T>* removeMin(Node<T>* curNode);
    void destroyTree(Node<T>* curNode);

    public:
        AvlTree(Comparator _comp = Comparator()) : root(nullptr), size(0), comp(_comp) {}
        ~AvlTree() {destroyTree(root);} 

        void Add(const T& data);
        bool Has(const T& data);
        void Delete(const T& data);

};

template <typename T, typename Comparator>
Node<T>* AvlTree<T, Comparator>::removeMin(Node<T>* curNode) {
    if (!curNode->left) {
        return curNode->right;
    }

    curNode->left = removeMin(curNode->left);
    return doBalance(curNode);
}

template <typename T, typename Comparator>
Node<T>* AvlTree<T, Comparator>::findMin(Node<T>* curNode) {
    while(curNode->left) {
        curNode = curNode->left;
    }
    return curNode;
}

template <typename T, typename Comparator>
Node<T>* AvlTree<T, Comparator>::deleteInternal(Node<T>* curNode, const T& data) {
    if(!curNode) {
        return nullptr;
    }
    if (curNode->m_data < data) {
        curNode->right = deleteInternal(curNode->right, data);
    } else if (curNode->m_data > data) {
        curNode->left = deleteInternal(curNode->left, data);
    } else {
        Node<T>* left = curNode->left;
        Node<T>* right = curNode->right;

        delete curNode;

        if (!right) {
            return left;
        }

        Node<T>* min = findMin(right);
        min->right = removeMin(right);
        min->left = left;

        return doBalance(min);
    }

    return doBalance(curNode); 
}

template <typename T, typename Comparator>
void AvlTree<T, Comparator>::Delete(const T& data) {
    root = deleteInternal(root, data);
}

template <typename T, typename Comparator>
void AvlTree<T, Comparator>::destroyTree(Node<T>* curNode) {
    if (curNode) {
        destroyTree(curNode->left);
        destroyTree(curNode->right);
        delete curNode;
    }
}

template <typename T, typename Comparator>
Node<T>* AvlTree<T, Comparator>::addInternal(Node<T>* curNode, const T& data) {
    if (!curNode) {
        return new Node(data);
    }

    if (curNode->m_data <= data) {
        curNode->right = addInternal(curNode->right, data);
    } else {
        curNode->left = addInternal(curNode->left, data);
    }

    return doBalance(curNode);
}

template <typename T, typename Comparator>
void AvlTree<T, Comparator>::Add(const T& data) {
    root = addInternal(root, data); 
}

template <typename T, typename Comparator>
int AvlTree<T, Comparator>::getBalance(Node<T>* curNode) {
    return getHeight(curNode->right) - getHeight(curNode->left);
}

template <typename T, typename Comparator>
Node<T>* AvlTree<T, Comparator>::doBalance(Node<T>* curNode) {
    fixHeight(curNode);
    
    switch (getBalance(curNode))
    {
        case 2:
        {
            if (getBalance(curNode->right) < 0) { // не оч понял надо пересмотреть лекцию и семинар
                curNode->right = rotateRight(curNode->right);
            }
            return rotateLeft(curNode);
        }
        case -2:
        {
            if (getBalance(curNode->left) > 0) { // не оч понял надо пересмотреть лекцию и семинар
                curNode->left = rotateLeft(curNode->left);
            }
            return rotateRight(curNode);
        }
        default:
            return curNode;
    }
}

template <typename T, typename Comparator>
Node<T>* AvlTree<T, Comparator>::rotateLeft(Node<T>* curNode) {
    Node<T>* newRoot = curNode->right;
    curNode->right = newRoot->left;
    newRoot->left = curNode;
    fixHeight(curNode);
    fixHeight(newRoot);
    return newRoot;
}

template <typename T, typename Comparator>
Node<T>* AvlTree<T, Comparator>::rotateRight(Node<T>* curNode) {
    Node<T>* newRoot = curNode->left;
    curNode->left = newRoot->right;
    newRoot->right = curNode;
    fixHeight(curNode);
    fixHeight(newRoot);
    return newRoot;
}

template <typename T, typename Comparator>
void AvlTree<T, Comparator>::fixHeight(Node<T>* curNode) {
    curNode->height = std::max(getHeight(curNode->left), getHeight(curNode->right)) + 1;
}

template <typename T, typename Comparator>
size_t AvlTree<T, Comparator>::getHeight(Node<T>* curNode) {
    return curNode ? curNode->height : 0;
}

template <typename T, typename Comparator>
bool AvlTree<T, Comparator>::Has(const T& data) {
   Node<T>* curNode = root;
   while(curNode) {
        if (curNode->m_data == data) {
            return true;
        }
        if (curNode->m_data > data) {
            curNode = curNode->left;
        } else {
            curNode = curNode->right;
        }
   }
   return false;
}

int main() {

    AvlTree<std::string> tree;
    int size;
    std::cin >> size;

    int i = 0;
    int operation;
    std::string data;

    while(i < size) {

        std::cin >> operation >> data;

        switch(operation) {
            case 1:
            {
                tree.Add(data);
                std::cout << "something";
                break;
            }

            case 2:
            {
                tree.Delete(data);
                break;
            }
        }

        ++i;
    }

    return 0;
}
