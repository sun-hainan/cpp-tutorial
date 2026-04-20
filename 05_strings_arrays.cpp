// === 模块五：字符串与数组 ===
// 本文件覆盖：string、C风格字符串、字符数组、一维数组、二维数组、
//            指针数组、增删改查、排序、二分查找
// 编译：g++ -std=c++17 05_strings_arrays.cpp -o 05_strings_arrays.exe

#include <iostream>
#include <string>
#include <vector>
#include <cstring>     // C 风格字符串函数
#include <algorithm>   // sort, binary_search
#include <array>       // C++11 std::array

// ============================================================
// Q1: std::string 和 C 风格字符串有什么区别？
// ============================================================
void q1_string_types() {
    // C++ std::string（推荐）
    std::string cpp_str = "Hello";
    cpp_str += " World";                     // 拼接
    cpp_str.append("!");
    std::cout << "Q1: std::string = " << cpp_str << std::endl;
    std::cout << "Q1: length = " << cpp_str.length() << ", size = " << cpp_str.size() << std::endl;

    // 常用操作
    std::string sub = cpp_str.substr(0, 5);   // "Hello"
    size_t pos = cpp_str.find("World");       // 查找子串
    std::cout << "Q1: find 'World' at pos = " << pos << std::endl;

    // 替换（C++11 起）
    cpp_str.replace(6, 5, "C++");              // "Hello C++!"
    std::cout << "Q1: after replace = " << cpp_str << std::endl;

    // C++17 string_view（C++17 不拷贝的字符串视图）
    std::string_view sv = cpp_str;            // 零拷贝视图
    std::cout << "Q1: string_view = " << sv << std::endl;

    // C 风格字符串（char 数组，以 '\0' 结尾）
    const char* c_str = "Hello";              // 字符串字面量，存储在只读段
    // c_str[0] = 'J';  // ❌ 未定义行为：字面量是只读的

    char mutable_str[] = "Hello";             // 可变字符数组
    mutable_str[0] = 'J';                     // ✅ 可以修改
    std::cout << "Q1: mutable_str = " << mutable_str << std::endl;

    // C 风格字符串长度（手动计算）
    size_t len = std::strlen(c_str);         // 不含 '\0'
    std::cout << "Q1: strlen = " << len << std::endl;

    // C++ string 与 C string 互转
    std::string cpp = c_str;                  // C → C++
    const char* c = cpp.c_str();             // C++ → C（返回 const char*）
    char c_array[100];
    std::strncpy(c_array, cpp.c_str(), sizeof(c_array) - 1);
    c_array[sizeof(c_array) - 1] = '\0';      // 确保以 '\0' 结尾

    // 深入原理：
    // - std::string: 堆上分配的动态字符串，包含 capacity/size/ptr
    // - 字符串字面量: 存储在.rodata（只读数据段），编译器可能合并相同字面量
    // - 字符数组: 栈上分配（如未超出栈大小）

    // 对比 Python：
    // Python str 是不可变 Unicode 对象，与 C++ string 有根本区别
    // 对比 Rust：
    // Rust: String（堆，可变）/ &str（字符串切片，视图）
    // let s1: String = String::from("hello");
    // let s2: &str = "hello";
    std::cout << "Q1: 优先使用 std::string，C 风格字符串仅用于 C 兼容" << std::endl;
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
// Q2: 一维数组和 std::array 的区别是什么？
// ============================================================
void q2_one_dim_array() {
    // C 风格数组（长度必须是编译期常量，除非动态分配）
    int raw_arr[5] = {1, 2, 3, 4, 5};  // 栈上分配
    std::cout << "Q2: C 数组: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << raw_arr[i] << " ";
    }
    std::cout << std::endl;

    // std::array（C++11，比 C 数组更安全）
    std::array<int, 5> cpp_arr = {1, 2, 3, 4, 5};
    std::cout << "Q2: std::array: ";
    for (const auto& x : cpp_arr) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    // std::array 优势：知道大小、支持迭代器、支持 STL 算法
    std::cout << "Q2: array.size() = " << cpp_arr.size() << std::endl;
    std::cout << "Q2: array.max_size() = " << cpp_arr.max_size() << std::endl;

    // C++17 结构化绑定
    auto [first, second, third, fourth, fifth] = cpp_arr;
    std::cout << "Q2: 结构化绑定: first=" << first << ", second=" << second << std::endl;

    // 动态数组（C 风格，需要手动管理内存）
    int* dyn_arr = new int[5];
    for (int i = 0; i < 5; ++i) dyn_arr[i] = i + 1;
    // delete[] dyn_arr;  // 必须手动释放，容易泄漏

    // C++11 unique_ptr 管理动态数组
    auto smart_arr = std::make_unique<int[]>(5);
    for (int i = 0; i < 5; ++i) smart_arr[i] = i * 2;
    std::cout << "Q2: smart_arr[2] = " << smart_arr[2] << std::endl;
    // 自动释放，无需手动 delete

    // 深入原理：
    // - C 数组：连续的内存块，无元数据
    // - std::array: 包装 C 数组，但多了 size() 等成员函数
    // - 数组名 decay：传给函数时退化为指针，失去 size 信息
    // - void func(int arr[5]) → 实际上是 void func(int* arr)

    // 对比 Python：
    // Python list 是动态数组，支持任意类型（对象引用数组）
    // 对比 Rust：
    // Rust: [i32; 5] 是固定长度数组，&[i32] 是切片
    std::cout << "Q2: 优先使用 std::array 或 vector" << std::endl;
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
// Q3: 二维数组如何定义和使用？
// ============================================================
void q3_two_dim_array() {
    // C 风格二维数组
    int matrix[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };
    std::cout << "Q3: C 二维数组:" << std::endl;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << matrix[i][j] << "\t";
        }
        std::cout << std::endl;
    }

    // 内存布局：行优先（Row-Major）
    // matrix[0][0], matrix[0][1], ..., matrix[0][3], matrix[1][0], ...
    std::cout << "Q3: 内存地址连续: &matrix[0][0]=" << &matrix[0][0]
              << ", &matrix[0][1]=" << &matrix[0][1] << std::endl;

    // C++11 std::array 二维
    std::array<std::array<int, 4>, 3> cpp_matrix = {{
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    }};

    // 动态二维数组（指针数组）
    int rows = 3, cols = 4;
    int** dyn_matrix = new int*[rows];
    for (int i = 0; i < rows; ++i) {
        dyn_matrix[i] = new int[cols];
        for (int j = 0; j < cols; ++j) {
            dyn_matrix[i][j] = (i * cols) + j + 1;
        }
    }
    // 释放内存
    for (int i = 0; i < rows; ++i) {
        delete[] dyn_matrix[i];
    }
    delete[] dyn_matrix;

    // C++17 一维 vector 模拟二维（内存更连续）
    std::vector<int> flat_matrix(rows * cols);
    for (int i = 0; i < rows * cols; ++i) {
        flat_matrix[i] = i + 1;
    }
    auto at = [&](int r, int c) { return flat_matrix[r * cols + c]; };
    std::cout << "Q3: 扁平化矩阵 at(1,2) = " << at(1, 2) << std::endl;

    // 深入原理：
    // - 二维数组在内存中是连续的一维块
    // - int matrix[3][4] → 编译器计算偏移量: i*4+j
    // - 指针数组的每一行是独立分配的，可能不连续

    // 对比 Python：
    // Python: [[1,2,3], [4,5,6]] 是 list of lists，不保证内存连续
    // 对比 Rust：
    // Rust: [[i32; 4]; 3] 是真正的二维数组，内存连续
    std::cout << "Q3: 二维数组内存行优先连续，按行遍历效率更高" << std::endl;
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
// Q4: 数组的增删改查如何实现？
// ============================================================
void q4_array_crud() {
    // 使用 std::vector 实现动态数组的增删改查
    std::vector<int> vec = {10, 20, 30};

    // 增：末尾追加
    vec.push_back(40);
    vec.emplace_back(50);          // C++11 更高效，原地构造
    std::cout << "Q4: push_back后: ";
    for (int v : vec) std::cout << v << " ";
    std::cout << std::endl;

    // 删：按位置删除
    vec.erase(vec.begin() + 1);    // 删除第二个元素
    std::cout << "Q4: erase后: ";
    for (int v : vec) std::cout << v << " ";
    std::cout << std::endl;

    // 改：按索引修改
    vec[0] = 100;
    vec.at(1) = 200;               // at() 会做边界检查
    std::cout << "Q4: 修改后: ";
    for (int v : vec) std::cout << v << " ";
    std::cout << std::endl;

    // 查：按值查找
    auto it = std::find(vec.begin(), vec.end(), 30);
    if (it != vec.end()) {
        std::cout << "Q4: 找到 30，位置 = " << std::distance(vec.begin(), it) << std::endl;
    }

    // 按条件查找
    it = std::find_if(vec.begin(), vec.end(), [](int x) { return x > 100; });
    if (it != vec.end()) {
        std::cout << "Q4: 第一个 > 100 的值 = " << *it << std::endl;
    }

    // 插入到指定位置
    vec.insert(vec.begin() + 1, 150);
    std::cout << "Q4: insert后: ";
    for (int v : vec) std::cout << v << " ";
    std::cout << std::endl;

    // 深入原理：
    // - vector::push_back 可能触发 reallocation（容量翻倍，O(n)）
    // - reserve() 预分配可以避免 reallocation
    // - erase/insert 在中间是 O(n)（需要移动元素）

    // 对比 Python：
    // Python list 的 append/insert/remove 与 vector 类似
    // 对比 Rust：
    // Rust Vec<T> 是 vector 的等价物，push/insert/remove 行为类似
    std::cout << "Q4: vector 是 C++ 最常用的动态数组" << std::endl;
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
// Q5: 如何对数组进行排序？
// ============================================================
void q5_sorting() {
    std::vector<int> nums = {5, 2, 8, 1, 9, 3, 7, 4, 6};

    // std::sort（C++11，introsort 算法，O(n log n)）
    std::sort(nums.begin(), nums.end());
    std::cout << "Q5: 升序排序: ";
    for (int n : nums) std::cout << n << " ";
    std::cout << std::endl;

    // 降序（C++17 用 std::greater）
    std::sort(nums.begin(), nums.end(), std::greater<int>());
    std::cout << "Q5: 降序排序: ";
    for (int n : nums) std::cout << n << " ";
    std::cout << std::endl;

    // 自定义比较函数
    nums = {5, 2, 8, 1, 9, 3};
    std::sort(nums.begin(), nums.end(), [](int a, int b) {
        return a > b;  // 降序
    });
    std::cout << "Q5: lambda降序: ";
    for (int n : nums) std::cout << n << " ";
    std::cout << std::endl;

    // 部分排序（C++11）
    nums = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    std::partial_sort(nums.begin(), nums.begin() + 3, nums.end());  // 前3个最小
    std::cout << "Q5: 前3个最小: ";
    for (int i = 0; i < 3; ++i) std::cout << nums[i] << " ";
    std::cout << std::endl;

    // nth_element（C++11，O(n)，找第n小的元素）
    nums = {5, 2, 8, 1, 9, 3};
    std::nth_element(nums.begin(), nums.begin() + 2, nums.end());  // 第3小
    std::cout << "Q5: 第3小 = " << nums[2] << std::endl;

    // 稳定排序（保持相等元素顺序）
    std::vector<std::pair<int, char>> pairs = {{1,'b'}, {2,'a'}, {1,'a'}};
    std::stable_sort(pairs.begin(), pairs.end());
    std::cout << "Q5: 稳定排序: ";
    for (auto& p : pairs) std::cout << "(" << p.first << "," << p.second << ") ";
    std::cout << std::endl;

    // 深入原理：
    // - std::sort 使用 introsort（三路快排+堆排+插入排的混合）
    // - 平均 O(n log n)，最坏 O(n log n)（保证不退化）
    // - C++17 对 small ranges 用插入排序优化

    // 对比 Python：
    // Python: sorted(nums) / nums.sort()（TimSort，O(n log n)）
    // 对比 Rust：
    // Rust: vec.sort() / vec.sort_by()（ Patty-cuts 排序）
    std::cout << "Q5: std::sort 是 C++ 标准排序，覆盖大多数场景" << std::endl;
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
// Q6: 如何实现二分查找？
// ============================================================
void q6_binary_search() {
    std::vector<int> sorted = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};

    // std::binary_search（C++11）：只返回是否存在
    bool found = std::binary_search(sorted.begin(), sorted.end(), 7);
    std::cout << "Q6: binary_search(7) = " << found << std::endl;
    std::cout << "Q6: binary_search(8) = " << std::binary_search(sorted.begin(), sorted.end(), 8) << std::endl;

    // std::lower_bound：找第一个 >= value 的位置
    auto it = std::lower_bound(sorted.begin(), sorted.end(), 7);
    if (it != sorted.end()) {
        std::cout << "Q6: lower_bound(7) = " << *it
                  << "，位置 = " << std::distance(sorted.begin(), it) << std::endl;
    }

    // std::upper_bound：找第一个 > value 的位置
    it = std::upper_bound(sorted.begin(), sorted.end(), 7);
    if (it != sorted.end()) {
        std::cout << "Q6: upper_bound(7) = " << *it
                  << "，位置 = " << std::distance(sorted.begin(), it) << std::endl;
    }

    // 手写二分查找（理解原理）
    auto manual_binary_search = [&](int target) -> int {
        int left = 0, right = static_cast<int>(sorted.size()) - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;  // 防止 (left+right) 溢出
            if (sorted[mid] == target) {
                return mid;
            } else if (sorted[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        return -1;  // 未找到
    };

    std::cout << "Q6: 手写二分 7 在位置 " << manual_binary_search(7) << std::endl;
    std::cout << "Q6: 手写二分 8 返回 " << manual_binary_search(8) << std::endl;

    // 范围版本（C++17）
    auto range = std::equal_range(sorted.begin(), sorted.end(), 7);
    std::cout << "Q6: equal_range(7): [" << std::distance(sorted.begin(), range.first)
              << ", " << std::distance(sorted.begin(), range.second) << ")" << std::endl;

    // 深入原理：
    // - 二分查找：每次将搜索空间减半，O(log n)
    // - 要求：有序数组（升序或降序）
    // - 整数溢出：用 mid = left + (right - left) / 2 而非 (left+right)/2

    // 对比 Python：
    // Python: bisect.bisect_left(sorted, 7) — C++ lower_bound 等价
    // 对比 Rust：
    // Rust: sorted.binary_search(&7)，返回 Result<usize, usize>
    std::cout << "Q6: 二分查找 O(log n)，要求数组已排序" << std::endl;
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
// Q7: 指针数组和数组指针有什么区别？
// ============================================================
void q7_pointer_array() {
    // 指针数组：数组的元素是指针
    int a = 1, b = 2, c = 3;
    int* ptr_arr[3] = {&a, &b, &c};  // 3个int* 组成的数组
    std::cout << "Q7: 指针数组: ";
    for (int i = 0; i < 3; ++i) {
        std::cout << *ptr_arr[i] << " ";
    }
    std::cout << std::endl;

    // 数组指针：指向整个数组的指针
    int arr[3] = {10, 20, 30};
    int (*arr_ptr)[3] = &arr;  // 指向包含3个int的数组的指针
    std::cout << "Q7: 数组指针 *arr_ptr: ";
    for (int i = 0; i < 3; ++i) {
        std::cout << (*arr_ptr)[i] << " ";
    }
    std::cout << std::endl;

    // 指针的指针（二级指针）
    int** ptr_to_ptr = &ptr_arr[0];  // 指向指针数组第一个元素的指针
    std::cout << "Q7: 二级指针 **ptr_to_ptr = " << **ptr_to_ptr << std::endl;

    // C++11 nullptr_t 数组
    std::nullptr_t nulls[3];  // 存储 nullptr_t 的数组

    // 深入原理：
    // - int* ptr[3]: ptr 是数组，元素类型 int*，大小 3
    // - int (*ptr)[3]: ptr 是指针，指向 int[3]，步进单位是整个数组
    // - 内存布局：指针数组元素是指针变量，指向别处

    // 对比 Python：
    // Python 没有指针，但有类似概念：
    // list_of_lists = [[1,2], [3,4]]  # 类似指针数组
    // 对比 Rust：
    // Rust 没有指针数组概念，有引用数组：let refs: [&i32; 3] = [&a, &b, &c];
    std::cout << "Q7: int* arr[n]（指针数组）vs int (*ptr)[n]（数组指针）" << std::endl;
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
    std::cout << "C++ 专家教程 - 模块五：字符串与数组" << std::endl;
    std::cout << "============================================" << std::endl;

    q1_string_types();
    q2_one_dim_array();
    q3_two_dim_array();
    q4_array_crud();
    q5_sorting();
    q6_binary_search();
    q7_pointer_array();

    std::cout << "\n[模块五完成]" << std::endl;
    return 0;
}
