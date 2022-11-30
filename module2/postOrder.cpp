// Дано число N ≤ 104 и последовательность целых чисел из [-231..231] длиной N. Требуется построить бинарное дерево, заданное наивным порядком вставки. Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое поддерево root. Выведите элементы в порядке post-order (снизу вверх).

// Рекурсия запрещена.

// Пример 1
// Ввод	Вывод
// 10
// 7
// 2
// 10
// 8
// 5
// 3
// 6
// 4
// 1
// 9


#include <iostream>
#include <stack>
#include <queue>
#include <cassert>

/////////////////////////////////////////////////////////////Узел дерева/////////////////////////////////////////////////////////////

template<typename T>
class Node {
    public:
        Node(T data = T(), Node<T>* parent = nullptr, Node<T>* left = nullptr, Node<T>* right = nullptr) :
        m_data(data), m_left(left), m_right(right)
        {
        }
        ~Node() = default;
        T m_data;
        Node<T>* m_parent;
        Node<T>* m_left;
        Node<T>* m_right;
};

/////////////////////////////////////////////////////////////Компаратор/////////////////////////////////////////////////////////////

template<typename T>
class MinComparator {
public:
    bool operator() (const Node<T>& left, const Node<T>& right) const {
        return left.m_data < right.m_data;
    }
};

/////////////////////////////////////////////////////////////Дерево/////////////////////////////////////////////////////////////

template<typename T, class Comparator = MinComparator<T>>
class BinaryTree {
public:
    BinaryTree(const Comparator& _comparator =  MinComparator<T>());    // готово
    BinaryTree(const T&) = delete;                                      // готово
    ~BinaryTree();                                                      // готово                                   
    BinaryTree& operator=(const BinaryTree &other) = delete;            // готово

    size_t get_size() { return m_size; }                              
    bool is_empty() const { return m_size == 0; }                     

    void push_back(const T& value);                                     // готово
    void level_order_tree();                                            // готово
    void post_order();

private:
    Node<T>* m_root;
    size_t m_size;
    Comparator m_comparator;
};


template<typename T, class Comparator>
BinaryTree<T, Comparator>::BinaryTree(const Comparator& _comparator) : m_size(0), m_comparator(_comparator) {
    m_root = new Node<T>;
}

template<typename T, class Comparator>
BinaryTree<T, Comparator>::~BinaryTree() {
    
    std::queue<Node<T>*> my_queue;
	my_queue.push(m_root);

	while (!(my_queue.empty())) {

		Node<T>* deleted_node = my_queue.front();
		my_queue.pop();

		if (deleted_node->m_left != nullptr) {
			my_queue.push(deleted_node->m_left);
        }

		if (deleted_node->m_right != nullptr) {
			my_queue.push(deleted_node->m_right);
        }
        delete deleted_node;
        deleted_node = nullptr;
	}
    
}

template<typename T, class Comparator>
void BinaryTree<T, Comparator>::push_back(const T& value) {

    if (!m_size) {
        m_root->m_data = value;
        ++m_size;
        return;
    }

    Node<T>* current_node = m_root;
    Node<T>* inserted_node = new Node<T>(value);
    Node<T>* current_parent;

    while (current_node != nullptr) {

        current_parent = current_node;

        if (m_comparator(*inserted_node, *current_node)) {
            current_node = current_node->m_left;
        } else {
            current_node = current_node->m_right;
        }
    }

    inserted_node->m_parent = current_parent;
    if (m_comparator(*inserted_node, *current_parent)) {
        current_parent->m_left = inserted_node;
    } else {
        current_parent->m_right = inserted_node;
    }

    inserted_node->m_left = nullptr;
    inserted_node->m_right = nullptr;
    ++m_size;
}

template<typename T, class Comparator>
void BinaryTree<T, Comparator>::level_order_tree() {
    std::queue<Node<T>*> my_queue;
    my_queue.push(m_root);

	while (!(my_queue.empty())) {

		Node<T>* node = my_queue.front();
		std::cout << node->m_data << " ";

		my_queue.pop();

		if (node->m_left != nullptr) {
			my_queue.push(node->m_left);
        }

		if (node->m_right != nullptr) {
			my_queue.push(node->m_right);
        }
	}
}

template<typename T, class Comparator>
void BinaryTree<T, Comparator>::post_order() {
    std::stack<Node<T>*> myStack1, myStack2;
    myStack1.push(m_root);

    while(!myStack1.empty()) {
        Node<T>* temp = myStack1.top();
        myStack1.pop();
        
        myStack2.push(temp);
        
        if (temp->m_left) {
            myStack1.push(temp->m_left);
        }
        if (temp->m_right != NULL) {
            myStack1.push(temp->m_right);
        }
    }

    while(!myStack2.empty()) {
        std::cout << myStack2.top()->m_data << " ";
        myStack2.pop();
    }

    std::cout << std::endl;
}

/////////////////////////////////////////////////////////////User's action/////////////////////////////////////////////////////////////

void input() {
    BinaryTree<int> my_binary_tree;

    size_t size = 0;
    std::cin >> size;
    assert(size != 0);

    for (size_t i = 0; i < size; ++i) {
        int temp_buffer;
        std::cin >> temp_buffer;
        my_binary_tree.push_back(temp_buffer);
    }

    my_binary_tree.post_order();
}


int main() {
    input();

    return 0;
}