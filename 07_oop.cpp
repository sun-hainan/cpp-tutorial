// === 模块七：面向对象 OOP ===
// 本文件覆盖：封装、继承、多态、类与对象、构造/析构、拷贝构造、移动语义、
//            修饰符、抽象类、虚函数、接口、内部类
// 编译：g++ -std=c++17 07_oop.cpp -o 07_oop.exe

#include <iostream>
#include <string>
#include <vector>
#include <memory>      // smart pointers
#include <utility>     // std::move, std::forward

// ============================================================
// Q1: 类和对象的基本概念是什么？
// ============================================================
class Point {
public:
    // 构造函数的几种形式
    Point() : x_(0), y_(0) { }                           // 默认构造
    Point(int x, int y) : x_(x), y_(y) { }               // 参数构造

    // C++11 委托构造
    Point(int val) : Point(val, val) { }                 // 委托给上面的两个参数构造

    // C++11 列表初始化
    Point(std::initializer_list<int> il) : x_(*il.begin()), y_(*(il.begin()+1)) { }

    // 成员函数
    void set(int x, int y) { x_ = x; y_ = y; }
    void print() const { std::cout << "Q1: Point(" << x_ << "," << y_ << ")" << std::endl; }

    // C++17 inline 静态成员变量
    inline static int instance_count = 0;

private:
    int x_, y_;
};

void q1_class_object() {
    // 创建对象
    Point p1;                  // 默认构造
    Point p2(3, 4);            // 参数构造
    Point p3(5);               // C++11 委托构造
    Point p4 = {6, 7};         // 列表初始化（C++11）

    p1.print();
    p2.print();
    p3.print();
    p4.print();

    std::cout << "Q1: instance_count = " << Point::instance_count << std::endl;

    // C++11 unique_ptr 管理对象生命周期
    auto sp = std::make_unique<Point>(8, 9);
    sp->print();

    // 深入原理：
    // - 类是模板，对象是实例
    // - 对象大小 = 成员变量大小 + 可能的虚函数表指针（4/8 字节）
    // - 成员函数通过 this 指针访问对象数据

    // 对比 Python：
    // Python 类与 C++ 类似，但所有方法第一个参数是 self
    // Python 没有构造/析构，有 __init__/__del__
    // 对比 Rust：
    // Rust 没有类和继承，用 impl 块实现方法，trait 实现多态
    std::cout << "Q1: 类是蓝图，对象是实例，封装是核心" << std::endl;
}

// ============================================================
// Q2: 构造、析构、拷贝构造、移动构造的区别是什么？
// ============================================================
class Resource {
public:
    Resource() : data_(new int[100]) {
        std::cout << "Q2: 默认构造，分配资源" << std::endl;
    }

    // 析构函数
    ~Resource() {
        delete[] data_;
        std::cout << "Q2: 析构函数，释放资源" << std::endl;
    }

    // 拷贝构造（深拷贝）
    Resource(const Resource& other) : data_(new int[100]) {
        for (int i = 0; i < 100; ++i) data_[i] = other.data_[i];
        std::cout << "Q2: 拷贝构造（深拷贝）" << std::endl;
    }

    // 移动构造（C++11）
    Resource(Resource&& other) noexcept : data_(other.data_) {
        other.data_ = nullptr;  // 转移所有权，不拷贝数据
        std::cout << "Q2: 移动构造（所有权转移）" << std::endl;
    }

    // 拷贝赋值
    Resource& operator=(const Resource& other) {
        if (this != &other) {
            delete[] data_;
            data_ = new int[100];
            for (int i = 0; i < 100; ++i) data_[i] = other.data_[i];
            std::cout << "Q2: 拷贝赋值" << std::endl;
        }
        return *this;
    }

    // 移动赋值（C++11）
    Resource& operator=(Resource&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            other.data_ = nullptr;
            std::cout << "Q2: 移动赋值" << std::endl;
        }
        return *this;
    }

    bool is_valid() const { return data_ != nullptr; }

private:
    int* data_;
};

void q2_constructor_destructor() {
    std::cout << "\n--- 拷贝构造 ---" << std::endl;
    Resource r1;
    Resource r2 = r1;              // 拷贝构造

    std::cout << "\n--- 移动构造 ---" << std::endl;
    Resource r3 = std::move(r1);   // 移动构造，r1 变为空

    std::cout << "\n--- 拷贝赋值 ---" << std::endl;
    Resource r4;
    r4 = r2;                       // 拷贝赋值

    std::cout << "\n--- 移动赋值 ---" << std::endl;
    Resource r5;
    r5 = std::move(r4);            // 移动赋值

    std::cout << "\n--- 离开作用域 ---" << std::endl;
    // 析构顺序与构造顺序相反（LIFO）

    // C++11 = default / = delete
    // Resource() = default;           // 使用编译器生成的默认实现
    // Resource(const Resource&) = delete;  // 禁用拷贝

    // 深入原理：
    // - 构造函数：初始化对象
    // - 析构函数：清理资源（RAII 基础）
    // - 拷贝构造：复制所有成员（深拷贝需要自定义）
    // - 移动构造：窃取资源，将指针转移到新对象，原对象置空
    // - 移动语义避免不必要的深拷贝，提升性能

    // 对比 Python：
    // Python 没有移动构造，对象赋值是引用传递
    // __copy__/__deepcopy__ 控制拷贝行为
    // 对比 Rust：
    // Rust 有 Copy trait（按位复制）和 Clone trait（显式克隆）
    // Rust 有 Drop trait（类似析构函数）
    std::cout << "Q2: 拷贝vs移动：前者复制数据，后者转移所有权" << std::endl;
}

// ============================================================
// Q3: 继承的访问控制和派生方式是什么？
// ============================================================
class Animal {
public:
    Animal(const std::string& name) : name_(name) { }
    virtual ~Animal() = default;  // 基类析构函数必须是 virtual

    virtual void speak() const {
        std::cout << "Q3: " << name_ << " 发出声音" << std::endl;
    }

    void eat() const { std::cout << "Q3: " << name_ << " 在吃东西" << std::endl; }

protected:
    std::string name_;  // 派生类可访问
};

// 公有继承
class Dog : public Animal {
public:
    Dog(const std::string& name) : Animal(name) { }

    void speak() const override {
        std::cout << "Q3: " << name_ << " 汪汪！" << std::endl;
    }

    void fetch() const {
        std::cout << "Q3: " << name_ << " 捡球" << std::endl;
    }
};

// 私有继承
class SmartDog : private Animal {
public:
    SmartDog(const std::string& name) : Animal(name) { }

    void guard() const {
        std::cout << "Q3: " << name_ << " 看家（私有继承）" << std::endl;
    }

    // 使用基类成员需要通过自己的接口
    void speak_loud() const { speak(); }  // OK，公开化
};

// 保护继承
class Cat : protected Animal {
public:
    Cat(const std::string& name) : Animal(name) { }

    void speak() const override {
        std::cout << "Q3: " << name_ << " 喵喵！" << std::endl;
    }
};

void q3_inheritance() {
    // 公有继承：is-a 关系
    Dog d("旺财");
    d.speak();    // 调用 Dog::speak
    d.eat();      // 调用 Animal::eat
    // d.name_  // ❌ 私有，访问不了

    // 私有继承：has-a 关系（类似组合）
    SmartDog sd("小黄");
    // sd.speak();  // ❌ 私有继承，speak 不可见
    sd.speak_loud();
    sd.guard();

    // 多态：通过基类指针调用派生类实现
    Animal* animal_ptr = &d;  // 公有继承允许
    animal_ptr->speak();      // 调用 Dog::speak（虚函数）

    // 深入原理：
    // - public 继承：保留基类各成员的访问级别
    // - protected 继承：public/protected → protected
    // - private 继承：所有 → private
    // - 内存布局：派生类对象包含基类子对象（首地址相同）

    // 对比 Python：
    // Python: class Dog(Animal): — 所有继承都是 public
    // Python 用 __slots__ 控制内存，用 MRO 解决多继承
    // 对比 Rust：
    // Rust 没有继承，用 trait 实现多态
    std::cout << "Q3: public继承=is-a，private继承=has-a，protected继承少见" << std::endl;
}

// ============================================================
// Q4: 虚函数和多态的原理是什么？
// ============================================================
class Shape {
public:
    virtual ~Shape() = default;

    // 纯虚函数
    virtual double area() const = 0;
    virtual void describe() const {
        std::cout << "Q4: 图形" << std::endl;
    }

    // 非虚函数
    void non_virtual() const {
        std::cout << "Q4: Shape::non_virtual" << std::endl;
    }
};

class Circle : public Shape {
public:
    Circle(double r) : radius_(r) { }

    double area() const override {
        return 3.14159 * radius_ * radius_;
    }

    void describe() const override {
        std::cout << "Q4: 圆，半径=" << radius_ << std::endl;
    }

private:
    double radius_;
};

class Rectangle : public Shape {
public:
    Rectangle(double w, double h) : width_(w), height_(h) { }

    double area() const override {
        return width_ * height_;
    }

    void describe() const override {
        std::cout << "Q4: 矩形，宽=" << width_ << "，高=" << height_ << std::endl;
    }

private:
    double width_, height_;
};

void q4_polymorphism() {
    // 基类指针调用派生类实现（运行时多态）
    std::vector<Shape*> shapes;
    shapes.push_back(new Circle(5.0));
    shapes.push_back(new Rectangle(4.0, 3.0));

    for (const auto* s : shapes) {
        s->describe();        // 虚函数，运行时绑定
        s->non_virtual();      // 非虚，编译时绑定
    }

    // C++11 unique_ptr 避免手动 delete
    std::vector<std::unique_ptr<Shape>> smart_shapes;
    smart_shapes.push_back(std::make_unique<Circle>(3.0));
    smart_shapes.push_back(std::make_unique<Rectangle>(2.0, 5.0));

    std::cout << "Q4: 使用智能指针:" << std::endl;
    for (const auto& s : smart_shapes) {
        s->describe();
        std::cout << "Q4: 面积 = " << s->area() << std::endl;
    }

    // RTTI：dynamic_cast 安全向下转型
    Circle* c = dynamic_cast<Circle*>(shapes[0]);
    if (c) {
        std::cout << "Q4: 确认是 Circle，半径=" << c << std::endl;
    }

    // 清理
    for (auto* s : shapes) delete s;

    // 深入原理：
    // - 虚函数表（vtable）：每个有虚函数的类有一个虚表指针（vptr）
    // - 对象内存布局：vptr 在对象开头，后跟成员变量
    // - 虚函数调用：obj->vptr[index](obj) — 间接跳转
    // - 纯虚函数：vtable 中是 nullptr，类无法实例化（抽象类）

    // 对比 Python：
    // Python 所有方法都是虚函数（运行时绑定）
    // __slots__ 可以优化内存
    // 对比 Rust：
    // Rust 用 trait 对象（dyn Trait）实现类似多态
    // let shape: Box<dyn Shape> = Box::new(Circle{ radius: 5.0 });
    std::cout << "Q4: 虚函数通过 vtable 实现运行时多态" << std::endl;
}

// ============================================================
// Q5: static、const、mutable 修饰符如何使用？
// ============================================================
class Counter {
public:
    Counter() { ++count_; }

    static int count_;       // 静态成员：所有对象共享

    static int get_count() { // 静态成员函数
        return count_;
    }

    void set_value(int v) const {  // const 成员函数
        // value_ = v;  // ❌ const 成员不能修改成员
        // value_ 地址是可变的，但内容不可变
    }

    void set_value_mutable(int v) const {
        mutable int* hack = const_cast<int*>(&value_);  // 不推荐！
        *hack = v;  // 技术上可以绕过 const
    }

    int get_value() const { return value_; }

    void non_const_method() {
        std::cout << "Q5: 非 const 方法" << std::endl;
    }

private:
    int value_ = 0;
};

int Counter::count_ = 0;  // 静态成员定义

void q5_modifiers() {
    Counter c1, c2, c3;
    std::cout << "Q5: 静态成员 count_ = " << Counter::get_count() << std::endl;
    std::cout << "Q5: 静态成员 count_ = " << c1.get_count() << std::endl;

    // const 对象
    const Counter c_const;
    // c_const.non_const_method();  // ❌ const 对象只能调用 const 方法

    // const 引用/指针
    const Counter& ref = c1;
    const Counter* ptr = &c1;

    // 深入原理：
    // - static 成员变量：存储在全局/静态区，整个程序只有一份
    // - static 成员函数：没有 this 指针，不能访问非 static 成员
    // - const 成员函数：承诺不修改对象（但可以有 mutable 成员例外）
    // - mutable：即使在 const 对象中也可修改，用于缓存等

    // 对比 Python：
    // Python: class var = value — 是类属性，类似 static
    // @property — 类似 const getter
    // 对比 Rust：
    // Rust: static: 'static 生命周期，全局唯一
    // Rust: const: 编译期常量
    // Rust: &self — 可变引用 &mut self / 不可变引用 &self
    std::cout << "Q5: static=类级别共享，const=不可变，mutable=const中的可变" << std::endl;
}

// ============================================================
// Q6: 什么是抽象类和接口？
// ============================================================
class IDrawable {
public:
    virtual ~IDrawable() = default;
    virtual void draw() const = 0;  // 纯虚函数 → 接口
    virtual void resize(double factor) = 0;
};

class ShapeInterface {
public:
    virtual ~ShapeInterface() = default;
    virtual double get_area() const = 0;
    virtual std::string get_name() const = 0;
};

// C++ 中"接口"约定：用纯虚函数的抽象类
class IPlayable {
public:
    virtual ~IPlayable() = default;
    virtual void play() const = 0;
    virtual void stop() const = 0;
};

// 多接口实现（类似 Java/C#）
class VideoPlayer : public IDrawable, public IPlayable {
public:
    void draw() const override { std::cout << "Q6: 绘制视频" << std::endl; }
    void resize(double factor) override { std::cout << "Q6: 调整大小 x" << factor << std::endl; }
    void play() const override { std::cout << "Q6: 播放视频" << std::endl; }
    void stop() const override { std::cout << "Q6: 停止播放" << std::endl; }
};

void q6_interface() {
    VideoPlayer player;
    player.draw();
    player.play();
    player.stop();

    // 多态通过接口
    IDrawable* drawable = &player;
    IPlayable* playable = &player;
    drawable->draw();
    playable->play();

    // 深入原理：
    // - C++ 没有 interface 关键字，用纯虚函数类代替
    // - 抽象类不能实例化（可以有成员变量和实现的方法）
    // - 派生类必须实现所有纯虚函数，否则仍是抽象类

    // 对比 Java/C#：
    // Java: interface IPlayable { void play(); }
    // Java: class VideoPlayer implements IDrawable, IPlayable
    // C#: interface 同 Java，abstract class 与 C++ 类似
    // 对比 Rust：
    // Rust 用 trait 作为接口
    // impl IDrawable for VideoPlayer { fn draw(&self) { ... } }
    std::cout << "Q6: C++ 用纯虚函数抽象类作为接口" << std::endl;
}

// ============================================================
// Q7: 内部类（嵌套类）和局部类是什么？
// ============================================================
class Outer {
public:
    Outer(int val) : value_(val) { }

    // 内部类（嵌套类）
    class Inner {
    public:
        Inner(Outer* o) : outer_(o) { }
        void access_outer() const {
            // 内部类可以访问外部类的所有成员（包括 private）
            // std::cout << outer_->value_;  // OK，可访问
            std::cout << "Q7: Inner 访问外部类" << std::endl;
        }
        void display() const {
            std::cout << "Q7: 内部类方法" << std::endl;
        }
    };

    Inner create_inner() { return Inner(this); }

    void outer_method() {
        Inner i(this);
        i.display();
    }

private:
    int value_;
};

void q7_nested_class() {
    Outer outer(42);
    Outer::Inner inner(&outer);  // 通过外部类名访问内部类
    inner.display();
    inner.access_outer();
    outer.outer_method();

    // 局部类（定义在函数内部）
    auto make_local_class = []() {
        class Local {
        public:
            int value;
            void print() const { std::cout << "Q7: 局部类 value=" << value << std::endl; }
        };
        return Local{123};
    };
    auto local_obj = make_local_class();
    local_obj.print();

    // 深入原理：
    // - 嵌套类：作用域在外部类内，完整独立类
    // - 嵌套类可以访问外部类的 private/protected 成员
    // - 局部类：作用域在函数内，功能受限（不能有静态成员）
    // - 两者都不影响内存布局（内存取决于成员变量）

    // 对比 Python：
    // Python 可以定义嵌套类，行为类似
    // class Outer:
    //     class Inner: pass
    // 对比 Rust：
    // Rust 可以用 mod 嵌套，不能在函数内定义类型
    std::cout << "Q7: 嵌套类在外部类作用域内，可访问外部类所有成员" << std::endl;
}

// ============================================================
// 主函数入口
// ============================================================
int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "C++ 专家教程 - 模块七：面向对象 OOP" << std::endl;
    std::cout << "============================================" << std::endl;

    q1_class_object();
    std::cout << "\n---" << std::endl;
    q2_constructor_destructor();
    std::cout << "\n---" << std::endl;
    q3_inheritance();
    std::cout << "\n---" << std::endl;
    q4_polymorphism();
    std::cout << "\n---" << std::endl;
    q5_modifiers();
    std::cout << "\n---" << std::endl;
    q6_interface();
    std::cout << "\n---" << std::endl;
    q7_nested_class();

    std::cout << "\n[模块七完成]" << std::endl;
    return 0;
}
