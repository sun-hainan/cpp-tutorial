// === 模块十五：数据库编程 ===
// 本文件覆盖：SQL、ODBC、MySQL Connector、事务、ORM、参数化查询
// 编译：g++ -std=c++17 15_database.cpp -o 15_database.exe
// 注意：需要安装 MySQL Connector/C++ 或其他数据库驱动

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>

// ============================================================
// Q1: SQL 基础和常用语句
// ============================================================
void q1_sql_basics() {
    // SQL 四大基本操作（CRUD）

    // 1. INSERT - 插入
    std::cout << "Q1: INSERT 示例" << std::endl;
    std::cout << "INSERT INTO users (name, age, email) VALUES ('Alice', 25, 'alice@example.com');" << std::endl;

    // 2. SELECT - 查询
    std::cout << "Q1: SELECT 示例" << std::endl;
    std::cout << "SELECT id, name, age FROM users WHERE age > 18 ORDER BY age DESC LIMIT 10;" << std::endl;

    // 3. UPDATE - 更新
    std::cout << "Q1: UPDATE 示例" << std::endl;
    std::cout << "UPDATE users SET age = 26 WHERE name = 'Alice';" << std::endl;

    // 4. DELETE - 删除
    std::cout << "Q1: DELETE 示例" << std::endl;
    std::cout << "DELETE FROM users WHERE id = 5;" << std::endl;

    // 常用子句
    // WHERE - 条件过滤
    // ORDER BY column [ASC|DESC] - 排序
    // LIMIT n - 限制数量
    // JOIN - 表连接（INNER/LEFT/RIGHT）
    // GROUP BY - 分组
    // HAVING - 分组后过滤
    // DISTINCT - 去重

    // 聚合函数
    // COUNT(), SUM(), AVG(), MIN(), MAX()

    // SQL 注入漏洞演示（错误做法）
    // std::string query = "SELECT * FROM users WHERE name = '" + user_input + "'";
    // 如果 user_input = "' OR '1'='1"，整张表被暴露！

    // 深入原理：
    // - SQL 是声明式语言，描述"做什么"而非"怎么做"
    // - 数据库引擎负责优化执行计划
    // - 索引加速查询（类似书的目录）

    // 对比 Python：
    // Python: sqlite3, psycopg2, mysql-connector-python
    // SQLAlchemy ORM 封装 SQL
    // 对比 Rust：
    // Rust: diesel, sqlx（编译时验证 SQL）
    std::cout << "Q1: SQL 是数据库操作的标准语言" << std::endl;
}

// ============================================================
// Q2: 参数化查询和 SQL 注入防护
// ============================================================
void q2_parameterized_queries() {
    // 参数化查询（Prepared Statements）：将 SQL 结构与数据分离

    // ❌ 不安全：字符串拼接
    // std::string unsafe = "SELECT * FROM users WHERE name = '" + name + "'";

    // ✅ 安全：参数化查询
    std::cout << "Q2: 参数化查询示例（伪代码）" << std::endl;
    std::cout << "PREPARE stmt FROM 'SELECT * FROM users WHERE name = ?';" << std::endl;
    std::cout << "EXECUTE stmt USING @name;" << std::endl;

    // MySQL Connector/C++ 示例结构
    // sql::Driver* driver = sql::mysql::get_driver_instance();
    // std::unique_ptr<sql::Connection> conn(driver->connect(url, user, pass));
    // std::unique_ptr<sql::PreparedStatement> pstmt(
    //     conn->prepareStatement("SELECT * FROM users WHERE name = ?"));
    // pstmt->setString(1, "Alice");
    // std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

    // ODBC 参数化查询
    // SQLHSTMT hstmt;
    // SQLPrepare(hstmt, "SELECT * FROM users WHERE name = ?", SQL_NTS);
    // SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, name, strlen(name), nullptr);
    // SQLExecute(hstmt);

    // 为什么参数化查询安全？
    // - 数据被当作纯数据，不会被解释为 SQL 代码
    // - ' OR '1'='1' 被当作字符串字面量处理

    // C++17 安全实践
    std::string user_name = "Alice'; DROP TABLE users; --";  // 恶意输入
    // 在参数化查询中，这只是普通字符串，不会被执行

    std::cout << "Q2: 参数化查询是防止 SQL 注入的最佳实践" << std::endl;
}

// ============================================================
// Q3: 事务和 ACID 特性
// ============================================================
void q3_transactions() {
    // 事务：原子性执行的 SQL 语句序列

    // ACID 特性：
    // - Atomicity（原子性）：全部成功或全部失败
    // - Consistency（一致性）：事务前后数据库状态一致
    // - Isolation（隔离性）：并发事务互不干扰
    // - Durability（持久性）：提交后数据永久保存

    std::cout << "Q3: 事务控制语句" << std::endl;
    std::cout << "START TRANSACTION;" << std::endl;
    std::cout << "  UPDATE accounts SET balance = balance - 100 WHERE id = 1;" << std::endl;
    std::cout << "  UPDATE accounts SET balance = balance + 100 WHERE id = 2;" << std::endl;
    std::cout << "COMMIT;" << std::endl;
    std::cout << "-- 或 ROLLBACK; -- 出错时回滚" << std::endl;

    // 隔离级别（Isolation Levels）
    // | 级别          | 脏读   | 不可重复读 | 幻读   |
    // |-------------|-------|---------|------|
    // | Read Uncommitted | 可能   | 可能     | 可能   |
    // | Read Committed  | 不可能 | 可能     | 可能   |
    // | Repeatable Read | 不可能 | 不可能    | 可能   |
    // | Serializable    | 不可能 | 不可能    | 不可能  |

    // C++ 事务示例（伪代码）
    // conn->setAutoCommit(false);
    // try {
    //     stmt->execute("UPDATE accounts SET balance = balance - 100 WHERE id = 1");
    //     stmt->execute("UPDATE accounts SET balance = balance + 100 WHERE id = 2");
    //     conn->commit();
    // } catch (...) {
    //     conn->rollback();
    // }

    // 深入原理：
    // - 事务日志（WAL - Write Ahead Log）
    // - 两阶段提交（2PC）用于分布式事务
    // - MVCC（多版本并发控制）

    // 对比 Python：
    // Python: connection.begin() / connection.commit() / connection.rollback()
    // 对比 Rust：
    // Rust: transaction.commit() / transaction.rollback()
    std::cout << "Q3: 事务确保数据一致性，ACID 是事务的核心特性" << std::endl;
}

// ============================================================
// Q4: MySQL Connector/C++ 基本使用
// ============================================================
void q4_mysql_connector() {
    std::cout << "Q4: MySQL Connector/C++ 使用示例（需要安装驱动）" << std::endl;

    // 典型 MySQL Connector/C++ 代码结构：
    // try {
    //     sql::Driver* driver = sql::mysql::get_driver_instance();
    //     auto conn = driver->connect("tcp://127.0.0.1:3306", "user", "password");
    //     conn->setSchema("testdb");
    //
    //     // 查询
    //     std::unique_ptr<sql::Statement> stmt(conn->createStatement());
    //     std::unique_ptr<sql::ResultSet> res(
    //         stmt->executeQuery("SELECT id, name, age FROM users"));
    //     while (res->next()) {
    //         std::cout << "ID: " << res->getInt("id")
    //                   << ", Name: " << res->getString("name")
    //                   << ", Age: " << res->getInt("age") << std::endl;
    //     }
    //
    //     // 预编译语句
    //     std::unique_ptr<sql::PreparedStatement> pstmt(
    //         conn->prepareStatement("INSERT INTO users (name, age) VALUES (?, ?)"));
    //     pstmt->setString(1, "Bob");
    //     pstmt->setInt(2, 30);
    //     pstmt->execute();
    //
    // } catch (sql::SQLException& e) {
    //     std::cerr << "SQL Error: " << e.what() << std::endl;
    // }

    // 编译安装：
    // Ubuntu: sudo apt install libmysqlcppconn-dev
    // 编译: g++ -std=c++17 $(mysql_config --cflags) source.cpp $(mysql_config --libs)

    std::cout << "Q4: MySQL Connector/C++ 提供面向对象的数据库接口" << std::endl;
}

// ============================================================
// Q5: ODBC（开放数据库连接）概述
// ============================================================
void q5_odbc() {
    // ODBC：跨平台数据库访问标准

    std::cout << "Q5: ODBC API 基本流程" << std::endl;
    std::cout << "1. SQLAllocHandle - 分配环境句柄" << std::endl;
    std::cout << "2. SQLSetEnvAttr - 设置环境属性" << std::endl;
    std::cout << "3. SQLAllocHandle - 分配连接句柄" << std::endl;
    std::cout << "4. SQLConnect - 连接数据库" << std::endl;
    std::cout << "5. SQLExecDirect - 执行 SQL" << std::endl;
    std::cout << "6. SQLFetch / SQLGetData - 获取结果" << std::endl;
    std::cout << "7. SQLFreeHandle - 释放资源" << std::endl;

    // ODBC 伪代码
    // SQLHENV henv; SQLHDBC hdbc; SQLHSTMT hstmt;
    // SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    // SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    // SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
    // SQLConnect(hdbc, "MyDSN", SQL_NTS, "user", SQL_NTS, "pass", SQL_NTS);
    // SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
    // SQLExecDirect(hstmt, (SQLCHAR*)"SELECT * FROM users", SQL_NTS);
    // while (SQLFetch(hstmt) == SQL_SUCCESS) {
    //     char name[256]; SQLGetData(hstmt, 1, SQL_C_CHAR, name, 256, nullptr);
    //     std::cout << name << std::endl;
    // }
    // SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    // SQLDisconnect(hdbc);
    // SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
    // SQLFreeHandle(SQL_HANDLE_ENV, henv);

    // ODBC vs Connector/C++
    // ODBC: 更通用，Windows/Linux/macOS，支持多种数据库
    // Connector/C++: 更易用，专为 MySQL 优化

    // 对比 Python：
    // Python: pyodbc / JayDeBeApi
    // 对比 Rust：
    // Rust: odbc crate
    std::cout << "Q5: ODBC 是数据库访问的通用标准" << std::endl;
}

// ============================================================
// Q6: ORM（对象关系映射）简介
// ============================================================
void q6_orm() {
    // ORM：将数据库表映射为编程语言的类/对象

    // C++ ORM 库：
    // - SOCI：类似 JDBC 的 ORM
    // - ODB：C++ ORM 编译器（基于 C++11）
    // - DBMSTL：轻量级 ORM

    // ORM 概念示例
    // 表：users (id, name, age, email)
    // class User {
    // public:
    //     int id;
    //     std::string name;
    //     int age;
    //     std::string email;
    // };
    // ORM 操作：
    // User u;
    // db.load(u, 1);  // SELECT * FROM users WHERE id = 1
    // u.age = 26;
    // db.store(u);    // UPDATE users SET age = 26 WHERE id = 1
    // db.erase(u);    // DELETE FROM users WHERE id = 1

    // ORM vs 手写 SQL
    // | 方面      | ORM       | 手写 SQL    |
    // |----------|-----------|-------------|
    // | 开发效率    | 高        | 低           |
    // | 性能       | 略低（可能有额外开销）| 高（精确控制）    |
    // | 可读性      | 好（面向对象） | 好（SQL 直观）    |
    // | 复杂查询    | 困难        | 灵活          |
    // | 移植性      | 好（抽象层）  | 差（方言差异）     |

    // C++ ORM 最佳实践：
    // - 简单 CRUD 用 ORM
    // - 复杂查询用手写 SQL
    // - 混合使用

    // 对比 Python：
    // SQLAlchemy（Python 最流行的 ORM）
    // Django ORM
    // 对比 Rust：
    // Diesel: 编译时检查的 ORM
    std::cout << "Q6: ORM 提高开发效率，复杂场景仍需手写 SQL" << std::endl;
}

// ============================================================
// Q7: 连接池和性能优化
// ============================================================
void q7_connection_pool() {
    // 连接池：复用数据库连接，避免频繁创建/销毁的开销

    std::cout << "Q7: 连接池原理" << std::endl;
    std::cout << "1. 启动时创建 N 个连接" << std::endl;
    std::cout << "2. 请求时从池中取可用连接" << std::endl;
    std::cout << "3. 使用完毕归还连接池" << std::endl;
    std::cout << "4. 空闲连接可被复用" << std::endl;

    // 连接池参数
    // - 最小连接数
    // - 最大连接数
    // - 连接超时
    // - 空闲超时
    // - 健康检查

    // C++ 连接池实现示例（概念）
    // class ConnectionPool {
    //     std::queue<std::unique_ptr<sql::Connection>> pool_;
    //     std::mutex mutex_;
    //     std::condition_variable cv_;
    // public:
    //     std::unique_ptr<sql::Connection> acquire() {
    //         std::unique_lock<std::mutex> lock(mutex_);
    //         cv_.wait(lock, [&]{ return !pool_.empty(); });
    //         return std::move(pool_.front());
    //     }
    //     void release(std::unique_ptr<sql::Connection> conn) {
    //         std::lock_guard<std::mutex> lock(mutex_);
    //         pool_.push(std::move(conn));
    //         cv_.notify_one();
    //     }
    // };

    // 性能优化技巧
    // 1. 使用连接池
    // 2. 批量插入（INSERT INTO ... VALUES (...), (...), ...)
    // 3. 预编译语句复用
    // 4. 合理使用索引
    // 5. 分页查询代替大结果集
    // 6. 异步查询（数据库异步 API）

    std::cout << "Q7: 连接池 + 预编译语句 + 合理索引 = 高性能数据库访问" << std::endl;
}

// ============================================================
// 主函数入口
// ============================================================
int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "C++ 专家教程 - 模块十五：数据库编程" << std::endl;
    std::cout << "============================================" << std::endl;

    q1_sql_basics();
    q2_parameterized_queries();
    q3_transactions();
    q4_mysql_connector();
    q5_odbc();
    q6_orm();
    q7_connection_pool();

    std::cout << "\n[模块十五完成]" << std::endl;
    std::cout << "注意：数据库编程需要安装对应驱动和库" << std::endl;
    return 0;
}
