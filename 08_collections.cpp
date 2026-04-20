// === 模块八：集合框架（STL） ===
// 本文件覆盖：vector/list/deque/set/map/unordered_map、迭代器、泛型、模板、STL算法
// 编译：g++ -std=c++17 08_collections.cpp -o 08_collections.exe

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <array>
#include <algorithm>
#include <iterator>
#include <functional>
#include <numeric>   // accumulate

// ============================================================
// Q1: vector vs list vs deque 有什么区别？
// ============================================================
void q1_vector_list_deque() {
    // std::vector：动态数组
    // - 优点：O(1) 随机访问，内存连续（CPU 缓存友好）
    // - 缺点：中间插入/删除 O(n)，扩容有代价
    std::vector<int> vec = {1, 2, 3, 4, 5};
    vec.push_back(6);
    vec.insert(vec.begin() + 2, 99);  // 中间插入 O(n)
    std::cout << "Q1: vector[3] = " << vec[3] << std::endl;
    std::cout << "Q1: vector.capacity() = " << vec.capacity() << std::endl;

    // std::list：双向链表
    // - 优点：O(1) 插入/删除（已知位置），不支持随机访问
    // - 缺点：无缓存友好，额外内存存指针
    std::list<int> lst = {1, 2, 3};
    lst.push_back(4);
    lst.push_front(0);
    auto it = lst.begin();
    ++it; ++it;
    lst.insert(it, 99);  // O(1) 插入
    std::cout << "Q1: list size = " << lst.size() << std::endl;

    // std::deque：双端队列（分段连续）
    // - 优点：两端 O(1) 插入/删除，随机访问（略慢于 vector）
    // - 缺点：中间操作慢，内存不连续
    std::deque<int> deq = {1, 2, 3};
    deq.push_front(0);
    deq.push_back(4);
    std::cout << "Q1: deque[2] = " << deq[2] << std::endl;

    // 选择指南：
    // - 大多数场景：vector（默认选择）
    // - 需要大量中间插入/删除：list
    // - 需要两端操作：deque

    // 深入原理：
    // - vector: 单块内存，capacity 翻倍策略（均摊 O(1)）
    // - list: 节点间通过指针链接
    // - deque: 多块固定大小内存块 + 映射表，两端操作只需映射表操作

    // 对比 Python：
    // Python list = vector + 一些额外功能（可存任意类型）
    // Python collections.deque = C++ deque
    // 对比 Rust：
    // Rust: Vec<T> = vector，VecDeque<T> = deque，LinkedList<T> = list
    std::cout << "Q1: 默认用 vector，需要中间操作用 list，两端操作用 deque" << std::endl;
}


// 对比 Go：
// // 单行注释（与 C++ 相同）
// /* 块注释 */
// Go 没有预处理器和宏，const/var/func 是语言关键字
// Go 的 error 作为返回值而非异常，风格更接近 C
// 对比 Lua：
// -- 单行注释（Lua 只有这一种注释风格）
// --[[ 多行注释 ]]--（块注释）
// Lua 没有预处理指令，没有编译器宏，一切皆表// ============================================================
// Q2: set 和 map 的底层实现是什么？
// ============================================================
void q2_set_map() {
    // std::set：有序集合（红黑树实现）
    // - 特点：自动排序，元素唯一，无重复
    // - 操作：insert/erase/find O(log n)
    std::set<int> s = {5, 2, 8, 1, 9};
    s.insert(3);
    s.erase(2);
    std::cout << "Q2: set: ";
    for (int x : s) std::cout << x << " ";  // 1 3 5 8 9
    std::cout << std::endl;

    auto it_s = s.find(5);
    if (it_s != s.end()) {
        std::cout << "Q2: 找到 5" << std::endl;
    }

    // std::map：有序键值对（红黑树实现）
    std::map<std::string, int> m = {
        {"Alice", 25},
        {"Bob", 30},
        {"Charlie", 35}
    };
    m["David"] = 40;                    // 下标操作（不存在则插入）
    m.erase("Bob");
    std::cout << "Q2: map: ";
    for (const auto& [k, v] : m) {       // C++17 结构化绑定
        std::cout << k << ":" << v << " ";
    }
    std::cout << std::endl;

    // lower_bound / upper_bound
    auto lb = m.lower_bound("C");
    if (lb != m.end()) {
        std::cout << "Q2: lower_bound('C') = " << lb->first << std::endl;
    }

    // std::multiset / std::multimap：允许重复键
    std::multiset<int> ms = {1, 2, 2, 3, 3, 3};
    std::cout << "Q2: multiset count(3) = " << ms.count(3) << std::endl;

    // 深入原理：
    // - std::set/map 底层是红黑树（自平衡二叉搜索树）
    // - 红黑树：O(log n) 插入/删除/查找，保证树高 ~log n
    // - 有序性保证：中序遍历有序

    // 对比 Python：
    // Python: set() = hash set，dict = hash map（无序！直到 Python 3.7+ 保证插入顺序）
    // 对比 Rust：
    // Rust: BTreeSet<T> / BTreeMap<K, V> = 有序版本
    // std::collections::HashSet / HashMap = 无序版本
    std::cout << "Q2: set/map = 红黑树，自动排序，O(log n)" << std::endl;
}


// 对比 Go：
// // 单行注释（与 C++ 相同）
// /* 块注释 */
// Go 没有预处理器和宏，const/var/func 是语言关键字
// Go 的 error 作为返回值而非异常，风格更接近 C
// 对比 Lua：
// -- 单行注释（Lua 只有这一种注释风格）
// --[[ 多行注释 ]]--（块注释）
// Lua 没有预处理指令，没有编译器宏，一切皆表// ============================================================
// Q3: unordered_set 和 unordered_map 是什么？如何使用？
// ============================================================
void q3_unordered_containers() {
    // std::unordered_set：哈希表实现
    // - 优点：平均 O(1) 插入/删除/查找
    // - 缺点：无序，最坏情况 O(n)
    std::unordered_set<int> us = {5, 2, 8, 1, 9};
    us.insert(3);
    us.erase(2);
    std::cout << "Q3: unordered_set: ";
    for (int x : us) std::cout << x << " ";
    std::cout << std::endl;

    // 桶操作（C++11）
    std::cout << "Q3: bucket_count = " << us.bucket_count() << std::endl;
    std::cout << "Q3: load_factor = " << us.load_factor() << std::endl;

    // std::unordered_map：哈希表键值对
    std::unordered_map<std::string, int> um = {
        {"Alice", 25},
        {"Bob", 30}
    };
    um["Charlie"] = 35;
    std::cout << "Q3: unordered_map: ";
    for (const auto& [k, v] : um) {
        std::cout << k << ":" << v << " ";
    }
    std::cout << std::endl;

    // C++20 特性（预览）
    // um.contains("Alice")  // C++20：检查键是否存在

    // 自定义哈希函数
    // struct MyHash {
    //     size_t operator()(const MyType& t) const { return hash(t.key); }
    // };

    // 深入原理：
    // - 哈希表：用哈希函数将键映射到桶
    // - 冲突处理：链地址法（桶内链表 → 红黑树，C++11+）
    // - rehash：当 load_factor > max_load_factor 时触发

    // 对比 Python：
    // Python dict/set = C++ unordered_map/unordered_set
    // Python 哈希函数可自定义（__hash__）
    // 对比 Rust：
    // Rust: HashSet / HashMap 使用 SipHash 抵抗哈希泛洪
    std::cout << "Q3: unordered_map = 哈希表，平均 O(1)，最坏 O(n)" << std::endl;
}


// 对比 Go：
// // 单行注释（与 C++ 相同）
// /* 块注释 */
// Go 没有预处理器和宏，const/var/func 是语言关键字
// Go 的 error 作为返回值而非异常，风格更接近 C
// 对比 Lua：
// -- 单行注释（Lua 只有这一种注释风格）
// --[[ 多行注释 ]]--（块注释）
// Lua 没有预处理指令，没有编译器宏，一切皆表// ============================================================
// Q4: 迭代器的分类和使用方法是什么？
// ============================================================
void q4_iterators() {
    std::vector<int> vec = {10, 20, 30, 40, 50};

    // 迭代器类型
    auto it_begin = vec.begin();
    auto it_end = vec.end();

    // 正向迭代
    std::cout << "Q4: 正向: ";
    for (auto it = it_begin; it != it_end; ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 反向迭代（C++11）
    std::cout << "Q4: 反向: ";
    for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // C++11 范围for（底层使用迭代器）
    for (int x : vec) std::cout << x << " ";
    std::cout << std::endl;

    // 流迭代器（用于 IO）
    std::cout << "Q4: 输入整数（Ctrl+D结束）:" << std::endl;
    // std::vector<int> input_vec((std::istream_iterator<int>(std::cin)),
    //                              std::istream_iterator<int>());

    // 迭代器分类（Category）：
    // InputIterator  ← OutputIterator
    // ForwardIterator ← BidirectionalIterator ← RandomAccessIterator ← ContiguousIterator

    // 随机访问迭代器（vector）
    auto it = vec.begin();
    it += 2;          // O(1) 跳转
    std::cout << "Q4: it+2 = " << *it << std::endl;
    std::cout << "Q4: it[1] = " << it[1] << std::endl;  // 指针算术
    std::cout << "Q4: distance = " << std::distance(vec.begin(), vec.end()) << std::endl;

    // 插入迭代器（适配器）
    std::vector<int> dest;
    std::copy(vec.begin(), vec.end(), std::back_inserter(dest));

    // 反向迭代器转正向
    auto rit = vec.rbegin();
    auto fwd_from_rev = rit.base();  // 转为正向迭代器

    // 深入原理：
    // - 迭代器是容器的视图，封装了指针操作
    // - begin() 返回 iterator，end() 返回 past-the-end 迭代器
    // - C++20 引入 std::ranges，提供了更强大的范围操作

    // 对比 Python：
    // Python 有 __iter__/__next__ 协议，等价于迭代器
    // 对比 Rust：
    // Rust 有 IntoIterator trait，任何实现了它的类型都可迭代
    std::cout << "Q4: 迭代器是 STL 算法与容器之间的桥梁" << std::endl;
}


// 对比 Go：
// // 单行注释（与 C++ 相同）
// /* 块注释 */
// Go 没有预处理器和宏，const/var/func 是语言关键字
// Go 的 error 作为返回值而非异常，风格更接近 C
// 对比 Lua：
// -- 单行注释（Lua 只有这一种注释风格）
// --[[ 多行注释 ]]--（块注释）
// Lua 没有预处理指令，没有编译器宏，一切皆表// ============================================================
// Q5: STL 算法有哪些常用操作？
// ============================================================
void q5_stl_algorithms() {
    std::vector<int> v = {5, 2, 8, 1, 9, 3, 7};

    // 排序
    std::sort(v.begin(), v.end());
    std::cout << "Q5: 排序后: ";
    for (int x : v) std::cout << x << " ";
    std::cout << std::endl;

    // 部分排序
    std::partial_sort(v.begin(), v.begin() + 3, v.end());  // 前3最小
    std::cout << "Q5: 前3最小: " << v[0] << " " << v[1] << " " << v[2] << std::endl;

    // nth_element
    v = {5, 2, 8, 1, 9, 3, 7};
    std::nth_element(v.begin(), v.begin() + 3, v.end());  // 第4小
    std::cout << "Q5: 第4小 = " << v[3] << std::endl;

    // 二分查找
    std::sort(v.begin(), v.end());
    bool found = std::binary_search(v.begin(), v.end(), 5);
    std::cout << "Q5: binary_search(5) = " << found << std::endl;

    auto lb = std::lower_bound(v.begin(), v.end(), 5);
    auto ub = std::upper_bound(v.begin(), v.end(), 5);
    std::cout << "Q5: [lb, ub) = [" << *lb << ", " << *ub << ")" << std::endl;

    // 查找
    v = {1, 2, 3, 4, 5, 3, 6};
    auto it = std::find(v.begin(), v.end(), 3);
    if (it != v.end()) std::cout << "Q5: 找到 3 在位置 " << std::distance(v.begin(), it) << std::endl;

    auto count = std::count(v.begin(), v.end(), 3);
    std::cout << "Q5: 3 的个数 = " << count << std::endl;

    // 条件查找
    it = std::find_if(v.begin(), v.end(), [](int x) { return x > 4; });
    if (it != v.end()) std::cout << "Q5: 第一个 > 4 = " << *it << std::endl;

    // 替换/删除
    v = {1, 2, 3, 4, 5};
    std::replace(v.begin(), v.end(), 3, 33);
    std::cout << "Q5: 替换后: ";
    for (int x : v) std::cout << x << " ";
    std::cout << std::endl;

    // 去重
    v = {1, 1, 2, 2, 3, 3, 3};
    auto last = std::unique(v.begin(), v.end());
    v.erase(last, v.end());
    std::cout << "Q5: 去重后: ";
    for (int x : v) std::cout << x << " ";
    std::cout << std::endl;

    // 变换
    v = {1, 2, 3, 4, 5};
    std::vector<int> squared;
    std::transform(v.begin(), v.end(), std::back_inserter(squared),
                   [](int x) { return x * x; });
    std::cout << "Q5: 平方: ";
    for (int x : squared) std::cout << x << " ";
    std::cout << std::endl;

    // 累加
    int sum = std::accumulate(v.begin(), v.end(), 0);
    std::cout << "Q5: sum = " << sum << std::endl;

    // C++17  reductions
    // std::reduce(v.begin(), v.end(), 0);  // 并行版 accumulate

    // 深入原理：
    // - STL 算法通过迭代器操作，不直接操作容器
    // - 算法分为：非修改序列算法、修改序列算法、排序算法、数值算法
    // - C++17 parallel execution：std::execution::par 等并行版本

    // 对比 Python：
    // Python 有内置函数：sorted(), filter(), map(), reduce()
    // C++ STL 算法类型更丰富，C++20 引入 range 进一步简化
    std::cout << "Q5: STL 算法覆盖大多数常见操作，优先用标准算法" << std::endl;
}

// ============================================================
// Q6: 模板的基本语法和类型推导是什么？
// ============================================================
void q6_templates() {
    // 函数模板
    template<typename T>
    T maximum(T a, T b) {
        return a > b ? a : b;
    }
    std::cout << "Q6: maximum(3, 5) = " << maximum(3, 5) << std::endl;
    std::cout << "Q6: maximum(3.14, 2.71) = " << maximum(3.14, 2.71) << std::endl;
    std::cout << "Q6: maximum(\"apple\", \"banana\") = " << maximum("apple", "banana") << std::endl;

    // 类模板
    template<typename T>
    class Holder {
    public:
        Holder(T val) : value_(val) { }
        T get() const { return value_; }
    private:
        T value_;
    };
    Holder<int> h1(42);
    Holder<std::string> h2("hello");
    std::cout << "Q6: Holder<int> = " << h1.get() << std::endl;
    std::cout << "Q6: Holder<string> = " << h2.get() << std::endl;

    // C++17 类模板参数推导（CTAD）
    Holder h3 = 100;  // 自动推导 Holder<int>
    std::cout << "Q6: CTAD: " << h3.get() << std::endl;

    // 模板参数默认值
    template<typename T = int, int N = 10>
    class FixedArray {
        T data_[N];
    public:
        T& operator[](int i) { return data_[i]; }
        int size() const { return N; }
    };
    FixedArray<> fa;  // 使用默认值：int[10]
    FixedArray<double, 5> fa2;  // double[5]
    std::cout << "Q6: FixedArray size = " << fa.size() << std::endl;
    std::cout << "Q6: FixedArray<double,5> size = " << fa2.size() << std::endl;

    // C++14 constexpr 模板
    template<int N>
    constexpr int factorial = factorial<N-1> * N;
    template<>
    constexpr int factorial<0> = 1;
    static_assert(factorial<5> == 120, "factorial(5) == 120");

    // 深入原理：
    // - 模板是编译时多态（静多态）
    // - 实例化：编译器根据实参生成具体代码（Template Instantiation）
    // - 模板错误：发生在实例化时（"两次查找"规则）

    // 对比 Python：
    // Python 没有编译期模板，类型注解（type hints）只是文档
    // 对比 Rust：
    // Rust 泛型也是编译时单态化（monomorphization）
    // Rust trait 是 C++ 模板约束的更正式形式
    std::cout << "Q6: 模板 = 编译时多态，代码膨胀换类型安全" << std::endl;
}


// 对比 Go：
// // 单行注释（与 C++ 相同）
// /* 块注释 */
// Go 没有预处理器和宏，const/var/func 是语言关键字
// Go 的 error 作为返回值而非异常，风格更接近 C
// 对比 Lua：
// -- 单行注释（Lua 只有这一种注释风格）
// --[[ 多行注释 ]]--（块注释）
// Lua 没有预处理指令，没有编译器宏，一切皆表// ============================================================
// Q7: 泛型编程在 STL 中如何体现？
// ============================================================
void q7_generic_programming() {
    // 泛型：编写与类型无关的代码

    // Lambda 作为泛型谓词
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto evens = std::count_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; });
    std::cout << "Q7: 偶数个数 = " << evens << std::endl;

    // std::function + lambda（类型擦除）
    std::function<bool(int)> is_positive = [](int x) { return x > 0; };
    std::cout << "Q7: is_positive(-5) = " << is_positive(-5) << std::endl;

    // 通用累加器
    auto accumulate_generic = [](auto first, auto last, auto init, auto op) {
        for (auto it = first; it != last; ++it) {
            init = op(init, *it);
        }
        return init;
    };
    std::vector<int> nums = {1, 2, 3, 4, 5};
    int sum = accumulate_generic(nums.begin(), nums.end(), 0,
                                  [](int a, int b) { return a + b; });
    std::cout << "Q7: 泛型累加 sum = " << sum << std::endl;

    // C++17 constexpr if 选择实现
    // template<typename T>
    // void process(const T& val) {
    //     if constexpr (std::is_integral_v<T>) {
    //         // 整数处理
    //     } else if constexpr (std::is_floating_point_v<T>) {
    //         // 浮点处理
    //     }
    // }

    // C++20 concepts（概念）— 更清晰的泛型约束
    // template<typename T>
    //     requires std::integral<T>
    // T add(T a, T b) { return a + b; }

    // 深入原理：
    // - 泛型通过模板实现，实例化时生成具体类型代码
    // - STL 容器和算法都是泛型的，通过迭代器解耦
    // - C++20 concepts 引入了编译期约束，更好的错误信息

    // 对比 Python：
    // Python 动态类型天生泛型，但无编译期检查
    // 对比 Rust：
    // Rust trait = C++ concept（更正式），泛型约束更清晰
    std::cout << "Q7: STL = 泛型编程范式，迭代器连接容器与算法" << std::endl;
}


// 对比 Go：
// // 单行注释（与 C++ 相同）
// /* 块注释 */
// Go 没有预处理器和宏，const/var/func 是语言关键字
// Go 的 error 作为返回值而非异常，风格更接近 C
// 对比 Lua：
// -- 单行注释（Lua 只有这一种注释风格）
// --[[ 多行注释 ]]--（块注释）
// Lua 没有预处理指令，没有编译器宏，一切皆表// ============================================================
// 主函数入口
// ============================================================
int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "C++ 专家教程 - 模块八：集合框架（STL）" << std::endl;
    std::cout << "============================================" << std::endl;

    q1_vector_list_deque();
    q2_set_map();
    q3_unordered_containers();
    q4_iterators();
    q5_stl_algorithms();
    q6_templates();
    q7_generic_programming();

    std::cout << "\n[模块八完成]" << std::endl;
    return 0;
}

// ================================================================
// 【练习题】
// ================================================================
// 1. 对比 `std::vector` 和 `std::list` 在插入、查找、删除操作上的性能差异
// 2. 用 `std::map` 实现一个词频统计程序，读取一段文本输出最高频的 10 个词
// 3. 用 `std::sort` + lambda 对自定义结构体按多个字段排序
// 4. 用 `std::accumulate` 或 `std::reduce` 实现并行归约，统计容器元素之和
// 5. 对比 `std::set` 和 `std::unordered_set` 在查找性能和有序性上的取舍
