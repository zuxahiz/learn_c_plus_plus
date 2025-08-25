# 结构型模式详解

结构型模式关注类和对象的组合，形成更大的结构。这些模式描述如何将类或对象结合在一起形成更大的结构。

## 📋 目录
1. [适配器模式](#适配器模式)
2. [桥接模式](#桥接模式)
3. [组合模式](#组合模式)
4. [装饰器模式](#装饰器模式)
5. [外观模式](#外观模式)
6. [享元模式](#享元模式)
7. [代理模式](#代理模式)

---

## 适配器模式

### 🎯 模式意图
将一个类的接口转换成客户希望的另一个接口。适配器模式使得原本由于接口不兼容而不能一起工作的那些类可以一起工作。

### 🤔 为什么需要？
- **接口不兼容** - 现有类的接口与需要的接口不匹配
- **复用现有代码** - 不想修改已有的稳定代码
- **第三方库集成** - 集成外部库时接口不一致

### 💡 适用场景
- 使用一个已经存在的类，但接口不符合需要
- 创建一个可以复用的类，与不相关的类协同工作
- 需要使用几个已经存在的子类，但不可能对每一个都进行子类化

### 🏗️ 实现示例

```cpp
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// 目标接口（客户期望的接口）
class MediaPlayer {
public:
    virtual ~MediaPlayer() = default;
    virtual void play(const std::string& audioType, const std::string& fileName) = 0;
};

// 被适配者：高级媒体播放器
class AdvancedMediaPlayer {
public:
    virtual ~AdvancedMediaPlayer() = default;
    virtual void playVlc(const std::string& fileName) = 0;
    virtual void playMp4(const std::string& fileName) = 0;
};

// 具体的被适配者：VLC播放器
class VlcPlayer : public AdvancedMediaPlayer {
public:
    void playVlc(const std::string& fileName) override {
        std::cout << "Playing vlc file: " << fileName << std::endl;
    }
    
    void playMp4(const std::string& fileName) override {
        // VLC播放器不支持MP4，空实现
    }
};

// 具体的被适配者：MP4播放器
class Mp4Player : public AdvancedMediaPlayer {
public:
    void playVlc(const std::string& fileName) override {
        // MP4播放器不支持VLC，空实现
    }
    
    void playMp4(const std::string& fileName) override {
        std::cout << "Playing mp4 file: " << fileName << std::endl;
    }
};

// 适配器类
class MediaAdapter : public MediaPlayer {
private:
    std::unique_ptr<AdvancedMediaPlayer> advancedPlayer;
    
public:
    MediaAdapter(const std::string& audioType) {
        if (audioType == "vlc") {
            advancedPlayer = std::make_unique<VlcPlayer>();
        } else if (audioType == "mp4") {
            advancedPlayer = std::make_unique<Mp4Player>();
        }
    }
    
    void play(const std::string& audioType, const std::string& fileName) override {
        if (audioType == "vlc") {
            advancedPlayer->playVlc(fileName);
        } else if (audioType == "mp4") {
            advancedPlayer->playMp4(fileName);
        }
    }
};

// 具体媒体播放器（只支持基本格式）
class AudioPlayer : public MediaPlayer {
private:
    std::unique_ptr<MediaAdapter> adapter;
    
public:
    void play(const std::string& audioType, const std::string& fileName) override {
        if (audioType == "mp3") {
            std::cout << "Playing mp3 file: " << fileName << std::endl;
        } else if (audioType == "vlc" || audioType == "mp4") {
            adapter = std::make_unique<MediaAdapter>(audioType);
            adapter->play(audioType, fileName);
        } else {
            std::cout << "Invalid media. " << audioType << " format not supported" << std::endl;
        }
    }
};
```

---

## 桥接模式

### 🎯 模式意图
将抽象部分与它的实现部分分离，使它们都可以独立地变化。

### 🤔 为什么需要？
- **避免继承爆炸** - 多维度变化会导致类数量爆炸
- **运行时切换实现** - 需要在运行时选择不同的实现
- **实现细节隐藏** - 对客户完全隐藏实现细节

### 🏗️ 实现示例

```cpp
// 实现接口
class DrawingAPI {
public:
    virtual ~DrawingAPI() = default;
    virtual void drawCircle(double x, double y, double radius) = 0;
    virtual void drawRectangle(double x, double y, double width, double height) = 0;
};

// 具体实现A：控制台绘制
class ConsoleDrawing : public DrawingAPI {
public:
    void drawCircle(double x, double y, double radius) override {
        std::cout << "Console: Drawing Circle at (" << x << "," << y 
                  << ") with radius " << radius << std::endl;
    }
    
    void drawRectangle(double x, double y, double width, double height) override {
        std::cout << "Console: Drawing Rectangle at (" << x << "," << y 
                  << ") with size " << width << "x" << height << std::endl;
    }
};

// 具体实现B：图形界面绘制
class GUIDrawing : public DrawingAPI {
public:
    void drawCircle(double x, double y, double radius) override {
        std::cout << "GUI: Rendering smooth circle at (" << x << "," << y 
                  << ") with radius " << radius << std::endl;
    }
    
    void drawRectangle(double x, double y, double width, double height) override {
        std::cout << "GUI: Rendering rectangle with shadows at (" << x << "," << y 
                  << ") with size " << width << "x" << height << std::endl;
    }
};

// 抽象类
class Shape {
protected:
    std::unique_ptr<DrawingAPI> drawingAPI;
    
public:
    Shape(std::unique_ptr<DrawingAPI> api) : drawingAPI(std::move(api)) {}
    virtual ~Shape() = default;
    virtual void draw() = 0;
    virtual void resize(double factor) = 0;
};

// 扩展抽象类：圆形
class Circle : public Shape {
private:
    double x, y, radius;
    
public:
    Circle(double x, double y, double radius, std::unique_ptr<DrawingAPI> api)
        : Shape(std::move(api)), x(x), y(y), radius(radius) {}
    
    void draw() override {
        drawingAPI->drawCircle(x, y, radius);
    }
    
    void resize(double factor) override {
        radius *= factor;
    }
};
```

---

## 组合模式

### 🎯 模式意图
将对象组合成树形结构以表示"部分-整体"的层次结构。组合模式使得用户对单个对象和组合对象的使用具有一致性。

### 🏗️ 实现示例

```cpp
// 组件抽象类
class FileSystemComponent {
public:
    virtual ~FileSystemComponent() = default;
    virtual void display(int depth = 0) const = 0;
    virtual long getSize() const = 0;
    virtual std::string getName() const = 0;
    
    // 默认实现（对叶子节点抛出异常）
    virtual void add(std::shared_ptr<FileSystemComponent> component) {
        throw std::runtime_error("Cannot add to leaf node");
    }
    
    virtual void remove(std::shared_ptr<FileSystemComponent> component) {
        throw std::runtime_error("Cannot remove from leaf node");
    }
    
    virtual std::shared_ptr<FileSystemComponent> getChild(int index) {
        throw std::runtime_error("Leaf node has no children");
    }
};

// 叶子节点：文件
class File : public FileSystemComponent {
private:
    std::string name;
    long size;
    
public:
    File(const std::string& name, long size) : name(name), size(size) {}
    
    void display(int depth = 0) const override {
        std::string indent(depth * 2, ' ');
        std::cout << indent << "File: " << name << " (" << size << " bytes)" << std::endl;
    }
    
    long getSize() const override {
        return size;
    }
    
    std::string getName() const override {
        return name;
    }
};

// 组合节点：目录
class Directory : public FileSystemComponent {
private:
    std::string name;
    std::vector<std::shared_ptr<FileSystemComponent>> children;
    
public:
    Directory(const std::string& name) : name(name) {}
    
    void display(int depth = 0) const override {
        std::string indent(depth * 2, ' ');
        std::cout << indent << "Directory: " << name << " (" << getSize() << " bytes total)" << std::endl;
        
        for (const auto& child : children) {
            child->display(depth + 1);
        }
    }
    
    long getSize() const override {
        long totalSize = 0;
        for (const auto& child : children) {
            totalSize += child->getSize();
        }
        return totalSize;
    }
    
    std::string getName() const override {
        return name;
    }
    
    void add(std::shared_ptr<FileSystemComponent> component) override {
        children.push_back(component);
    }
    
    void remove(std::shared_ptr<FileSystemComponent> component) override {
        children.erase(
            std::remove(children.begin(), children.end(), component),
            children.end()
        );
    }
    
    std::shared_ptr<FileSystemComponent> getChild(int index) override {
        if (index >= 0 && index < children.size()) {
            return children[index];
        }
        return nullptr;
    }
    
    int getChildCount() const {
        return children.size();
    }
};
```

---

## 装饰器模式

### 🎯 模式意图
动态地给一个对象添加一些额外的职责。就增加功能来说，装饰器模式相比生成子类更为灵活。

### 🤔 为什么需要？
- **动态扩展** - 运行时添加或移除功能
- **避免子类爆炸** - 不用为每种功能组合创建子类
- **遵循开闭原则** - 对扩展开放，对修改封闭

### 🏗️ 实现示例

```cpp
#include <iostream>
#include <memory>
#include <string>

// 抽象组件
class Coffee {
public:
    virtual ~Coffee() = default;
    virtual std::string getDescription() const = 0;
    virtual double getCost() const = 0;
};

// 具体组件：基础咖啡
class SimpleCoffee : public Coffee {
public:
    std::string getDescription() const override {
        return "Simple Coffee";
    }
    
    double getCost() const override {
        return 2.0;
    }
};

// 抽象装饰器
class CoffeeDecorator : public Coffee {
protected:
    std::unique_ptr<Coffee> coffee;
    
public:
    CoffeeDecorator(std::unique_ptr<Coffee> c) : coffee(std::move(c)) {}
    
    std::string getDescription() const override {
        return coffee->getDescription();
    }
    
    double getCost() const override {
        return coffee->getCost();
    }
};

// 具体装饰器：牛奶
class MilkDecorator : public CoffeeDecorator {
public:
    MilkDecorator(std::unique_ptr<Coffee> c) : CoffeeDecorator(std::move(c)) {}
    
    std::string getDescription() const override {
        return coffee->getDescription() + ", Milk";
    }
    
    double getCost() const override {
        return coffee->getCost() + 0.5;
    }
};

// 具体装饰器：糖
class SugarDecorator : public CoffeeDecorator {
public:
    SugarDecorator(std::unique_ptr<Coffee> c) : CoffeeDecorator(std::move(c)) {}
    
    std::string getDescription() const override {
        return coffee->getDescription() + ", Sugar";
    }
    
    double getCost() const override {
        return coffee->getCost() + 0.2;
    }
};

void demonstrateDecorator() {
    auto coffee = std::make_unique<SimpleCoffee>();
    coffee = std::make_unique<MilkDecorator>(std::move(coffee));
    coffee = std::make_unique<SugarDecorator>(std::move(coffee));
    
    std::cout << coffee->getDescription() << " costs $" << coffee->getCost() << std::endl;
}
```

---

## 外观模式

### 🎯 模式意图
为子系统中的一组接口提供一个一致的界面，外观模式定义了一个高层接口，这个接口使得这一子系统更加容易使用。

### 🤔 为什么需要？
- **简化接口** - 为复杂子系统提供简单接口
- **降低耦合** - 减少客户端与子系统的依赖
- **分层设计** - 更好的系统分层

### 🏗️ 实现示例

```cpp
// 子系统组件
class CPU {
public:
    void freeze() { std::cout << "CPU: Freezing processor" << std::endl; }
    void jump(long position) { std::cout << "CPU: Jumping to " << position << std::endl; }
    void execute() { std::cout << "CPU: Executing instructions" << std::endl; }
};

class Memory {
public:
    void load(long position, const std::string& data) {
        std::cout << "Memory: Loading '" << data << "' at " << position << std::endl;
    }
};

class HardDrive {
public:
    std::string read(long lba, int size) {
        std::cout << "HardDrive: Reading " << size << " bytes from " << lba << std::endl;
        return "boot data";
    }
};

// 外观类
class ComputerFacade {
private:
    CPU cpu;
    Memory memory;
    HardDrive hardDrive;
    
public:
    void start() {
        std::cout << "Computer starting..." << std::endl;
        cpu.freeze();
        memory.load(0, hardDrive.read(0, 1024));
        cpu.jump(0);
        cpu.execute();
        std::cout << "Computer started successfully!" << std::endl;
    }
};

void demonstrateFacade() {
    ComputerFacade computer;
    computer.start();  // 简单的一个调用，隐藏了复杂的启动过程
}
```

---

## 享元模式

### 🎯 模式意图
运用共享技术有效地支持大量细粒度的对象。

### 🤔 为什么需要？
- **内存优化** - 减少对象数量，节省内存
- **性能提升** - 共享相同状态的对象
- **大量对象** - 需要创建大量相似对象时

### 🏗️ 实现示例

```cpp
#include <unordered_map>

// 享元接口
class CharacterFlyweight {
public:
    virtual ~CharacterFlyweight() = default;
    virtual void display(int size, const std::string& color) const = 0;
};

// 具体享元
class ConcreteCharacter : public CharacterFlyweight {
private:
    char symbol;  // 内部状态
    
public:
    ConcreteCharacter(char c) : symbol(c) {}
    
    void display(int size, const std::string& color) const override {
        std::cout << "Character: " << symbol << ", Size: " << size 
                  << ", Color: " << color << std::endl;
    }
};

// 享元工厂
class CharacterFactory {
private:
    static std::unordered_map<char, std::shared_ptr<CharacterFlyweight>> characters;
    
public:
    static std::shared_ptr<CharacterFlyweight> getCharacter(char c) {
        if (characters.find(c) == characters.end()) {
            characters[c] = std::make_shared<ConcreteCharacter>(c);
            std::cout << "Creating flyweight for character: " << c << std::endl;
        }
        return characters[c];
    }
    
    static size_t getCreatedCount() {
        return characters.size();
    }
};

std::unordered_map<char, std::shared_ptr<CharacterFlyweight>> CharacterFactory::characters;

void demonstrateFlyweight() {
    std::string text = "HELLO WORLD";
    
    for (char c : text) {
        if (c != ' ') {
            auto character = CharacterFactory::getCharacter(c);
            character->display(12, "Black");  // 外部状态
        }
    }
    
    std::cout << "Total character flyweights created: " 
              << CharacterFactory::getCreatedCount() << std::endl;
}
```

---

## 代理模式

### 🎯 模式意图
为其他对象提供一种代理以控制对这个对象的访问。

### 🤔 为什么需要？
- **访问控制** - 控制对象的访问权限
- **延迟加载** - 按需加载大对象
- **缓存代理** - 缓存计算结果
- **远程代理** - 访问远程对象

### 🏗️ 实现示例

```cpp
// 抽象主题
class Image {
public:
    virtual ~Image() = default;
    virtual void display() = 0;
    virtual int getSize() const = 0;
};

// 真实主题：高分辨率图像
class HighResolutionImage : public Image {
private:
    std::string filename;
    int size;
    
    void loadFromDisk() {
        std::cout << "Loading high resolution image: " << filename << std::endl;
        // 模拟加载耗时
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
public:
    HighResolutionImage(const std::string& filename) : filename(filename), size(1024*1024) {
        loadFromDisk();
    }
    
    void display() override {
        std::cout << "Displaying high resolution image: " << filename << std::endl;
    }
    
    int getSize() const override {
        return size;
    }
};

// 代理：图像代理
class ImageProxy : public Image {
private:
    std::string filename;
    mutable std::unique_ptr<HighResolutionImage> realImage;
    int estimatedSize;
    
public:
    ImageProxy(const std::string& filename) : filename(filename), estimatedSize(1024*1024) {
        std::cout << "Created proxy for image: " << filename << std::endl;
    }
    
    void display() override {
        if (!realImage) {
            realImage = std::make_unique<HighResolutionImage>(filename);
        }
        realImage->display();
    }
    
    int getSize() const override {
        if (realImage) {
            return realImage->getSize();
        }
        return estimatedSize;  // 返回估计大小，避免加载
    }
};

void demonstrateProxy() {
    std::cout << "=== Proxy Pattern Demo ===" << std::endl;
    
    // 创建代理，此时不会加载真实图像
    std::unique_ptr<Image> image1 = std::make_unique<ImageProxy>("photo1.jpg");
    std::unique_ptr<Image> image2 = std::make_unique<ImageProxy>("photo2.jpg");
    
    // 获取大小信息，仍然不会加载真实图像
    std::cout << "Image1 size: " << image1->getSize() << " bytes" << std::endl;
    std::cout << "Image2 size: " << image2->getSize() << " bytes" << std::endl;
    
    // 只有在真正需要显示时才加载图像
    image1->display();
    // image2 没有被显示，所以不会被加载
}
```

## 📊 结构型模式总结

这7种结构型模式为我们提供了灵活的对象组合方式：

### 🎯 核心价值
1. **适配器模式** - 接口转换，兼容性
2. **桥接模式** - 抽象与实现分离
3. **组合模式** - 树形结构，统一处理
4. **装饰器模式** - 动态功能扩展
5. **外观模式** - 简化复杂接口
6. **享元模式** - 对象共享，内存优化
7. **代理模式** - 访问控制，延迟加载

### 💡 使用建议
- **接口不兼容** → 适配器模式
- **功能扩展** → 装饰器模式
- **简化访问** → 外观模式
- **内存优化** → 享元模式
- **访问控制** → 代理模式

这些模式帮助我们构建更灵活、更高效的软件架构！🏗️
