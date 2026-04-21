// === 模块十二：反射与元编程 ===
// 本文件覆盖：typeid、dynamic_cast、模板元编程、Traits、概念Constraints、
//            constexpr if、宏展开
// 编译：g++ -std=c++17 12_reflection_metaprogramming.cpp -o 12_reflection_metaprogramming.exe

#include <iostream>
#include <string>
#include <typeinfo>
#include <type_traits>
#include <vector>
#include <memory>
#include <cxxabi.h>

// ============================================================
// Q1: typeid 和运行时类型信息（RTTI）如何使用？
// ============================================================
class Base {
public:
    virtual ~Base() = default;  // 有虚函数才有 RTTI
    virtual void foo() { std::cout << "Q1: Base::foo" << std::endl; }
};

class Derived : public Base {
public:
    void foo() override { std::cout << "Q1: Derived::foo" << std::endl; }
    void derived_only() { std::cout << "Q1: Derived::derived_only" << std::endl; }
};

void q1_rtti() {
    Base* b1 = new Base();
    Base* b2 = new Derived();

    // typeid：获取类型信息
    std::cout << "Q1: typeid(*b1).name() = " << b1->type().name() << std::endl;
    std::cout << "Q1: typeid(*b2).name() = " << b2->type().name() << std::endl;

    // 判断类型是否相同
    if (typeid(*b1) == typeid(*b2)) {
        std::cout << "Q1: b1 和 b2 类型相同" << std::endl;
    } else {
        std::cout << "Q1: b1 和 b2 类型不同" << std::endl;
    }

    // 虚函数表指针（vptr）是 RTTI 实现的关键
    // 编译器在 vtable 的第一个 slot 存储 type_info 指针

    // 注意：无虚函数的类没有 RTTI
    struct NoRtti { int x; };
    // struct NoRtti* np;
    // typeid(*np).name() // ❌ 编译错误：NoRtti 没有虚函数

    delete b1;
    delete b2;

    // 深入原理：
    // - RTTI = RunTime Type Information
    // - typeid 返回 const std::type_info& 对象
    // - type_info 重载了 operator== 和 operator!=
    // - .name() 返回实现定义的类型名字（可能是 mangled name）

    // 对比 Python：
    // Python: type(obj) 返回类型对象，isinstance() 检查继承关系
    // 对比 Rust：
    // Rust 没有 RTTI，用 trait object 实现运行时多态
    std::cout << "Q1: RTTI 依赖虚函数表，开销小但有限" << std::endl;
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
// Q2: dynamic_cast 的用法和原理是什么？
// ============================================================
void q2_dynamic_cast() {
    Base* base_ptr = new Derived();
    Derived* derived_ptr = new Derived();

    // 向下转型（Downcast）
    Derived* d = dynamic_cast<Derived*>(base_ptr);
    if (d) {
        std::cout << "Q2: 向下转型成功" << std::endl;
        d->derived_only();
    } else {
        std::cout << "Q2: 向下转型失败（base_ptr 不是 Derived）" << std::endl;
    }

    // 不安全的向下转型
    Base* fake_base = new Base();
    Derived* bad = dynamic_cast<Derived*>(fake_base);
    if (bad == nullptr) {
        std::cout << "Q2: 安全的空指针检查（fake_base 不是 Derived）" << std::endl;
    }
    delete fake_base;

    // 引用转型（失败时抛出 std::bad_cast）
    try {
        Derived& d_ref = dynamic_cast<Derived&>(*base_ptr);
        d_ref.derived_only();
    } catch (const std::bad_cast& e) {
        std::cout << "Q2: bad_cast 异常" << std::endl;
    }

    // 交叉转型（Cross-cast）
    class A { public: virtual ~A() = default; };
    class B { public: virtual ~B() = default; };
    class C : public A, public B { };

    A* a_ptr = new C();
    B* b_ptr = dynamic_cast<B*>(a_ptr);  // 通过 A→C→B
    if (b_ptr) std::cout << "Q2: 交叉转型成功" << std::endl;

    delete a_ptr;

    // dynamic_cast 原理：
    // - 查询虚函数表，找到 type_info
    // - 在类的继承链中查找目标类型
    // - 多继承时需要沿着 vptr 链查找

    // 性能开销：可能需要遍历继承链，O(n) 最坏情况

    // 对比 Python：
    // Python: isinstance(obj, Derived) / obj.__class__
    // 对比 Rust：
    // Rust 没有 dynamic_cast，用 trait 对象和 downcast 方法
    std::cout << "Q2: dynamic_cast 依赖 RTTI，失败返回 nullptr" << std::endl;
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
// Q3: type_traits 的常用工具有哪些？
// ============================================================
void q3_type_traits() {
    // C++ 类型特征：编译期类型查询

    // 基本类型查询
    std::cout << "Q3: std::is_integral<int>::value = "
              << std::is_integral<int>::value << std::endl;
    std::cout << "Q3: std::is_floating_point<double>::value = "
              << std::is_floating_point<double>::value << std::endl;
    std::cout << "Q3: std::is_pointer<int*>::value = "
              << std::is_pointer<int*>::value << std::endl;
    std::cout << "Q3: std::is_array<int[10]>::value = "
              << std::is_array<int[10]>::value << std::endl;

    // C++14 类型特征变量模板（_v 后缀）
    std::cout << "Q3: std::is_integral_v<int> = "
              << std::is_integral_v<int> << std::endl;

    // 类型关系
    std::cout << "Q3: std::is_base_of<Base, Derived>::value = "
              << std::is_base_of<Base, Derived>::value << std::endl;
    std::cout << "Q3: std::is_same<int, int32_t>::value = "
              << std::is_same<int, int32_t>::value << std::endl;

    // 类型转换
    std::cout << "Q3: std::is_convertible<int*, void*>::value = "
              << std::is_convertible<int*, void*>::value << std::endl;

    // remove_reference / add_pointer 等
    using T1 = std::remove_reference<int&>::type;      // int
    using T2 = std::add_pointer<int>::type;            // int*
    using T3 = std::decay<const int&>::type;           // int
    std::cout << "Q3: remove_reference<int&> = int" << std::endl;
    std::cout << "Q3: add_pointer<int> = int*" << std::endl;

    // enable_if：条件启用函数模板
    template<typename T,
             typename = std::enable_if_t<std::is_integral_v<T>>>
    T double_val(T v) { return v * 2; }

    std::cout << "Q3: double_val(5) = " << double_val(5) << std::endl;
    // double_val(3.14);  // ❌ 编译错误：不是 integral 类型

    // C++17 if constexpr 实现分支
    auto process = [](auto val) {
        if constexpr (std::is_integral_v<decltype(val)>) {
            return val * 2;
        } else if constexpr (std::is_floating_point_v<decltype(val)>) {
            return val * 2.0;
        } else {
            return 0;  // 不支持的类型
        }
    };
    std::cout << "Q3: process(5) = " << process(5) << std::endl;
    std::cout << "Q3: process(3.14) = " << process(3.14) << std::endl;

    // 深入原理：
    // - type_traits 是编译期查询，不产生运行时代码（大多）
    // - 编译器根据模板实例化推断类型属性
    // - enable_if 通过 SFINAE 控制函数重载

    // 对比 Python：
    // Python 的 type() / isinstance() 是运行时检查
    // 对比 Rust：
    // Rust: std::marker::Sized，std::any::Any
    // Rust trait 系统 = C++ concepts
    std::cout << "Q3: type_traits 是编译期类型检测" << std::endl;
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
// Q4: 什么是模板元编程（TMP）？
// ============================================================
template<int N>
struct Factorial {
    static const int value = N * Factorial<N - 1>::value;
};

template<>
struct Factorial<0> {
    static const int value = 1;
};

template<int N>
struct Fibonacci {
    static const int value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
};

template<>
struct Fibonacci<0> { static const int value = 0; };
template<>
struct Fibonacci<1> { static const int value = 1; };

// 编译期循环（模板递归）
template<int N, int M>
struct Add {
    static const int value = N + M;
};

void q4_template_metaprogramming() {
    // 模板元编程：编译期计算

    std::cout << "Q4: Factorial<5>::value = " << Factorial<5>::value << std::endl;
    std::cout << "Q4: Factorial<10>::value = " << Factorial<10>::value << std::endl;
    std::cout << "Q4: Fibonacci<10>::value = " << Fibonacci<10>::value << std::endl;

    // C++14 constexpr 更简洁（避免模板元编程的复杂性）
    constexpr int fib14(int n) {
        if (n <= 1) return n;
        return fib14(n - 1) + fib14(n - 2);
    }
    std::cout << "Q4: fib14(10) = " << fib14(10) << std::endl;

    // Type List（类型列表）
    struct NullType {};
    template<typename H, typename T>
    struct TypeList { using Head = H; using Tail = T; };

    using TL = TypeList<int, TypeList<double, TypeList<char, NullType>>>;
    using First = typename TL::Head;    // int
    using Second = typename TL::Tail::Head;  // double

    // 编译期类型选择
    std::cout << "Q4: TypeList 用于类型计算" << std::endl;

    // 深入原理：
    // - TMP 基于模板实例化，编译器执行计算
    // - 递归终止需要特化
    // - 编译器是 TMP 的"解释器"
    // - 错误消息可能非常长（模板展开的副作用）

    // 对比 Python：
    // Python 没有编译期计算，可以用 functools.lru_cache 缓存计算结果
    // 对比 Rust：
    // Rust const fn 类似 C++ constexpr，但更强大
    std::cout << "Q4: 模板元编程 = 编译期计算，功能强大但代码复杂" << std::endl;
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
// Q5: constexpr if 和 SFINAE 是什么？
// ============================================================
template<typename T>
void q5_sfinae_impl(T* ptr, std::true_type) {
    std::cout << "Q5: T 是指针类型: " << ptr << std::endl;
}

template<typename T>
void q5_sfinae_impl(T, std::false_type) {
    std::cout << "Q5: T 不是指针类型" << std::endl;
}

template<typename T>
void q5_sfinae(T val) {
    q5_sfinae_impl(&val, std::is_pointer<T>{});
}

void q5_constexpr_if() {
    q5_sfinae(42);
    q5_sfinae(&q5_constexpr_if);

    // C++17 constexpr if：编译期分支（不需要 SFINAE）
    auto process_v2 = [](auto val) {
        if constexpr (std::is_pointer_v<decltype(val)>) {
            return *val;
        } else if constexpr (std::is_integral_v<decltype(val)>) {
            return val;
        } else {
            return decltype(val){};
        }
    };

    int x = 42;
    std::cout << "Q5: process_v2(10) = " << process_v2(10) << std::endl;
    std::cout << "Q5: process_v2(&x) = " << process_v2(&x) << std::endl;

    // constexpr if 消除了 SFINAE 的复杂性
    // if constexpr 条件在编译期求值，不满足的分支被丢弃（不编译）

    // 深入原理：
    // - SFINAE：Substitution Failure Is Not An Error
    // - 模板参数替换失败时不报错，而是从重载集中移除该候选
    // - constexpr if 直接在编译期决定分支

    // 对比 Python：
    // Python 没有编译期检查，类型都在运行时
    // 对比 Rust：
    // Rust 用 trait bound 和 impl specialization 实现类似功能
    std::cout << "Q5: constexpr if 简化了编译期分支，避免 SFINAE 复杂性" << std::endl;
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
// Q6: C++20 Concepts（概念）是什么？
// ============================================================
#if __cplusplus >= 202002L  // C++20
// concept Addable = requires(T a, T b) { a + b; };
// template<Addable T> T add(T a, T b) { return a + b; }
#endif

// C++17 等效写法（用 enable_if）
template<typename T,
         typename = std::enable_if_t<std::is_integral_v<T>>>
T add_17(T a, T b) { return a + b; }

void q6_concepts_preview() {
    std::cout << "Q6: C++17 enable_if 等效 C++20 Concepts" << std::endl;

    // C++20 concepts 简化模板约束
    // template<std::integral T>
    // T gcd(T a, T b) { while (b != 0) { T t = b; b = a % b; a = t; } return a; }

    // requires 子句（C++20）
    // template<typename T>
    //     requires std::integral<T>
    // T square(T x) { return x * x; }

    std::cout << "Q6: C++20 Concepts 使模板约束更清晰" << std::endl;
    std::cout << "Q6: GCC 11+/Clang 13+ 支持 Concepts" << std::endl;
}

// ============================================================
// Q7: 宏展开和宏函数的注意事项
// ============================================================
#define MAX(a, b) ((a) > (b) ? (a) : (b))  // 好的写法：加括号
#define BAD_MAX(a, b) (a > b ? a : b)     // 可能出问题

// 多语句宏：用 do { } while(0) 包裹
#define SAFE_MACRO(x) \
    do { \
        int temp = (x); \
        std::cout << "Q7: 宏内 temp = " << temp << std::endl; \
    } while(0)

void q7_macros() {
    // 宏展开
    std::cout << "Q7: MAX(3, 5) = " << MAX(3, 5) << std::endl;

    // 宏的副作用陷阱
    int a = 3, b = 5;
    // std::cout << "Q7: MAX(++a, b) = " << MAX(++a, b) << std::endl;
    // 实际展开为：((++a) > (b) ? (++a) : (b))
    // a 被递增两次！结果不可预测

    // 宏函数 vs inline 函数
    // 宏：文本替换，无类型检查，可能有副作用
    // inline：函数，有类型检查，无副作用

    // C++ 建议：用 constexpr 函数代替宏常量，用 inline 函数代替宏函数

    // C++17 [[maybe_unused]] 抑制警告
    [[maybe_unused]] int unused_var = 42;

    // C++17 [[nodiscard]] 提示忽略返回值
    // [[nodiscard]] int important_func() { return 42; }

    // 深入原理：
    // - 宏在预处理阶段展开，是纯文本替换
    // - 宏没有作用域概念
    // - inline 函数由编译器处理，有作用域和类型检查

    // 对比 Python：
    // Python 没有宏，用函数代替
    // 对比 Rust：
    // Rust 有 macro_rules! 宏，功能更强大（卫生宏）
    std::cout << "Q7: 宏是文本替换，谨慎使用，优先选择 constexpr/inline" << std::endl;
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
    std::cout << "C++ 专家教程 - 模块十二：反射与元编程" << std::endl;
    std::cout << "============================================" << std::endl;

    q1_rtti();
    q2_dynamic_cast();
    q3_type_traits();
    q4_template_metaprogramming();
    q5_constexpr_if();
    q6_concepts_preview();
    q7_macros();

    std::cout << "\n[模块十二完成]" << std::endl;
    return 0;
}

// ================================================================
// [【练习题】]
// ================================================================
// 1. 用 typeid 获取类型信息，实现一个汅起型别划函数
// 2. 编写一个模板函数计算 1+2+...+n 的性能，对比这些与计数和逐排
// 3. 用 std::enable_if 实现模板函数选择性释达
// 4. 实现一个 C++20 concept 类型约束，斋走同一个函数的多种实现
// 5. 用 \#define 实现一个简易的编译期汇转换工具
