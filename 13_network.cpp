// === 模块十三：网络编程 ===
// 本文件覆盖：socket、TCP、UDP、select、epoll、HTTP协议、Boost.Asio预览
// 编译：g++ -std=c++17 13_network.cpp -o 13_network.exe
// 注意：Windows 需要 -lws2_32，Linux 需要 -pthread

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <thread>
#include <chrono>
#include <atomic>

// 跨平台 socket 头文件
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef int socklen_t;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>
    typedef int SOCKET;
    const int INVALID_SOCKET = -1;
    const int SOCKET_ERROR = -1;
#endif

// ============================================================
// Q1: 什么是 socket？TCP 和 UDP 的区别是什么？
// ============================================================
void q1_socket_tcp_udp() {
    // Socket：网络通信的端点（IP + Port）

    // TCP vs UDP 对比：
    // | 特性       | TCP              | UDP              |
    // |-----------|------------------|------------------|
    // | 连接       | 面向连接          | 无连接            |
    // | 可靠性      | 可靠（有重传/确认） | 不可靠            |
    // | 顺序       | 按序到达           | 不保证顺序         |
    // | 速度       | 较慢              | 快                |
    // | 开销       | 头部 20-60 字节    | 8 字节            |
    // | 适用场景     | 文件传输/网页/HTTP | 视频/语音/DNS     |

    // Socket 类型
    // SOCK_STREAM = TCP
    // SOCK_DGRAM = UDP
    // SOCK_RAW = 原始套接字（自定义协议）

    // IP 地址和端口
    // struct sockaddr_in {
    //     short sin_family;      // AF_INET（IPv4）
    //     unsigned short sin_port; // 端口号（网络字节序）
    //     struct in_addr sin_addr;  // IP 地址
    //     char sin_zero[8];      // 填充
    // };

    // 网络字节序（大端）和主机字节序（小端）
    // htons() / htonl()：主机→网络
    // ntohs() / ntohl()：网络→主机

    std::cout << "Q1: Socket = IP + Port，TCP 可靠/UDP 快速" << std::endl;
}

// ============================================================
// Q2: 如何实现 TCP 服务器和客户端？
// ============================================================
bool g_winsock_init = false;

void q2_tcp_server_client() {
#ifdef _WIN32
    // Windows 初始化 Winsock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "Q2: WSAStartup 失败" << std::endl;
        return;
    }
    g_winsock_init = true;
#endif

    // 创建 socket
    SOCKET server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_sock == INVALID_SOCKET) {
        std::cerr << "Q2: socket 创建失败" << std::endl;
        return;
    }

    // 绑定地址
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = INADDR_ANY;  // 监听所有网卡

    if (bind(server_sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cerr << "Q2: bind 失败，端口可能被占用" << std::endl;
#ifdef _WIN32
        closesocket(server_sock);
        WSACleanup();
#else
        close(server_sock);
#endif
        return;
    }

    // 监听连接
    if (listen(server_sock, 5) == SOCKET_ERROR) {
        std::cerr << "Q2: listen 失败" << std::endl;
#ifdef _WIN32
        closesocket(server_sock);
        WSACleanup();
#else
        close(server_sock);
#endif
        return;
    }

    std::cout << "Q2: TCP 服务器监听端口 8888..." << std::endl;
    std::cout << "Q2: （演示用，非阻塞模式仅列出 API）" << std::endl;

    // 完整实现需要多线程或 select/epoll
    // 典型 TCP 服务器循环：
    // while (true) {
    //     SOCKET client = accept(server_sock, nullptr, nullptr);
    //     // 处理 client
    //     std::thread t([client]() {
    //         // recv(client, buffer, sizeof(buffer), 0);
    //         // send(client, response, strlen(response), 0);
    //         // closesocket(client);
    //     });
    //     t.detach();
    // }

#ifdef _WIN32
    closesocket(server_sock);
    WSACleanup();
#else
    close(server_sock);
#endif

    // 深入原理：
    // - TCP 三次握手：SYN → SYN-ACK → ACK
    // - TCP 四次挥手：FIN → ACK → FIN → ACK
    // - socket 是文件描述符（Linux）或句柄（Windows）

    // 对比 Python：
    // Python: socket.socket(), bind(), listen(), accept(), connect()
    // import socket; s = socket.socket(); s.bind(('127.0.0.1', 8888))
    // 对比 Rust：
    // Rust: std::net::TcpListener / TcpStream
    std::cout << "Q2: TCP 编程：socket → bind → listen → accept → recv/send" << std::endl;
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
// Q3: select 和 epoll 的区别是什么？
// ============================================================
void q3_select_epoll() {
    // I/O 多路复用：单线程管理多个 socket

    // select：跨平台，fd_setsize 限制（Linux 1024）
    // fd_set readfds;
    // FD_ZERO(&readfds);
    // FD_SET(socket_fd, &readfds);
    // struct timeval timeout = {1, 0};  // 1 秒超时
    // int n = select(max_fd + 1, &readfds, nullptr, nullptr, &timeout);
    // if (FD_ISSET(socket_fd, &readfds)) { /* 可读 */ }

    // epoll（Linux 独有，更高效）
    // int epfd = epoll_create1(0);
    // struct epoll_event ev = { .events = EPOLLIN, .data.fd = sock_fd };
    // epoll_ctl(epfd, EPOLL_CTL_ADD, sock_fd, &ev);
    // struct epoll_event events[10];
    // int n = epoll_wait(epfd, events, 10, -1);  // -1 = 无限等待

    // epoll vs select 对比：
    // | 特性        | select              | epoll               |
    // |------------|---------------------|---------------------|
    // | 最大 fd 数   | FD_SETSIZE (1024)   | 无限制（受内存）       |
    // | 时间复杂度    | O(n)（线性扫描）      | O(1)（直接索引）       |
    // | 触发方式      | 水平触发             | 水平 + 边缘触发        |
    // | 内核数据结构    | 每次调用传入           | 每次调用传入/复用       |
    // | 跨平台        | 跨平台               | 仅 Linux             |

    // C++17 / Boost.Asio：跨平台异步 I/O
    // Boost.Asio 在 Linux 底层用 epoll

    std::cout << "Q3: select O(n) 有 fd 限制，epoll O(1) 无限制" << std::endl;
}

// ============================================================
// Q4: UDP 如何编程？
// ============================================================
void q4_udp() {
    // UDP：无连接，不可靠但高效

    // 创建 UDP socket
    // SOCKET udp_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // 绑定（服务器）
    // bind(udp_sock, ...);

    // 发送（客户端）
    // sendto(sock, buffer, size, 0, (sockaddr*)&dest_addr, sizeof(dest_addr));

    // 接收
    // recvfrom(sock, buffer, sizeof(buffer), 0, (sockaddr*)&src_addr, &addr_len);

    // UDP 示例：DNS 查询 / NTP 时间同步

    std::cout << "Q4: UDP: socket → recvfrom/sendto（无连接）" << std::endl;
}

// ============================================================
// Q5: HTTP 协议的基本结构是什么？
// ============================================================
void q5_http_protocol() {
    // HTTP 请求格式：
    // GET /index.html HTTP/1.1\r\n
    // Host: www.example.com\r\n
    // User-Agent: C++ Client\r\n
    // \r\n
    // [可选 body]

    // HTTP 响应格式：
    // HTTP/1.1 200 OK\r\n
    // Content-Type: text/html\r\n
    // Content-Length: 123\r\n
    // \r\n
    // <html>...</html>

    // 状态码：
    // 200 OK
    // 301/302 重定向
    // 400 Bad Request
    // 401 Unauthorized
    // 403 Forbidden
    // 404 Not Found
    // 500 Internal Server Error

    // C++17 简单 HTTP 客户端（使用 socket）
    std::cout << "Q5: HTTP 是文本协议，请求/响应格式清晰" << std::endl;

    // C++ HTTP 库推荐：
    // - libcurl（成熟稳定）
    // - Boost.Asio（异步 HTTP 客户端）
    // - httplib（C++11 单头文件库）
    // - cpp-httplib：auto res = cli.Get("/hello");
}

// ============================================================
// Q6: Boost.Asio 预览
// ============================================================
void q6_boost_asio_preview() {
    // Boost.Asio：异步 I/O 的跨平台库（C++11 前是 Boost，现在独立）
    // 支持：TCP/UDP/SSL/串口/定时器/信号

    // 注意：需要安装 Boost 库
    // #include <boost/asio.hpp>

    // 同步 TCP 服务器（Asio 风格）
    // using boost::asio::ip::tcp;
    // tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8888));
    // while (true) {
    //     tcp::socket socket(io_context);
    //     acceptor.accept(socket);
    //     std::string msg = "Hello from Asio";
    //     boost::asio::write(socket, boost::asio::buffer(msg));
    // }

    // 异步服务器（Asio 核心优势）
    // async_accept(socket, [&](const error_code& ec) {
    //     if (!ec) { /* 处理连接 */ }
    //     do_accept();  // 继续接受下一个
    // });

    // C++20 networking::net TS（C++ 标准库的 Asio）
    // namespace net = std::experimental::net;
    // 标准化进行中

    std::cout << "Q6: Boost.Asio 是生产级异步 I/O 库" << std::endl;
    std::cout << "Q6: C++20 std::net TS 标准化 Asio 接口" << std::endl;
}

// ============================================================
// Q7: 网络编程的常见问题和安全考虑
// ============================================================
void q7_network_security() {
    // 常见问题：

    // 1. 字节序（大小端）
    // 始终用 htons/ntohs 处理端口，htonl/ntohl 处理 IP

    // 2. Socket 描述符泄漏
    // RAII 封装 SOCKET：析构时closesocket/close

    // 3. 缓冲区溢出
    // 检查 recv 返回值，不信任输入大小

    // 4. 整数溢出
    // Content-Length 可能是恶意的，需要验证

    // 5. DNS 欺骗
    // 验证证书（HTTPS）

    // 6. 数据竞争
    // 多线程访问 socket 需要加锁

    // 安全建议：
    // - 优先使用 HTTPS
    // - 限制连接数/频率（防止 DoS）
    // - 输入验证（所有外部数据不可信）
    // - 使用成熟库（libcurl/Asio）而非手写解析器

    // 对比 Python：
    // Python: requests 库封装好了 HTTPS / 证书验证
    // 对比 Rust：
    // Rust: hyper（HTTP 客户端/服务器），tokio 异步运行时

    std::cout << "Q7: 网络安全：验证输入、使用 TLS、限制资源" << std::endl;
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
    std::cout << "C++ 专家教程 - 模块十三：网络编程" << std::endl;
    std::cout << "============================================" << std::endl;

    q1_socket_tcp_udp();
    q2_tcp_server_client();
    q3_select_epoll();
    q4_udp();
    q5_http_protocol();
    q6_boost_asio_preview();
    q7_network_security();

    std::cout << "\n[模块十三完成]" << std::endl;
    std::cout << "注意：网络编程需要链接对应库（Linux: -pthread）" << std::endl;
    return 0;
}

// ================================================================
// 【练习题】
// ================================================================
// 1. 实现一个简易的 TCP 客户端单向连接程序
// 2. 用 select 实现一个单输入多输出的网络服务器
// 3. 编写一个 UDP 广播程序，对比 TCP 和 UDP 的差异
// 4. 用 POSIX socket API 实现一个简易的 HTTP 客户端，能发送一个 GET 请求
// 5. 实现一个基于 epoll 的并发多输入外网络服务器

