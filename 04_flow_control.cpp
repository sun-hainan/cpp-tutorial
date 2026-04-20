// === 模块四：流程控制 ===
// 本文件覆盖：if/else/switch、循环（for/while/do-while）、break/continue/goto
// 编译：g++ -std=c++17 04_flow_control.cpp -o 04_flow_control.exe

#include <iostream>
#include <string>
#include <vector>
#include <map>

// ============================================================
// Q1: if/else 的用法和常见陷阱是什么？
// ============================================================
void q1_if_else() {
    // 基本 if 语句
    int score = 85;
    if (score >= 60) {
        std::cout << "Q1: 及格" << std::endl;
    } else {
        std::cout << "Q1: 不及格" << std::endl;
    }

    // else-if 链
    if (score >= 90) {
        std::cout << "Q1: 优秀" << std::endl;
    } else if (score >= 80) {
        std::cout << "Q1: 良好" << std::endl;
    } else if (score >= 70) {
        std::cout << "Q1: 中等" << std::endl;
    } else if (score >= 60) {
        std::cout << "Q1: 及格" << std::endl;
    } else {
        std::cout << "Q1: 不及格" << std::endl;
    }

    // C++17 if with initializer
    std::map<std::string, int> ages = {{"Alice", 25}, {"Bob", 30}};
    if (auto it = ages.find("Alice"); it != ages.end()) {
        std::cout << "Q1: Alice's age = " << it->second << std::endl;
    } else {
        std::cout << "Q1: Alice not found" << std::endl;
    }

    // C++17 constexpr if（用于模板）
    // if constexpr (std::is_integral_v<T>) { ... }

    // 常见陷阱1：悬空 else（解决：使用花括号明确块）
    int x = 0;
    if (x > 0)
        if (x > 10)
            std::cout << "x > 10" << std::endl;
        else
            std::cout << "x is 0~10" << std::endl;  // else 绑定到内层 if！
    // 正确写法：用花括号
    if (x > 0) {
        if (x > 10) {
            std::cout << "x > 10" << std::endl;
        }
    } else {
        std::cout << "x <= 0" << std::endl;
    }

    // 常见陷阱2：赋值误写为比较
    // if (x = 10)  // ❌ 赋值！不是比较！
    if (x == 10) {  // ✅ 比较
        std::cout << "x == 10" << std::endl;
    }

    // 深入原理：
    // - if 条件必须是标量类型（非零=true，零=false）
    // - 汇编层面：通常用 je/jne 条件跳转实现
    // - else 最近的匹配规则由文法决定（LR 解析）

    // 对比 Python：
    // Python 用 elif（C++: else if），没有大括号，靠缩进确定块
    // 对比 Rust：
    // Rust: if 必须是明确的布尔表达式（C++ 会把非零当 true，Rust 不会）
    std::cout << "Q1: if/else: 用括号明确块，避免悬空 else" << std::endl;
}

// ============================================================
// Q2: switch 语句如何正确使用？C++17 有什么改进？
// ============================================================
void q2_switch() {
    enum class Color { Red, Green, Blue };
    Color c = Color::Green;

    // C++11: 支持 enum class（强类型枚举）
    switch (c) {
        case Color::Red:
            std::cout << "Q2: 红色" << std::endl;
            break;
        case Color::Green:
            std::cout << "Q2: 绿色" << std::endl;
            break;
        case Color::Blue:
            std::cout << "Q2: 蓝色" << std::endl;
            break;
        // C++11: 不再有隐式 default 到 int 转换
    }

    // C++17: 初始化 + switch 结合
    switch (auto status = 200; status) {
        case 200:
            std::cout << "Q2: OK" << std::endl;
            break;
        case 404:
            std::cout << "Q2: Not Found" << std::endl;
            break;
        default:
            std::cout << "Q2: Unknown status" << std::endl;
            break;
    }

    // switch 字符串（C++11/14/17 不直接支持，用 if-else 或 map）
    std::string cmd = "start";
    if (cmd == "start") {
        std::cout << "Q2: 启动" << std::endl;
    } else if (cmd == "stop") {
        std::cout << "Q2: 停止" << std::endl;
    }

    // C++17 表达式折叠：需要辅助函数
    // C++20: std::format 支持更好的输出

    // 常见陷阱：忘记 break 导致 fall-through
    int v = 2;
    switch (v) {
        case 1:
        case 2:
        case 3:
            std::cout << "Q2: v 是 1~3 之一" << std::endl;
            break;  // 只在最末尾 break
    }

    // C++17 如果确实需要 fall-through，用 [[fallthrough]] 标注
    switch (v) {
        case 1:
            std::cout << "one" << std::endl;
            [[fallthrough]];  // C++17 显式声明 fall-through
        case 2:
            std::cout << "Q2: fall-through 到这里（显式声明）" << std::endl;
            break;
    }

    // 深入原理：
    // - switch 生成跳转表（jump table），O(1) 查找
    // - case 值必须是整数类型或可转换为整数（浮点不行）
    // - 编译器可能优化为二分查找或跳转表

    // 对比 Python：
    // Python 用 match...case（C++20 前的 switch 替代）
    // 对比 Rust：
    // Rust 用 match（表达式，必须穷举所有情况）
    std::cout << "Q2: switch 是跳转表，比 else-if 更高效" << std::endl;
}

// ============================================================
// Q3: for 循环有哪些现代写法？
// ============================================================
void q3_for_loops() {
    std::vector<int> nums = {1, 2, 3, 4, 5};

    // 传统 for 循环
    for (int i = 0; i < 5; ++i) {
        std::cout << "Q3: 传统 for i=" << i << std::endl;
    }

    // C++11 范围for（最常用）
    for (int n : nums) {
        std::cout << "Q3: 范围for n=" << n << std::endl;
    }

    // C++11 auto 简化范围for
    for (auto& n : nums) {
        n *= 2;  // 引用可修改原数组
    }
    for (const auto& n : nums) {
        std::cout << "Q3: 修改后 n=" << n << std::endl;
    }

    // C++20: for with init（类似 if with init）
    std::map<std::string, int> scores = {{"Alice", 90}, {"Bob", 85}};
    for (auto it = scores.begin(); it = scores.begin(), it != scores.end(); ++it) {
        // ...
    }

    // 循环展开（手动）
    for (int i = 0; i < 4; ++i) {
        std::cout << "Q3: 循环 i=" << i << std::endl;
        std::cout << "Q3: 循环 i=" << (i + 1) << std::endl;
    }

    // C++17 结构化绑定在循环中
    std::vector<std::pair<std::string, int>> entries = {{"a", 1}, {"b", 2}};
    for (const auto& [key, value] : entries) {  // C++17 结构化绑定
        std::cout << "Q3: key=" << key << ", value=" << value << std::endl;
    }

    // 常见陷阱：循环变量作用域
    for (int i = 0; i < 3; ++i) {
        std::cout << "Q3: ";
    }
    // std::cout << i;  // ❌ i 已超出作用域（C++03 是不同的！）

    // 深入原理：
    // - 范围for底层调用 begin()/end()（可重载）
    // - 编译器可能将简单循环优化为 SIMD 指令

    // 对比 Python：
    // for x in nums: print(x)  # Python 范围for类似 C++11
    // 对比 Rust：
    // for n in &nums { ... }  # Rust 的迭代器更强大
    std::cout << "Q3: 范围for + auto 是现代 C++ 最推荐的写法" << std::endl;
}

// ============================================================
// Q4: while 和 do-while 的区别和使用场景
// ============================================================
void q4_while_do() {
    // while：先判断条件
    int count = 0;
    while (count < 3) {
        std::cout << "Q4: while count=" << count << std::endl;
        ++count;
    }

    // do-while：先执行，再判断（至少执行一次）
    count = 0;
    do {
        std::cout << "Q4: do-while count=" << count << std::endl;
        ++count;
    } while (count < 3);

    // 常见场景：用户输入验证（do-while 适合）
    // do {
    //     std::cout << "输入 1-10: ";
    //     std::cin >> num;
    // } while (num < 1 || num > 10);

    // 无限循环（C++11 推荐写法）
    // while (true) { ... }
    // for (;;) { ... }  // 两个等价
    // C++20: []() { while (true) { /* ... */ break; } }();

    // break 退出循环
    count = 0;
    while (true) {
        if (count >= 5) {
            std::cout << "Q4: 达到上限，退出" << std::endl;
            break;
        }
        ++count;
    }

    // 深入原理：
    // - while: jmp 到条件判断处
    // - do-while: 先执行 body，然后 jmp 到条件判断
    // - break 翻译为 jmp 到循环外的标签

    // 对比 Python：
    // Python: while True: if condition: break
    // Python 没有 do-while，用 while + break 模拟
    // 对比 Rust：
    // Rust: while 相同，没有 do-while，有 loop { break; }
    std::cout << "Q4: do-while 保证至少执行一次" << std::endl;
}

// ============================================================
// Q5: break、continue、goto 各自的使用场景
// ============================================================
void q5_break_continue_goto() {
    // break：跳出最近一层循环（switch 也用）
    std::cout << "Q5: break 示例" << std::endl;
    for (int i = 0; i < 10; ++i) {
        if (i == 5) {
            std::cout << "Q5: 遇到 5，跳出循环" << std::endl;
            break;
        }
        std::cout << i << " ";
    }
    std::cout << std::endl;

    // continue：跳过本次迭代，进入下次
    std::cout << "Q5: continue 示例（跳过奇数）" << std::endl;
    for (int i = 0; i < 8; ++i) {
        if (i % 2 == 1) {
            continue;  // 奇数跳过
        }
        std::cout << i << " ";  // 0 2 4 6
    }
    std::cout << std::endl;

    // goto：跳转（争议最大，谨慎使用）
    // 合法场景1：跳出深层嵌套循环
    std::cout << "Q5: goto 示例（跳出深层嵌套）" << std::endl;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                if (k == 1) {
                    std::cout << "Q5: 找到目标，跳出全部循环" << std::endl;
                    goto found_target;  // 跳出三层嵌套
                }
            }
        }
    }
found_target:
    std::cout << "Q5: 已到达目标位置" << std::endl;

    // 合法场景2：统一错误处理（函数早期退出）
    // void process() {
    //     if (!init()) goto cleanup;
    //     if (!load()) goto cleanup;
    //     // ...
    // cleanup:
    //     release();
    // }

    // 深入原理：
    // - break/continue 是结构化的跳转（只能往外跳）
    // - goto 是无限制跳转，可以往前/往后跳
    // - goto 如果跳到一个变量的作用域内（C++ 中）会导致问题

    // 对比 Python：
    // Python 有 break/continue，但没有 goto
    // 用异常实现类似 goto 的效果
    // 对比 Rust：
    // Rust 有 break（有标签），没有 goto
    // 'outer: loop { break 'outer; }
    std::cout << "Q5: break/continue 优先，goto 仅在必要时使用" << std::endl;
}

// ============================================================
// Q6: 循环性能优化的常见技巧
// ============================================================
void q6_loop_optimization() {
    std::vector<int> data(1000000, 1);

    // 陷阱1：在循环内重复计算长度
    // 慢：
    // for (int i = 0; i < data.size(); ++i) { ... }  // 每次都调用 size()

    // 快：缓存 size()
    int n = static_cast<int>(data.size());  // 或 size_t n = data.size()
    for (int i = 0; i < n; ++i) {
        // data[i]...
    }
    std::cout << "Q6: 循环长度已优化" << std::endl;

    // 陷阱2：反向迭代器 vs 正向迭代器
    // 有些情况下 reverse iterators 更快

    // 陷阱3：循环展开（编译器通常自动做）
    // 手动展开示例
    int sum = 0;
    int i = 0;
    for (; i + 4 <= n; i += 4) {
        sum += data[i] + data[i+1] + data[i+2] + data[i+3];
    }
    for (; i < n; ++i) {
        sum += data[i];
    }
    std::cout << "Q6: sum = " << sum << std::endl;

    // C++17：如果循环体足够简单，编译器可能向量化（SIMD）

    // 对比 Python：
    // Python 循环很慢，用列表推导/NumPy 向量化加速
    // 对比 Rust：
    // Rust 迭代器是零成本抽象，编译器能做激进优化
    std::cout << "Q6: 现代编译器优化能力强，保持代码清晰优先" << std::endl;
}

// ============================================================
// Q7: C++17/20 范围库（Ranges）对循环的影响
// ============================================================
void q7_ranges_preview() {
    std::vector<int> nums = {5, 2, 8, 1, 9, 3};

    // C++20 ranges（编译需要 -std=c++20）
    // for (int n : nums | std::views::filter([](int x){ return x % 2 == 0; })
    //                    | std::views::transform([](int x){ return x * 2; })) {
    //     std::cout << n << " ";
    // }

    // C++17 等效实现
    for (const auto& n : nums) {
        if (n % 2 == 0) {
            std::cout << n * 2 << " ";
        }
    }
    std::cout << std::endl;

    std::cout << "Q7: C++20 ranges 提供函数式风格的惰性求值" << std::endl;
    std::cout << "Q7: C++17 用 STL 算法配合 lambda 更安全" << std::endl;
}

// ============================================================
// 主函数入口
// ============================================================
int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "C++ 专家教程 - 模块四：流程控制" << std::endl;
    std::cout << "============================================" << std::endl;

    q1_if_else();
    q2_switch();
    q3_for_loops();
    q4_while_do();
    q5_break_continue_goto();
    q6_loop_optimization();
    q7_ranges_preview();

    std::cout << "\n[模块四完成]" << std::endl;
    return 0;
}
