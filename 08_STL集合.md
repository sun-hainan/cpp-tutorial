# 模块八：STL 集合

## 学习目标

- 掌握 vector、list、deque 的特点和选择
- 理解 set、map 的红黑树实现
- 掌握 unordered_set、unordered_map 的哈希表实现
- 熟练使用迭代器分类和 STL 算法
- 理解模板的基本语法
- 掌握泛型编程的核心概念

---

## 1. vector vs list vs deque

### 生活类比

> - **vector** 像**连续排布的抽屉柜**——存取任何一个抽屉都很快（O(1)），但要在中间插入新抽屉很慢
> - **list** 像**手链上的珠子**——每个珠子只和前后珠子相连，在任意位置插入很快，但找第 N 个珠子要数一遍
> - **deque** 像**两排背靠背的抽屉**——两端插入都快，中间插入慢

### 概念讲解

| 容器 | 随机访问 | 中间插入 | 两端操作 | 内存 | CPU 缓存 |
|------|---------|---------|---------|------|---------|
| vector | O(1) | O(n) | push_back O(1) | 连续 | 友好 |
| list | O(n) | O(1) | push_front/back O(1) | 分散 | 不友好 |
| deque | O(1)* | O(n) | 两端 O(1) | 分段连续 | 中等 |

### 代码示例

```cpp
// vector：默认首选，O(1) 随机访问，O(n) 中间插入
std::vector<int> vec = {1, 2, 3, 4, 5};
vec.push_back(6);
vec.insert(vec.begin() + 2, 99);  // O(n) 中间插入

// list：双向链表，O(1) 任意位置插入/删除
std::list<int> lst = {1, 2, 3};
lst.push_front(0);  // O(1) 头部插入
auto it = lst.begin(); ++it; ++it;
lst.insert(it, 99);  // O(1) 插入

// deque：双端队列，两端 O(1)
std::deque<int> deq = {1, 2, 3};
deq.push_front(0);  // O(1) 头部插入
deq.push_back(4);   // O(1) 尾部插入
```

### 选择指南

```
选择建议：
✅ 大多数场景 → vector（默认首选）
✅ 大量中间插入/删除 → list
✅ 需要两端操作 → deque
```

---

## 2. set 与 map：红黑树实现

### 生活类比

> **set/map** 就像一个**有序字典**——按拼音字母顺序排列，每次查词都要翻到对应页面附近。适合需要**有序遍历**的场景。

### 概念讲解

| 容器 | 特点 | 底层 | 复杂度 |
|------|------|------|--------|
| std::set | 有序集合，元素唯一 | 红黑树 | O(log n) |
| std::map | 有序键值对 | 红黑树 | O(log n) |
| std::multiset | 有序集合，允许重复 | 红黑树 | O(log n) |
| std::multimap | 有序键值对，允许重复键 | 红黑树 | O(log n) |

### 代码示例

```cpp
// std::set：有序集合
std::set<int> s = {5, 2, 8, 1, 9};
s.insert(3);
s.erase(2);
for (int x : s) std::cout << x << " ";  // 自动排序: 1 3 5 8 9

// std::map：有序键值对
std::map<std::string, int> m = {
    {"Alice", 25}, {"Bob", 30}, {"Charlie", 35}
};
m["David"] = 40;  // 下标操作（不存在则插入）
for (const auto& [k, v] : m) {
    std::cout << k << ":" << v << " ";
}

// lower_bound / upper_bound
auto lb = m.lower_bound("C");  // 第一个 >= "C"
```

---

## 3. unordered_set 与 unordered_map：哈希表实现

### 生活类比

> **unordered_set/map** 就像一个**图书馆的借阅系统**——每本书有一个编号（哈希值），查找时直接翻到对应编号的柜子，而不是按字母顺序一页页翻。

### 概念讲解

| 容器 | 特点 | 底层 | 平均复杂度 | 最坏复杂度 |
|------|------|------|-----------|-----------|
| unordered_set | 无序集合 | 哈希表 | O(1) | O(n) |
| unordered_map | 无序键值对 | 哈希表 | O(1) | O(n) |

### 代码示例

```cpp
// std::unordered_set
std::unordered_set<int> us = {5, 2, 8, 1, 9};
us.insert(3);
us.erase(2);
std::cout << "bucket_count = " << us.bucket_count() << std::endl;

// std::unordered_map
std::unordered_map<std::string, int> um = {
    {"Alice", 25}, {"Bob", 30}
};
um["Charlie"] = 35;
for (const auto& [k, v] : um) {
    std::cout << k << ":" << v << " ";
}
```

### unordered vs ordered 对比

| 操作 | unordered | ordered |
|------|-----------|---------|
| 插入/查找 | O(1) 平均 | O(log n) |
| 遍历顺序 | 哈希表顺序 | 键的排序顺序 |
| 内存 | 额外桶开销 | 较小 |
| 适用场景 | 高性能查找 | 需要有序遍历 |

---

## 4. 迭代器：容器与算法的"桥梁"

### 生活类比

> **迭代器**就像图书馆的**书签**——指向书中的某一页，可以前后翻页，不需要知道书架是怎么排列的。

### 概念讲解

```
迭代器分类（从弱到强）：
InputIterator → ForwardIterator → BidirectionalIterator → RandomAccessIterator → ContiguousIterator
```

| 迭代器类型 | 支持操作 | 容器 |
|-----------|---------|------|
| InputIterator | ++, ==, !=, * | 输入流 |
| ForwardIterator | ++, ==, !=, * | forward_list |
| BidirectionalIterator | ++, -- | list, set, map |
| RandomAccessIterator | ++, --, +, -, [] | vector, deque |
| ContiguousIterator | 连续内存 | vector, array |

### 代码示例

```cpp
std::vector<int> vec = {10, 20, 30, 40, 50};

// 正向迭代
for (auto it = vec.begin(); it != vec.end(); ++it) {
    std::cout << *it << " ";
}

// C++11 范围 for（底层使用迭代器）
for (int x : vec) std::cout << x << " ";

// 反向迭代
for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
    std::cout << *it << " ";
}

// 随机访问
auto it = vec.begin();
it += 2;          // O(1) 跳转
std::cout << *(it + 1) << std::endl;
```

---

## 5. STL 算法：常用操作一览

### 概念讲解

| 类别 | 算法 | 说明 |
|------|------|------|
| 排序 | sort, stable_sort, partial_sort, nth_element | O(n log n) |
| 查找 | find, find_if, binary_search, lower_bound | O(n) / O(log n) |
| 变换 | transform, copy, remove | O(n) |
| 聚合 | accumulate, count, count_if | O(n) |
| 边界 | min, max, min_element | O(n) |

### 代码示例

```cpp
std::vector<int> v = {5, 2, 8, 1, 9, 3, 7};

// 排序
std::sort(v.begin(), v.end());  // 升序
std::sort(v.begin(), v.end(), std::greater<int>());  // 降序

// 二分查找（需要先排序）
std::sort(v.begin(), v.end());
bool found = std::binary_search(v.begin(), v.end(), 5);

// 查找
auto it = std::find(v.begin(), v.end(), 3);
auto cnt = std::count_if(v.begin(), v.end(), [](int x){ return x % 2 == 0; });

// 变换
std::vector<int> squared;
std::transform(v.begin(), v.end(), std::back_inserter(squared),
               [](int x){ return x * x; });

// 累加
int sum = std::accumulate(v.begin(), v.end(), 0);
```

---

## 6. 模板：编译期的"万能模具"

### 生活类比

> **模板**就像一个**3D 打印机的图纸**——定义一次，可以打印出不同材质、不同尺寸的同款产品。

### 概念讲解

```cpp
// 函数模板
template<typename T>
T maximum(T a, T b) {
    return a > b ? a : b;
}
maximum(3, 5);       // T = int
maximum(3.14, 2.71);  // T = double

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

// C++17 类模板参数推导（CTAD）
Holder h3 = 100;  // 自动推导 Holder<int>
```

---

## 7. 泛型编程：STL 的核心思想

### 概念讲解

**泛型** = 编写与**类型无关**的代码

```
STL 泛型核心：
  容器（Container）→ 提供数据存储
      ↓
  迭代器（Iterator）→ 提供遍历接口
      ↓
  算法（Algorithm）→ 操作数据
```

### 代码示例

```cpp
// Lambda 作为泛型谓词
std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
auto evens = std::count_if(v.begin(), v.end(),
                           [](int x){ return x % 2 == 0; });

// std::function + lambda（类型擦除）
std::function<bool(int)> is_positive = [](int x){ return x > 0; };

// C++17 if constexpr 实现编译期分支
auto process = [](auto val) {
    if constexpr (std::is_integral_v<decltype(val)>) {
        return val * 2;
    } else if constexpr (std::is_floating_point_v<decltype(val)>) {
        return val * 2.0;
    }
};
```

---

## 本章小结

| 容器 | 底层 | 查找 | 插入 | 适用场景 |
|------|------|------|------|---------|
| vector | 数组 | O(1) | O(n) | 默认首选 |
| list | 双向链表 | O(n) | O(1) | 大量中间操作 |
| deque | 分段数组 | O(1) | 两端 O(1) | 两端操作 |
| set/map | 红黑树 | O(log n) | O(log n) | 需要有序 |
| unordered_set/map | 哈希表 | O(1) 平均 | O(1) 平均 | 高性能查找 |

---

## 课后练习

1. 对比 `std::vector` 和 `std::list` 在插入、查找、删除操作上的性能差异
2. 用 `std::map` 实现一个词频统计程序，读取一段文本输出最高频的 10 个词
3. 用 `std::sort` + lambda 对自定义结构体按多个字段排序
4. 用 `std::accumulate` 或 `std::reduce` 实现并行归约，统计容器元素之和
5. 对比 `std::set` 和 `std::unordered_set` 在查找性能和有序性上的取舍
