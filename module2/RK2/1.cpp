#include <iostream>
#include <cassert>
#include <stack>
#include <memory>

template<typename T>
class IsLessDefault;

template<typename T>
class ActionDefault;

template<typename T>
struct Node {
    Node<T>* m_parent;
    Node<T>* left_child;
    Node<T>* right_child;
    int height;
    size_t size;
    T value;

    Node() : m_parent(nullptr), left_child(nullptr), right_child(nullptr), height(1), size(1) {}
    Node(T _value) : m_parent(nullptr), left_child(nullptr), right_child(nullptr), height(1), size(1), value(_value) {}
};

template<typename T, class IsLess = IsLessDefault<T>>class Tree {    Node<T>* root;

    IsLess isLess;

    int bFactor(Node<T>* begin) {return Height(begin->right_child) - Height(begin->left_child);}

    Node<T>* rotateRight(Node<T>* begin);
    Node<T>* rotateLeft(Node<T>* begin);
    void Balance(Node<T>* begin);

    void fixHeight(Node<T>* begin);
    void fixSize(Node<T>* begin);

    int Height(Node<T>* node) {return node ? node->height : 0;}
    size_t Size(Node<T>* node) {return node ? node->size : 0;}


    Node<T>* Find(T& elem);
    Node<T>* popMin(Node<T>* begin);



public:
    Tree(const IsLess& is_less = IsLessDefault<T>());
    Tree(const T&) = delete;
    ~Tree();

    Tree& operator=(const Tree &other) = delete;

    void Add(const T& elem);

    void Erase(T& elem);
    T& findKStat(size_t index, Node<T>* begin);

    Node<T>* Root() const {return root;}

    size_t Size() {return root->size;}
    bool isEmpty() const {return root->size == 0;}

    template<class Action = ActionDefault<T>>
    void preOrderTree(Node<T>* begin, const Action& act);

    template<class Action = ActionDefault<T>>
    void inOrderTree(Node<T>* begin, const Action& act);

    template<class Action = ActionDefault<T>>
    void postOrderTree(Node<T>* begin, const Action& act);
};

template<typename T>
class IsLessDefault {
public:
    bool operator() (const Node<T>& l, const Node<T>& r) const {return l.value < r.value;}
};

template<typename T>
class ActionDefault {
public:
    void operator() (const Node<T>& it) const {}
};

template<typename T>
class Printer {
public:
    void operator() (const Node<T>& it) const {
        std::cout << "[" << it.value << "] ";
    }
};

template<typename T, class IsLess>
Tree<T, IsLess>::Tree(const IsLess& is_less) : isLess(is_less) {
    root = new Node<T>;
    root->size = 0;
}

template<typename T, class IsLess>
Tree<T, IsLess>::~Tree() {
    this->postOrderTree(root, [](Node<T>& it) { delete &it; });
}

template<typename T, class IsLess>
void Tree<T, IsLess>::fixHeight(Node<T>* begin) {
    if (!begin) {
        return;
    }

	int hl = Height(begin->left_child);
	int hr = Height(begin->right_child);

    begin->height = ((hl > hr) ? hl : hr) + 1;

    fixHeight(begin->m_parent);
}
template<typename T, class IsLess>
void Tree<T, IsLess>::fixSize(Node<T>* begin) {
   if (!begin) {
        return;
    }

    begin->size = Size(begin->left_child) + Size(begin->right_child) + 1;

    fixSize(begin->m_parent);
}
template<typename T, class IsLess>
Node<T>* Tree<T, IsLess>::rotateRight(Node<T>* begin) {	Node<T>* tmp_left = begin->left_child;
    begin->left_child = tmp_left->right_child;
    if (tmp_left->right_child) {
        tmp_left->right_child->m_parent = begin;
    }
	tmp_left->right_child = begin;

tmp_left->m_parent = begin->m_parent;

    if (begin == Root()) {        tmp_left->m_parent = nullptr;
        root = tmp_left;
   } else f (isLess(tmp_left->value, begin->m_parent->value)) {
        begin->m_parent->left_child = tmp_left;
     else 
        begin->m_parent->right_child = tmp_left;    }    begin->m_parent = tmp_left;

   fixHeight(begin);	fixHeight(tmp_left);

fixSize(begin);
	fixSize(tmp_left);

    return tmp_left;
}

template<typename T, class IsLess>
Node<T>* Tree<T, IsLess>::rotateLeft(Node<T>* begin) {
	Node<T>* tmp_right = begin->right_child;
    begin->right_child = tmp_right->left_child;
    if (tmp_right->left_child) {
        tmp_right->left_child->m_parent = begin;
    }
	tmp_right->left_child = begin;

tmp_right->m_parent = begin->m_parent;

    if (begin == Root()) {        tmp_right->m_parent = nullptr;
        root = tmp_right;
   } else f (isLess(tmp_right->value, begin->m_parent->value)) {
        begin->m_parent->left_child = tmp_right;
     else         begin->m_parent->right_child = tmp_right;
    }    begin->m_parent = tmp_right;
	fixHeight(begin);	fixHeight(tmp_right);

fixSize(begin);
	fixSize(tmp_right);

	return tmp_right;
}

template<typename T, class IsLess>
void Tree<T, IsLess>::Balance(Node<T>* begin) {
    if (!begin) {
        return;
    }

	fixHeight(begin);
    fixSize(begin);
	if (bFactor(begin) == 2) {
		if (bFactor(begin->right_child) < 0) {
            rotateRight(begin->right_child);
        }
        rotateLeft(begin);
        return;
	}

	if (bFactor(begin) == -2) {
		if (bFactor(begin->left_child) > 0 ) {
            rotateLeft(begin->left_child);
            rotateRight(begin);
        } else {
		    rotateRight(begin);
        }
        return;
	}

    Balance(begin->m_parent);
}

template<typename T, class IsLess>
void Tree<T, IsLess>::Add(const T& elem) {
   if (root->size == 0) {
        root->value = elem;
        ++root->size;
        return;
    }

    Node<T>* it = root;
    Node<T>* new_node = new Node<T>;
    new_node->value = elem;
    new_node->height = 1;

    Node<T>* saved_m_parent;
    while (it) {
        saved_m_parent = it;        if (isLess(*new_node, *it)) {
            it = it->left_chld;
       } else {
            it = it->right_chld;
        }
    }

    new_node->m_parent = saved_m_parent;
    if (isLess(*new_node, *saved_m_parent)) {
        saved_m_parent->left_child = newnode;
     else {
       saved_m_parent->right_child = newnode;
    }    Balance(saved_m_parent);
}template<typename T, class IsLess>
Node<T>* Tree<T, IsLess>::Find(T& elem) {
   Node<T>* it = root;
    auto finded_node = std::make_unique<Node<T>>();
    finded_node->value = elem;

    while (it) {
        if (it->value == elem) {
            return it;
        }

        if (isLess(*finded_node, *it)) {
            it = it->left_child;
        } else {
            it = it->right_child;
        }
    }
    return nullptr;
}


template<typename T, class IsLess>
Node<T>* Tree<T, IsLess>::popMin(Node<T>* begin) {
    Node<T>* it = begin;
    while (it->left_child) {
        it = it->left_child;
    }

    if (it->right_child) {
        std::swap(it->value, it->right_child->value);
        Node<T>* saved_min = it->right_child;
        it->right_child->m_parent = nullptr;
        it->right_child = nullptr;
        Balance(it);        return saved_min;
    } else {
       Node<T>* saved_m_parent = it->m_parent;
        if (it == begin) {
            it->m_parent->right_child = nullpt;
        } else {            it->m_parent->left_child = nullptr;        }
       it->m_parent = nullptr;
        Balance(saved_m_parent);
       return it;    }
    return it;

template<typename T, class IsLess>
void Tree<T, IsLess>::Erase(T& elem) {
    Node<T>* del_node = Find(elem);
    if (!del_node) {
        return;
    }

    if (del_node->right_child) {
        Node<T>* min = popMin(del_node->right_child);
        del_node->value = min->value;
        delete min;
        return;
    } else {
        if (!del_node->left_child) {
            Node<T>* saved_m_parent = del_node->m_parent;
            if (del_node == root) {
                return;            }

           if (isLess(*del_node, *del_node->m_parent)) {
                del_node->m_parent->left_child = nullptr;
            } else {                del_node->m_parent->rigt_child = nullptr;
            }           del_node->m_parent = nullptr;            Balance(saved_m_parent);
            delete del_node;           return
        }
       del_node->left_child->m_parent = del_node->m_parent;
        if (del_node == root) {            root = del_node->left_child;
            del_node->left_child->m_parent = nullptr;
       } else if (isLess(*del_node->left_child, *del_node->m_parent)) {
            del_node->m_parent->left_child = del_node>left_child;
         else {
            del_node->m_parent->right_child = del_node>left_child;        }        Balance(del_node->left_child);

       delete del_node;
    }

}

template<typename T, class IsLess>
T& Tree<T, IsLess>::findKStat(size_t index, Node<T>* begin) {
    if (!begin->left_child && !begin->right_child) {
        return begin->value;
    }

    int size_left = begin->left_child ? begin->left_child->size : 0;
    if (index - size_left == 0) {
        return begin->value;
    } else if ((int)(index - size_left) > 0) {
       return findKStat(index - 1 - size_left, begin->right_child);
    } else {
       return findKStat(index, begin->left_child);
    }
}

template<typename T, class IsLess>
template<class Action>
void Tree<T, IsLess>::preOrderTree(Node<T>* begin, const Action& act) {
    Node<T>* it = begin;
    std::stack<Node<T>*> stack;

    while (!stack.empty() || it != nullptr) {
        if (it != nullptr) {
            act(*it);

            if (it->right_child) {
                stack.push(it->right_child);
            }

            it = it->left_child;
        } else {
            it = stack.top();
            stack.pop();
        }
    }
}

template<typename T, class IsLess>
template<class Action>
void Tree<T, IsLess>::inOrderTree(Node<T>* begin, const Action& act) {
    Node<T>* it = begin;
    std::stack<Node<T>*> stack;

    while (!stack.empty() || it != nullptr) {
        if (it != nullptr) {
            stack.push(it);
            it = it->left_child;
        } else {
            it = stack.top();
            stack.pop();

            act(*it);
            it = it->right_child;
        }
    }
}

template<typename T, class IsLess>
template<class Action>
void Tree<T, IsLess>::postOrderTree(Node<T>* begin, const Action& act) {
    Node<T>* it = begin;
    std::stack<Node<T>*> stack;
    Node<T>* last_visited_node = nullptr;
    Node<T>* top_node = nullptr;

    while (!stack.empty() || it != nullptr) {
        if (it != nullptr) {
            stack.push(it);
            it = it->left_child;
        } else {
            top_node = stack.top();
            if (!(top_node->right_child == nullptr) && !(last_visited_node == top_node->right_child)) {
                it = top_node->right_child;
            } else {
                stack.pop();
                last_visited_node = top_node;
                act(*top_node);
            }
        }
    }
}


int main() {
    auto tree = std::make_unique<Tree<int>>();

    int tmp;
    while (std::cin >> tmp) {
        tree->Add(tmp);
    }

    bool result = true;
    tree->preOrderTree(tree->Root(), [&result, value = tree->Root()->value](const Node<int>& it) {
    result = result && (it.value == value);});

    std::cout << result << std::endl;
    return 0;
}
