// === 模块十六：工程化与设计思想 ===
// 本文件覆盖：单例、工厂、代理、策略、观察者、六大原则、SOLID、Git、CMake、命名空间
// 编译：g++ -std=c++17 16_engineering_design.cpp -o 16_engineering_design.exe

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include <mutex>

// ============================================================
// Q1: 设计模式：单例、工厂、代理
// ============================================================
class Singleton {
public:
    static Singleton& instance() {
        static Singleton inst;  // C++11 保证线程安全
        return inst;
    }

    void set_value(int v) { value_ = v; }
    int get_value() const { return value_; }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

private:
    Singleton() : value_(0) { }
    int value_;
};

// 简单工厂
class Product {
public:
    virtual ~Product() = default;
    virtual std::string operation() const = 0;
};

class ConcreteProductA : public Product {
public:
    std::string operation() const override { return "ProductA"; }
};

class ConcreteProductB : public Product {
public:
    std::string operation() const override { return "ProductB"; }
};

class SimpleFactory {
public:
    std::unique_ptr<Product> create(const std::string& type) {
        if (type == "A") return std::make_unique<ConcreteProductA>();
        if (type == "B") return std::make_unique<ConcreteProductB>();
        return nullptr;
    }
};

// 工厂方法
class FactoryMethod {
public:
    virtual ~FactoryMethod() = default;
    virtual std::unique_ptr<Product> create() const = 0;
};

class FactoryMethodA : public FactoryMethod {
public:
    std::unique_ptr<Product> create() const override {
        return std::make_unique<ConcreteProductA>();
    }
};

// 代理模式
class RealService {
public:
    std::string do_work() {
        return "RealService doing work";
    }
};

class ProxyService {
public:
    ProxyService() : real_(std::make_unique<RealService>()) { }

    std::string do_work() {
        // 前置处理
        std::cout << "Q1: 代理：权限检查..." << std::endl;
        // 调用真实服务
        std::string result = real_->do_work();
        // 后置处理
        std::cout << "Q1: 代理：日志记录..." << std::endl;
        return result;
    }

private:
    std::unique_ptr<RealService> real_;
};

void q1_design_patterns() {
    // 单例
    Singleton& s1 = Singleton::instance();
    Singleton& s2 = Singleton::instance();
    s1.set_value(42);
    std::cout << "Q1: Singleton s2.value = " << s2.get_value() << std::endl;

    // 工厂
    SimpleFactory factory;
    auto p1 = factory.create("A");
    auto p2 = factory.create("B");
    std::cout << "Q1: 工厂: " << p1->operation() << ", " << p2->operation() << std::endl;

    // 代理
    ProxyService proxy;
    std::cout << "Q1: 代理结果: " << proxy.do_work() << std::endl;

    // 对比 Python：
    // 单例：模块变量 / Borg 模式
    // 工厂：函数返回实例
    // 代理：装饰器或包装函数
    // 对比 Rust：
    // 单例：懒加载 static / Arc<Mutex<T>>
    // 工厂：trait + impl
    // 代理：trait + wrapper struct
    std::cout << "Q1: 单例=全局唯一，工厂=对象创建，代理=控制访问" << std::endl;
}

// ============================================================
// Q2: 策略模式和观察者模式
// ============================================================
// 策略模式
class SortStrategy {
public:
    virtual ~SortStrategy() = default;
    virtual std::vector<int> sort(std::vector<int> data) const = 0;
};

class QuickSortStrategy : public SortStrategy {
public:
    std::vector<int> sort(std::vector<int> data) const override {
        std::sort(data.begin(), data.end());
        return data;
    }
};

class BubbleSortStrategy : public SortStrategy {
public:
    std::vector<int> sort(std::vector<int> data) const override {
        for (int i = 0; i < static_cast<int>(data.size()) - 1; ++i) {
            for (int j = 0; j < static_cast<int>(data.size()) - i - 1; ++j) {
                if (data[j] > data[j + 1]) std::swap(data[j], data[j + 1]);
            }
        }
        return data;
    }
};

class Context {
public:
    void set_strategy(std::unique_ptr<SortStrategy> s) { strategy_ = std::move(s); }
    std::vector<int> execute_sort(const std::vector<int>& data) {
        return strategy_->sort(std::vector<int>(data));
    }
private:
    std::unique_ptr<SortStrategy> strategy_;
};

// 观察者模式
class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const std::string& msg) = 0;
};

class Subject {
public:
    void attach(std::shared_ptr<Observer> obs) { observers_.push_back(obs); }
    void detach(std::shared_ptr<Observer> obs) {
        observers_.erase(std::remove(observers_.begin(), observers_.end(), obs), observers_.end());
    }
    void notify(const std::string& msg) {
        for (auto& obs : observers_) obs->update(msg);
    }
private:
    std::vector<std::shared_ptr<Observer>> observers_;
};

class ConcreteObserver : public Observer {
public:
    explicit ConcreteObserver(const std::string& name) : name_(name) { }
    void update(const std::string& msg) override {
        std::cout << "Q2: 观察者 " << name_ << " 收到: " << msg << std::endl;
    }
private:
    std::string name_;
};

void q2_strategy_observer() {
    // 策略模式
    Context ctx;
    std::vector<int> data = {5, 2, 8, 1, 9};

    ctx.set_strategy(std::make_unique<QuickSortStrategy>());
    auto result1 = ctx.execute_sort(data);
    std::cout << "Q2: QuickSort: ";
    for (int x : result1) std::cout << x << " ";
    std::cout << std::endl;

    ctx.set_strategy(std::make_unique<BubbleSortStrategy>());
    auto result2 = ctx.execute_sort(data);
    std::cout << "Q2: BubbleSort: ";
    for (int x : result2) std::cout << x << " ";
    std::cout << std::endl;

    // 观察者模式
    Subject subject;
    auto obs1 = std::make_shared<ConcreteObserver>("Obs1");
    auto obs2 = std::make_shared<ConcreteObserver>("Obs2");
    subject.attach(obs1);
    subject.attach(obs2);
    subject.notify("事件A发生了！");

    std::cout << "Q2: 策略=可替换算法族，观察者=一对多通知" << std::endl;
}

// ============================================================
// Q3: SOLID 六大原则
// ============================================================
void q3_solid_principles() {
    // S - Single Responsibility Principle（单一职责）
    // 一个类只负责一件事
    // class User { save_to_db(); send_email(); }  // ❌ 违反 SRP
    // class User { } + class UserRepository { save(); } + class EmailService { send(); }  // ✅

    // O - Open/Closed Principle（开闭原则）
    // 对扩展开放，对修改关闭
    // 通过继承/组合扩展，而非修改原有代码

    // L - Liskov Substitution Principle（里氏替换）
    // 子类必须能替换基类而不破坏程序
    // 如果 Bird::fly() 被 Penguin 继承，则违反 LSP

    // I - Interface Segregation Principle（接口隔离）
    // 多个专用接口优于一个通用接口
    // interface Worker + interface Flyer 比一个巨大的 interface Animal 好

    // D - Dependency Inversion Principle（依赖反转）
    // 依赖抽象而非具体实现
    // 高层模块不应该依赖低层模块
    // void process(std::unique_ptr<Reader> r) { ... }  // 依赖 Reader 抽象

    // 六大原则补充：DRY（Don't Repeat Yourself）
    // 相同逻辑不要重复出现

    // KISS（Keep It Simple, Stupid）
    // 保持简单，避免过度设计

    // YAGNI（You Aren't Gonna Need It）
    // 不要为将来可能用不到的功能提前实现

    std::cout << "Q3: SOLID: 单一职责、开闭原则、里氏替换、接口隔离、依赖反转" << std::endl;
}

// ============================================================
// Q4: CMake 基础
// ============================================================
void q4_cmake() {
    std::cout << "Q4: CMakeLists.txt 示例结构" << std::endl;
    std::cout << R"(
cmake_minimum_required(VERSION 3.14)
project(MyProject VERSION 1.0 LANGUAGES CXX)

# C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 编译选项
add_compile_options(-Wall -Wextra -Werror)

# 头文件目录
include_directories(include)

# 源文件目录
aux_source_directory(src SRC_LIST)

# 生成可执行文件
add_executable(myapp ${SRC_LIST})

# 生成库
add_library(mylib STATIC src/lib.cpp)
target_link_libraries(myapp PRIVATE mylib)

# 测试
enable_testing()
add_subdirectory(tests)

# 安装
install(TARGETS myapp DESTINATION bin)
install(FILES header.h DESTINATION include)
)" << std::endl;

    // CMake 常用命令
    // cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
    // cmake --build build
    // cmake --install build

    // 对比 Makefile：
    // Makefile 更底层，更复杂
    // CMake 生成 Makefile/Ninja，降低学习成本

    // 对比 Python：
    // setuptools / pyproject.toml（Python 包管理）
    std::cout << "Q4: CMake 是 C++ 标准构建工具" << std::endl;
}

// ============================================================
// Q5: Git 常用命令和工作流
// ============================================================
void q5_git() {
    std::cout << "Q5: Git 常用命令" << std::endl;
    std::cout << R"(
# 初始化和克隆
git init
git clone https://github.com/user/repo.git

# 基本操作
git add .
git commit -m "message"
git push origin main
git pull origin main

# 分支
git branch feature
git checkout feature
git switch feature  # C++17 风格
git merge feature
git branch -d feature

# 查看状态
git status
git log --oneline --graph
git diff

# 暂存
git stash
git stash pop

# 多人协作
git fetch origin
git rebase origin/main
# 或
git pull --rebase origin main
)" << std::endl;

    // Git 工作流
    // 1. Git Flow: master/main + develop + feature/hotfix/release
    // 2. GitHub Flow: main + feature branch + PR
    // 3. trunk-based: 短生命周期分支

    // C++ 项目 .gitignore 示例
    std::cout << R"(
*.o
*.a
*.so
*.dll
*.exe
build/
.vscode/
.idea/
*.swp
)" << std::endl;

    std::cout << "Q5: Git 是代码版本管理的标准工具" << std::endl;
}

// ============================================================
// Q6: 命名空间的使用和最佳实践
// ============================================================
namespace project {
    namespace module_a {
        class Config {
        public:
            void init() { std::cout << "Q6: module_a::Config::init" << std::endl; }
        };
    }

    namespace module_b {
        class Config {
        public:
            void load() { std::cout << "Q6: module_b::Config::load" << std::endl; }
        };
    }
}

void q6_namespace() {
    // 基本使用
    project::module_a::Config ca;
    ca.init();
    project::module_b::Config cb;
    cb.load();

    // using 声明
    using project::module_a::Config;
    Config c;  // 不需要完整路径
    (void)c;

    // 匿名命名空间：文件内作用域
    namespace {
        int file_local = 1;  // 相当于 static（C++17 建议用匿名命名空间）
    }

    // inline 命名空间（C++11）
    // namespace v1 { inline namespace v2 { class X; } }

    // 命名空间别名
    namespace alias = project::module_a;
    alias::Config* cap = &ca;

    // 最佳实践：
    // - 避免 using namespace std;（尤其在头文件）
    // - 头文件用完整命名空间
    // - 实现文件用 using 简化

    // 对比 Python：
    // Python: import module / from module import name
    // 对比 Rust：
    // Rust: mod / use，作用域规则类似

    std::cout << "Q6: 命名空间防止命名冲突，组织代码结构" << std::endl;
}

// ============================================================
// Q7: 代码审查和质量保障
// ============================================================
void q7_code_review() {
    // 代码审查要点
    // 1. 逻辑正确性
    // 2. 边界条件和错误处理
    // 3. 性能（复杂度、内存）
    // 4. 安全（注入、溢出）
    // 5. 可读性和可维护性

    // C++ 代码质量工具
    // - Clang-Tidy：静态分析
    // - Cppcheck：静态分析
    // - Valgrind：内存检测
    // - AddressSanitizer：内存错误
    // - GCC/Clang -Wall -Wextra -Wpedantic -Werror

    // 编译选项示例
    std::cout << R"(
# 推荐编译选项
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror \
    -O2 -DNDEBUG \
    -fsanitize=address,undefined \
    source.cpp -o program
)" << std::endl;

    // Google C++ Style Guide: https://google.github.io/styleguide/cppguide.html
    // C++ Core Guidelines: https://isocpp.github.io/CppCoreGuidelines/

    // 单元测试框架
    // - Google Test (gtest)
    // - Catch2
    // - doctest（C++17）

    std::cout << "Q7: 工具 + 规范 + 审查 = 高质量代码" << std::endl;
}

// ============================================================
// 主函数入口
// ============================================================
int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "C++ 专家教程 - 模块十六：工程化与设计思想" << std::endl;
    std::cout << "============================================" << std::endl;

    q1_design_patterns();
    q2_strategy_observer();
    q3_solid_principles();
    q4_cmake();
    q5_git();
    q6_namespace();
    q7_code_review();

    std::cout << "\n[模块十六完成]" << std::endl;
    return 0;
}
