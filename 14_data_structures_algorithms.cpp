// === 模块十四：数据结构与算法 ===
// 本文件覆盖：链表、栈、队列、哈希表、树、二叉树、红黑树、排序算法、查找算法、复杂度分析
// 编译：g++ -std=c++17 14_data_structures_algorithms.cpp -o 14_data_structures_algorithms.exe

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <chrono>
#include <cassert>

// ============================================================
// Q1: 如何实现链表？
// ============================================================
template<typename T>
struct ListNode {
    T data;
    ListNode* next;
    ListNode(const T& d) : data(d), next(nullptr) { }
};

template<typename T>
class LinkedList {
public:
    LinkedList() : head_(nullptr), size_(0) { }

    ~LinkedList() {
        while (head_) {
            ListNode<T>* tmp = head_;
            head_ = head_->next;
            delete tmp;
        }
    }

    void push_front(const T& val) {
        ListNode<T>* node = new ListNode<T>(val);
        node->next = head_;
        head_ = node;
        ++size_;
    }

    void push_back(const T& val) {
        ListNode<T>* node = new ListNode<T>(val);
        if (!head_) {
            head_ = node;
        } else {
            ListNode<T>* cur = head_;
            while (cur->next) cur = cur->next;
            cur->next = node;
        }
        ++size_;
    }

    bool find(const T& val) const {
        ListNode<T>* cur = head_;
        while (cur) {
            if (cur->data == val) return true;
            cur = cur->next;
        }
        return false;
    }

    void reverse() {
        ListNode<T>* prev = nullptr;
        ListNode<T>* cur = head_;
        while (cur) {
            ListNode<T>* next = cur->next;
            cur->next = prev;
            prev = cur;
            cur = next;
        }
        head_ = prev;
    }

    void print() const {
        ListNode<T>* cur = head_;
        while (cur) {
            std::cout << cur->data << " ";
            cur = cur->next;
        }
        std::cout << std::endl;
    }

    size_t size() const { return size_; }

private:
    ListNode<T>* head_;
    size_t size_;
};

void q1_linked_list() {
    LinkedList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_front(0);
    list.push_back(3);
    std::cout << "Q1: 链表: ";
    list.print();

    std::cout << "Q1: 查找 2: " << list.find(2) << std::endl;
    std::cout << "Q1: 查找 99: " << list.find(99) << std::endl;

    list.reverse();
    std::cout << "Q1: 反转后: ";
    list.print();

    // 深入原理：
    // - 链表：节点通过指针链接
    // - push_front: O(1)，push_back: O(n)（无尾指针）
    // - 访问第 i 个元素：O(n)
    // - 内存：节点分散，不连续

    // 对比 Python：
    // Python list = 动态数组（不是链表）
    // Python collections.deque = 双向链表
    // 对比 Rust：
    // Rust: Vec<T> = vector，LinkedList<T> = 链表
    std::cout << "Q1: 链表插入/删除 O(1)，随机访问 O(n)" << std::endl;
}

// ============================================================
// Q2: 栈和队列的实现和应用场景
// ============================================================
void q2_stack_queue() {
    // std::stack（LIFO）
    std::stack<int> s;
    s.push(1); s.push(2); s.push(3);
    std::cout << "Q2: stack top = " << s.top() << std::endl;
    s.pop();
    std::cout << "Q2: pop 后 top = " << s.top() << std::endl;

    // 栈应用：括号匹配
    auto is_balanced = [](const std::string& str) -> bool {
        std::stack<char> st;
        for (char c : str) {
            if (c == '(' || c == '[' || c == '{') {
                st.push(c);
            } else if (c == ')' || c == ']' || c == '}') {
                if (st.empty()) return false;
                char top = st.top(); st.pop();
                if ((c == ')' && top != '(') ||
                    (c == ']' && top != '[') ||
                    (c == '}' && top != '{')) {
                    return false;
                }
            }
        }
        return st.empty();
    };
    std::cout << "Q2: \"([])\" 平衡=" << is_balanced("([])") << std::endl;
    std::cout << "Q2: \"([)]\" 平衡=" << is_balanced("([)") << std::endl;

    // std::queue（FIFO）
    std::queue<int> q;
    q.push(1); q.push(2); q.push(3);
    std::cout << "Q2: queue front = " << q.front() << std::endl;
    q.pop();
    std::cout << "Q2: pop 后 front = " << q.front() << std::endl;

    // std::deque 是 queue 的默认底层容器
    // std::deque 是分段连续数组，两端操作 O(1)

    // 优先队列（堆）
    std::priority_queue<int> pq;  // 大顶堆
    pq.push(5); pq.push(1); pq.push(3);
    std::cout << "Q2: priority_queue top = " << pq.top() << std::endl;

    // 小顶堆
    std::priority_queue<int, std::vector<int>, std::greater<int>> min_pq;
    min_pq.push(5); min_pq.push(1); min_pq.push(3);
    std::cout << "Q2: min_pq top = " << min_pq.top() << std::endl;

    // 深入原理：
    // - 栈：push/pop/top O(1)
    // - 队列：enqueue/dequeue/front O(1)
    // - 优先队列：push/pop O(log n)，top O(1)

    // 对比 Python：
    // Python: list 作为栈，collections.deque 作为队列
    // 对比 Rust：
    // Rust: VecDeque<T> = deque，BinaryHeap<T> = 堆
    std::cout << "Q2: 栈=括号匹配/函数调用，队列=任务调度，堆=TopK" << std::endl;
}

// ============================================================
// Q3: 哈希表的实现原理
// ============================================================
void q3_hash_table() {
    // std::unordered_map 底层是哈希表

    std::unordered_map<std::string, int> dict = {
        {"apple", 1},
        {"banana", 2},
        {"cherry", 3}
    };

    // 插入和查找
    dict["date"] = 4;
    auto it = dict.find("banana");
    if (it != dict.end()) {
        std::cout << "Q3: banana = " << it->second << std::endl;
    }

    // 遍历
    std::cout << "Q3: 遍历: ";
    for (const auto& [k, v] : dict) {
        std::cout << k << ":" << v << " ";
    }
    std::cout << std::endl;

    // 自定义哈希函数
    // struct MyHash {
    //     size_t operator()(const Key& k) const {
    //         return std::hash<Key>{}(k);
    //     }
    // };

    // 哈希冲突处理：链地址法
    std::cout << "Q3: bucket_count = " << dict.bucket_count() << std::endl;

    // 深入原理：
    // - 哈希函数将键映射到桶（0 ~ bucket_count-1）
    // - 冲突：多个键映射到同一桶（链地址法解决）
    // - 负载因子 > max_load_factor 时 rehash
    // - 查找/插入/删除平均 O(1)，最坏 O(n)（大量冲突）

    // 对比 Python：
    // Python dict 使用开放地址法（类似）
    // 对比 Rust：
    // Rust: HashMap<K, V> 使用 SipHash（抗哈希泛洪）
    std::cout << "Q3: 哈希表平均 O(1)，是键值存储的首选" << std::endl;
}

// ============================================================
// Q4: 树和二叉树如何遍历？
// ============================================================
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) { }
};

void q4_tree_traversal() {
    // 构建二叉树
    //       1
    //      / \
    //     2   3
    //    / \   \
    //   4   5   6
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->right->right = new TreeNode(6);

    // 前序遍历（根-左-右）
    auto preorder = [](auto&& self, TreeNode* node) -> void {
        if (!node) return;
        std::cout << node->val << " ";
        self(self, node->left);
        self(self, node->right);
    };
    std::cout << "Q4: 前序: ";
    preorder(preorder, root);
    std::cout << std::endl;

    // 中序遍历（左-根-右）
    auto inorder = [](auto&& self, TreeNode* node) -> void {
        if (!node) return;
        self(self, node->left);
        std::cout << node->val << " ";
        self(self, node->right);
    };
    std::cout << "Q4: 中序: ";
    inorder(inorder, root);
    std::cout << std::endl;

    // 后序遍历（左-右-根）
    auto postorder = [](auto&& self, TreeNode* node) -> void {
        if (!node) return;
        self(self, node->left);
        self(self, node->right);
        std::cout << node->val << " ";
    };
    std::cout << "Q4: 后序: ";
    postorder(postorder, root);
    std::cout << std::endl;

    // 层序遍历（队列）
    std::queue<TreeNode*> q;
    q.push(root);
    std::cout << "Q4: 层序: ";
    while (!q.empty()) {
        TreeNode* node = q.front(); q.pop();
        std::cout << node->val << " ";
        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }
    std::cout << std::endl;

    // 递归 vs 迭代：递归简洁但有栈溢出风险

    // 深入原理：
    // - 前序：创建/复制树
    // - 中序：二叉搜索树得到有序序列
    // - 后序：释放树（子节点先于父节点）
    // - 层序：广度优先（BFS）

    // 对比 Python：
    // Python: collections.deque 实现 BFS，递归同样适用
    // 对比 Rust：
    // Rust: Box<Node> 管理树节点，没有递归 limit
    std::cout << "Q4: 前序/中序/后序 + 层序 = 树的四大遍历" << std::endl;
}

// ============================================================
// Q5: 红黑树是什么？std::map 底层如何实现？
// ============================================================
void q5_red_black_tree() {
    // 红黑树：自平衡二叉搜索树
    // 性质：
    // 1. 节点非红即黑
    // 2. 根节点是黑色
    // 3. 叶子节点（NIL）是黑色
    // 4. 红节点的子节点必须是黑色
    // 5. 从任一节点到叶子的路径黑高相同

    // std::map 底层是红黑树（std::set 相同）
    std::map<int, std::string> m = {
        {3, "three"},
        {1, "one"},
        {4, "four"},
        {1, "ONE"}  // 键重复，不插入（保留原有）
    };
    std::cout << "Q5: map: ";
    for (const auto& [k, v] : m) {
        std::cout << k << ":" << v << " ";
    }
    std::cout << std::endl;

    // 红黑树操作复杂度：O(log n)
    // - 插入：最多 2 次旋转，O(log n)
    // - 删除：最多 3 次旋转，O(log n)

    // std::map vs std::unordered_map
    // | 操作     | map        | unordered_map |
    // |---------|-------------|---------------|
    // | 查找     | O(log n)   | O(1) 平均     |
    // | 有序遍历   | 支持        | 不支持         |
    // | 稳定迭代器  | 支持        | 可能失效       |
    // | 内存开销    | 较小        | 较大（桶）      |

    // 深入原理：
    // - 红黑树通过着色和旋转保持近似平衡
    // - 最长路径 ≤ 2 × 最短路径（平衡保证）
    // - 用于 Linux 内核调度器、epoll、map/set

    // 对比 Python：
    // Python dict = 哈希表（无序，Python 3.7+ 保证插入顺序）
    // 对比 Rust：
    // Rust: BTreeMap = B树实现的 map
    // Rust: std::collections::HashMap = 哈希表
    std::cout << "Q5: 红黑树 O(log n) 有序，AVL 树更严格但插入/删除更慢" << std::endl;
}

// ============================================================
// Q6: 常见排序算法有哪些？复杂度分析
// ============================================================
void q6_sorting_algorithms() {
    std::vector<int> arr = {64, 34, 25, 12, 22, 11, 90};

    // 1. 冒泡排序 O(n²) - 稳定
    auto bubble_sort = [](std::vector<int>& a) {
        int n = static_cast<int>(a.size());
        for (int i = 0; i < n - 1; ++i) {
            for (int j = 0; j < n - i - 1; ++j) {
                if (a[j] > a[j + 1]) std::swap(a[j], a[j + 1]);
            }
        }
    };

    // 2. 选择排序 O(n²) - 不稳定
    auto selection_sort = [](std::vector<int>& a) {
        int n = static_cast<int>(a.size());
        for (int i = 0; i < n - 1; ++i) {
            int min_idx = i;
            for (int j = i + 1; j < n; ++j) {
                if (a[j] < a[min_idx]) min_idx = j;
            }
            std::swap(a[i], a[min_idx]);
        }
    };

    // 3. 插入排序 O(n²) - 稳定，小数据量高效
    auto insertion_sort = [](std::vector<int>& a) {
        for (int i = 1; i < static_cast<int>(a.size()); ++i) {
            int key = a[i];
            int j = i - 1;
            while (j >= 0 && a[j] > key) {
                a[j + 1] = a[j];
                --j;
            }
            a[j + 1] = key;
        }
    };

    // 4. 归并排序 O(n log n) - 稳定，分治
    auto merge = [](auto&& self, std::vector<int>& a, int l, int m, int r) -> void {
        std::vector<int> left(a.begin() + l, a.begin() + m + 1);
        std::vector<int> right(a.begin() + m + 1, a.begin() + r + 1);
        int i = 0, j = 0, k = l;
        while (i < static_cast<int>(left.size()) && j < static_cast<int>(right.size())) {
            a[k++] = (left[i] <= right[j]) ? left[i++] : right[j++];
        }
        while (i < static_cast<int>(left.size())) a[k++] = left[i++];
        while (j < static_cast<int>(right.size())) a[k++] = right[j++];
    };
    auto merge_sort = [&](auto&& self, std::vector<int>& a, int l, int r) -> void {
        if (l < r) {
            int m = l + (r - l) / 2;
            self(self, a, l, m);
            self(self, a, m + 1, r);
            merge(merge, a, l, m, r);
        }
    };

    // 排序算法对比表：
    // | 算法        | 时间复杂度     | 空间复杂度 | 稳定 |
    // |------------|--------------|---------|------|
    // | 冒泡排序     | O(n²)        | O(1)    | 稳定 |
    // | 选择排序     | O(n²)        | O(1)    | 不稳定 |
    // | 插入排序     | O(n²)        | O(1)    | 稳定 |
    // | 归并排序     | O(n log n)   | O(n)    | 稳定 |
    // | 快速排序     | O(n log n)均摊 | O(log n) | 不稳定 |
    // | 堆排序       | O(n log n)   | O(1)    | 不稳定 |

    // std::sort = introsort（快排+堆排+插入排）
    std::vector<int> test = {5, 2, 8, 1, 9, 3};
    std::sort(test.begin(), test.end());
    std::cout << "Q6: std::sort: ";
    for (int x : test) std::cout << x << " ";
    std::cout << std::endl;

    std::cout << "Q6: C++17 std::sort 是工业级排序" << std::endl;
}

// ============================================================
// Q7: 查找算法和复杂度分析
// ============================================================
void q7_search_algorithms() {
    std::vector<int> sorted = {1, 3, 5, 7, 9, 11, 13, 15};

    // 1. 顺序查找 O(n)
    auto sequential_search = [](const std::vector<int>& arr, int target) -> int {
        for (int i = 0; i < static_cast<int>(arr.size()); ++i) {
            if (arr[i] == target) return i;
        }
        return -1;
    };
    std::cout << "Q7: 顺序查找 7: pos=" << sequential_search(sorted, 7) << std::endl;
    std::cout << "Q7: 顺序查找 8: pos=" << sequential_search(sorted, 8) << std::endl;

    // 2. 二分查找 O(log n)
    auto binary_search = [](const std::vector<int>& arr, int target) -> int {
        int left = 0, right = static_cast<int>(arr.size()) - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (arr[mid] == target) return mid;
            else if (arr[mid] < target) left = mid + 1;
            else right = mid - 1;
        }
        return -1;
    };
    std::cout << "Q7: 二分查找 7: pos=" << binary_search(sorted, 7) << std::endl;
    std::cout << "Q7: 二分查找 8: pos=" << binary_search(sorted, 8) << std::endl;

    // 3. 哈希查找 O(1) 平均
    std::unordered_map<int, int> hash;
    for (int i = 0; i < static_cast<int>(sorted.size()); ++i) {
        hash[sorted[i]] = i;
    }
    auto it = hash.find(7);
    std::cout << "Q7: 哈希查找 7: pos=" << (it != hash.end() ? it->second : -1) << std::endl;

    // 复杂度分析总结：
    // | 数据结构       | 查找       | 插入     | 删除     |
    // |--------------|-----------|---------|---------|
    // | 数组（有序）    | O(log n)  | O(n)    | O(n)    |
    // | 数组（无序）    | O(n)      | O(1)    | O(n)    |
    // | 链表          | O(n)      | O(1)    | O(1)*   |
    // | 哈希表        | O(1) 均摊   | O(1) 均摊 | O(1) 均摊 |
    // | BST          | O(log n)  | O(log n)| O(log n)|
    // | 红黑树        | O(log n)  | O(log n)| O(log n)|

    // 对比 Python：
    // Python: list.index() = 顺序查找，bisect = 二分查找
    // 对比 Rust：
    // Rust: Vec.index() = O(1)，BTreeMap / HashMap

    std::cout << "Q7: 选择查找算法要权衡数据规模和操作频率" << std::endl;
}

// ============================================================
// 主函数入口
// ============================================================
int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "C++ 专家教程 - 模块十四：数据结构与算法" << std::endl;
    std::cout << "============================================" << std::endl;

    q1_linked_list();
    q2_stack_queue();
    q3_hash_table();
    q4_tree_traversal();
    q5_red_black_tree();
    q6_sorting_algorithms();
    q7_search_algorithms();

    std::cout << "\n[模块十四完成]" << std::endl;
    return 0;
}
