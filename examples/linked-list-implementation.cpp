#include <iostream>
#include <stdexcept>
using namespace std;

// 链表节点结构
template<typename T>
struct Node {
    T data;
    Node<T>* next;
    
    Node(const T& value) : data(value), next(nullptr) {}
};

// 单链表类
template<typename T>
class LinkedList {
private:
    Node<T>* head;
    Node<T>* tail;
    size_t size_;
    
public:
    // 构造函数
    LinkedList() : head(nullptr), tail(nullptr), size_(0) {}
    
    // 析构函数
    ~LinkedList() {
        clear();
    }
    
    // 拷贝构造函数
    LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr), size_(0) {
        Node<T>* current = other.head;
        while (current != nullptr) {
            push_back(current->data);
            current = current->next;
        }
    }
    
    // 赋值操作符
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            Node<T>* current = other.head;
            while (current != nullptr) {
                push_back(current->data);
                current = current->next;
            }
        }
        return *this;
    }
    
    // 清空链表
    void clear() {
        while (head != nullptr) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
        head = tail = nullptr;
        size_ = 0;
    }
    
    // 检查是否为空
    bool empty() const {
        return head == nullptr;
    }
    
    // 获取大小
    size_t size() const {
        return size_;
    }
    
    // 在头部插入元素
    void push_front(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        newNode->next = head;
        head = newNode;
        if (tail == nullptr) {
            tail = head;
        }
        size_++;
    }
    
    // 在尾部插入元素
    void push_back(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        if (tail == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        size_++;
    }
    
    // 在指定位置插入元素
    void insert(size_t position, const T& value) {
        if (position > size_) {
            throw out_of_range("Position out of range");
        }
        
        if (position == 0) {
            push_front(value);
            return;
        }
        
        if (position == size_) {
            push_back(value);
            return;
        }
        
        Node<T>* current = head;
        for (size_t i = 0; i < position - 1; ++i) {
            current = current->next;
        }
        
        Node<T>* newNode = new Node<T>(value);
        newNode->next = current->next;
        current->next = newNode;
        size_++;
    }
    
    // 删除头部元素
    void pop_front() {
        if (empty()) {
            throw runtime_error("List is empty");
        }
        
        Node<T>* temp = head;
        head = head->next;
        delete temp;
        
        if (head == nullptr) {
            tail = nullptr;
        }
        size_--;
    }
    
    // 删除尾部元素
    void pop_back() {
        if (empty()) {
            throw runtime_error("List is empty");
        }
        
        if (head == tail) {
            delete head;
            head = tail = nullptr;
        } else {
            Node<T>* current = head;
            while (current->next != tail) {
                current = current->next;
            }
            delete tail;
            tail = current;
            tail->next = nullptr;
        }
        size_--;
    }
    
    // 删除指定位置的元素
    void remove(size_t position) {
        if (position >= size_) {
            throw out_of_range("Position out of range");
        }
        
        if (position == 0) {
            pop_front();
            return;
        }
        
        Node<T>* current = head;
        for (size_t i = 0; i < position - 1; ++i) {
            current = current->next;
        }
        
        Node<T>* temp = current->next;
        current->next = temp->next;
        
        if (temp == tail) {
            tail = current;
        }
        
        delete temp;
        size_--;
    }
    
    // 获取指定位置的元素
    T& at(size_t position) {
        if (position >= size_) {
            throw out_of_range("Position out of range");
        }
        
        Node<T>* current = head;
        for (size_t i = 0; i < position; ++i) {
            current = current->next;
        }
        
        return current->data;
    }
    
    // 查找元素
    Node<T>* find(const T& value) {
        Node<T>* current = head;
        while (current != nullptr) {
            if (current->data == value) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }
    
    // 反转链表
    void reverse() {
        if (size_ <= 1) return;
        
        Node<T>* prev = nullptr;
        Node<T>* current = head;
        Node<T>* next = nullptr;
        
        tail = head;  // 更新尾节点
        
        while (current != nullptr) {
            next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }
        
        head = prev;
    }
    
    // 打印链表
    void print() const {
        Node<T>* current = head;
        cout << "链表内容: ";
        while (current != nullptr) {
            cout << current->data;
            if (current->next != nullptr) {
                cout << " -> ";
            }
            current = current->next;
        }
        cout << " -> nullptr" << endl;
        cout << "大小: " << size_ << endl;
    }
};

// 测试函数
void testLinkedList() {
    cout << "=== 链表测试 ===" << endl;
    
    LinkedList<int> list;
    
    // 测试插入操作
    cout << "\n1. 测试插入操作:" << endl;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_front(0);
    list.insert(2, 10);
    list.print();
    
    // 测试访问操作
    cout << "\n2. 测试访问操作:" << endl;
    cout << "位置0的元素: " << list.at(0) << endl;
    cout << "位置2的元素: " << list.at(2) << endl;
    
    // 测试查找操作
    cout << "\n3. 测试查找操作:" << endl;
    Node<int>* found = list.find(10);
    if (found) {
        cout << "找到元素10" << endl;
    }
    
    // 测试删除操作
    cout << "\n4. 测试删除操作:" << endl;
    list.pop_front();
    list.pop_back();
    list.remove(1);
    list.print();
    
    // 测试反转操作
    cout << "\n5. 测试反转操作:" << endl;
    list.reverse();
    list.print();
    
    // 测试拷贝构造
    cout << "\n6. 测试拷贝构造:" << endl;
    LinkedList<int> list2 = list;
    list2.print();
    
    // 测试赋值操作
    cout << "\n7. 测试赋值操作:" << endl;
    LinkedList<int> list3;
    list3 = list;
    list3.print();
}

// 主函数
int main() {
    try {
        testLinkedList();
    } catch (const exception& e) {
        cerr << "错误: " << e.what() << endl;
        return 1;
    }
    
    cout << "\n=== 测试完成 ===" << endl;
    return 0;
}
