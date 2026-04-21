// === 模块十七：信息分发与事件系统 ===
// 本文件覆盖：观察者模式、Event/Listener、信号槽、消息队列、回调、中间件、事件循环
// 编译：g++ -std=c++17 17_event_system.cpp -o 17_event_system.exe

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>
#include <queue>
#include <mutex>
#include <atomic>
#include <chrono>
#include <variant>

// ============================================================
// Q1: 观察者模式（Observer Pattern）是什么？
// ============================================================
class EventBus;

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void on_event(const std::string& event_type, const std::string& data) = 0;
};

class EventBus {
public:
    void subscribe(const std::string& event_type, IObserver* observer) {
        observers_[event_type].push_back(observer);
    }

    void unsubscribe(const std::string& event_type, IObserver* observer) {
        auto& list = observers_[event_type];
        list.erase(std::remove(list.begin(), list.end(), observer), list.end());
    }

    void publish(const std::string& event_type, const std::string& data) {
        auto it = observers_.find(event_type);
        if (it != observers_.end()) {
            for (auto* obs : it->second) {
                obs->on_event(event_type, data);
            }
        }
    }

private:
    std::map<std::string, std::vector<IObserver*>> observers_;
};

class ConcreteObserver : public IObserver {
public:
    explicit ConcreteObserver(const std::string& name) : name_(name) { }

    void on_event(const std::string& event_type, const std::string& data) override {
        std::cout << "Q1: 观察者 [" << name_ << "] 收到事件: "
                  << event_type << ", 数据: " << data << std::endl;
    }

private:
    std::string name_;
};

void q1_observer_pattern() {
    EventBus bus;
    ConcreteObserver obs1("Obs1");
    ConcreteObserver obs2("Obs2");

    bus.subscribe("user.login", &obs1);
    bus.subscribe("user.login", &obs2);
    bus.subscribe("user.logout", &obs1);

    bus.publish("user.login", "Alice logged in");
    bus.publish("user.logout", "Bob logged out");

    // 对比 Qt 的信号槽：
    // connect(sender, &Sender::signal, receiver, &Receiver::slot);
    // Qt 的 MOC（元对象编译器）自动生成信号槽代码

    // 对比 JavaScript：
    // element.addEventListener('click', handler);
    // 事件委托模式

    // 对比 Rust：
    // 使用 trait 对象和 Vec<Box<dyn Observer>>
    std::cout << "Q1: 观察者 = 一对多依赖，事件驱动的基础模式" << std::endl;
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
// Q2: 信号槽（Signal/Slot）机制如何实现？
// ============================================================
class Signal {
public:
    using Slot = std::function<void()>;

    int connect(Slot slot) {
        int id = next_id_++;
        slots_[id] = std::move(slot);
        return id;
    }

    void disconnect(int id) {
        slots_.erase(id);
    }

    void emit() {
        for (auto& [id, slot] : slots_) {
            slot();
        }
    }

    // 带数据的信号
    template<typename... Args>
    void emit_with(Args&&... args) {
        for (auto& [id, slot] : slots_) {
            slot(std::forward<Args>(args)...);
        }
    }

private:
    int next_id_ = 0;
    std::map<int, Slot> slots_;
};

// 带类型的信号槽
template<typename... Args>
class TypedSignal {
public:
    using Handler = std::function<void(Args...)>;

    int connect(Handler handler) {
        int id = next_id_++;
        handlers_[id] = std::move(handler);
        return id;
    }

    void disconnect(int id) {
        handlers_.erase(id);
    }

    void emit(Args... args) {
        for (auto& [id, h] : handlers_) {
            h(args...);
        }
    }

private:
    int next_id_ = 0;
    std::map<int, Handler> handlers_;
};

void q2_signal_slot() {
    // 无参信号槽
    Signal on_ready;
    auto conn1 = on_ready.connect([] { std::cout << "Q2: Handler1 called\n"; });
    auto conn2 = on_ready.connect([] { std::cout << "Q2: Handler2 called\n"; });

    on_ready.emit();
    std::cout << "Q2: ---" << std::endl;
    on_ready.disconnect(conn1);
    on_ready.emit();  // 只有 Handler2

    // 带参数信号槽
    TypedSignal<std::string, int> on_data;
    on_data.connect([](const std::string& name, int value) {
        std::cout << "Q2: Data: " << name << " = " << value << std::endl;
    });
    on_data.emit("score", 100);

    // 对比 Qt 信号槽：
    // Qt: QObject::connect() 支持跨线程自动 QueuedConnection
    // Qt 信号可以重载，槽函数需要匹配签名

    // 对比 libsigc++ / boost::signals2：
    // boost::signals2 是线程安全的信号槽实现

    std::cout << "Q2: 信号槽 = 安全的回调容器，支持连接/断开" << std::endl;
}

// ============================================================
// Q3: 消息队列（Message Queue）如何实现？
// ============================================================
template<typename T>
class MessageQueue {
public:
    void push(T msg) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(std::move(msg));
    }

    bool pop(T& msg) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) return false;
        msg = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

private:
    mutable std::mutex mutex_;
    std::queue<T> queue_;
};

// 消息结构
struct Message {
    std::string type;
    std::variant<int, std::string> payload;
};

void q3_message_queue() {
    MessageQueue<Message> mq;

    // 生产者
    mq.push(Message{"login", std::string("Alice")});
    mq.push(Message{"score", 100});
    mq.push(Message{"logout", std::string("Bob")});

    std::cout << "Q3: 队列大小 = " << mq.size() << std::endl;

    // 消费者
    Message msg;
    while (mq.pop(msg)) {
        std::cout << "Q3: 处理消息: " << msg.type << " -> ";
        if (std::holds_alternative<int>(msg.payload)) {
            std::cout << std::get<int>(msg.payload) << std::endl;
        } else {
            std::cout << std::get<std::string>(msg.payload) << std::endl;
        }
    }

    // 线程安全队列用于生产者-消费者
    // std::thread producer([&]() { mq.push(produce()); });
    // std::thread consumer([&]() { while (auto msg = mq.pop()) process(msg); });

    // 对比：
    // ZeroMQ / RabbitMQ / Kafka 是分布式消息队列
    // C++17 用 std::variant 替代联合体

    std::cout << "Q3: 消息队列 = 异步通信，解耦生产者和消费者" << std::endl;
}

// ============================================================
// Q4: 事件循环（Event Loop）如何实现？
// ============================================================
class EventLoop {
public:
    using Task = std::function<void()>;

    void post(Task task) {
        std::lock_guard<std::mutex> lock(mutex_);
        tasks_.push(std::move(task));
    }

    void run() {
        running_ = true;
        while (running_) {
            // 处理任务
            std::vector<Task> local_tasks;
            {
                std::lock_guard<std::mutex> lock(mutex_);
                local_tasks.swap(tasks_);
            }
            for (auto& task : local_tasks) {
                if (task) task();
            }

            // 简单的终止条件（实际应用需要更复杂的逻辑）
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void stop() {
        running_ = false;
    }

    bool is_running() const { return running_; }

private:
    mutable std::mutex mutex_;
    std::vector<Task> tasks_;
    std::atomic<bool> running_{false};
};

void q4_event_loop() {
    EventLoop loop;

    // 投递任务
    loop.post([] { std::cout << "Q4: 任务1\n"; });
    loop.post([] { std::cout << "Q4: 任务2\n"; });
    loop.post([] { std::cout << "Q4: 任务3\n"; });

    // 启动事件循环（在单独线程）
    std::thread t([&loop]() {
        std::cout << "Q4: 事件循环开始\n";
        loop.run();
    });

    // 模拟主线程继续工作
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    loop.post([] { std::cout << "Q4: 动态添加任务\n"; });
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    loop.stop();
    t.join();
    std::cout << "Q4: 事件循环结束\n";

    // 对比：
    // - Node.js: 基于 libuv 的事件循环
    // - Qt: QEventLoop + QApplication
    // - libuv: C 语言的异步 I/O 库
    // - ASIO: Boost.Asio 的事件循环

    std::cout << "Q4: 事件循环 = 单线程异步处理，常见于 GUI 和网络服务器" << std::endl;
}

// ============================================================
// Q5: 中间件（Middleware）模式
// ============================================================
class MiddlewareChain {
public:
    using Handler = std::function<void(int)>;

    void use(const std::string& name, Handler handler) {
        middlewares_.push_back({name, std::move(handler)});
    }

    void execute(int request) {
        int index = 0;
        std::function<void(int)> next = [&](int req) {
            if (index < static_cast<int>(middlewares_.size())) {
                auto& mw = middlewares_[index++];
                std::cout << "Q5: 进入中间件: " << mw.name << std::endl;
                mw.handler(req);
            } else {
                std::cout << "Q5: 最终处理请求: " << req << std::endl;
            }
        };
        next(request);
    }

private:
    struct Middleware {
        std::string name;
        Handler handler;
    };
    std::vector<Middleware> middlewares_;
};

void q5_middleware() {
    MiddlewareChain chain;

    // 日志中间件
    chain.use("Logger", [](int req) {
        std::cout << "Q5: [LOG] 处理请求: " << req << std::endl;
    });

    // 认证中间件
    chain.use("Auth", [](int req) {
        std::cout << "Q5: [AUTH] 检查权限..." << std::endl;
    });

    // 业务逻辑中间件
    chain.use("Handler", [](int req) {
        std::cout << "Q5: [HANDLER] 处理业务: " << req << std::endl;
    });

    chain.execute(42);

    // 对比：
    // - Express.js 中间件模式
    // - ASP.NET Core Middleware
    // - gRPC Interceptors

    std::cout << "Q5: 中间件 = 链式处理，职责分离，便于组合" << std::endl;
}

// ============================================================
// Q6: 回调（Callback）和异步模式
// ============================================================
void q6_callback_async() {
    // 回调函数类型
    using Callback = std::function<void(int, const std::string&)>;

    // 模拟异步操作
    auto async_operation = [](Callback cb) {
        std::thread t([cb]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            cb(200, "Operation completed");
        });
        t.detach();
    };

    // 注册回调
    async_operation([](int code, const std::string& msg) {
        std::cout << "Q6: 回调收到: code=" << code << ", msg=" << msg << std::endl;
    });

    // Promise/Future 模式（C++11）
    std::cout << "Q6: Future/Promise 替代回调，避免回调地狱" << std::endl;

    // C++20 coro（协程）预览
    // co_await / co_return / co_yield
    // auto result = co_await async_task();

    // 对比 Python：
    // Python: 回调 / asyncio（协程）
    // async def fetch(): return await asyncio.to_thread(...)
    // 对比 JavaScript：
    // JavaScript: 回调 → Promise → async/await

    std::cout << "Q6: 回调是异步基础，但 Promise/async 更清晰" << std::endl;
}

// ============================================================
// Q7: Qt 信号槽 vs 自定义事件系统
// ============================================================
void q7_qt_vs_custom() {
    // Qt 信号槽特性：
    // 1. MOC（元对象编译器）生成元数据
    // 2. 跨线程通信（QueuedConnection）
    // 3. 自动断开连接（QObject 生命周期）
    // 4. 信号可以断开后自动连接
    // 5. 线程安全

    // 自定义 vs Qt：
    // | 特性           | 自定义        | Qt           |
    // |--------------|--------------|--------------|
    // | 依赖           | 仅 STL        | Qt 框架       |
    // | 线程安全        | 需自己实现      | 内置           |
    // | 跨线程          | 需要手动处理    | 自动 Queued   |
    // | 内存管理        | 需手动管理      | parent/child |
    // | 性能           | 可能更快       | 略开销         |

    // Boost.Signals2：类似 Qt 的信号槽实现
    // #include <boost/signals2.hpp>
    // boost::signals2::signal<void()> on_click;
    // on_click.connect([](){ std::cout << "clicked\n"; });
    // on_click();

    // 实际建议：
    // - 小项目：自定义信号槽
    // - 大项目：使用成熟库（Qt / Boost.Signals2）
    // - 高性能网络：事件循环 + 直接回调

    std::cout << "Q7: 选择合适的事件系统，Qt 自带最完整" << std::endl;
}

// ============================================================
// 主函数入口
// ============================================================
int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "C++ 专家教程 - 模块十七：信息分发与事件系统" << std::endl;
    std::cout << "============================================" << std::endl;

    q1_observer_pattern();
    q2_signal_slot();
    q3_message_queue();
    q4_event_loop();
    q5_middleware();
    q6_callback_async();
    q7_qt_vs_custom();

    std::cout << "\n[模块十七完成]" << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "C++ 专家教程 - 全部 17 个模块完成！" << std::endl;
    std::cout << "============================================" << std::endl;
    return 0;
}

// ================================================================
// 【练习题】
// ================================================================
// 1. 用 std::function 和 std::bind 实现一个简化的事件总线（EventBus），支持订阅和发布
// 2. 实现一个线程安全的生产者-消费者消息队列，用 std::mutex 和 std::condition_variable 同步
// 3. 用 std::async 和 std::future 实现一个异步任务调度器，支持任务提交和结果获取
// 4. 实现一个状态机模式，演示一个简单订单系统从创建到完成的生命周期状态转换
// 5. 对比 Qt 的信号槽机制和手写的 std::function 回调方案，讨论两者在耦合度和性能上的差异

