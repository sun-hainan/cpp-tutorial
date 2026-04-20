// === 模块十一：并发编程 ===
// 本文件覆盖：线程thread、锁mutex、条件变量condition_variable、原子操作atomic、
//            死锁、线程池、future/promise
// 编译：g++ -std=c++17 -pthread 11_concurrency.cpp -o 11_concurrency.exe

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <future>
#include <vector>
#include <queue>
#include <chrono>
#include <functional>

// ============================================================
// Q1: 如何创建和管理线程？
// ============================================================
void q1_thread_basics() {
    // C++11 线程创建
    auto worker = [](int id) {
        std::cout << "Q1: 线程 " << id << " 开始执行" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Q1: 线程 " << id << " 执行完毕" << std::endl;
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back(worker, i);
    }

    // 等待所有线程完成（join）
    for (auto& t : threads) {
        t.join();  // 主线程阻塞等待
    }
    std::cout << "Q1: 所有线程执行完毕" << std::endl;

    // detach：线程在后台运行（守护线程）
    // 注意：detach 后，主线程不应再访问线程内资源
    // auto detached = std::thread([]{ std::cout << " detached\n"; });
    // detached.detach();

    // std::thread::hardware_concurrency()：CPU 核心数
    std::cout << "Q1: 硬件并发级别: " << std::thread::hardware_concurrency() << std::endl;

    // C++20 jthread（自动 join）
    // std::jthread jt([](int x){ std::cout << x << "\n"; }, 42);
    // 自动 join，无需手动调用

    // 深入原理：
    // - std::thread 创建系统线程（POSIX threads / Windows threads）
    // - 线程栈大小：默认约 8MB（可配置）
    // - 线程调度：操作系统决定（时间片轮转）

    // 对比 Python：
    // Python: threading.Thread(target=worker, args=(id,)).start()
    // Python GIL 限制同一时刻只有一个线程执行 Python 字节码
    // 对比 Rust：
    // Rust: std::thread::spawn(|| { ... })
    // Rust jthread（C++20 类似）在 Rust 1.63+ 可用
    std::cout << "Q1: std::thread 创建线程，join 等待完成" << std::endl;
}

// ============================================================
// Q2: mutex 如何实现线程同步？
// ============================================================
std::mutex mtx;
int shared_counter = 0;

void q2_mutex() {
    // 基本 mutex
    {
        std::lock_guard<std::mutex> lock(mtx);  // RAII 加锁
        ++shared_counter;
    }  // 析构时自动解锁

    // std::unique_lock：更灵活的 RAII 锁
    {
        std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
        lock.lock();
        ++shared_counter;
        lock.unlock();  // 可提前解锁
        lock.lock();
        ++shared_counter;
    }

    // std::lock（避免死锁的原子多锁获取）
    std::mutex m1, m2;
    // std::lock(m1, m2);  // 原子获取多个锁

    // C++17 scoped_lock（多锁_guard）
    // {
    //     std::scoped_lock lock(m1, m2);  // C++17
    //     // 同时持有 m1 和 m2
    // }

    // std::timed_mutex：带超时的锁
    std::timed_mutex tmtx;
    bool got_lock = tmtx.try_lock_for(std::chrono::milliseconds(100));
    if (got_lock) {
        std::cout << "Q2: 获取 timed_mutex 成功" << std::endl;
        tmtx.unlock();
    } else {
        std::cout << "Q2: 获取 timed_mutex 超时" << std::endl;
    }

    // 深入原理：
    // - mutex 依赖操作系统同步原语（pthread_mutex / CRITICAL_SECTION）
    // - 加锁有开销（用户态→内核态跳转），避免频繁加解锁
    // - 自旋锁：短临界区用自旋避免内核态切换

    // 对比 Python：
    // Python: threading.Lock()，with lock: ...
    // 对比 Rust：
    // Rust: std::sync::Mutex<T>，lock().unwrap()
    std::cout << "Q2: 锁保护共享数据，粒度要合适" << std::endl;
}

// ============================================================
// Q3: 什么是死锁？如何避免？
// ============================================================
std::mutex mtx_a, mtx_b;

void q3_deadlock() {
    // 死锁场景：两个线程互相等待对方持有的锁

    // 避免死锁的原则：
    // 1. 固定加锁顺序（所有线程按相同顺序获取锁）
    // 2. 使用 std::lock 同时获取多个锁
    // 3. 避免在持锁时调用用户代码

    auto process_a = [&]() {
        // 正确做法：始终按 A→B 顺序
        std::lock_guard<std::mutex> lock_a(mtx_a);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::lock_guard<std::mutex> lock_b(mtx_b);
        std::cout << "Q3: 线程 A 完成" << std::endl;
    };

    auto process_b_correct = [&]() {
        // 正确：按 A→B 顺序（与 A 一致）
        std::lock_guard<std::mutex> lock_a(mtx_a);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::lock_guard<std::mutex> lock_b(mtx_b);
        std::cout << "Q3: 线程 B 完成" << std::endl;
    };

    // 死锁示例：
    auto process_b_wrong = [&]() {
        // 错误：按 B→A 顺序，可能死锁
        std::lock_guard<std::mutex> lock_b(mtx_b);  // B 先
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::lock_guard<std::mutex> lock_a(mtx_a);  // A 后
        std::cout << "Q3: 线程 B（错误顺序）完成" << std::endl;
    };

    // 用 scoped_lock 避免死锁（C++17）
    // auto process_c = [&]() {
    //     std::scoped_lock lock(mtx_a, mtx_b);  // 原子获取
    //     std::cout << "Q3: 线程 C 完成" << std::endl;
    // };

    // 其他避免死锁的策略：
    // - 使用 std::try_lock 尝试获取
    // - 限制锁的持有时间
    // - 使用读写锁（std::shared_mutex，C++17）

    std::cout << "Q3: 避免死锁：固定顺序/原子多锁/缩小临界区" << std::endl;
}

// ============================================================
// Q4: 原子操作（atomic）如何使用？
// ============================================================
std::atomic<int> atomic_counter(0);  // 原子整数
std::atomic<bool> ready(false);

void q4_atomic() {
    // atomic 提供无锁线程安全操作
    atomic_counter.store(42);               // 原子写入
    int val = atomic_counter.load();         // 原子读取
    std::cout << "Q4: atomic load = " << val << std::endl;

    // 原子读-改-写操作
    int old = atomic_counter.fetch_add(1);   // 返回旧值
    std::cout << "Q4: fetch_add 旧值 = " << old << std::endl;
    std::cout << "Q4: fetch_add 后 = " << atomic_counter.load() << std::endl;

    // CAS（Compare-And-Swap）
    int expected = 43;
    bool exchanged = atomic_counter.compare_exchange_strong(expected, 50);
    std::cout << "Q4: CAS 结果 = " << exchanged
              << ", counter = " << atomic_counter.load() << std::endl;

    // atomic<bool] 特殊操作
    bool r = false;
    ready.store(true, std::memory_order_release);  // 内存顺序

    // 内存顺序（memory_order）：
    // - memory_order_relaxed：无同步要求
    // - memory_order_acquire：获取屏障
    // - memory_order_release：释放屏障
    // - memory_order_acq_rel：获取+释放
    // - memory_order_seq_cst：顺序一致（默认，最安全）

    // C++20 原子 wait/notify
    // atomic_counter.wait(50);  // 阻塞直到值改变

    // 深入原理：
    // - 原子操作由 CPU 提供（x86 LOCK prefix / ARM LDREX/STREX）
    // - 保证操作的原子性：不可分割，硬件保证可见性
    // - 原子整数通常比 mutex 快（不需要内核态切换）

    // 对比 Python：
    // Python: asyncio / threading.Lock（无原生 atomic）
    // Python 的 int 是原子操作（C 实现层面）
    // 对比 Rust：
    // Rust: std::sync::atomic::AtomicI32 等
    // Rust atomic 语义与 C++ 完全一致
    std::cout << "Q4: atomic 是无锁并发的基础，性能优于锁" << std::endl;
}

// ============================================================
// Q5: 条件变量（condition_variable）如何使用？
// ============================================================
std::mutex cv_mutex;
std::condition_variable cv;
bool data_ready = false;

void q5_condition_variable() {
    // 条件变量：线程间等待某个条件成立

    // 等待线程
    auto waiter = [&]() {
        std::unique_lock<std::mutex> lock(cv_mutex);
        cv.wait(lock, [] { return data_ready; });  // 条件为真才返回
        std::cout << "Q5: 等待线程收到通知，数据已准备好" << std::endl;
    };

    // 通知线程
    auto notifier = [&]() {
        {
            std::lock_guard<std::mutex> lock(cv_mutex);
            data_ready = true;
        }
        cv.notify_one();  // 通知一个等待线程
        // cv.notify_all();  // 通知所有等待线程
    };

    std::thread w(waiter);
    std::thread n(notifier);

    w.join();
    n.join();

    // 生产者-消费者模式示例
    std::queue<int> queue;
    std::mutex queue_mutex;
    std::condition_variable queue_cv;

    auto producer = [&]() {
        for (int i = 0; i < 5; ++i) {
            {
                std::lock_guard<std::mutex> lock(queue_mutex);
                queue.push(i);
                std::cout << "Q5: 生产: " << i << std::endl;
            }
            queue_cv.notify_one();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    };

    auto consumer = [&]() {
        while (true) {
            std::unique_lock<std::mutex> lock(queue_mutex);
            queue_cv.wait(lock, [&] { return !queue.empty(); });
            int val = queue.front();
            queue.pop();
            std::cout << "Q5: 消费: " << val << std::endl;
            if (val == 4) break;  // 消费完退出
        }
    };

    std::thread p(producer);
    std::thread c(consumer);
    p.join();
    c.join();

    // 深入原理：
    // - cv.wait() 内部：释放锁 → 阻塞 → 被通知 → 重新获取锁
    // - spurious wakeup：wait 后可能无故醒来，必须用条件检查
    // - notify 丢失：必须在持锁状态下检查条件

    std::cout << "Q5: 条件变量用于线程间事件通知" << std::endl;
}

// ============================================================
// Q6: future/promise 和 async 是什么？
// ============================================================
void q6_future_promise() {
    // std::promise + std::future：线程间传递结果
    std::promise<int> promise;
    std::future<int> future = promise.get_future();

    auto worker = [&](std::promise<int>& p) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        p.set_value(42);  // 设置结果
    };

    std::thread t(worker, std::ref(promise));
    int result = future.get();  // 阻塞直到结果就绪
    std::cout << "Q6: future.get() = " << result << std::endl;
    t.join();

    // std::async：更高级的异步调用
    auto async_task = [](int x) -> int {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        return x * 2;
    };

    std::future<int> f1 = std::async(std::launch::async, async_task, 21);
    std::future<int> f2 = std::async(std::launch::deferred, async_task, 10);

    std::cout << "Q6: async 结果1 = " << f1.get() << std::endl;
    std::cout << "Q6: async 结果2 = " << f2.get() << std::endl;

    // std::launch::async | std::launch::deferred
    // async 默认可能是 deferred（实现定义），明确用 async 保证

    // 异常传播
    std::promise<void> p_exc;
    std::future<void> f_exc = p_exc.get_future();
    p_exc.set_exception(std::make_exception_ptr(std::runtime_error("async error")));
    try {
        f_exc.get();
    } catch (const std::exception& e) {
        std::cout << "Q6: 捕获异步异常: " << e.what() << std::endl;
    }

    // C++20 std::jthread 与 std::future 结合
    // std::future 可以用于取消异步任务

    // 深入原理：
    // - future/promise 通过共享状态通信（可能用条件变量实现）
    // - get() 只能调用一次（shared_future 可多次 get）
    // - async 可能使用线程池（避免每次创建新线程）

    // 对比 Python：
    // Python: concurrent.futures.Future / asyncio.Future
    // asyncio.run() / await 语法
    // 对比 Rust：
    // Rust: std::future::Future / async/await（需要 tokio 等运行时）
    std::cout << "Q6: async 是最简洁的异步调用方式" << std::endl;
}

// ============================================================
// Q7: 什么是线程池？如何实现？
// ============================================================
class ThreadPool {
public:
    ThreadPool(size_t n_threads) : stop_(false) {
        for (size_t i = 0; i < n_threads; ++i) {
            workers_.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(mutex_);
                        cv_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
                        if (stop_ && tasks_.empty()) return;
                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }
                    task();  // 执行任务
                }
            });
        }
    }

    template<typename F>
    void enqueue(F&& f) {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            if (stop_) throw std::runtime_error("线程池已停止");
            tasks_.emplace(std::forward<F>(f));
        }
        cv_.notify_one();
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            stop_ = true;
        }
        cv_.notify_all();
        for (auto& w : workers_) {
            if (w.joinable()) w.join();
        }
    }

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool stop_;
};

void q7_thread_pool() {
    ThreadPool pool(4);

    std::vector<std::future<int>> futures;
    for (int i = 0; i < 8; ++i) {
        futures.emplace_back(
            pool.enqueue([i]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                return i * i;
            })
        );
        // 注意：pool.enqueue 返回 void，这里用另一种方式
    }

    // 正确的线程池用法
    std::vector<std::future<void>> void_futures;
    for (int i = 0; i < 4; ++i) {
        void_futures.emplace_back(
            pool.enqueue([i]() {
                std::cout << "Q7: 任务 " << i << " 在线程 "
                          << std::this_thread::get_id() << " 执行" << std::endl;
            })
        );
    }
    for (auto& f : void_futures) f.get();

    // 注意：实际项目用 std::async 或专业库（ThreadPool-T好了）

    // C++20 std::executeoner（单线程执行器）
    // C++ 标准库尚未提供开箱即用的线程池

    // 深入原理：
    // - 线程池：预创建线程，避免频繁创建/销毁线程的开销
    // - work-stealing：空闲线程从其他线程队列"偷"任务
    // - 线程池大小：CPU 密集型 = 核心数，IO 密集型可 > 核心数

    // 对比 Python：
    // concurrent.futures.ThreadPoolExecutor / ProcessPoolExecutor
    // 对比 Rust：
    // tokio / async-std 等异步运行时
    std::cout << "Q7: 线程池避免频繁创建线程，生产环境推荐使用" << std::endl;
}

// ============================================================
// 主函数入口
// ============================================================
int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "C++ 专家教程 - 模块十一：并发编程" << std::endl;
    std::cout << "============================================" << std::endl;

    q1_thread_basics();
    q2_mutex();
    q3_deadlock();
    q4_atomic();
    q5_condition_variable();
    q6_future_promise();
    q7_thread_pool();

    std::cout << "\n[模块十一完成]" << std::endl;
    return 0;
}
