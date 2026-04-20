// === 模块九：IO 流与文件 ===
// 本文件覆盖：iostream、fstream、stringstream、文件读写、路径、序列化、二进制IO、缓冲流
// 编译：g++ -std=c++17 09_io_files.cpp -o 09_io_files.exe

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>      // std::setw, std::setprecision
#include <cstdio>      // C 文件操作（部分）

// ============================================================
// Q1: iostream 的基本使用和格式化 IO 是什么？
// ============================================================
void q1_iostream_basics() {
    // 标准输入输出
    std::cout << "Q1: Hello " << "C++" << "!" << std::endl;
    std::cerr << "Q1: 错误信息（无缓冲）" << std::endl;
    std::clog << "Q1: 日志信息（有缓冲）" << std::endl;

    // 输入（C++11/14/17）
    std::cout << "Q1: 请输入一个整数: ";
    // int num;
    // std::cin >> num;
    // std::cout << "Q1: 您输入了: " << num << std::endl;

    // 格式化输出
    std::cout << "Q1: 十进制 " << 42 << std::endl;
    std::cout << std::hex << "Q1: 十六进制 " << 42 << std::endl;
    std::cout << std::oct << "Q1: 八进制 " << 42 << std::endl;
    std::cout << std::dec;  // 恢复十进制

    std::cout << std::setw(10) << std::setfill('0') << 42 << std::endl;
    std::cout << std::setprecision(3) << std::fixed << 3.1415926 << std::endl;

    // C++17 std::string_view 输出
    std::string_view sv = "Hello C++17";
    std::cout << "Q1: string_view: " << sv << std::endl;

    // 布尔输出
    std::cout << std::boolalpha << true << " / " << false << std::endl;
    std::cout << std::noboolalpha << true << " / " << false << std::endl;

    // C++20 std::format（预览，C++20 才完全支持）
    // std::cout << std::format("Q1: 格式化: {}/{}\n", 3, 4);

    // 深入原理：
    // - iostream 使用缓冲，cerr/clog 不缓冲
    // - operator<< 和 operator>> 是重载的成员函数
    // - 格式化状态存储在 ios_base::fmtflags 中
    // - endl 刷新缓冲区并换行，"\n" 不刷新（更高效）

    // 对比 Python：
    // print() 默认换行，可用 sep=... end=...
    // input() = std::cin >>，但返回 str 类型
    // 对比 Rust：
    // println!("{}", value); 格式化字符串
    // use std::io::{self, Write}; io::stdout().write()
    std::cout << "Q1: iostream 是类型安全的格式化 IO" << std::endl;
}

// ============================================================
// Q2: 文件读写如何实现？
// ============================================================
void q2_file_io() {
    const char* filename = "test.txt";
    const char* binname = "test.bin";

    // ============ 文本文件写入 ============
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Q2: 无法打开文件写入" << std::endl;
        return;
    }
    out << "Line 1: Hello" << std::endl;
    out << "Line 2: " << 42 << std::endl;
    out << "Line 3: " << 3.14159 << std::endl;
    out.close();
    std::cout << "Q2: 文本文件已写入: " << filename << std::endl;

    // ============ 文本文件读取 ============
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Q2: 无法打开文件读取" << std::endl;
        return;
    }
    std::string line;
    int line_num = 0;
    while (std::getline(in, line)) {  // 按行读取
        std::cout << "Q2: [" << ++line_num << "] " << line << std::endl;
    }
    in.close();

    // ============ C++17 文件系统路径 ============
    // std::filesystem::path p = filename;
    // std::cout << "Q2: 文件名: " << p.filename() << std::endl;
    // std::cout << "Q2: 扩展名: " << p.extension() << std::endl;

    // ============ 二进制文件写入 ============
    std::ofstream bout(binname, std::ios::binary);
    if (bout) {
        int arr[] = {1, 2, 3, 4, 5};
        bout.write(reinterpret_cast<const char*>(arr), sizeof(arr));
        bout.close();
        std::cout << "Q2: 二进制文件已写入: " << binname << std::endl;
    }

    // ============ 二进制文件读取 ============
    std::ifstream bin(binname, std::ios::binary);
    if (bin) {
        int read_arr[5];
        bin.read(reinterpret_cast<char*>(read_arr), sizeof(read_arr));
        std::cout << "Q2: 二进制读取: ";
        for (int x : read_arr) std::cout << x << " ";
        std::cout << std::endl;
        bin.close();
    }

    // ============ fstream（可同时读写） ============
    std::fstream fs(filename, std::ios::in | std::ios::out);
    if (fs) {
        fs << "追加内容";  // 写入当前位置
        fs.seekg(0);      // 定位到文件开头
        std::getline(fs, line);
        std::cout << "Q2: 追加后首行: " << line << std::endl;
        fs.close();
    }

    // 清理测试文件
    std::remove(filename);
    std::remove(binname);

    // 深入原理：
    // - 文本模式：写入 "\n"，读取时 "\r\n" → "\n"（Windows）
    // - 二进制模式：无转换
    // - fstream 内部使用 C 的 FILE* 封装
    // - 文件指针：seekg（读）/ seekp（写）/ seek（同时）

    // 对比 Python：
    // with open("file.txt", "r") as f: content = f.read()
    // with open("file.bin", "wb") as f: f.write(bytes)
    // 对比 Rust：
    // std::fs::write("file.txt", "content")?
    // std::fs::read("file.bin")?
    std::cout << "Q2: 文本用流，二进制用二进制模式" << std::endl;
}

// ============================================================
// Q3: stringstream 有什么用？
// ============================================================
void q3_stringstream() {
    // 字符串流：内存中的 IO

    // ============ 格式化字符串 ============
    std::ostringstream oss;
    oss << "Name: " << "Alice" << ", Age: " << 25
        << ", Score: " << 95.5 << std::endl;
    std::string formatted = oss.str();
    std::cout << "Q3: 格式化: " << formatted;

    // ============ 解析字符串 ============
    std::string input = "100 3.14 hello";
    std::istringstream iss(input);
    int i;
    double d;
    std::string s;
    iss >> i >> d >> s;
    std::cout << "Q3: 解析: int=" << i << ", double=" << d << ", string=" << s << std::endl;

    // ============ 类型转换（代替 to_string） ============
    int val = 42;
    std::ostringstream t;
    t << val;
    std::string str_val = t.str();

    std::string str_num = "3.14";
    std::istringstream p(str_num);
    double pi;
    p >> pi;
    std::cout << "Q3: 字符串转数字: " << pi << std::endl;

    // ============ 按行分割字符串 ============
    std::string text = "Line1\nLine2\nLine3";
    std::istringstream lines(text);
    std::string line;
    while (std::getline(lines, line)) {
        std::cout << "Q3: " << line << std::endl;
    }

    // ============ 清空并重用 ============
    oss.str("");  // 清空内容
    oss.clear();  // 重置状态标志（C++98 兼容）

    // 深入原理：
    // - stringstream 在内存中分配缓冲区
    // - rdbuf() 可以直接获取底层缓冲区用于零拷贝操作
    // - std::ostringstream 用于构造字符串
    // - std::istringstream 用于解析字符串

    // 对比 Python：
    // str.format() 或 f-string（C++ stringstream 等价）
    // int("42") / str(42)（Python 内置转换）
    // 对比 Rust：
    // format!() 宏，parse::<T>() 方法
    std::cout << "Q3: stringstream 是内存中的流，用于字符串的 IO 操作" << std::endl;
}

// ============================================================
// Q4: 什么是缓冲区？cin/cout 的缓冲区如何工作？
// ============================================================
void q4_buffer() {
    // 缓冲区：临时存储 IO 数据，提高效率

    // cout 的缓冲区
    std::cout << "Q4: 输出（无换行，不刷新）";
    // std::cout.flush();  // 手动刷新

    // endl vs "\n"
    std::cout << "Q4: endl 换行并刷新: " << std::endl;  // 刷新缓冲
    std::cout << "Q4: \\n 只换行\n";                      // 不刷新

    // 字符串缓冲区
    std::ostringstream buf;
    buf << "追加数据" << 123;
    std::cout << "Q4: 缓冲区内容: " << buf.str() << std::endl;

    // cin 的缓冲区
    // 输入先存入缓冲区，行模式（按 Enter）
    // std::string input;
    // std::getline(std::cin, input);  // 读取一行

    // 同步点
    // std::ios_base::sync_with_stdio(false);  // 关闭同步，性能更高
    // std::cin.tie(nullptr);                   // cin 不自动 tie cout

    // 读写同一个文件
    std::stringstream ss;
    ss << "Test data";
    std::string read_back;
    ss >> read_back;
    std::cout << "Q4: 读写同一 stringstream: " << read_back << std::endl;

    // 深入原理：
    // - 默认 C++ stream 与 C stdio 同步（安全但慢）
    // - 关闭同步：性能提升 2-10 倍
    // - 缓冲区类型：全缓冲（文件）、行缓冲（终端）、无缓冲（cerr）

    // 对比 Python：
    // Python 的 sys.stdout 有缓冲机制（-u 无缓冲）
    // 对比 Rust：
    // Rust 的 stdout().lock() 返回锁，影响缓冲行为
    std::cout << "Q4: 缓冲区减少系统调用，提高 IO 效率" << std::endl;
}

// ============================================================
// Q5: 如何实现简单的序列化？
// ============================================================
void q5_serialization() {
    // 简单序列化：结构体 → 二进制 → 反序列化

    struct Person {
        char name[32];
        int age;
        double score;
    };

    const char* filename = "person.bin";

    // 序列化（写入）
    Person p1 = {};
    std::strncpy(p1.name, "Alice", sizeof(p1.name) - 1);
    p1.age = 25;
    p1.score = 95.5;

    std::ofstream out(filename, std::ios::binary);
    out.write(reinterpret_cast<const char*>(&p1), sizeof(p1));
    out.close();
    std::cout << "Q5: 序列化完成，文件大小: " << sizeof(p1) << " 字节" << std::endl;

    // 反序列化（读取）
    std::ifstream in(filename, std::ios::binary);
    Person p2;
    in.read(reinterpret_cast<char*>(&p2), sizeof(p2));
    std::cout << "Q5: 反序列化: " << p2.name
              << ", age=" << p2.age << ", score=" << p2.score << std::endl;
    in.close();

    // 简单文本序列化（JSON 风格）
    std::ostringstream json;
    json << "{";
    json << "\"name\": \"" << "Bob" << "\", ";
    json << "\"age\": " << 30 << ", ";
    json << "\"score\": " << 88.0;
    json << "}";
    std::cout << "Q5: JSON 风格: " << json.str() << std::endl;

    // C++17 用 std::string_view 避免不必要的拷贝

    // 清理
    std::remove(filename);

    // 注意：
    // - 二进制序列化依赖内存布局，跨平台可能有问题
    // - 跨平台推荐使用 protobuf、msgpack 等库
    // - C++17 的 std::byte（C++17）更适合二进制数据

    // 对比 Python：
    // pickle.dump(obj, file) / pickle.load(file)
    // 对比 Rust：
    // serde 库，bincode 等二进制序列化格式
    std::cout << "Q5: 简单序列化用于持久化，复杂场景用专业库" << std::endl;
}

// ============================================================
// Q6: IO 错误处理如何进行？
// ============================================================
void q6_io_error_handling() {
    // 检查流状态
    std::istringstream iss("42");
    int val;
    if (iss >> val) {
        std::cout << "Q6: 解析成功: " << val << std::endl;
    } else {
        std::cout << "Q6: 解析失败" << std::endl;
    }

    // 解析失败时流状态
    std::istringstream bad("abc");
    if (!(bad >> val)) {
        std::cout << "Q6: 解析 'abc' 失败" << std::endl;
    }

    // 文件操作错误检查
    std::ifstream file("nonexistent.txt");
    if (!file) {
        std::cout << "Q6: 文件不存在或无法打开" << std::endl;
    }
    // file.is_open()  // 更明确的检查

    // 文件读取完整检测
    std::istringstream data("1 2 3");
    int num;
    while (data >> num) {
        std::cout << "Q6: 读取数字: " << num << std::endl;
    }
    if (!data.eof()) {
        std::cout << "Q6: 读取中断（非 EOF 原因）" << std::endl;
    }

    // ios_base::failure 异常（C++11 起）
    std::ostringstream out;
    out.exceptions(std::ios::failbit | std::ios::badbit);
    try {
        out << "test";
    } catch (const std::ios_base::failure& e) {
        std::cout << "Q6: IO exception: " << e.what() << std::endl;
    }

    // 深入原理：
    // - 流状态位：goodbit / eofbit / failbit / badbit
    // - operator>> 和 operator<< 在失败时设置 failbit
    // - fail() 检查 failbit，eof() 检查 eofbit，bad() 检查 badbit

    // 对比 Python：
    // Python 用异常：FileNotFoundError, ValueError
    // 对比 Rust：
    // Rust 用 Result<T, io::Error>
    std::cout << "Q6: IO 操作后要检查流状态或捕获异常" << std::endl;
}

// ============================================================
// 主函数入口
// ============================================================
int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "C++ 专家教程 - 模块九：IO 流与文件" << std::endl;
    std::cout << "============================================" << std::endl;

    q1_iostream_basics();
    q2_file_io();
    q3_stringstream();
    q4_buffer();
    q5_serialization();
    q6_io_error_handling();

    std::cout << "\n[模块九完成]" << std::endl;
    return 0;
}
