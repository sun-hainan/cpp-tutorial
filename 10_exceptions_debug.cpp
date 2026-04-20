// === 模块十：异常处理与调试 ===
// 本文件覆盖：try/catch/throw、异常类、栈展开、断点、gdb/lldb、日志
// 编译：g++ -std=c++17 10_exceptions_debug.cpp -o 10_exceptions_debug.exe

#include <iostream>
#include <string>
#include <stdexcept>   // 标准异常
#include <vector>
#include <memory>
#include <exception>
#include <sstream>

// ============================================================
// Q1: C++ 异常处理的基本机制是什么？
// ============================================================
void q1_exception_basics() {
    // throw：抛出异常
    auto divide = [](int a, int b) -> double {
        if (b == 0) {
            throw std::runtime_error("除数不能为零");
        }
        return static_cast<double>(a) / b;
    };

    // try-catch：捕获和处理异常
    try {
        double result = divide(10, 2);
        std::cout << "Q1: 10/2 = " << result << std::endl;
        result = divide(10, 0);  // 抛出异常
        std::cout << "Q1: 这行不会执行" << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "Q1: 捕获异常: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        // 基类 catch 放后面（更具体的前面）
        std::cout << "Q1: 其他异常: " << e.what() << std::endl;
    } catch (...) {
        // 捕获所有异常（最后防线）
        std::cout << "Q1: 未知异常" << std::endl;
    }

    // 深入原理：
    // - throw 创建异常对象（通常在堆上）
    // - 栈展开（Stack Unwinding）：逐层退出函数，调用析构函数
    // - 异常传播：沿着调用链向上传播，直到被 catch 捕获
    // - 汇编层面：编译器生成"landing pad"处理异常

    // 对比 Python：
    // Python: try/except 与 C++ 类似
    // raise RuntimeError("msg") / try: ... except RuntimeError as e: ...
    // 对比 Rust：
    // Rust 没有异常，用 Result<T, E> 表示错误
    // ? 运算符传播错误，panic!() 相当于不可恢复的异常
    std::cout << "Q1: 异常用于处理罕见错误，构造函数优先 RAII" << std::endl;
}

// ============================================================
// Q2: 标准异常类层次是什么？自定义异常如何设计？
// ============================================================
class MyException : public std::runtime_error {
public:
    MyException(const std::string& msg, int code)
        : std::runtime_error(msg), code_(code) { }
    int code() const { return code_; }
private:
    int code_;
};

void q2_exception_hierarchy() {
    // C++ 标准异常层次
    // std::exception
    //   ├── std::logic_error
    //   │     ├── std::invalid_argument
    //   │     ├── std::length_error
    //   │     └── std::out_of_range
    //   ├── std::runtime_error
    //   │     ├── std::overflow_error
    //   │     ├── std::underflow_error
    //   │     └── std::range_error
    //   ├── std::bad_alloc（new 失败）
    //   ├── std::bad_cast（dynamic_cast 失败）
    //   └── std::bad_exception

    // 抛出标准异常
    try {
        throw std::invalid_argument("无效参数");
    } catch (const std::invalid_argument& e) {
        std::cout << "Q2: invalid_argument: " << e.what() << std::endl;
    }

    try {
        throw std::out_of_range("超出范围");
    } catch (const std::out_of_range& e) {
        std::cout << "Q2: out_of_range: " << e.what() << std::endl;
    }

    // 抛出自定义异常
    try {
        throw MyException("业务错误", 1001);
    } catch (const MyException& e) {
        std::cout << "Q2: 自定义异常: " << e.what()
                  << ", code=" << e.code() << std::endl;
    }

    // noexcept（C++11）：声明函数不抛异常
    // void safe_func() noexcept { }
    // 如果 safe_func 抛异常，std::terminate() 被调用

    // 深入原理：
    // - 异常对象通过拷贝构造创建（所以要传引用 const &）
    // - 如果拷贝构造抛异常，std::terminate 被调用
    // - noexcept 优化：编译器可以跳过栈展开，生成更小更快的代码

    // 对比 Python：
    // Python 所有异常继承自 BaseException
    // 自定义异常应该继承 Exception
    // 对比 Rust：
    // Rust: 自定义错误类型应该实现 std::error::Error trait
    std::cout << "Q2: 异常类层次清晰，自定义异常继承 std::runtime_error" << std::endl;
}

// ============================================================
// Q3: RAII 如何配合异常实现资源管理？
// ============================================================
class ResourceGuard {
public:
    ResourceGuard() {
        std::cout << "Q3: 资源获取" << std::endl;
    }
    ~ResourceGuard() {
        std::cout << "Q3: 资源释放（即使异常也会调用）" << std::endl;
    }
    void do_work() {
        std::cout << "Q3: 执行工作" << std::endl;
    }
    void may_throw() {
        throw std::runtime_error("工作中出错");
    }
};

void q3_raii_exception() {
    // RAII + 异常：保证资源释放
    ResourceGuard rg;
    try {
        rg.do_work();
        rg.may_throw();  // 抛异常
        std::cout << "Q3: 这行不会执行" << std::endl;
    } catch (...) {
        std::cout << "Q3: 异常被捕获" << std::endl;
    }
    // 析构函数仍然被调用！

    // 智能指针是 RAII 的经典例子
    auto sp = std::make_unique<int>(42);
    // 即使抛异常，sp 也会正确释放内存

    // lock_guard 是 RAII 管理互斥锁的例子
    // std::mutex mtx;
    // std::lock_guard<std::mutex> lock(mtx);  // 构造加锁，析构解锁
    // // ... 操作保护区 ...
    // // 即使抛异常，锁也会被正确释放

    // 深入原理：
    // - 栈展开时，每个对象的析构函数都会被调用
    // - 只要析构函数不抛异常，栈展开就安全
    // - noexcept 析构函数：隐式的（C++11），析构默认 noexcept

    // 对比 Python：
    // Python: with 语句（context manager）等价于 RAII
    // with open("file") as f: ... # 自动关闭
    // 对比 Rust：
    // Rust: Drop trait 实现 RAII
    // Rust 更严格：Drop trait 不能panic
    std::cout << "Q3: RAII 确保异常安全，析构函数是最后防线" << std::endl;
}

// ============================================================
// Q4: 如何使用 GDB/LLDB 调试 C++ 程序？
// ============================================================
void q4_debugger() {
    // GDB / LLDB 基本命令
    std::cout << "Q4: 调试命令示例" << std::endl;

    // 编译时加 -g 启用调试信息
    // g++ -g -std=c++17 -O0 source.cpp -o program

    // GDB 基本命令：
    // (gdb) break main          // 设置断点
    // (gdb) run                  // 运行程序
    // (gdb) next / n             // 单步执行（不进入函数）
    // (gdb) step / s             // 单步执行（进入函数）
    // (gdb) print var            // 打印变量
    // (gdb) info locals          // 打印局部变量
    // (gdb) backtrace / bt       // 打印调用栈
    // (gdb) continue / c         // 继续执行
    // (gdb) delete breakpoints   // 删除所有断点
    // (gdb) watch var            // 监视变量变化
    // (gdb) set var x = 10       // 修改变量值

    // LLDB 命令（macOS）：
    // (lldb) breakpoint set --name main
    // (lldb) process launch
    // (lldb) frame variable      // 当前帧变量
    // (lldb) thread step-inst

    int demo_val = 42;
    std::cout << "Q4: demo_val = " << demo_val << std::endl;

    // 条件断点：
    // (gdb) break 10 if i == 5

    // 多线程调试：
    // (gdb) info threads
    // (gdb) thread 2

    // 深入原理：
    // - 调试信息存储在 .debug_info / .debug_line / .debug_str 节中
    // - DWARF 是 Linux 下最常用的调试信息格式
    // - -O0 保证调试时程序行为与源码一致（-O2 可能重排/内联）

    // 对比 Python：
    // Python: pdb / ipdb
    // python -m pdb script.py
    // 对比 Rust：
    // Rust: rust-gdb / rust-lldb（Cargo 内置支持）
    // cargo run --debug
    std::cout << "Q4: 调试前先读源码，单步跟踪找问题" << std::endl;
}

// ============================================================
// Q5: 日志系统如何实现？
// ============================================================
enum class LogLevel { DEBUG, INFO, WARN, ERROR, FATAL };

class Logger {
public:
    static Logger& instance() {
        static Logger instance;
        return instance;
    }

    void log(LogLevel level, const std::string& msg) {
        if (level < min_level_) return;
        std::lock_guard<std::mutex> lock(mutex_);
        std::cout << "[" << level_to_string(level) << "] "
                  << msg << std::endl;
    }

    void set_level(LogLevel level) { min_level_ = level; }

    // 便捷方法
    void debug(const std::string& msg) { log(LogLevel::DEBUG, msg); }
    void info(const std::string& msg) { log(LogLevel::INFO, msg); }
    void warn(const std::string& msg) { log(LogLevel::WARN, msg); }
    void error(const std::string& msg) { log(LogLevel::ERROR, msg); }
    void fatal(const std::string& msg) { log(LogLevel::FATAL, msg); }

private:
    Logger() : min_level_(LogLevel::DEBUG) { }
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string level_to_string(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO:  return "INFO ";
            case LogLevel::WARN:  return "WARN ";
            case LogLevel::ERROR: return "ERROR";
            case LogLevel::FATAL: return "FATAL";
        }
        return "UNKNOWN";
    }

    LogLevel min_level_;
    std::mutex mutex_;
};

void q5_logging() {
    Logger& logger = Logger::instance();
    logger.set_level(LogLevel::INFO);

    logger.debug("调试信息（不显示）");
    logger.info("这是信息日志");
    logger.warn("警告：某些资源不足");
    logger.error("错误：操作失败");

    // C++20 <format> 日志（预览）
    // std::cout << std::format("Q5: 格式化日志: val={}, name={}", 42, "Alice") << std::endl;

    // 实际项目推荐使用成熟的日志库：spdlog, log4cplus, glog

    // 深入原理：
    // - 日志系统通常是多线程安全的（加锁或无锁环形缓冲区）
    // - 高性能日志：异步日志，批量写入
    // - 日志级别：DEBUG < INFO < WARN < ERROR < FATAL

    // 对比 Python：
    // logging 模块：logging.info(), logging.warning()
    // structlog 库：更结构化的日志
    // 对比 Rust：
    // log crate + env_logger 或 tracing
    std::cout << "Q5: 日志是运行时调试的核心工具" << std::endl;
}

// ============================================================
// Q6: 常见调试技巧和内存错误检测
// ============================================================
void q6_debugging_techniques() {
    // 1. 断言（assert）
    #include <cassert>
    int val = 42;
    assert(val > 0 && "值必须为正数");  // 调试断言
    std::cout << "Q6: assert 通过" << std::endl;

    // NDEBUG 宏禁用 assert
    // #define NDEBUG
    // #include <cassert>  // assert 变成空操作

    // C++11 static_assert（编译期断言）
    static_assert(sizeof(int) >= 4, "int 至少 4 字节");
    static_assert(sizeof(void*) == 8, "仅支持 64 位系统");

    // 2. 内存错误检测
    // 地址消毒剂（AddressSanitizer，ASan）：
    // g++ -fsanitize=address -g source.cpp -o program
    // 检测：越界访问(use-after-free, buffer overflow)

    // 3. Valgrind（Linux/macOS）
    // valgrind --leak-check=full ./program
    // 检测：内存泄漏、使用未初始化内存

    // 4. 线程消毒剂（ThreadSanitizer）
    // g++ -fsanitize=thread -g source.cpp -o program
    // 检测：数据竞争

    // 5. 未定义行为检测（UBSanitizer）
    // g++ -fsanitize=undefined source.cpp -o program

    // 示例：检测未初始化内存
    int uninit;
    // std::cout << "未初始化值: " << uninit;  // 未定义行为！

    std::cout << "Q6: 内存错误：善用 AddressSanitizer 和 Valgrind" << std::endl;

    // 对比 Python：
    // Python 有 gc 模块和 objgraph 检测内存问题
    // 对比 Rust：
    // Rust 所有权和生命周期系统在编译期消除大部分内存错误
    std::cout << "Q6: 编译器警告全开（-Wall -Wextra），善用 sanitizer" << std::endl;
}

// ============================================================
// 主函数入口
// ============================================================
int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "C++ 专家教程 - 模块十：异常处理与调试" << std::endl;
    std::cout << "============================================" << std::endl;

    q1_exception_basics();
    q2_exception_hierarchy();
    q3_raii_exception();
    q4_debugger();
    q5_logging();
    q6_debugging_techniques();

    std::cout << "\n[模块十完成]" << std::endl;
    return 0;
}
