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
- **对比教学**：与 Python/Rust/Lua 等语言对比，加深理解
- **C++17/20**：覆盖 auto/lambda/智能指针/Concepts 等现代特性
- **可运行代码**：每个文件完整可运行，g++ 编译即用

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
