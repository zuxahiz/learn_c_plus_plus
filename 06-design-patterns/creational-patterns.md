# 创建型模式详解

创建型模式专注于对象的创建过程，目标是使系统独立于如何创建、组合和表示对象。这些模式封装了系统中对象创建的知识，隐藏了类的实例创建细节。

## 📋 目录
1. [工厂方法模式](#工厂方法模式)
2. [抽象工厂模式](#抽象工厂模式)
3. [单例模式](#单例模式)
4. [建造者模式](#建造者模式)
5. [原型模式](#原型模式)

---

## 工厂方法模式

### 🎯 模式意图
定义一个创建对象的接口，让子类决定实例化哪一个类。工厂方法使一个类的实例化延迟到其子类。

### 🤔 为什么需要？
- **解耦创建和使用** - 客户端不需要知道具体产品类
- **支持扩展** - 新增产品类型无需修改现有代码
- **统一接口** - 通过统一的工厂接口创建对象

### 💡 适用场景
- 编译时无法确定要创建的对象类型
- 系统需要独立于产品的创建过程
- 需要提供一个产品类库，只显示接口而不显示实现

### 🏗️ 实现示例

```cpp
#include <iostream>
#include <memory>
#include <string>

// 抽象产品
class Document {
public:
    virtual ~Document() = default;
    virtual void open() = 0;
    virtual void save() = 0;
    virtual void close() = 0;
    virtual std::string getType() const = 0;
};

// 具体产品：文本文档
class TextDocument : public Document {
public:
    void open() override {
        std::cout << "Opening text document..." << std::endl;
    }
    
    void save() override {
        std::cout << "Saving text document..." << std::endl;
    }
    
    void close() override {
        std::cout << "Closing text document..." << std::endl;
    }
    
    std::string getType() const override {
        return "Text Document";
    }
};

// 具体产品：PDF文档
class PDFDocument : public Document {
public:
    void open() override {
        std::cout << "Opening PDF document..." << std::endl;
    }
    
    void save() override {
        std::cout << "Saving PDF document..." << std::endl;
    }
    
    void close() override {
        std::cout << "Closing PDF document..." << std::endl;
    }
    
    std::string getType() const override {
        return "PDF Document";
    }
};

// 抽象工厂
class DocumentFactory {
public:
    virtual ~DocumentFactory() = default;
    virtual std::unique_ptr<Document> createDocument() = 0;
    
    // 模板方法：定义文档处理流程
    void processDocument() {
        auto doc = createDocument();
        doc->open();
        doc->save();
        doc->close();
    }
};

// 具体工厂：文本文档工厂
class TextDocumentFactory : public DocumentFactory {
public:
    std::unique_ptr<Document> createDocument() override {
        return std::make_unique<TextDocument>();
    }
};

// 具体工厂：PDF文档工厂
class PDFDocumentFactory : public DocumentFactory {
public:
    std::unique_ptr<Document> createDocument() override {
        return std::make_unique<PDFDocument>();
    }
};

// 客户端代码
class Application {
private:
    std::unique_ptr<DocumentFactory> factory;
    
public:
    Application(std::unique_ptr<DocumentFactory> f) : factory(std::move(f)) {}
    
    void createAndProcessDocument() {
        factory->processDocument();
    }
};

// 使用示例
void demonstrateFactoryMethod() {
    std::cout << "=== Factory Method Pattern Demo ===" << std::endl;
    
    // 创建文本文档应用
    auto textApp = std::make_unique<Application>(
        std::make_unique<TextDocumentFactory>()
    );
    textApp->createAndProcessDocument();
    
    std::cout << std::endl;
    
    // 创建PDF文档应用
    auto pdfApp = std::make_unique<Application>(
        std::make_unique<PDFDocumentFactory>()
    );
    pdfApp->createAndProcessDocument();
}
```

### ✅ 优点
- 遵循开放封闭原则，易于扩展
- 避免了客户端与具体产品的耦合
- 符合单一职责原则

### ❌ 缺点
- 增加了系统复杂性
- 每增加一个产品，都需要增加一个具体工厂类

---

## 抽象工厂模式

### 🎯 模式意图
提供一个创建一系列相关或相互依赖对象的接口，而无需指定它们具体的类。

### 🤔 为什么需要？
- **产品族管理** - 需要创建一组相关的产品
- **平台无关性** - 支持多个平台或风格
- **一致性保证** - 确保产品族中的对象能够一起工作

### 💡 适用场景
- 系统需要独立于其产品的创建、组合和表示
- 系统需要由多个产品族中的一个来配置
- 需要强调一系列相关产品对象的设计以便进行联合使用

### 🏗️ 实现示例

```cpp
#include <iostream>
#include <memory>
#include <string>

// 抽象产品A：按钮
class Button {
public:
    virtual ~Button() = default;
    virtual void render() = 0;
    virtual void onClick() = 0;
};

// 抽象产品B：文本框
class TextBox {
public:
    virtual ~TextBox() = default;
    virtual void render() = 0;
    virtual void setText(const std::string& text) = 0;
};

// Windows风格按钮
class WindowsButton : public Button {
public:
    void render() override {
        std::cout << "Rendering Windows-style button" << std::endl;
    }
    
    void onClick() override {
        std::cout << "Windows button clicked!" << std::endl;
    }
};

// Windows风格文本框
class WindowsTextBox : public TextBox {
private:
    std::string text;
    
public:
    void render() override {
        std::cout << "Rendering Windows-style textbox: " << text << std::endl;
    }
    
    void setText(const std::string& text) override {
        this->text = text;
    }
};

// Mac风格按钮
class MacButton : public Button {
public:
    void render() override {
        std::cout << "Rendering Mac-style button" << std::endl;
    }
    
    void onClick() override {
        std::cout << "Mac button clicked!" << std::endl;
    }
};

// Mac风格文本框
class MacTextBox : public TextBox {
private:
    std::string text;
    
public:
    void render() override {
        std::cout << "Rendering Mac-style textbox: " << text << std::endl;
    }
    
    void setText(const std::string& text) override {
        this->text = text;
    }
};

// 抽象工厂
class GUIFactory {
public:
    virtual ~GUIFactory() = default;
    virtual std::unique_ptr<Button> createButton() = 0;
    virtual std::unique_ptr<TextBox> createTextBox() = 0;
};

// Windows工厂
class WindowsFactory : public GUIFactory {
public:
    std::unique_ptr<Button> createButton() override {
        return std::make_unique<WindowsButton>();
    }
    
    std::unique_ptr<TextBox> createTextBox() override {
        return std::make_unique<WindowsTextBox>();
    }
};

// Mac工厂
class MacFactory : public GUIFactory {
public:
    std::unique_ptr<Button> createButton() override {
        return std::make_unique<MacButton>();
    }
    
    std::unique_ptr<TextBox> createTextBox() override {
        return std::make_unique<MacTextBox>();
    }
};

// 客户端应用
class Application {
private:
    std::unique_ptr<GUIFactory> factory;
    std::unique_ptr<Button> button;
    std::unique_ptr<TextBox> textBox;
    
public:
    Application(std::unique_ptr<GUIFactory> f) : factory(std::move(f)) {
        button = factory->createButton();
        textBox = factory->createTextBox();
    }
    
    void render() {
        button->render();
        textBox->setText("Hello, World!");
        textBox->render();
    }
    
    void handleClick() {
        button->onClick();
    }
};

// 工厂选择器
std::unique_ptr<GUIFactory> createFactory(const std::string& osType) {
    if (osType == "Windows") {
        return std::make_unique<WindowsFactory>();
    } else if (osType == "Mac") {
        return std::make_unique<MacFactory>();
    }
    return nullptr;
}

// 使用示例
void demonstrateAbstractFactory() {
    std::cout << "=== Abstract Factory Pattern Demo ===" << std::endl;
    
    // Windows应用
    auto windowsApp = std::make_unique<Application>(
        createFactory("Windows")
    );
    std::cout << "Windows Application:" << std::endl;
    windowsApp->render();
    windowsApp->handleClick();
    
    std::cout << std::endl;
    
    // Mac应用
    auto macApp = std::make_unique<Application>(
        createFactory("Mac")
    );
    std::cout << "Mac Application:" << std::endl;
    macApp->render();
    macApp->handleClick();
}
```

### ✅ 优点
- 分离了具体的类
- 易于交换产品族
- 有利于产品的一致性

### ❌ 缺点
- 难以支持新种类的产品
- 增加了系统的抽象性和理解难度

---

## 单例模式

### 🎯 模式意图
确保一个类只有一个实例，并提供一个全局访问点。

### 🤔 为什么需要？
- **资源控制** - 某些资源（如数据库连接）应该只有一个实例
- **全局状态** - 需要一个全局的配置或状态管理器
- **性能考虑** - 避免创建多个重量级对象

### 💡 适用场景
- 系统只需要一个实例来协调行为
- 实例应该可以被客户端访问而不需要全局变量
- 确保实例的唯一性

### 🏗️ 实现示例

```cpp
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

// 线程安全的单例模式（C++11 局部静态变量版本）
class Logger {
private:
    Logger() = default;
    
public:
    // 删除拷贝构造函数和赋值操作符
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    // 删除移动构造函数和移动赋值操作符
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;
    
    // 获取单例实例
    static Logger& getInstance() {
        static Logger instance;  // C++11保证线程安全
        return instance;
    }
    
    void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::cout << "[LOG] " << message << std::endl;
    }
    
    void setLogLevel(const std::string& level) {
        std::lock_guard<std::mutex> lock(mutex_);
        logLevel_ = level;
        std::cout << "[INFO] Log level set to: " << level << std::endl;
    }
    
    std::string getLogLevel() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return logLevel_;
    }

private:
    mutable std::mutex mutex_;
    std::string logLevel_ = "INFO";
};

// 传统的双重检查锁定单例（仅用于教学目的）
class ConfigManager {
private:
    static ConfigManager* instance_;
    static std::mutex mutex_;
    
    ConfigManager() = default;
    
public:
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    
    static ConfigManager* getInstance() {
        // 第一次检查
        if (instance_ == nullptr) {
            std::lock_guard<std::mutex> lock(mutex_);
            // 第二次检查（双重检查）
            if (instance_ == nullptr) {
                instance_ = new ConfigManager();
            }
        }
        return instance_;
    }
    
    void setConfig(const std::string& key, const std::string& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        configs_[key] = value;
        std::cout << "[CONFIG] Set " << key << " = " << value << std::endl;
    }
    
    std::string getConfig(const std::string& key) const {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = configs_.find(key);
        return it != configs_.end() ? it->second : "";
    }
    
    static void cleanup() {
        std::lock_guard<std::mutex> lock(mutex_);
        delete instance_;
        instance_ = nullptr;
    }

private:
    mutable std::mutex mutex_;
    std::map<std::string, std::string> configs_;
};

// 静态成员定义
ConfigManager* ConfigManager::instance_ = nullptr;
std::mutex ConfigManager::mutex_;

// 使用智能指针的单例模式
class DatabaseConnection {
private:
    DatabaseConnection() {
        std::cout << "Establishing database connection..." << std::endl;
    }
    
public:
    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;
    
    static std::shared_ptr<DatabaseConnection> getInstance() {
        static std::once_flag flag;
        static std::shared_ptr<DatabaseConnection> instance;
        
        std::call_once(flag, []() {
            instance = std::shared_ptr<DatabaseConnection>(new DatabaseConnection());
        });
        
        return instance;
    }
    
    void executeQuery(const std::string& query) {
        std::cout << "[DB] Executing query: " << query << std::endl;
    }
    
    ~DatabaseConnection() {
        std::cout << "Closing database connection..." << std::endl;
    }
};

// 模板单例基类
template<typename T>
class Singleton {
protected:
    Singleton() = default;
    
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;
    
    static T& getInstance() {
        static T instance;
        return instance;
    }
};

// 使用模板单例基类
class GameSettings : public Singleton<GameSettings> {
    friend class Singleton<GameSettings>;  // 允许基类访问私有构造函数
    
private:
    GameSettings() = default;
    
public:
    void setDifficulty(const std::string& difficulty) {
        difficulty_ = difficulty;
        std::cout << "[GAME] Difficulty set to: " << difficulty << std::endl;
    }
    
    std::string getDifficulty() const {
        return difficulty_;
    }

private:
    std::string difficulty_ = "Normal";
};

// 测试函数
void testSingletonInThreads() {
    std::vector<std::thread> threads;
    
    // 创建多个线程测试单例的线程安全性
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([i]() {
            Logger& logger = Logger::getInstance();
            logger.log("Message from thread " + std::to_string(i));
            
            auto db = DatabaseConnection::getInstance();
            db->executeQuery("SELECT * FROM users WHERE id = " + std::to_string(i));
        });
    }
    
    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }
}

// 使用示例
void demonstrateSingleton() {
    std::cout << "=== Singleton Pattern Demo ===" << std::endl;
    
    // Logger单例测试
    Logger& logger1 = Logger::getInstance();
    Logger& logger2 = Logger::getInstance();
    
    std::cout << "Logger instances are same: " << (&logger1 == &logger2) << std::endl;
    
    logger1.setLogLevel("DEBUG");
    logger2.log("This is a test message");
    std::cout << "Log level: " << logger2.getLogLevel() << std::endl;
    
    std::cout << std::endl;
    
    // ConfigManager单例测试
    ConfigManager* config1 = ConfigManager::getInstance();
    ConfigManager* config2 = ConfigManager::getInstance();
    
    std::cout << "Config instances are same: " << (config1 == config2) << std::endl;
    
    config1->setConfig("database_url", "localhost:5432");
    std::cout << "Database URL: " << config2->getConfig("database_url") << std::endl;
    
    std::cout << std::endl;
    
    // GameSettings模板单例测试
    GameSettings& settings1 = GameSettings::getInstance();
    GameSettings& settings2 = GameSettings::getInstance();
    
    std::cout << "GameSettings instances are same: " << (&settings1 == &settings2) << std::endl;
    
    settings1.setDifficulty("Hard");
    std::cout << "Game difficulty: " << settings2.getDifficulty() << std::endl;
    
    std::cout << std::endl;
    
    // 多线程测试
    std::cout << "Testing singleton in multiple threads:" << std::endl;
    testSingletonInThreads();
    
    // 清理资源
    ConfigManager::cleanup();
}
```

### ✅ 优点
- 控制实例数量，节省内存
- 提供全局访问点
- 延迟初始化，按需创建

### ❌ 缺点
- 违反单一职责原则
- 难以进行单元测试
- 在多线程环境下需要特别小心

---

## 建造者模式

### 🎯 模式意图
将一个复杂对象的构建与它的表示分离，使得同样的构建过程可以创建不同的表示。

### 🤔 为什么需要？
- **复杂对象构建** - 对象有很多属性，构造函数参数过多
- **构建步骤控制** - 需要分步骤构建对象
- **不同表示** - 同样的构建过程可以创建不同的产品

### 💡 适用场景
- 创建复杂对象的算法应该独立于该对象的组成部分
- 构造过程必须允许被构造的对象有不同的表示
- 对象有非常复杂的内部结构

### 🏗️ 实现示例

```cpp
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// 产品类：计算机
class Computer {
private:
    std::string cpu_;
    std::string gpu_;
    std::string ram_;
    std::string storage_;
    std::string motherboard_;
    std::string powerSupply_;
    std::vector<std::string> peripherals_;
    
public:
    // Setter方法
    void setCPU(const std::string& cpu) { cpu_ = cpu; }
    void setGPU(const std::string& gpu) { gpu_ = gpu; }
    void setRAM(const std::string& ram) { ram_ = ram; }
    void setStorage(const std::string& storage) { storage_ = storage; }
    void setMotherboard(const std::string& motherboard) { motherboard_ = motherboard; }
    void setPowerSupply(const std::string& powerSupply) { powerSupply_ = powerSupply; }
    void addPeripheral(const std::string& peripheral) { 
        peripherals_.push_back(peripheral); 
    }
    
    // 显示配置
    void showConfiguration() const {
        std::cout << "Computer Configuration:" << std::endl;
        std::cout << "  CPU: " << cpu_ << std::endl;
        std::cout << "  GPU: " << gpu_ << std::endl;
        std::cout << "  RAM: " << ram_ << std::endl;
        std::cout << "  Storage: " << storage_ << std::endl;
        std::cout << "  Motherboard: " << motherboard_ << std::endl;
        std::cout << "  Power Supply: " << powerSupply_ << std::endl;
        std::cout << "  Peripherals: ";
        for (const auto& peripheral : peripherals_) {
            std::cout << peripheral << " ";
        }
        std::cout << std::endl;
    }
    
    // 获取总价格（示例）
    double getTotalPrice() const {
        // 这里只是示例，实际应该根据配置计算
        double basePrice = 500.0;
        if (cpu_.find("Intel i9") != std::string::npos) basePrice += 400;
        if (gpu_.find("RTX 4080") != std::string::npos) basePrice += 800;
        return basePrice;
    }
};

// 抽象建造者
class ComputerBuilder {
protected:
    std::unique_ptr<Computer> computer_;
    
public:
    ComputerBuilder() : computer_(std::make_unique<Computer>()) {}
    virtual ~ComputerBuilder() = default;
    
    // 重置建造者
    void reset() {
        computer_ = std::make_unique<Computer>();
    }
    
    // 构建步骤
    virtual ComputerBuilder& buildCPU() = 0;
    virtual ComputerBuilder& buildGPU() = 0;
    virtual ComputerBuilder& buildRAM() = 0;
    virtual ComputerBuilder& buildStorage() = 0;
    virtual ComputerBuilder& buildMotherboard() = 0;
    virtual ComputerBuilder& buildPowerSupply() = 0;
    virtual ComputerBuilder& buildPeripherals() = 0;
    
    // 获取产品
    std::unique_ptr<Computer> getResult() {
        return std::move(computer_);
    }
};

// 具体建造者：游戏电脑
class GamingComputerBuilder : public ComputerBuilder {
public:
    ComputerBuilder& buildCPU() override {
        computer_->setCPU("Intel Core i9-13900K");
        return *this;
    }
    
    ComputerBuilder& buildGPU() override {
        computer_->setGPU("NVIDIA RTX 4080");
        return *this;
    }
    
    ComputerBuilder& buildRAM() override {
        computer_->setRAM("32GB DDR5-5600");
        return *this;
    }
    
    ComputerBuilder& buildStorage() override {
        computer_->setStorage("2TB NVMe SSD");
        return *this;
    }
    
    ComputerBuilder& buildMotherboard() override {
        computer_->setMotherboard("ASUS ROG STRIX Z790-E");
        return *this;
    }
    
    ComputerBuilder& buildPowerSupply() override {
        computer_->setPowerSupply("850W 80+ Gold");
        return *this;
    }
    
    ComputerBuilder& buildPeripherals() override {
        computer_->addPeripheral("Gaming Keyboard");
        computer_->addPeripheral("Gaming Mouse");
        computer_->addPeripheral("4K Gaming Monitor");
        return *this;
    }
};

// 具体建造者：办公电脑
class OfficeComputerBuilder : public ComputerBuilder {
public:
    ComputerBuilder& buildCPU() override {
        computer_->setCPU("Intel Core i5-13600");
        return *this;
    }
    
    ComputerBuilder& buildGPU() override {
        computer_->setGPU("Integrated Graphics");
        return *this;
    }
    
    ComputerBuilder& buildRAM() override {
        computer_->setRAM("16GB DDR4-3200");
        return *this;
    }
    
    ComputerBuilder& buildStorage() override {
        computer_->setStorage("512GB SATA SSD");
        return *this;
    }
    
    ComputerBuilder& buildMotherboard() override {
        computer_->setMotherboard("MSI B660M PRO-B");
        return *this;
    }
    
    ComputerBuilder& buildPowerSupply() override {
        computer_->setPowerSupply("450W 80+ Bronze");
        return *this;
    }
    
    ComputerBuilder& buildPeripherals() override {
        computer_->addPeripheral("Standard Keyboard");
        computer_->addPeripheral("Optical Mouse");
        computer_->addPeripheral("24-inch Monitor");
        return *this;
    }
};

// 指挥者类
class ComputerDirector {
public:
    // 构建完整计算机
    std::unique_ptr<Computer> buildFullComputer(ComputerBuilder& builder) {
        return builder.buildCPU()
                     .buildGPU()
                     .buildRAM()
                     .buildStorage()
                     .buildMotherboard()
                     .buildPowerSupply()
                     .buildPeripherals()
                     .getResult();
    }
    
    // 构建基础计算机（无外设）
    std::unique_ptr<Computer> buildBasicComputer(ComputerBuilder& builder) {
        return builder.buildCPU()
                     .buildGPU()
                     .buildRAM()
                     .buildStorage()
                     .buildMotherboard()
                     .buildPowerSupply()
                     .getResult();
    }
    
    // 构建自定义计算机
    std::unique_ptr<Computer> buildCustomComputer(ComputerBuilder& builder, 
                                                  bool needHighEndGPU = false,
                                                  bool needExtraRAM = false) {
        builder.buildCPU()
               .buildRAM()
               .buildStorage()
               .buildMotherboard()
               .buildPowerSupply();
        
        if (needHighEndGPU) {
            builder.buildGPU();
        }
        
        if (needExtraRAM) {
            // 可以在这里修改RAM配置
        }
        
        return builder.getResult();
    }
};

// 现代C++风格的流式构建器
class ModernComputerBuilder {
private:
    std::unique_ptr<Computer> computer_;
    
public:
    ModernComputerBuilder() : computer_(std::make_unique<Computer>()) {}
    
    ModernComputerBuilder& withCPU(const std::string& cpu) {
        computer_->setCPU(cpu);
        return *this;
    }
    
    ModernComputerBuilder& withGPU(const std::string& gpu) {
        computer_->setGPU(gpu);
        return *this;
    }
    
    ModernComputerBuilder& withRAM(const std::string& ram) {
        computer_->setRAM(ram);
        return *this;
    }
    
    ModernComputerBuilder& withStorage(const std::string& storage) {
        computer_->setStorage(storage);
        return *this;
    }
    
    ModernComputerBuilder& withMotherboard(const std::string& motherboard) {
        computer_->setMotherboard(motherboard);
        return *this;
    }
    
    ModernComputerBuilder& withPowerSupply(const std::string& powerSupply) {
        computer_->setPowerSupply(powerSupply);
        return *this;
    }
    
    ModernComputerBuilder& withPeripheral(const std::string& peripheral) {
        computer_->addPeripheral(peripheral);
        return *this;
    }
    
    std::unique_ptr<Computer> build() {
        return std::move(computer_);
    }
};

// 使用示例
void demonstrateBuilder() {
    std::cout << "=== Builder Pattern Demo ===" << std::endl;
    
    ComputerDirector director;
    
    // 构建游戏电脑
    std::cout << "Building Gaming Computer:" << std::endl;
    GamingComputerBuilder gamingBuilder;
    auto gamingPC = director.buildFullComputer(gamingBuilder);
    gamingPC->showConfiguration();
    std::cout << "Total Price: $" << gamingPC->getTotalPrice() << std::endl;
    
    std::cout << std::endl;
    
    // 构建办公电脑
    std::cout << "Building Office Computer:" << std::endl;
    OfficeComputerBuilder officeBuilder;
    auto officePC = director.buildFullComputer(officeBuilder);
    officePC->showConfiguration();
    std::cout << "Total Price: $" << officePC->getTotalPrice() << std::endl;
    
    std::cout << std::endl;
    
    // 构建基础游戏电脑（无外设）
    std::cout << "Building Basic Gaming Computer:" << std::endl;
    GamingComputerBuilder basicGamingBuilder;
    auto basicGamingPC = director.buildBasicComputer(basicGamingBuilder);
    basicGamingPC->showConfiguration();
    
    std::cout << std::endl;
    
    // 使用现代流式构建器
    std::cout << "Building Custom Computer with Fluent Interface:" << std::endl;
    auto customPC = ModernComputerBuilder()
        .withCPU("AMD Ryzen 9 7950X")
        .withGPU("AMD RX 7900 XTX")
        .withRAM("64GB DDR5-6000")
        .withStorage("4TB NVMe SSD")
        .withMotherboard("ASUS ROG CROSSHAIR X670E")
        .withPowerSupply("1000W 80+ Platinum")
        .withPeripheral("Mechanical Keyboard")
        .withPeripheral("Precision Mouse")
        .withPeripheral("Ultrawide Monitor")
        .build();
    
    customPC->showConfiguration();
    std::cout << "Total Price: $" << customPC->getTotalPrice() << std::endl;
}
```

### ✅ 优点
- 将复杂对象的创建过程封装起来
- 允许对象通过多个步骤来创建，并且可以改变过程
- 隐藏产品内部结构
- 产品的建造过程和产品本身分离

### ❌ 缺点
- 增加了系统复杂性
- 如果产品差异很大，不适合使用建造者模式

---

## 原型模式

### 🎯 模式意图
用原型实例指定创建对象的种类，并且通过拷贝这些原型创建新的对象。

### 🤔 为什么需要？
- **创建成本高** - 对象创建过程复杂或耗时
- **避免子类爆炸** - 不想为每种产品创建工厂子类
- **运行时确定类型** - 需要在运行时确定要创建的对象类型

### 💡 适用场景
- 系统应该独立于它的产品创建、构成和表示
- 要实例化的类是在运行时刻指定的
- 避免创建一个与产品类层次平行的工厂类层次

### 🏗️ 实现示例

```cpp
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// 抽象原型
class Shape {
public:
    virtual ~Shape() = default;
    virtual std::unique_ptr<Shape> clone() const = 0;
    virtual void draw() const = 0;
    virtual std::string getType() const = 0;
    
    // 设置属性
    virtual void setPosition(double x, double y) = 0;
    virtual void setColor(const std::string& color) = 0;
};

// 具体原型：圆形
class Circle : public Shape {
private:
    double radius_;
    double x_, y_;
    std::string color_;
    
public:
    Circle(double radius, double x = 0, double y = 0, const std::string& color = "black")
        : radius_(radius), x_(x), y_(y), color_(color) {}
    
    // 深拷贝
    std::unique_ptr<Shape> clone() const override {
        return std::make_unique<Circle>(radius_, x_, y_, color_);
    }
    
    void draw() const override {
        std::cout << "Drawing Circle: radius=" << radius_ 
                  << ", position=(" << x_ << "," << y_ << ")"
                  << ", color=" << color_ << std::endl;
    }
    
    std::string getType() const override {
        return "Circle";
    }
    
    void setPosition(double x, double y) override {
        x_ = x;
        y_ = y;
    }
    
    void setColor(const std::string& color) override {
        color_ = color;
    }
    
    void setRadius(double radius) {
        radius_ = radius;
    }
    
    double getRadius() const { return radius_; }
};

// 具体原型：矩形
class Rectangle : public Shape {
private:
    double width_, height_;
    double x_, y_;
    std::string color_;
    
public:
    Rectangle(double width, double height, double x = 0, double y = 0, 
              const std::string& color = "black")
        : width_(width), height_(height), x_(x), y_(y), color_(color) {}
    
    std::unique_ptr<Shape> clone() const override {
        return std::make_unique<Rectangle>(width_, height_, x_, y_, color_);
    }
    
    void draw() const override {
        std::cout << "Drawing Rectangle: size=" << width_ << "x" << height_
                  << ", position=(" << x_ << "," << y_ << ")"
                  << ", color=" << color_ << std::endl;
    }
    
    std::string getType() const override {
        return "Rectangle";
    }
    
    void setPosition(double x, double y) override {
        x_ = x;
        y_ = y;
    }
    
    void setColor(const std::string& color) override {
        color_ = color;
    }
    
    void setSize(double width, double height) {
        width_ = width;
        height_ = height;
    }
    
    double getWidth() const { return width_; }
    double getHeight() const { return height_; }
};

// 复杂对象：图形组合
class ShapeGroup : public Shape {
private:
    std::vector<std::unique_ptr<Shape>> shapes_;
    double x_, y_;
    std::string name_;
    
public:
    ShapeGroup(const std::string& name, double x = 0, double y = 0)
        : name_(name), x_(x), y_(y) {}
    
    void addShape(std::unique_ptr<Shape> shape) {
        shapes_.push_back(std::move(shape));
    }
    
    std::unique_ptr<Shape> clone() const override {
        auto cloned = std::make_unique<ShapeGroup>(name_, x_, y_);
        for (const auto& shape : shapes_) {
            cloned->addShape(shape->clone());
        }
        return cloned;
    }
    
    void draw() const override {
        std::cout << "Drawing ShapeGroup '" << name_ << "' at (" << x_ << "," << y_ << "):" << std::endl;
        for (const auto& shape : shapes_) {
            std::cout << "  ";
            shape->draw();
        }
    }
    
    std::string getType() const override {
        return "ShapeGroup";
    }
    
    void setPosition(double x, double y) override {
        x_ = x;
        y_ = y;
    }
    
    void setColor(const std::string& color) override {
        for (auto& shape : shapes_) {
            shape->setColor(color);
        }
    }
    
    size_t getShapeCount() const {
        return shapes_.size();
    }
};

// 原型管理器
class PrototypeManager {
private:
    std::unordered_map<std::string, std::unique_ptr<Shape>> prototypes_;
    
public:
    void registerPrototype(const std::string& name, std::unique_ptr<Shape> prototype) {
        prototypes_[name] = std::move(prototype);
    }
    
    std::unique_ptr<Shape> createShape(const std::string& name) {
        auto it = prototypes_.find(name);
        if (it != prototypes_.end()) {
            return it->second->clone();
        }
        return nullptr;
    }
    
    void listPrototypes() const {
        std::cout << "Available prototypes:" << std::endl;
        for (const auto& [name, prototype] : prototypes_) {
            std::cout << "  " << name << " (" << prototype->getType() << ")" << std::endl;
        }
    }
};

// 文档类（使用原型模式）
class Document {
private:
    std::vector<std::unique_ptr<Shape>> shapes_;
    std::string title_;
    
public:
    Document(const std::string& title) : title_(title) {}
    
    void addShape(std::unique_ptr<Shape> shape) {
        shapes_.push_back(std::move(shape));
    }
    
    void render() const {
        std::cout << "Document: " << title_ << std::endl;
        std::cout << "==================" << std::endl;
        for (const auto& shape : shapes_) {
            shape->draw();
        }
        std::cout << std::endl;
    }
    
    // 创建文档副本
    std::unique_ptr<Document> clone() const {
        auto cloned = std::make_unique<Document>(title_ + " (Copy)");
        for (const auto& shape : shapes_) {
            cloned->addShape(shape->clone());
        }
        return cloned;
    }
    
    size_t getShapeCount() const {
        return shapes_.size();
    }
};

// 游戏对象原型示例
class GameObject {
public:
    virtual ~GameObject() = default;
    virtual std::unique_ptr<GameObject> clone() const = 0;
    virtual void update() = 0;
    virtual void render() const = 0;
    virtual std::string getType() const = 0;
};

class Enemy : public GameObject {
private:
    std::string type_;
    int health_;
    int damage_;
    double speed_;
    
public:
    Enemy(const std::string& type, int health, int damage, double speed)
        : type_(type), health_(health), damage_(damage), speed_(speed) {}
    
    std::unique_ptr<GameObject> clone() const override {
        return std::make_unique<Enemy>(type_, health_, damage_, speed_);
    }
    
    void update() override {
        // 更新敌人逻辑
    }
    
    void render() const override {
        std::cout << "Enemy: " << type_ << " (HP:" << health_ 
                  << ", DMG:" << damage_ << ", SPD:" << speed_ << ")" << std::endl;
    }
    
    std::string getType() const override {
        return type_;
    }
    
    void takeDamage(int damage) {
        health_ -= damage;
        if (health_ < 0) health_ = 0;
    }
};

// 使用示例
void demonstratePrototype() {
    std::cout << "=== Prototype Pattern Demo ===" << std::endl;
    
    // 创建原型管理器
    PrototypeManager manager;
    
    // 注册原型
    manager.registerPrototype("small_circle", 
        std::make_unique<Circle>(5.0, 0, 0, "blue"));
    manager.registerPrototype("medium_rectangle", 
        std::make_unique<Rectangle>(10.0, 8.0, 0, 0, "green"));
    
    // 创建复杂原型
    auto house = std::make_unique<ShapeGroup>("house");
    house->addShape(std::make_unique<Rectangle>(20, 15, 0, 0, "brown"));  // 房屋主体
    house->addShape(std::make_unique<Rectangle>(3, 6, 2, 0, "darkbrown")); // 门
    house->addShape(std::make_unique<Circle>(1, 4, 3, "yellow"));         // 门把手
    
    manager.registerPrototype("house_template", std::move(house));
    
    manager.listPrototypes();
    std::cout << std::endl;
    
    // 创建文档并添加形状
    Document doc("My Drawing");
    
    // 使用原型创建对象
    auto circle1 = manager.createShape("small_circle");
    if (circle1) {
        circle1->setPosition(10, 10);
        doc.addShape(std::move(circle1));
    }
    
    auto rect1 = manager.createShape("medium_rectangle");
    if (rect1) {
        rect1->setPosition(20, 20);
        rect1->setColor("red");
        doc.addShape(std::move(rect1));
    }
    
    auto house1 = manager.createShape("house_template");
    if (house1) {
        house1->setPosition(30, 30);
        doc.addShape(std::move(house1));
    }
    
    // 再创建一个房子
    auto house2 = manager.createShape("house_template");
    if (house2) {
        house2->setPosition(60, 30);
        house2->setColor("lightblue");  // 改变颜色
        doc.addShape(std::move(house2));
    }
    
    // 渲染原始文档
    doc.render();
    
    // 创建文档副本
    auto docCopy = doc.clone();
    docCopy->render();
    
    std::cout << "Original document shapes: " << doc.getShapeCount() << std::endl;
    std::cout << "Copied document shapes: " << docCopy->getShapeCount() << std::endl;
    
    std::cout << std::endl;
    
    // 游戏对象原型示例
    std::cout << "Game Object Prototype Example:" << std::endl;
    
    // 创建敌人原型
    auto zombiePrototype = std::make_unique<Enemy>("Zombie", 100, 25, 2.0);
    auto skeletonPrototype = std::make_unique<Enemy>("Skeleton", 80, 30, 2.5);
    
    // 克隆敌人
    std::vector<std::unique_ptr<GameObject>> enemies;
    
    for (int i = 0; i < 3; ++i) {
        enemies.push_back(zombiePrototype->clone());
        enemies.push_back(skeletonPrototype->clone());
    }
    
    std::cout << "Spawned enemies:" << std::endl;
    for (const auto& enemy : enemies) {
        enemy->render();
    }
}
```

### ✅ 优点
- 减少子类的创建
- 隐藏具体产品类
- 在运行时增加和删除产品
- 改变值以指定新对象

### ❌ 缺点
- 每个类都必须配备一个克隆方法
- 深拷贝和浅拷贝问题需要仔细考虑

---

## 📊 创建型模式对比

| 模式 | 主要目的 | 适用场景 | 优点 | 缺点 |
|------|----------|----------|------|------|
| 工厂方法 | 创建对象的接口 | 不知道具体类型 | 解耦，易扩展 | 类层次复杂 |
| 抽象工厂 | 创建产品族 | 多平台，多风格 | 产品一致性 | 难支持新产品 |
| 单例 | 唯一实例 | 全局状态管理 | 控制实例数 | 难以测试 |
| 建造者 | 分步构建复杂对象 | 构造参数多 | 灵活构建 | 增加复杂性 |
| 原型 | 通过克隆创建 | 创建成本高 | 避免重复创建 | 深拷贝复杂 |

## 💡 最佳实践

### 选择建议
1. **简单对象创建** → 工厂方法
2. **复杂对象创建** → 建造者模式  
3. **全局唯一实例** → 单例模式
4. **相关对象族** → 抽象工厂
5. **克隆现有对象** → 原型模式

### 现代C++技巧
- 使用智能指针管理内存
- 利用移动语义提高性能
- 使用工厂函数而非工厂类
- 考虑使用std::make_unique和std::make_shared

创建型模式帮助我们更好地管理对象的创建过程，提高代码的灵活性和可维护性！
