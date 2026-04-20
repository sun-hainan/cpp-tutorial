// === 模块一：C++ 代码规范与基础 ===
// 本文件覆盖：注释、缩进、标识符、关键字、语句结束符、预处理指令、#include
// 编译：g++ -std=c++17 01_code_basics.cpp -o 01_code_basics.exe

#include <iostream>    // 标准输入输出流
#include <string>      // std::string
#include <vector>      // std::vector

// ============================================================
// Q1: 为什么 C++ 需要注释？有哪些注释风格？
// ============================================================
void q1_comments() {
    // 单行注释：编译时会被预处理器完全删除，不影响运行效率
    // 推荐用于简短说明、解释复杂逻辑、标记 TODO

    /*
     * 多行注释（块注释）：
     * 适用于较长说明、版权声明、函数接口描述。
     * 注意：块注释不能嵌套！
     */
    std::cout << "Q1: 注释风格演示" << std::endl;

    // 对比 Python：
    // # 单行注释（与 C++ 单行注释类似）
    // """
    // 多行字符串作为多行注释（实际是字符串，会被解释器处理）
    // """

    // 对比 Rust：
    // // 单行注释
    // /* 块注释 */
    // /// 文档注释（用于生成文档）
    // /*! 内部文档注释 */
}

// ============================================================
// Q2: C++ 缩进规范是什么？为什么很重要？
// ============================================================
void q2_indentation() {
    // C++ 没有强制缩进，但遵循规范让代码可读性极高
    // 主流风格：4 空格 / 2 空格 / Tab（建议 4 空格）
    // Google C++ Style Guide: 2 空格
    // LLVM: 4 空格

    // 示例：标准 if-else 缩进
    if (true) {
        std::cout << "正确缩进：语句在块内缩进一层" << std::endl;
    } else {
        std::cout << "else 与 if 对齐" << std::endl;
    }

    // 硬件视角：缩进不影响生成的机器码，纯粹是人类可读性
    // 编译器视角：编译器看到的是 token 流，空白符全部被忽略

    // 对比 Python：缩进是语法的一部分（强制 4 空格）
    // if True:
    //     print("Python 用缩进表示代码块")
    //     if True:
    //         print("嵌套缩进")
}

// ============================================================
// Q3: 什么是标识符？命名规则有哪些？
// ============================================================
void q3_identifiers() {
    // 标识符：变量、函数、类、命名空间等的名字
    // 规则：
    //   - 由字母、数字、下划线组成
    //   - 不能以数字开头
    //   - 区分大小写
    //   - 不能是关键字

    int age = 25;                    // ✅ 合法
    int _private = 1;               // ✅ 合法（下划线开头，但建议避免）
    int total_count = 100;          // ✅ 蛇形命名（snake_case）
    int maxValue = 200;             // ✅ 驼峰命名（camelCase）
    // int 2ndPlace = 3;            // ❌ 非法：数字开头
    // int class = 1;               // ❌ 非法：class 是关键字

    // 命名空间建议使用小写
    std::string user_name = "Alice";
    std::cout << "Q3: user_name = " << user_name << std::endl;

    // 对比 Rust：标识符规则类似，但支持 Unicode（emoji 也行）
    // let 你好 = 123;  // Rust 支持 Unicode 标识符
    // 对比 Lua：下划线+大写是保留的（如 _VERSION）
    // local _Special = 1  -- 建议避免
}

// ============================================================
// Q4: C++ 有哪些关键字？C++17/20 新增了哪些？
// ============================================================
void q4_keywords() {
    // C++98: auto, break, case, char, const, continue, default, do, double,
    //        else, enum, extern, float, for, goto, if, int, long, register,
    //        return, short, signed, sizeof, static, struct, switch, typedef,
    //        union, unsigned, void, volatile, while
    //
    // C++11 新增：alignas, alignof, char16_t, char32_t, constexpr, decltype,
    //            nullptr, static_assert, thread_local
    //
    // C++14 新增: [[deprecated]]
    //
    // C++17 新增: alignas(扩展), auto 推导增强, constexpr if, std::optional,
    //            std::variant, std::any, inline variables, namespace 嵌套
    //
    // C++20 新增: concept, requires, co_await, co_yield, co_return,
    //            char8_t, constinit, consteval, spaceship operator <=>,
    //            std::format, std::span, std::ranges, module, import

    // 示例：C++17 constexpr if
    constexpr int version = 17;
    if constexpr (version >= 17) {
        std::cout << "Q4: C++17+ 特性可用" << std::endl;
    }

    std::cout << "Q4: 关键字总数（截至 C++20）超过 90 个" << std::endl;
}

// ============================================================
// Q5: 语句结束符是什么？为什么分号必不可少？
// ============================================================
void q5_semicolon() {
    // C++ 中大多数语句以分号 ; 结尾
    int x = 10;     // 声明语句需要分号
    x = 20;         // 赋值语句需要分号
    ;               // 空语句（合法，但不推荐）

    // 注意：宏和模板参数列表后不需要分号（但加上也无害）
    // if, for, while 后面不加 ; ，否则 if/for/while 的主体变成空语句

    // 常见错误：忘记分号导致编译错误
    // if (true)
    //     std::cout << "No semicolon"  // 编译错误！
    // std::cout << "After if";         // 编译器可能把两个语句连起来解释

    std::cout << "Q5: 分号是语句结束的标志" << std::endl;

    // 对比 Python：不需要分号，靠换行判断语句结束
    // 对比 Lua：同样用换行判断语句结束，但允许显式分号
}

// ============================================================
// Q6: 预处理指令是什么？#include 做了什么？
// ============================================================
void q6_preprocessor() {
    // 预处理阶段：编译前对源代码进行文本替换
    // 预处理指令以 # 开头

    // #include：文件包含，将指定文件内容原地展开
    // #include <xxx>  → 查找系统/标准库目录
    // #include "xxx"  → 先查找当前目录，再查找系统目录

    // 示例：条件编译
    #define ENABLE_DEBUG 1
    #if ENABLE_DEBUG
        std::cout << "Q6: 调试模式已开启" << std::endl;
    #else
        std::cout << "Q6: 发布模式" << std::endl;
    #endif

    // #define：宏定义（文本替换，无类型检查）
    #define MAX_SIZE 1024
    std::cout << "Q6: MAX_SIZE = " << MAX_SIZE << std::endl;

    // C++17/20 推荐用 constexpr 变量代替宏常量
    constexpr int kMaxSize = 2048;

    // #pragma：编译器特定指令
    // #pragma once  // 防止头文件重复包含（等价于 include guard）

    // 其他预处理指令：#undef, #if, #elif, #else, #endif, #line, #error

    // 深入原理：
    // 编译器工作流程：
    //   源代码 → [预处理] → 预处理文件(.i) → [词法分析] → [语法分析]
    //          → [语义分析] → [优化] → [代码生成] → 目标文件(.o)
    //
    // 内存视角：#include 只是文本替换，不产生内存分配
    //          包含 <iostream> 会插入数千行代码，编译时消耗资源

    std::cout << "Q6: 预处理在编译前完成，是纯文本操作" << std::endl;
}

// ============================================================
// Q7: C++ 代码的基本结构是什么样的？
// ============================================================
void q7_code_structure() {
    // 标准 C++ 程序结构：
    // 1. 头文件包含（#include）
    // 2. using 声明（可选）
    // 3. 函数定义（必须有一个 main 函数）
    // 4. 类定义（可选）

    // main 函数：程序入口
    // 返回值：int（0 = 成功，非0 = 错误码）
    // 参数：argc（参数个数），argv（参数列表）

    // C++17/20 结构化绑定
    auto [name, age] = std::make_pair("Alice", 25);
    std::cout << "Q7: 结构化绑定 - name=" << name << ", age=" << age << std::endl;

    // C++17 if 语句支持初始化
    if (auto it = std::string("hello").find('e'); it != std::string::npos) {
        std::cout << "Q7: 找到 'e' 在位置: " << it << std::endl;
    }

    std::cout << "Q7: C++ 代码结构：头文件 → using → 函数/类 → main" << std::endl;
}

// ============================================================
// 主函数入口
// ============================================================
int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "C++ 专家教程 - 模块一：代码规范与基础" << std::endl;
    std::cout << "============================================" << std::endl;

    q1_comments();
    q2_indentation();
    q3_identifiers();
    q4_keywords();
    q5_semicolon();
    q6_preprocessor();
    q7_code_structure();

    std::cout << "\n[模块一完成]" << std::endl;
    return 0;
}
