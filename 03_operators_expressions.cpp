// === 模块三：运算符与表达式 ===
// 本文件覆盖：算术、赋值、关系、逻辑、位运算、三目运算符、优先级、
//            结合性、constexpr 表达式
// 编译：g++ -std=c++17 03_operators_expressions.cpp -o 03_operators_expressions.exe

#include <iostream>
#include <string>
#include <cmath>

// ============================================================
// Q1: C++ 有哪些算术运算符？需要注意什么？
// ============================================================
void q1_arithmetic_operators() {
    int a = 17, b = 4;

    std::cout << "Q1: 算术运算符演示" << std::endl;
    std::cout << "Q1: a=" << a << ", b=" << b << std::endl;
    std::cout << "Q1: a + b  = " << (a + b)  << std::endl;  // 加法
    std::cout << "Q1: a - b  = " << (a - b)  << std::endl;  // 减法
    std::cout << "Q1: a * b  = " << (a * b)  << std::endl;  // 乘法
    std::cout << "Q1: a / b  = " << (a / b)  << std::endl;  // 整数除法：4
    std::cout << "Q1: a % b  = " << (a % b)  << std::endl;  // 取模：1

    // C++11/14/17 数值分隔符
    long long big = 1'000'000'000LL;
    std::cout << "Q1: 数值分隔符 big = " << big << std::endl;

    // 注意：除零是未定义行为！
    // int c = 10 / 0;  // ❌ 运行时错误：除零

    // 深入原理：
    // - 整数除法向零取整（ C++11 规定）
    // - -7 / 2 = -3（向零），Python: -7 // 2 = -4（向下）
    std::cout << "Q1: -7 / 2 = " << (-7 / 2) << " (C++ 向零取整)" << std::endl;

    // 对比 Python：
    // Python: 17 / 4 = 4.25（浮点除法），17 // 4 = 4（整数除法）
    // 对比 Rust：
    // Rust: 17 / 4 = 4，17 % 4 = 1（取模结果与被除数同号）
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
// Q2: 赋值运算符有哪些？复合赋值有什么优势？
// ============================================================
void q2_assignment_operators() {
    int x = 10;  // 基本赋值

    x += 5;  // 复合赋值：x = x + 5
    std::cout << "Q2: x += 5  →  x = " << x << std::endl;

    x -= 3;  // x = x - 3
    std::cout << "Q2: x -= 3  →  x = " << x << std::endl;

    x *= 2;  // x = x * 2
    std::cout << "Q2: x *= 2  →  x = " << x << std::endl;

    x /= 4;  // x = x / 4
    std::cout << "Q2: x /= 4  →  x = " << x << std::endl;

    x %= 3;  // x = x % 3
    std::cout << "Q2: x %= 3  →  x = " << x << std::endl;

    // 位运算复合赋值
    int flags = 0b1010;
    flags &= 0b1100;  // flags = flags & 0b1100
    std::cout << "Q2: flags &= 0b1100 → flags = " << std::dec << flags
              << " (二进制 " << std::hex << flags << ")" << std::endl;
    flags |= 0b0001;  // 设置最低位
    std::cout << "Q2: flags |= 0b0001 → flags = " << std::dec << flags << std::endl;
    flags ^= 0b0101;  // 翻转
    std::cout << "Q2: flags ^= 0b0101 → flags = " << std::dec << flags << std::endl;

    // C++17 初始化与赋值结合
    int y = (x = 20, x + 5);  // 逗号表达式
    std::cout << "Q2: 逗号表达式: y = " << y << std::endl;

    // 深入原理：
    // - x += 5 生成的机器码比 x = x + 5 更少（少一次内存读写）
    // - 复合赋值先计算左边（地址），再计算右边，最后存回左边地址

    // 对比 Python：
    // Python 没有 ++/-- 运算符（存在但被移除）
    // Python: x += 5 与 C++ 相同
    // 对比 Lua：
    // Lua 支持 ++ 和 -- 作为语法糖
    std::cout << "Q2: 复合赋值更高效，减少了重复求值" << std::endl;
}

// ============================================================
// Q3: 关系运算符和逻辑运算符有什么特点？
// ============================================================
void q3_logical_operators() {
    int a = 10, b = 20;

    // 关系运算符
    std::cout << "Q3: 关系运算: " << std::endl;
    std::cout << "Q3: a == b → " << (a == b) << " (0=false, 1=true)" << std::endl;
    std::cout << "Q3: a != b → " << (a != b) << std::endl;
    std::cout << "Q3: a < b  → " << (a < b)  << std::endl;
    std::cout << "Q3: a > b  → " << (a > b)  << std::endl;
    std::cout << "Q3: a <= b → " << (a <= b) << std::endl;
    std::cout << "Q3: a >= b → " << (a >= b) << std::endl;

    // C++17 if with init（初始化语句）
    if (int lhs = a; lhs < b) {
        std::cout << "Q3: C++17 if-init: lhs < b 为真" << std::endl;
    }

    // 逻辑运算符：短路求值
    bool cond1 = false, cond2 = true;
    std::cout << "Q3: cond1 && cond2 = " << (cond1 && cond2) << " (短路)" << std::endl;
    std::cout << "Q3: cond1 || cond2 = " << (cond1 || cond2) << " (或)" << std::endl;
    std::cout << "Q3: !cond1        = " << (!cond1) << std::endl;

    // 短路求值示例
    int divide_by_zero_called = 0;
    bool short_circuit = false && (divide_by_zero_called++, true);
    std::cout << "Q3: 短路求值：divide_by_zero_called = "
              << divide_by_zero_called << " (说明右侧未执行)" << std::endl;

    // C++20 spaceship operator (<=>)
    auto result = a <=> b;
    if (result < 0) std::cout << "Q3: a < b (via <=>)" << std::endl;
    else if (result > 0) std::cout << "Q3: a > b (via <=>)" << std::endl;
    else std::cout << "Q3: a == b (via <=>)" << std::endl;

    // 深入原理：
    // - 短路求值：&& 遇 false 停，|| 遇 true 停
    // - 这允许编写类似：if (ptr != nullptr && ptr->value) ...
    // - bool 在内存中是 1 字节，但寄存器操作是单比特

    // 对比 Python：
    // Python: and/or/not（C++: &&/||/!），都有短路求值
    // 对比 Rust：
    // Rust: &&/||/! 语义与 C++ 相同
    std::cout << "Q3: 短路求值避免无效计算" << std::endl;
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
// Q4: 位运算的常见应用场景是什么？
// ============================================================
void q4_bitwise_apps() {
    // 常见位运算技巧
    unsigned int flags = 0b11110000;

    // 判断某位是否设置
    bool bit3_set = flags & (1 << 3);  // 检查第3位
    std::cout << "Q4: 第3位设置 = " << bit3_set << std::endl;

    // 设置某位
    flags |= (1 << 2);                 // 设置第2位
    std::cout << "Q4: 设置后 flags = " << std::hex << flags << std::endl;

    // 清除某位
    flags &= ~(1 << 3);                // 清除第3位
    std::cout << "Q4: 清除第3位后 = " << std::hex << flags << std::endl;

    // 翻转某位
    flags ^= (1 << 4);                 // 翻转第4位
    std::cout << "Q4: 翻转第4位后 = " << std::hex << flags << std::endl;

    // 提取低位连续位
    unsigned int mask = 0x0F;          // 低4位掩码
    unsigned int low4 = flags & mask;
    std::cout << "Q4: 低4位 = " << std::dec << low4 << std::endl;

    // 判断奇偶（不用 %2）
    int num = 37;
    bool is_odd = num & 1;
    std::cout << "Q4: " << num << " 是奇数 = " << is_odd << std::endl;

    // 交换两数（不用临时变量）
    int p = 5, q = 9;
    p = p ^ q;
    q = p ^ q;  // q = (p^q) ^ q = p
    p = p ^ q;  // p = (p^q) ^ p = q
    std::cout << "Q4: 交换后 p=" << p << ", q=" << q << std::endl;

    // 找出最高位
    unsigned int v = 0b10000000;
    int highest_bit = 31 - __builtin_clz(v);  // GCC/Clang
    std::cout << "Q4: 最高位位置 = " << highest_bit << std::endl;

    // 深入原理：
    // - 位运算是 CPU 指令，单周期完成（如 x86 AND, OR, XOR, SHL, SHR）
    // - 寄存器级别：标志寄存器（ZF, SF, OF 等）记录运算结果特征

    // 对比 Python：
    // Python: 支持 & | ^ ~ << >>，但整数无限精度，行为略有不同
    // 对比 Rust：
    // Rust 位运算与 C++ 完全一致，所有操作都是显式的
    std::cout << "Q4: 位运算是底层优化的核心手段" << std::endl;
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
// Q5: 运算符优先级和结合性如何记忆和使用？
// ============================================================
void q5_precedence_associativity() {
    // C++ 优先级表（简化版）
    // 1. ::       作用域解析（最高）
    // 2. () [] . -> ++ --   后置
    // 3. ++ -- ! ~ + - * & sizeof   一元
    // 4. .* ->*  成员指针
    // 5. * / %   乘除取模
    // 6. + -     加减
    // 7. << >>   移位
    // 8. < <= > >=   关系
    // 9. == !=   相等
    // 10. &      位与
    // 11. ^      位异或
    // 12. |      位或
    // 13. &&     逻辑与
    // 14. ||     逻辑或
    // 15. ?:     条件（三目）
    // 16. = += -= *= /= %= 赋值
    // 17. ,      逗号（最低）

    int result = 2 + 3 * 4;        // = 14（乘法优先）
    int result2 = (2 + 3) * 4;     // = 20（括号改变）

    // 结合性：同优先级从左到右
    int assoc = 10 - 3 - 2;        // = (10-3)-2 = 5（左结合）
    int assoc2 = 10 / 3 / 2;      // = (10/3)/2 = 1（整数除法）

    // 赋值右结合
    int x, y, z;
    x = y = z = 100;               // = (x = (y = (z = 100)))

    // 建议：不确定时用括号，不要依赖记忆优先级
    std::cout << "Q5: 2+3*4 = " << result << ", (2+3)*4 = " << result2 << std::endl;
    std::cout << "Q5: 10-3-2 = " << assoc << std::endl;

    // C++17 确认：括号永远是最清晰的写法
    auto complicated = (((x + y) * z) & 0xFF);

    // 深入原理：
    // - 优先级和结合性由文法决定（AST 构造规则）
    // - 编译器用"运算符优先级解析"构建表达式树

    // 对比 Python：
    // Python 有相同的优先级表，但更一致
    // 对比 Rust：
    // Rust 与 C++ 几乎完全一致
    std::cout << "Q5: 优先级：算术>比较>逻辑，建议用括号" << std::endl;
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
// Q6: 什么是三目运算符？有什么高级用法？
// ============================================================
void q6_ternary() {
    // 基本用法：condition ? expr1 : expr2
    int a = 10, b = 20;
    int min_val = (a < b) ? a : b;
    std::cout << "Q6: min(" << a << "," << b << ") = " << min_val << std::endl;

    // C++17 扩展：可以返回引用
    int& smaller = (a < b) ? a : b;
    std::cout << "Q6: smaller = " << smaller << std::endl;

    // C++17 特性：可以用于常量表达式
    constexpr int max_val = (a > b) ? a : b;

    // 嵌套三目运算符（不推荐，优先用括号）
    int score = 85;
    char grade = (score >= 90) ? 'A'
               : (score >= 80) ? 'B'
               : (score >= 70) ? 'C'
               : (score >= 60) ? 'D' : 'F';
    std::cout << "Q6: score=" << score << " → grade=" << grade << std::endl;

    // C++17 if-constexpr 替代复杂三目
    if constexpr (std::is_same_v<int, decltype(a)>) {
        std::cout << "Q6: a 是 int 类型" << std::endl;
    }

    // 深入原理：
    // - 三目运算符产生右值（通常情况下）
    // - C++17 保证如果返回左值，两边都是左值引用，类型相同
    // - 汇编层：通常实现为条件跳转（je/jne）

    // 对比 Python：
    // Python: a if condition else b（语法相同）
    // 对比 Rust：
    // Rust: if condition { a } else { b }（是表达式，不是运算符）
    // Rust: let x = if cond { 1 } else { 2 };  // let 是表达式
    std::cout << "Q6: 三目运算符是表达式，不是语句" << std::endl;
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
// Q7: constexpr 表达式有哪些高级用法？
// ============================================================
void q7_constexpr_advanced() {
    // constexpr 函数（C++14 支持多语句）
    constexpr int factorial(int n) {
        if (n <= 1) return 1;
        return n * factorial(n - 1);
    }

    constexpr int fact5 = factorial(5);  // 编译期计算：120
    std::cout << "Q7: constexpr factorial(5) = " << fact5 << std::endl;

    // C++17 constexpr string（C++20 才完整支持）
    // C++20 constexpr std::string_view 是标准用法
    constexpr std::string_view sv = "hello";
    static_assert(sv.length() == 5, "长度必须是5");

    // C++20 constexpr new/delete（C++20 才有完整 constexpr 堆操作）
    // constexpr int* ptr = new int(42);  // C++20
    // delete ptr;                         // C++20

    // constexpr lambda（C++17）
    auto const_lambda = [](int x) constexpr { return x * x; };
    constexpr int sq = const_lambda(12);

    // C++17 constexpr if：编译期分支
    // C++20 requires 概念

    // 深入原理：
    // - constexpr 告诉编译器："这个值/函数可以在编译期求值"
    // - 编译器会尝试在编译期计算，如果不可能则降级为运行时
    // - static_assert 的条件必须是编译期常量表达式

    // 对比 Rust：
    // Rust 的 const fn 类似，但更严格
    // const fn factorial(n: i32) -> i32 { ... }
    // 对比 Python：
    // Python 没有编译期计算（可以用 functools.lru_cache 缓存，但仍是运行时）
    std::cout << "Q7: constexpr 实现真正的编译期计算" << std::endl;
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
    std::cout << "C++ 专家教程 - 模块三：运算符与表达式" << std::endl;
    std::cout << "============================================" << std::endl;

    q1_arithmetic_operators();
    q2_assignment_operators();
    q3_logical_operators();
    q4_bitwise_apps();
    q5_precedence_associativity();
    q6_ternary();
    q7_constexpr_advanced();

    std::cout << "\n[模块三完成]" << std::endl;
    return 0;
}

// ================================================================
// 【练习题】
// ================================================================
// 1. 使用位运算实现 `int` 变量大小端字节序交换（不用循环或临时容器）
// 2. 写一个表达式使用三元运算符找出三个整数中的最大值，不使用 std::max
// 3. 验证运算符优先级：计算 `3 + 4 * 5 == 23 && 10 % 3 != 1` 的结果
// 4. 用 `constexpr` 实现编译期字符串反转，对比 C++17/20 的 constexpr 限制
// 5. 编写一个程序，对比 `i++` 和 `++i` 在循环中的性能差异（反汇编层面分析）
