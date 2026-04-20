// === 模块二：变量与数据类型 ===
// 本文件覆盖：变量、常量、作用域、生命周期、基本类型、引用类型、指针、
//            类型转换、空值 nullptr、进制、typedef
// 编译：g++ -std=c++17 02_variables_types.cpp -o 02_variables_types.exe

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>    // C++11 fixed-width integers

// ============================================================
// Q1: 什么是变量？变量在内存中如何存储？
// ============================================================
void q1_variable_memory() {
    // 变量：具有名称的内存空间
    // 内存视角：
    //   - 栈（Stack）：局部变量，编译器自动分配/释放
    //   - 堆（Heap）：动态分配（new/delete 或 smart pointer）
    //   - 全局/静态区：程序生命周期内存在

    int age = 25;  // 栈上分配 4 字节（通常）
    std::cout << "Q1: age 地址 = " << &age << ", 值 = " << age << std::endl;

    // 深入原理：
    // 变量名 → 编译器符号表 → 汇编层用地址（偏移量）访问
    // int age;        // 声明（分配内存）
    // age = 25;       // 赋值（写入内存）
    // int age = 25;   // 声明+定义（分配+初始化）

    // 对比 Python：
    // age = 25        # Python 是动态类型，age 是对象引用，类型信息存在对象上
    // 对比 Rust：
    // let age: i32 = 25;  // Rust 有所有权系统，变量默认不可变
    // let mut age = 25;   // 可变变量
}

// ============================================================
// Q2: C++ 有哪些基本数据类型？sizeof 是多少？
// ============================================================
void q2_basic_types() {
    // C++ 基本数据类型
    bool    is_valid = true;           // 1 字节（标准未规定，通常是 1）
    char    grade    = 'A';            // 1 字节
    int     count    = 100;            // 通常 4 字节（平台相关）
    float   price    = 99.99f;         // 4 字节
    double  salary   = 5000.50;        // 8 字节
    long    big_num  = 1'000'000L;     // C++14 数字分隔符，4/8 字节
    long long very_big = 1e12LL;       // 8 字节

    // C++11 fixed-width integers
    int8_t  byte_val  = 127;
    int16_t short_val = 32000;
    int32_t int32_val = 2'000'000'000;
    int64_t int64_val = 9'000'000'000'000LL;

    // 打印各类型大小
    std::cout << "Q2: sizeof(bool)    = " << sizeof(bool)    << " 字节" << std::endl;
    std::cout << "Q2: sizeof(char)    = " << sizeof(char)    << " 字节" << std::endl;
    std::cout << "Q2: sizeof(int)     = " << sizeof(int)     << " 字节" << std::endl;
    std::cout << "Q2: sizeof(float)  = " << sizeof(float)   << " 字节" << std::endl;
    std::cout << "Q2: sizeof(double)  = " << sizeof(double)  << " 字节" << std::endl;
    std::cout << "Q2: sizeof(long)    = " << sizeof(long)     << " 字节" << std::endl;
    std::cout << "Q2: sizeof(long long) = " << sizeof(long long) << " 字节" << std::endl;
    std::cout << "Q2: sizeof(int64_t) = " << sizeof(int64_t) << " 字节" << std::endl;

    // 深入原理：
    // - bool: 存储时可能是 1 字节，但逻辑上只有 true/false
    // - char: 至少 8 位，保证能表示基本字符集
    // - int: 至少 16 位，通常 32 位；保证能表示 -32767~32767
    // - long: 至少 32 位；long long 至少 64 位

    // 对比 Python：
    // Python 的 int 是任意精度整数（动态内存），没有固定大小
    // 对比 Rust：
    // i32, i64, f32, f64 等固定宽度类型，且 Rust 不允许隐式类型转换
}

// ============================================================
// Q3: 什么是作用域和生命周期？
// ============================================================
void q3_scope_lifetime() {
    // 作用域（Scope）：变量名可见的区域
    // 生命周期（Lifetime）：变量存在的时期

    int a = 10;  // 函数作用域，从此处开始可见

    // 块作用域
    {
        int b = 20;  // 只在这个块内可见
        std::cout << "Q3: 块内 a=" << a << ", b=" << b << std::endl;
    }
    // std::cout << b;  // ❌ 编译错误：b 已超出作用域

    // 全局变量（文件作用域）
    // static int global_val = 100;  // 静态存储期，程序结束时销毁

    // 类作用域
    class Widget {
    public:
        int member;  // 类成员，类作用域
    };

    // 深入原理：
    // - 栈变量：随函数调用自动创建，随返回自动销毁（RAII 基础）
    // - 堆变量：手动管理或通过智能指针自动管理
    // - 静态变量：程序启动时分配，程序结束时销毁，只初始化一次

    // 对比 Python：
    // Python 用 LEGB 规则（Local → Enclosing → Global → Built-in）
    // 对比 Lua：
    // Lua 没有块级作用域，只有函数级作用域，用 local 声明局部变量
    std::cout << "Q3: 作用域控制变量可见性，生命周期控制变量存在时间" << std::endl;
}

// ============================================================
// Q4: 指针和引用有什么区别？
// ============================================================
void q4_pointer_reference() {
    int x = 42;
    int* p = &x;        // 指针：存储变量的地址
    int& r = x;         // 引用：变量的别名（必须在创建时绑定）

    std::cout << "Q4: x     = " << x     << std::endl;
    std::cout << "Q4: p     = " << p     << " (地址)" << std::endl;
    std::cout << "Q4: *p    = " << *p     << " (解引用)" << std::endl;
    std::cout << "Q4: r     = " << r     << " (引用值)" << std::endl;

    r = 100;            // 通过引用修改，等同于修改 x
    std::cout << "Q4: 修改后 x = " << x << std::endl;

    // 指针 vs 引用：
    // | 特性         | 指针            | 引用              |
    // |-------------|-----------------|-------------------|
    // | 创建时必须初始化 | 可选（可以是 nullptr）| 必须绑定到一个对象  |
    // | 重新赋值      | 可修改指向        | 不可重新绑定        |
    // | 空值         | nullptr          | 无                |
    // | sizeof       | 平台相关（4/8）    | 等于原类型大小       |
    // | 算术运算      | 支持             | 不支持             |

    // 深入原理：
    // - 指针：变量，存储地址值，可以是 NULL/0/nullptr
    // - 引用：通常实现为常量指针（但语言层面是别名）
    // - 硬件视角：两者都是地址，但引用没有独立的地址空间

    // C++11 nullptr
    int* null_ptr = nullptr;  // 推荐用 nullptr 而不是 NULL 或 0
    if (null_ptr == nullptr) {
        std::cout << "Q4: nullptr 表示空指针" << std::endl;
    }

    // 对比 Python：
    // Python 所有变量都是对象引用，没有指针概念
    // a = 10; b = a  # b 也是对同一个对象的引用
    // 对比 Rust：
    // Rust 有引用（&T 和 &mut T），有借用的概念，编译时检查悬挂引用
    // let r1 = &x;   // 不可变引用
    // let r2 = &mut x;  // 可变引用（Rust 禁止同时存在可变和不可变引用）
}

// ============================================================
// Q5: 什么是类型转换？有哪些类型转换方式？
// ============================================================
void q5_type_conversion() {
    // C++ 类型转换分为：
    // 1. 隐式转换（自动）
    // 2. 显式转换（强制）

    // 隐式转换：编译器自动进行
    int i = 42;
    double d = i;           // int → double（可能丢失精度）
    std::cout << "Q5: 隐式转换 i→d: " << d << std::endl;

    // C 风格强制转换
    double pi = 3.14159;
    int truncated = (int)pi;  // 截断小数
    std::cout << "Q5: C风格转换: " << truncated << std::endl;

    // C++ 四种类型转换运算符
    double val = 98.76;
    // static_cast：编译期转换，用于相关类型（int↔double, void*↔T*）
    int sc = static_cast<int>(val);
    std::cout << "Q5: static_cast: " << sc << std::endl;

    // const_cast：移除或添加 const
    const int const_val = 100;
    int* modifiable = const_cast<int*>(&const_val);
    // 注意：修改 const 变量是未定义行为！

    // reinterpret_cast：位级重新解释（危险，仅必要时使用）
    int* int_ptr = &sc;
    char* char_ptr = reinterpret_cast<char*>(int_ptr);

    // dynamic_cast：运行时多态类型转换（需要 RTTI）
    // 用于类层次结构的 safe downcast

    // 深入原理：
    // - 隐式转换：编译器生成临时对象或扩展精度
    // - static_cast：编译时检查，生成直接的机器码
    // - const_cast：去掉 cv-qualifier（const/volatile）
    // - reinterpret_cast：不做任何检查，直接重新解释比特
    // - dynamic_cast：运行时查询 vtable，涉及虚函数表查找

    // 对比 Python：
    // Python 完全动态类型，自动转换（int+float=float）
    // 对比 Rust：
    // Rust 需要显式转换，用 as 做数值类型转换（as i32）
    // let x: f64 = 3.14; let y = x as i32;  // 显式
    std::cout << "Q5: 显式类型转换更安全，建议用 C++ 四种 cast" << std::endl;
}

// ============================================================
// Q6: 什么是 constexpr？常量表达式有什么优势？
// ============================================================
void q6_constexpr() {
    // constexpr：编译期常量表达式（C++11）
    // C++14/17/20 扩展了 constexpr 的能力

    constexpr int MAX_SIZE = 1000;          // 编译期求值
    constexpr int arr[5] = {1, 2, 3, 4, 5}; // 编译期已知数组大小

    // constexpr 函数（C++14 起可以包含多条语句）
    constexpr int square(int x) { return x * x; }
    constexpr int result = square(16);       // 编译期计算：256

    // C++17 constexpr lambda
    auto lambda = [](int x) constexpr { return x * 2; };
    constexpr int lambda_result = lambda(8);

    // C++20 constinit / consteval
    // constinit：保证静态初始化为常量
    // consteval：强制函数在编译期求值

    std::cout << "Q6: MAX_SIZE = " << MAX_SIZE << std::endl;
    std::cout << "Q6: square(16) = " << result << std::endl;
    std::cout << "Q6: constexpr lambda 8*2 = " << lambda_result << std::endl;

    // 深入原理：
    // - 没有 constexpr：常量可能需要运行时计算
    // - constexpr：编译器在编译时就计算出结果，运行时零成本
    // - 编译器会将 constexpr 值内联到所有使用点

    // 对比 Python：
    // Python 没有编译期常量概念，所有计算都是运行时的
    // MAX_SIZE = 1000  # 运行时赋值，Python 解释器不做优化
    // 对比 Rust：
    // const MAX_SIZE: i32 = 1000;  // Rust 的 const 是编译期常量
    // const fn 也可以用于编译期计算
}

// ============================================================
// Q7: 什么是 typedef 和 using？类型别名有什么用？
// ============================================================
void q7_type_alias() {
    // typedef：传统类型别名声明（C++98）
    typedef unsigned int uint_t;        // "uint_t 是 unsigned int 的别名"
    typedef int* IntPtr;

    // using：C++11 引入的别名声明（更清晰）
    using u32 = uint32_t;               // C++11 using 风格
    using String = std::string;
    using IntVec = std::vector<int>;

    uint_t a = 100;
    u32     b = 200;
    String  name = "Alice";
    IntVec  numbers = {1, 2, 3};

    std::cout << "Q7: typedef/using 类型别名: " << a << ", " << b << std::endl;

    // C++14 引入了类型特征（type traits）的别名模板
    // std::add_pointer<T>::type  →  C++14: std::add_pointer_t<T>

    // using 的优势：
    // 1. 语法更接近变量声明
    // 2. 支持模板别名（typedef 不支持）
    template<typename T>
    using Ptr = T*;  // 模板别名

    Ptr<int> p_int = &a;
    std::cout << "Q7: 模板别名 Ptr<int>: " << *p_int << std::endl;

    // 深入原理：
    // 类型别名不是创建新类型，只是现有类型的同义词
    // 编译器在符号表中用同一个基本类型处理它们

    // 对比 Python：
    // Python 没有类型别名概念，但可以用 TypeAlias（C++11 using 类似）
    // 对比 Rust：
    // type Alias = i32;  // Rust 也有类型别名，用 type 关键字
}

// ============================================================
// Q8: C++ 的进制表示和位运算基础
// ============================================================
void q8_radix() {
    // C++ 支持多种进制字面量
    int dec = 42;        // 十进制（默认）
    int hex = 0x2A;      // 十六进制（0x 前缀）
    int oct = 052;       // 八进制（0 前缀，C++11 起不建议）
    int bin = 0b101010; // 二进制（C++14 支持 0b 前缀）

    std::cout << "Q8: 十进制 42 = " << dec << std::endl;
    std::cout << "Q8: 十六进制 0x2A = " << hex << std::endl;
    std::cout << "Q8: 八进制 052 = " << oct << std::endl;
    std::cout << "Q8: 二进制 0b101010 = " << bin << std::endl;

    // 输出进制转换
    std::cout << std::hex << "Q8: 十进制42的十六进制 = " << 42 << std::endl;
    std::cout << std::dec;  // 恢复十进制

    // 位运算基础
    unsigned int flags = 0b00110101;
    unsigned int mask  = 0b00001000;  // 第3位掩码

    std::cout << "Q8: flags = " << std::dec << flags << std::endl;
    std::cout << "Q8: flags & mask = " << (flags & mask)  // 按位与：检查某位
              << " (flags第3位=" << ((flags & mask) ? "1" : "0") << ")" << std::endl;
    std::cout << "Q8: flags | mask = " << (flags | mask)   // 按位或：设置某位
              << " (设置后第3位=1)" << std::endl;
    std::cout << "Q8: flags ^ mask = " << (flags ^ mask)   // 按位异或：翻转某位
              << " (翻转后第3位)" << std::endl;
    std::cout << "Q8: ~flags = " << (~flags)                // 按位取反
              << " (全部位取反)" << std::endl;
    std::cout << "Q8: flags << 2 = " << (flags << 2)        // 左移：乘以2^n
              << " (左移2位)" << std::endl;
    std::cout << "Q8: flags >> 2 = " << (flags >> 2)        // 右移：除以2^n
              << " (右移2位)" << std::endl;

    // 深入原理：
    // - 位运算是 CPU 指令级别操作，最快的运算之一
    // - 左移可能溢出（对于有符号数是未定义行为！）
    // - 右移无符号数填0，有符号数行为取决于实现（算术 vs 逻辑右移）

    // 对比 Python：
    // Python 整数是任意精度，所以位运算可以处理超大数
    // 对比 Rust：
    // Rust 位运算与 C++ 类似，但对有符号数的移位是明确定义的（算术右移）
    std::cout << "Q8: 位运算是底层操作，硬件级别最高效" << std::endl;
}

// ============================================================
// 主函数入口
// ============================================================
int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "C++ 专家教程 - 模块二：变量与数据类型" << std::endl;
    std::cout << "============================================" << std::endl;

    q1_variable_memory();
    q2_basic_types();
    q3_scope_lifetime();
    q4_pointer_reference();
    q5_type_conversion();
    q6_constexpr();
    q7_type_alias();
    q8_radix();

    std::cout << "\n[模块二完成]" << std::endl;
    return 0;
}
