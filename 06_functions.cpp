// === 模块六：函数/方法 ===
// 本文件覆盖：定义、调用、实参形参、重载、重写、递归、值传递/引用传递/
//            指针传递、可变参数、lambda表达式、闭包
// 编译：g++ -std=c++17 06_functions.cpp -o 06_functions.exe

#include <iostream>
#include <string>
#include <vector>
#include <functional>  // std::function
#include <cstdarg>     // 可变参数

// ============================================================
// Q1: 函数如何定义和调用？函数签名的含义是什么？
// ============================================================
void q1_function_basics() {
    // 函数定义：返回类型 + 函数名 + 参数列表 + 函数体
    // 函数签名（Function Signature）：函数名 + 参数类型列表（不含返回类型）

    std::cout << "Q1: 函数签名示例" << std::endl;

    // 基本函数调用
    auto add = [](int a, int b) -> int { return a + b; };
    std::cout << "Q1: add(3, 4) = " << add(3, 4) << std::endl;

    // void 函数
    auto print_msg = [](const std::string& msg) {
        std::cout << "Q1: " << msg << std::endl;
    };
    print_msg("Hello C++");

    // 多返回值（C++11 可用 std::pair，C++17 用结构化绑定）
    auto divide = [](int a, int b) -> std::pair<int, int> {
        return {a / b, a % b};
    };
    auto [quotient, remainder] = divide(17, 5);  // C++17 结构化绑定
    std::cout << "Q1: 17/5 = " << quotient << " 余 " << remainder << std::endl;

    // 函数作为参数（函数指针）
    auto apply = [](int x, int y, int (*op)(int, int)) -> int {
        return op(x, y);
    };
    auto multiply = [](int a, int b) { return a * b; };
    std::cout << "Q1: apply(3, 4, multiply) = " << apply(3, 4, multiply) << std::endl;

    // 深入原理：
    // - 函数调用：call 指令，跳转到函数地址，保存返回地址到栈
    // - 参数传递：通常通过寄存器（rdi/rsi/rdx/...）或栈传递
    // - 函数签名用于函数重载解析和指针类型匹配

    // 对比 Python：
    // Python 函数是对象，可以作为参数传递，没有重载概念
    // 对比 Rust：
    // Rust fn 是函数类型，|| 是闭包（类似 lambda）
    std::cout << "Q1: 函数签名 = 函数名 + 参数类型，不含返回类型" << std::endl;
}

// ============================================================
// Q2: 值传递、引用传递、指针传递的区别是什么？
// ============================================================
void q2_pass_by() {
    int a = 10;
    int b = 20;
    std::string msg = "original";

    // 值传递：传递副本，不改变原变量
    auto by_value = [](int x) {
        x = 999;  // 只修改副本
        return x;
    };
    std::cout << "Q2: by_value(a) = " << by_value(a) << ", a = " << a << std::endl;

    // 引用传递：传递别名，修改会影响原变量
    auto by_reference = [](int& x) {
        x = 999;  // 修改原变量
        return x;
    };
    std::cout << "Q2: by_reference(b) = " << by_reference(b) << ", b = " << b << std::endl;

    // 指针传递：传递地址，可以通过解引用修改
    auto by_pointer = [](int* px) {
        if (px) *px = 888;  // 需要检查空指针
        return px ? *px : 0;
    };
    std::cout << "Q2: by_pointer(&a) = " << by_pointer(&a) << ", a = " << a << std::endl;

    // const 引用：可以读取但不修改
    auto const_ref = [](const std::string& s) -> size_t {
        // s += " modified";  // ❌ 编译错误
        return s.length();
    };
    std::cout << "Q2: const_ref(msg) = " << const_ref(msg) << std::endl;

    // 深入原理：
    // - 值传递：复制整个对象（大对象复制成本高）
    // - 引用传递：传递对象的地址（通常用寄存器），没有复制开销
    // - 指针传递：显式传递地址，需要解引用，比引用更危险
    // - const 引用：保证不修改，同时避免复制

    // C++ 最佳实践：
    // - 内置类型（int/double）：传值或 const 引用
    // - 大对象（string/vector）：传 const 引用或移动语义
    // - 需要修改时：传引用（&）

    // 对比 Python：
    // Python 一切皆引用，但不可变对象（int/str）类似值传递
    // 对比 Rust：
    // Rust: &T（不可变引用），&mut T（可变引用）
    // 默认 move 语义，复制代价高的类型需要 Clone 或 Copy trait
    std::cout << "Q2: 优先 const 引用，大对象用移动语义" << std::endl;
}

// ============================================================
// Q3: 函数重载和函数重写的区别是什么？
// ============================================================
void q3_overload_override() {
    // 函数重载（Overload）：同一作用域内，函数名相同，参数不同
    std::cout << "Q3: 函数重载示例" << std::endl;

    auto max = [](int a, int b) -> int {
        return a > b ? a : b;
    };
    auto max = [](double a, double b) -> double {
        return a > b ? a : b;
    };
    auto max = [](int a, int b, int c) -> int {
        return max(max(a, b), c);  // 调用两个参数版本
    };

    // C++17 不能仅通过返回类型重载！
    // auto func() -> int { return 1; }
    // auto func() -> double { return 1.0; }  // ❌ 冲突！

    // 函数重写（Override）：派生类重写基类的虚函数
    // 继承中的多态行为，见模块七

    // C++11 override 关键字：明确标记虚函数重写
    // struct Base {
    //     virtual void foo() const;
    // };
    // struct Derived : Base {
    //     void foo() const override;  // 编译检查：确实在重写
    // };

    // 深入原理：
    // - 重载解析（Overload Resolution）：编译器根据参数列表选择最佳匹配
    // - C++ 的名字修饰（Name Mangling）：不同签名的同名函数编码为不同符号名
    // - Linux: _Z3maxii / _Z3maxdd；Windows: ?max@@YAHHH@Z（不同平台不同）
    // - 重写涉及虚函数表（vtable），运行时绑定

    // 对比 Python：
    // Python 没有函数重载，后定义的同名函数覆盖前面的
    // Python 用默认参数和 *args/**kwargs 模拟
    // 对比 Rust：
    // Rust 用泛型函数和 trait bound 实现类似重载
    // impl Add for Point { fn add(...) }  // 实现 + 运算符重载
    std::cout << "Q3: 重载=同作用域同名不同参，重写=派生类覆盖基类虚函数" << std::endl;
}

// ============================================================
// Q4: 递归函数的原理和注意事项是什么？
// ============================================================
void q4_recursion() {
    // 递归：函数调用自身
    // 三要素：基准情况、递归调用、返回值

    // 阶乘（尾递归优化）
    auto factorial = [](auto&& self, int n) -> long long {
        if (n <= 1) return 1;
        return n * self(self, n - 1);  // 注意：n*factorial 需要 n* 的乘法后递归返回
    };
    std::cout << "Q4: factorial(5) = " << factorial(factorial, 5) << std::endl;

    // 标准递归写法
    auto fib = [](auto&& fib, int n) -> long long {
        if (n <= 1) return n;
        return fib(fib, n - 1) + fib(fib, n - 2);
    };
    std::cout << "Q4: fib(10) = " << fib(fib, 10) << std::endl;

    // 记忆化递归（C++11 lambda 模拟）
    auto fib_memo = [](int n) -> long long {
        static std::vector<long long> memo(101, -1);  // 缓存
        if (memo[n] != -1) return memo[n];
        if (n <= 1) return memo[n] = n;
        return memo[n] = fib_memo(n - 1) + fib_memo(n - 2);
    };
    std::cout << "Q4: fib_memo(40) = " << fib_memo(40) << std::endl;

    // 尾递归版本（编译器可优化为循环）
    auto factorial_tail = [](int n, long long acc = 1) -> long long {
        if (n <= 1) return acc;
        return factorial_tail(n - 1, n * acc);
    };
    std::cout << "Q4: factorial_tail(5) = " << factorial_tail(5) << std::endl;

    // 深入原理：
    // - 每次递归调用：压栈（保存返回地址、参数、局部变量）
    // - 递归深度过大：栈溢出（Stack Overflow）
    // - 尾递归优化（TCO）：当前栈帧可复用，不需要新栈帧
    // - C++ 编译器不保证 TCO，GCC/Clang 可用 -O2 或 -O3 优化

    // 对比 Python：
    // Python 默认递归深度限制 ~1000（可调 sys.setrecursionlimit）
    // Python 没有尾递归优化
    // 对比 Rust：
    // Rust 不保证 TCO，但可以用循环代替
    std::cout << "Q4: 递归要注意基准情况和栈深度，尾递归可被优化" << std::endl;
}

// ============================================================
// Q5: 可变参数（variadic）函数如何使用？
// ============================================================
void q5_variadic() {
    // C++11 可变参数模板
    auto sum = [](auto... args) {
        // 展开参数包
        return (args + ...);  // C++17 折叠表达式
    };
    std::cout << "Q5: sum(1, 2, 3, 4, 5) = " << sum(1, 2, 3, 4, 5) << std::endl;
    std::cout << "Q5: sum(1.1, 2.2, 3.3) = " << sum(1.1, 2.2, 3.3) << std::endl;

    // 打印任意数量参数
    auto print_all = [](auto... args) {
        ((std::cout << args << " "), ...);  // C++17 折叠表达式
        std::cout << std::endl;
    };
    print_all("Hello", 42, 3.14, 'A');

    // C 风格可变参数（不推荐，新代码用模板）
    // int printf(const char* format, ...);  // 标准库示例
    // #include <cstdarg>
    // va_list, va_start, va_arg, va_end

    // C++11 std::initializer_list（更安全的可变参数）
    auto sum_init_list = [](std::initializer_list<int> vals) -> int {
        int sum = 0;
        for (int v : vals) sum += v;
        return sum;
    };
    std::cout << "Q5: sum_init_list = " << sum_init_list({1, 2, 3, 4, 5}) << std::endl;

    // 深入原理：
    // - 可变参数模板：编译器在实例化时展开参数包
    // - 折叠表达式 (args + ...)：C++17，按从左到右顺序求值
    // - 递归展开：template<size_t I = 0, typename... Tp>
    //              auto get(Tp&&... args) { return std::get<I>(std::forward_as_tuple(args...)); }

    // 对比 Python：
    // Python: def func(*args, **kwargs) — 天生支持可变参数
    // 对比 Rust：
    // Rust: fn sum<T: Add>(...args: &[T]) — 用切片实现可变参数
    std::cout << "Q5: C++17 可变参数模板是类型安全的可变参数方案" << std::endl;
}

// ============================================================
// Q6: lambda 表达式的完整语法是什么？
// ============================================================
void q6_lambda() {
    // lambda 完整语法：
    // [capture] (params) -> ret_type { body }

    int x = 10;

    // 无捕获、无参数
    auto f1 = [] { return 42; };

    // 有参数
    auto f2 = [](int a, int b) { return a + b; };

    // 有返回类型
    auto f3 = [](double a) -> int { return static_cast<int>(a); };

    // 捕获列表：
    // []        - 不捕获
    // [=]       - 值捕获（拷贝）
    // [&]       - 引用捕获
    // [x]       - 只捕获 x（值）
    // [&x]      - 只捕获 x（引用）
    // [=, &x]   - 默认值捕获，x 引用捕获
    // [&, x]    - 默认引用捕获，x 值捕获

    // 值捕获：创建时拷贝
    auto captured_value = [x] { return x; };
    std::cout << "Q6: captured_value() = " << captured_value() << std::endl;

    // 引用捕获：引用原变量（注意生命周期）
    auto captured_ref = [&x] { return x; };

    // C++14 泛型 lambda
    auto generic = [](auto a, auto b) { return a + b; };
    std::cout << "Q6: generic(1, 2) = " << generic(1, 2) << std::endl;
    std::cout << "Q6: generic(1.5, 2.5) = " << generic(1.5, 2.5) << std::endl;

    // C++17 constexpr lambda
    constexpr auto square = [](int n) constexpr { return n * n; };
    constexpr int sq = square(5);

    // mutable lambda（C++14 可以修改值捕获的副本）
    int y = 5;
    auto mutable_lambda = [y]() mutable {
        y = 10;  // 修改副本，不影响外部 y
        return y;
    };
    std::cout << "Q6: mutable_lambda() = " << mutable_lambda()
              << ", y = " << y << std::endl;

    // C++17 初始化捕获（移动语义）
    std::vector<int> vec = {1, 2, 3};
    auto moved = [v = std::move(vec)]() mutable {
        return v.size();
    };
    std::cout << "Q6: moved() = " << moved() << std::endl;

    // std::function 包装 lambda
    std::function<int(int, int)> fn = [](int a, int b) { return a * b; };
    std::cout << "Q6: std::function call = " << fn(3, 4) << std::endl;

    // 深入原理：
    // - lambda 是匿名函数对象（仿函数）
    // - 编译器生成一个唯一的类，重载 operator()
    // - 捕获列表初始化该类的成员变量
    // - 没有捕获的 lambda 可以转换为函数指针

    // 对比 Python：
    // Python: lambda x: x * 2 — 与 C++ lambda 类似
    // Python lambda 只能是表达式，不能是语句块
    // 对比 Rust：
    // Rust: |x| x * 2 — 语法不同，语义类似
    // Rust 闭包有 Fn/FnMut/FnOnce trait，更严格
    std::cout << "Q6: lambda 是函数对象的语法糖，支持完整的函数语义" << std::endl;
}

// ============================================================
// Q7: 什么是闭包？lambda 如何实现闭包？
// ============================================================
void q7_closure() {
    // 闭包（Closure）：捕获外部变量的 lambda，使函数携带状态

    // 计数器闭包（函数工厂）
    auto make_counter = []() {
        int count = 0;  // 局部状态
        return [count]() mutable {  // 注意：count 是拷贝
            ++count;
            return count;
        };
    };

    auto counter1 = make_counter();
    std::cout << "Q7: counter1: " << counter1() << std::endl;
    std::cout << "Q7: counter1: " << counter1() << std::endl;
    std::cout << "Q7: counter1: " << counter1() << std::endl;

    // 引用捕获的闭包（共享状态）
    auto make_counter_ref = []() {
        int count = 0;
        return [&count]() -> int& {  // 返回引用
            return ++count;
        };
    };
    auto& counter2 = make_counter_ref();  // 需要保持引用有效
    // ⚠️ 警告：count 在函数返回后被销毁！这是悬挂引用！
    // C++14/17 应该使用智能指针或包装类

    // 正确的状态保持闭包（用 shared_ptr）
    auto make_counter_safe = []() {
        auto count = std::make_shared<int>(0);
        return [count]() mutable {
            ++(*count);
            return *count;
        };
    };
    auto counter3 = make_counter_safe();
    std::cout << "Q7: counter3: " << counter3() << std::endl;
    std::cout << "Q7: counter3: " << counter3() << std::endl;

    // 实际应用：延迟执行
    auto make_delayed_add = [](int a, int b) {
        return [=]() { return a + b; };  // 值捕获
    };
    auto delayed_fn = make_delayed_add(10, 20);
    std::cout << "Q7: 延迟计算: " << delayed_fn() << std::endl;

    // 深入原理：
    // - 闭包：lambda + 捕获的变量 = 函数+状态
    // - 闭包类（Closure Type）：编译器生成的仿函数类
    // - 状态生命周期：需要确保闭包存活期间捕获的引用仍然有效

    // 对比 Python：
    // Python 函数天然是闭包（LEGB 规则查找外部变量）
    // def counter():
    //     count = 0
    //     def inc(): nonlocal count; count += 1; return count
    //     return inc
    // 对比 Rust：
    // Rust 闭包通过 Fn/FnMut/FnOnce trait 实现
    // Rust 有明确的 move 语义和生命周期
    std::cout << "Q7: 闭包 = lambda + 状态，注意引用捕获的生命周期" << std::endl;
}

// ============================================================
// 主函数入口
// ============================================================
int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "C++ 专家教程 - 模块六：函数/方法" << std::endl;
    std::cout << "============================================" << std::endl;

    q1_function_basics();
    q2_pass_by();
    q3_overload_override();
    q4_recursion();
    q5_variadic();
    q6_lambda();
    q7_closure();

    std::cout << "\n[模块六完成]" << std::endl;
    return 0;
}
