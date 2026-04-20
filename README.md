# C++ 专家教程

> 系统化掌握 C++（覆盖 C++17/20 现代特性）

## 📚 课程概览

本教程包含 **17 个核心模块**，从 C++ 基础到工程实践，系统覆盖现代 C++ 开发所需的核心知识。

| 模块 | 主题 | 核心技能 |
|------|------|---------|
| 01 | 代码规范与基础 | 注释/缩进/标识符/预处理 |
| 02 | 变量与数据类型 | 变量/指针/引用/类型转换/constexpr |
| 03 | 运算符与表达式 | 算术/位运算/优先级/constexpr |
| 04 | 流程控制 | if/switch/循环/break/goto |
| 05 | 字符串与数组 | string/vector/排序/二分 |
| 06 | 函数/方法 | 重载/递归/值传递/lambda/闭包 |
| 07 | 面向对象 OOP | 类/继承/多态/构造/移动语义 |
| 08 | 集合框架 STL | vector/list/set/map/迭代器/算法 |
| 09 | IO 流与文件 | iostream/fstream/序列化 |
| 10 | 异常处理与调试 | try/catch/RAII/gdb/日志 |
| 11 | 并发编程 | thread/mutex/atomic/future/promise |
| 12 | 反射与元编程 | typeid/dynamic_cast/Traits/Concepts |
| 13 | 网络编程 | socket/TCP/UDP/select/epoll |
| 14 | 数据结构与算法 | 链表/树/红黑树/排序/查找 |
| 15 | 数据库编程 | SQL/ODBC/MySQL/事务/ORM |
| 16 | 工程化与设计思想 | 设计模式/SOLID/CMake/Git |
| 17 | 信息分发与事件系统 | 观察者/信号槽/消息队列/事件循环 |

## 🎯 教程特色

- **问题驱动**：每个模块通过 5-8 个核心问题引导学习
- **深入原理**：内存布局、编译器行为、硬件视角全方位解读
- **对比教学**：与 Python/Rust/Lua/Go 等语言对比，加深理解
- **C++17/20**：覆盖 auto/lambda/智能指针/Concepts 等现代特性
- **可运行代码**：每个文件完整可运行，g++ 编译即用
- **配套练习**：每章 3-5 道实践题目，巩固所学

## 👤 适用人群

| 群体 | 说明 |
|------|------|
| **计算机专业学生** | 体系化学习 C++ 数据结构、算法、系统编程 |
| **转行开发者** | 从 Python/JavaScript 等动态语言转向系统级开发 |
| **在职工程师** | 补充工程化、并发、网络等高级技能短板 |
| **竞赛选手** | 掌握 STL 算法、数据结构实现细节，提升编码效率 |
| **技术爱好者** | 理解 C++ 的底层机制（内存模型、模板元编程） |

前置要求：了解一门编程语言的基本概念（变量、函数、循环），不必须有 C/C++ 基础。

## 🧬 C++ 语言特色总结

C++ 是 **"高级语言的外壳 + 低级语言的内核"**，以下四大特性是其区别于其他语言的核心：

### 1. RAII（Resource Acquisition Is Initialization）
> "资源获取即初始化"——利用对象的构造/析构函数自动管理资源。

```cpp
// 文件句柄在 sp 离开作用域时自动关闭，无需手动 fclose
std::ifstream file("data.txt");
auto sp = std::make_unique<std::ifstream>("data.txt");
// 离开作用域 → unique_ptr 析构 → 文件关闭
```
- **对比 Python**：需用 `with open()` 显式管理
- **对比 Rust**：Rust 用 `Drop trait` 实现类似效果，C++ 用构造/析构
- **对比 Go**：需显式 `defer file.Close()`
- **对比 Lua**：无 RAII，靠手动或 GC

### 2. 模板（Template）—— 编译期多态
> 模板是 C++ 独有的泛型编程机制，支持模板特化、SFINAE、variadic templates。

```cpp
// 编译期计算阶乘——无任何运行时开销
template<int N>
constexpr int factorial = N * factorial<N - 1>;
template<>
constexpr int factorial<0> = 1;
static_assert(factorial<5> == 120);
```
- **对比 Python**：Python 用 `*args` 或 `typing.Generic`，无编译期展开
- **对比 Rust**：Rust 用 `impl<T>` 和 trait bound，概念类似但更严格
- **对比 Go**：Go 无泛型（1.18 前），用 `interface{}` 或代码生成
- **对比 Lua**：无模板，靠 table 和 metatable 模拟泛型

### 3. STL（Standard Template Library）
> C++ 标准库，提供容器、迭代器、算法三者分离的泛型框架。

```cpp
std::vector<int> v = {3, 1, 4, 1, 5};
std::sort(v.begin(), v.end());  // introsort（快排+堆排+插入排）
auto it = std::lower_bound(v.begin(), v.end(), 3);  // 二分查找
```
- **对比 Python**：Python 有 `list.sort()` / `sorted()`，C++ 的排序可替换算法
- **对比 Rust**：Rust 用 `std::collections` / `Iterator`，显式迭代器链
- **对比 Go**：Go 用 `sort.Slice()`，无容器内的泛型算法
- **对比 Lua**：Lua table 不是类型安全容器，需自行实现排序/查找

### 4. 零开销抽象（Zero-Overhead Abstraction）
> 你不为你不使用的东西付出代价。抽象层次不损失性能。

```cpp
// lambda 被编译器内联，无函数指针开销
auto add = [](int a, int b) { return a + b; };
std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; });
// 编译器将其内联展开，等价于手写循环
```
- **对比 Python**：Python 函数调用有 dict lookup + 解释器开销
- **对比 Rust**：Rust 同样追求零成本抽象，zero-cost guarantees
- **对比 Go**：Go 有接口派发（interface dispatch）轻微开销
- **对比 Lua**：解释器执行，有显著运行时开销

### 语言特色速查表

| 特性 | C++ | Python | Rust | Lua | Go |
|------|-----|--------|------|-----|-----|
| 内存管理 | 手动+RAII+智能指针 | GC | 所有权系统 | GC | GC |
| 泛型 | 模板 | duck typing | trait | metatable | 1.18前无 |
| 运行期多态 | 虚函数 | duck typing | trait object | metatable | interface |
| 编译期计算 | constexpr+模板 | 无 | const fn | 无 | 无 |
| 并发原生支持 | C++11 thread | asyncio/GIL | std::thread | coroutine | goroutine |
| 性能层级 | A（AOT编译） | C（解释） | A（AOT编译） | C（解释） | B（AOT编译） |
| 学习曲线 | 高 | 低 | 高 | 低 | 中 |

## 🛠 编译方法

```bash
# 单文件编译
g++ -std=c++17 01_code_basics.cpp -o 01_code_basics.exe

# 模块11（并发）需要 pthread
g++ -std=c++17 -pthread 11_concurrency.cpp -o 11_concurrency.exe

# CMake 项目编译（推荐）
mkdir build && cd build
cmake .. -DCMAKE_CXX_STANDARD=17
cmake --build .
```

## 📁 文件结构

```
cpp-tutorial/
├── 01_code_basics.cpp          # 模块一
├── 02_variables_types.cpp       # 模块二
├── 03_operators_expressions.cpp # 模块三
├── 04_flow_control.cpp          # 模块四
├── 05_strings_arrays.cpp        # 模块五
├── 06_functions.cpp              # 模块六
├── 07_oop.cpp                    # 模块七
├── 08_collections.cpp            # 模块八
├── 09_io_files.cpp               # 模块九
├── 10_exceptions_debug.cpp        # 模块十
├── 11_concurrency.cpp             # 模块十一
├── 12_reflection_metaprogramming.cpp # 模块十二
├── 13_network.cpp                 # 模块十三
├── 14_data_structures_algorithms.cpp # 模块十四
├── 15_database.cpp                # 模块十五
├── 16_engineering_design.cpp      # 模块十六
├── 17_event_system.cpp            # 模块十七
└── README.md
```

## 📖 每章学习指南

### 模块一 ~ 模块四：基础语法
从零开始，理解 C++ 的代码结构、变量系统、运算符和流程控制。这是所有编程语言的基础。

### 模块五 ~ 模块六：核心概念
字符串、数组、函数是 C++ 编程的核心载体。lambda 和闭包是 C++11 最重要的现代特性之一。

### 模块七：面向对象
OOP 是 C++ 最核心的编程范式。理解封装、继承、多态，以及构造/析构/移动语义。

### 模块八：STL
STL 是 C++ 标准库的核心。掌握容器、迭代器、算法，是高效 C++ 开发的基础。

### 模块九 ~ 模块十：系统编程
IO、文件、异常处理、调试——这些是生产级 C++ 程序的必备技能。

### 模块十一：并发编程
现代多核 CPU 的必备技能。线程、锁、原子操作是 C++11 引入的重要特性。

### 模块十二：元编程
type_traits、模板元编程、constexpr if、C++20 Concepts——编译期计算和类型系统的高级应用。

### 模块十三：网络编程
socket 编程、TCP/UDP、select/epoll、HTTP 协议。理解网络通信的基础。

### 模块十四：数据结构与算法
链表、树、排序、查找——计算机科学的核心知识。

### 模块十五：数据库编程
SQL、ODBC、MySQL Connector、事务、ORM。数据库是现代应用的基础。

### 模块十六：工程化
设计模式、SOLID 原则、CMake、Git。工程化是专业 C++ 开发者的必备技能。

### 模块十七：事件系统
观察者模式、信号槽、消息队列、事件循环——GUI 和分布式系统的基础。

## 🗺️ 学习路线图

### 第一阶段：C++ 基础（模块 1-6）→ 建议 2-3 周
```
目标：掌握 C++ 语法，能编写完整程序
路线：
  模块1-2 → 变量、类型、指针、引用（理解内存模型）
  模块3-4 → 表达式、流程控制（理解程序执行逻辑）
  模块5   → string/vector/array（理解线性数据结构）
  模块6   → 函数、lambda（理解抽象与封装）
练习：实现一个命令行计算器（支持四则运算、变量存储）
```

### 第二阶段：面向对象与 STL（模块 7-8）→ 建议 2-3 周
```
目标：掌握 OOP 范式和标准库容器
路线：
  模块7   → 类、继承、多态、移动语义（理解对象生命周期）
  模块8   → vector/list/set/map + 算法（掌握 STL 全家桶）
练习：用 STL 实现一个联系人管理程序（排序、查找、去重）
```

### 第三阶段：系统级编程（模块 9-11）→ 建议 2-3 周
```
目标：掌握 IO、异常处理、并发编程
路线：
  模块9   → 文件 IO、序列化、缓冲区管理
  模块10  → RAII、异常、调试工具（gdb、sanitizer）
  模块11  → 线程、锁、原子操作、future/promise
练习：实现一个多线程 HTTP 下载器
```

### 第四阶段：高级特性（模块 12-14）→ 建议 3-4 周
```
目标：掌握元编程、网络、数据结构与算法
路线：
  模块12  → type_traits、模板元编程、C++20 Concepts
  模块13  → socket、TCP/UDP、select/epoll、HTTP
  模块14  → 链表、树、哈希表、排序算法
练习：实现一个简易 Web 服务器（支持静态文件 + CGI）
```

### 第五阶段：工程化与扩展（模块 15-17）→ 建议 2-3 周
```
目标：掌握数据库、设计模式、事件驱动架构
路线：
  模块15  → SQL、ODBC/MySQL、事务、ORM 概念
  模块16  → 设计模式、SOLID、CMake、Git 团队协作
  模块17  → 观察者、信号槽、消息队列、事件循环
练习：用事件驱动模型重构 HTTP 服务器，加入中间件机制
```

### 持续提升建议
1. **刷题**：在 LeetCode/HackerRank 用 C++ 实现算法题，熟悉 STL 算法
2. **阅读源码**：研究 Boost、Abseil、Google Test 等高质量 C++ 项目
3. **参与开源**：为 LLVM、CMake、WebKit 等项目贡献 patch
4. **性能优化**：学会用 `perf`/`valgrind`/`sanitizer` 分析性能瓶颈
5. **C++20 新特性**：深入 concepts、coroutines、ranges、modules

## 📚 参考资料

- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/)
- [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- [cppreference.com](https://en.cppreference.com/)
- [C++17 标准文档](https://eel.is/c++draft/)

## 📝 许可证

MIT License

## 👤 作者

sun-hainan

---

> "C++ 是一个水平很高的语言，使用它的人必须同样水平很高。" — Bjarne Stroustrup
