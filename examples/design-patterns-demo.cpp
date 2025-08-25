/**
 * @file design-patterns-demo.cpp
 * @brief 设计模式综合演示程序
 * @author C++ Learning Guide
 * @date 2024
 * 
 * 这个文件演示了多种设计模式的实际应用，包括：
 * - 创建型模式：工厂方法、单例、建造者
 * - 结构型模式：装饰器、适配器、外观
 * - 行为型模式：观察者、策略、命令
 * - 现代C++模式：RAII、CRTP
 */

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <algorithm>
#include <chrono>
#include <thread>

// ================== 单例模式 ==================

class Logger {
private:
    Logger() = default;
    std::string logLevel = "INFO";
    
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }
    
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    void log(const std::string& level, const std::string& message) {
        std::cout << "[" << level << "] " << message << std::endl;
    }
    
    void setLogLevel(const std::string& level) {
        logLevel = level;
    }
};

// ================== 工厂方法模式 ==================

// 抽象产品
class Vehicle {
public:
    virtual ~Vehicle() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual std::string getType() const = 0;
};

// 具体产品
class Car : public Vehicle {
public:
    void start() override {
        std::cout << "Car engine started" << std::endl;
    }
    
    void stop() override {
        std::cout << "Car engine stopped" << std::endl;
    }
    
    std::string getType() const override {
        return "Car";
    }
};

class Motorcycle : public Vehicle {
public:
    void start() override {
        std::cout << "Motorcycle engine started" << std::endl;
    }
    
    void stop() override {
        std::cout << "Motorcycle engine stopped" << std::endl;
    }
    
    std::string getType() const override {
        return "Motorcycle";
    }
};

// 抽象工厂
class VehicleFactory {
public:
    virtual ~VehicleFactory() = default;
    virtual std::unique_ptr<Vehicle> createVehicle() = 0;
};

// 具体工厂
class CarFactory : public VehicleFactory {
public:
    std::unique_ptr<Vehicle> createVehicle() override {
        return std::make_unique<Car>();
    }
};

class MotorcycleFactory : public VehicleFactory {
public:
    std::unique_ptr<Vehicle> createVehicle() override {
        return std::make_unique<Motorcycle>();
    }
};

// ================== 建造者模式 ==================

class Computer {
private:
    std::string cpu;
    std::string gpu;
    std::string ram;
    std::string storage;
    
public:
    void setCPU(const std::string& cpu) { this->cpu = cpu; }
    void setGPU(const std::string& gpu) { this->gpu = gpu; }
    void setRAM(const std::string& ram) { this->ram = ram; }
    void setStorage(const std::string& storage) { this->storage = storage; }
    
    void showSpecs() const {
        std::cout << "Computer Specs:" << std::endl;
        std::cout << "  CPU: " << cpu << std::endl;
        std::cout << "  GPU: " << gpu << std::endl;
        std::cout << "  RAM: " << ram << std::endl;
        std::cout << "  Storage: " << storage << std::endl;
    }
};

class ComputerBuilder {
protected:
    std::unique_ptr<Computer> computer;
    
public:
    ComputerBuilder() : computer(std::make_unique<Computer>()) {}
    virtual ~ComputerBuilder() = default;
    
    virtual ComputerBuilder& buildCPU() = 0;
    virtual ComputerBuilder& buildGPU() = 0;
    virtual ComputerBuilder& buildRAM() = 0;
    virtual ComputerBuilder& buildStorage() = 0;
    
    std::unique_ptr<Computer> getResult() {
        return std::move(computer);
    }
};

class GamingComputerBuilder : public ComputerBuilder {
public:
    ComputerBuilder& buildCPU() override {
        computer->setCPU("Intel i9-13900K");
        return *this;
    }
    
    ComputerBuilder& buildGPU() override {
        computer->setGPU("RTX 4080");
        return *this;
    }
    
    ComputerBuilder& buildRAM() override {
        computer->setRAM("32GB DDR5");
        return *this;
    }
    
    ComputerBuilder& buildStorage() override {
        computer->setStorage("2TB NVMe SSD");
        return *this;
    }
};

// ================== 装饰器模式 ==================

class Beverage {
public:
    virtual ~Beverage() = default;
    virtual std::string getDescription() const = 0;
    virtual double getCost() const = 0;
};

class Coffee : public Beverage {
public:
    std::string getDescription() const override {
        return "Coffee";
    }
    
    double getCost() const override {
        return 2.0;
    }
};

class BeverageDecorator : public Beverage {
protected:
    std::unique_ptr<Beverage> beverage;
    
public:
    BeverageDecorator(std::unique_ptr<Beverage> b) : beverage(std::move(b)) {}
};

class MilkDecorator : public BeverageDecorator {
public:
    MilkDecorator(std::unique_ptr<Beverage> b) : BeverageDecorator(std::move(b)) {}
    
    std::string getDescription() const override {
        return beverage->getDescription() + ", Milk";
    }
    
    double getCost() const override {
        return beverage->getCost() + 0.5;
    }
};

class SugarDecorator : public BeverageDecorator {
public:
    SugarDecorator(std::unique_ptr<Beverage> b) : BeverageDecorator(std::move(b)) {}
    
    std::string getDescription() const override {
        return beverage->getDescription() + ", Sugar";
    }
    
    double getCost() const override {
        return beverage->getCost() + 0.2;
    }
};

// ================== 观察者模式 ==================

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const std::string& event) = 0;
};

class Subject {
private:
    std::vector<std::weak_ptr<Observer>> observers;
    
public:
    void attach(std::shared_ptr<Observer> observer) {
        observers.push_back(observer);
    }
    
    void notify(const std::string& event) {
        // 清理过期的观察者
        observers.erase(
            std::remove_if(observers.begin(), observers.end(),
                [](const std::weak_ptr<Observer>& weak_obs) {
                    return weak_obs.expired();
                }),
            observers.end()
        );
        
        // 通知所有观察者
        for (auto& weak_obs : observers) {
            if (auto obs = weak_obs.lock()) {
                obs->update(event);
            }
        }
    }
};

class NewsSubscriber : public Observer {
private:
    std::string name;
    
public:
    NewsSubscriber(const std::string& name) : name(name) {}
    
    void update(const std::string& event) override {
        std::cout << "Subscriber " << name << " received: " << event << std::endl;
    }
};

// ================== 策略模式 ==================

class SortStrategy {
public:
    virtual ~SortStrategy() = default;
    virtual void sort(std::vector<int>& data) = 0;
    virtual std::string getName() const = 0;
};

class BubbleSortStrategy : public SortStrategy {
public:
    void sort(std::vector<int>& data) override {
        int n = data.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (data[j] > data[j + 1]) {
                    std::swap(data[j], data[j + 1]);
                }
            }
        }
    }
    
    std::string getName() const override {
        return "Bubble Sort";
    }
};

class QuickSortStrategy : public SortStrategy {
public:
    void sort(std::vector<int>& data) override {
        if (!data.empty()) {
            quickSort(data, 0, data.size() - 1);
        }
    }
    
    std::string getName() const override {
        return "Quick Sort";
    }
    
private:
    void quickSort(std::vector<int>& arr, int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    }
    
    int partition(std::vector<int>& arr, int low, int high) {
        int pivot = arr[high];
        int i = low - 1;
        
        for (int j = low; j < high; j++) {
            if (arr[j] <= pivot) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]);
        return i + 1;
    }
};

class Sorter {
private:
    std::unique_ptr<SortStrategy> strategy;
    
public:
    void setStrategy(std::unique_ptr<SortStrategy> newStrategy) {
        strategy = std::move(newStrategy);
    }
    
    void sort(std::vector<int>& data) {
        if (strategy) {
            std::cout << "Using " << strategy->getName() << std::endl;
            auto start = std::chrono::high_resolution_clock::now();
            strategy->sort(data);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << "Sorting completed in " << duration.count() << " microseconds" << std::endl;
        }
    }
};

// ================== 命令模式 ==================

class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class Light {
private:
    bool isOn = false;
    
public:
    void turnOn() {
        isOn = true;
        std::cout << "Light is ON" << std::endl;
    }
    
    void turnOff() {
        isOn = false;
        std::cout << "Light is OFF" << std::endl;
    }
    
    bool getState() const { return isOn; }
};

class LightOnCommand : public Command {
private:
    Light* light;
    
public:
    LightOnCommand(Light* light) : light(light) {}
    
    void execute() override {
        light->turnOn();
    }
    
    void undo() override {
        light->turnOff();
    }
};

class LightOffCommand : public Command {
private:
    Light* light;
    
public:
    LightOffCommand(Light* light) : light(light) {}
    
    void execute() override {
        light->turnOff();
    }
    
    void undo() override {
        light->turnOn();
    }
};

class RemoteControl {
private:
    std::map<int, std::unique_ptr<Command>> commands;
    std::unique_ptr<Command> lastCommand;
    
public:
    void setCommand(int slot, std::unique_ptr<Command> command) {
        commands[slot] = std::move(command);
    }
    
    void pressButton(int slot) {
        if (commands.find(slot) != commands.end()) {
            commands[slot]->execute();
            lastCommand = std::move(commands[slot]);
        }
    }
    
    void pressUndo() {
        if (lastCommand) {
            lastCommand->undo();
        }
    }
};

// ================== RAII模式 ==================

class Timer {
private:
    std::chrono::high_resolution_clock::time_point start;
    std::string name;
    
public:
    Timer(const std::string& name) : name(name) {
        start = std::chrono::high_resolution_clock::now();
        std::cout << "Timer '" << name << "' started" << std::endl;
    }
    
    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Timer '" << name << "' finished: " 
                  << duration.count() << "ms" << std::endl;
    }
};

// ================== CRTP模式 ==================

template<typename Derived>
class Printable {
public:
    void print() const {
        static_cast<const Derived*>(this)->printImpl();
    }
};

class Document : public Printable<Document> {
private:
    std::string content;
    
public:
    Document(const std::string& content) : content(content) {}
    
    void printImpl() const {
        std::cout << "Printing document: " << content << std::endl;
    }
};

// ================== 演示函数 ==================

void demonstrateCreationalPatterns() {
    std::cout << "\n=== Creational Patterns Demo ===" << std::endl;
    
    // 单例模式
    Logger& logger = Logger::getInstance();
    logger.log("INFO", "Starting creational patterns demo");
    
    // 工厂方法模式
    std::cout << "\n--- Factory Method Pattern ---" << std::endl;
    auto carFactory = std::make_unique<CarFactory>();
    auto motorcycleFactory = std::make_unique<MotorcycleFactory>();
    
    auto car = carFactory->createVehicle();
    auto motorcycle = motorcycleFactory->createVehicle();
    
    car->start();
    motorcycle->start();
    car->stop();
    motorcycle->stop();
    
    // 建造者模式
    std::cout << "\n--- Builder Pattern ---" << std::endl;
    GamingComputerBuilder builder;
    auto computer = builder.buildCPU()
                          .buildGPU()
                          .buildRAM()
                          .buildStorage()
                          .getResult();
    computer->showSpecs();
}

void demonstrateStructuralPatterns() {
    std::cout << "\n=== Structural Patterns Demo ===" << std::endl;
    
    // 装饰器模式
    std::cout << "\n--- Decorator Pattern ---" << std::endl;
    auto beverage = std::make_unique<Coffee>();
    beverage = std::make_unique<MilkDecorator>(std::move(beverage));
    beverage = std::make_unique<SugarDecorator>(std::move(beverage));
    
    std::cout << "Order: " << beverage->getDescription() << std::endl;
    std::cout << "Cost: $" << beverage->getCost() << std::endl;
}

void demonstrateBehavioralPatterns() {
    std::cout << "\n=== Behavioral Patterns Demo ===" << std::endl;
    
    // 观察者模式
    std::cout << "\n--- Observer Pattern ---" << std::endl;
    Subject newsAgency;
    auto subscriber1 = std::make_shared<NewsSubscriber>("Alice");
    auto subscriber2 = std::make_shared<NewsSubscriber>("Bob");
    
    newsAgency.attach(subscriber1);
    newsAgency.attach(subscriber2);
    
    newsAgency.notify("Breaking News: Design Patterns are awesome!");
    
    // 策略模式
    std::cout << "\n--- Strategy Pattern ---" << std::endl;
    std::vector<int> data1 = {64, 34, 25, 12, 22, 11, 90};
    std::vector<int> data2 = data1;  // 拷贝用于不同算法测试
    
    Sorter sorter;
    
    std::cout << "Data to sort: ";
    for (int x : data1) std::cout << x << " ";
    std::cout << std::endl;
    
    sorter.setStrategy(std::make_unique<BubbleSortStrategy>());
    sorter.sort(data1);
    
    sorter.setStrategy(std::make_unique<QuickSortStrategy>());
    sorter.sort(data2);
    
    // 命令模式
    std::cout << "\n--- Command Pattern ---" << std::endl;
    Light light;
    RemoteControl remote;
    
    remote.setCommand(1, std::make_unique<LightOnCommand>(&light));
    remote.setCommand(2, std::make_unique<LightOffCommand>(&light));
    
    remote.pressButton(1);  // 开灯
    remote.pressButton(2);  // 关灯
    remote.pressUndo();     // 撤销（开灯）
}

void demonstrateModernPatterns() {
    std::cout << "\n=== Modern C++ Patterns Demo ===" << std::endl;
    
    // RAII模式
    std::cout << "\n--- RAII Pattern ---" << std::endl;
    {
        Timer timer("RAII Demo");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // Timer会在作用域结束时自动销毁并显示耗时
    }
    
    // CRTP模式
    std::cout << "\n--- CRTP Pattern ---" << std::endl;
    Document doc("Important Document Content");
    doc.print();
}

void demonstratePatternCombination() {
    std::cout << "\n=== Pattern Combination Demo ===" << std::endl;
    
    // 组合使用多种模式
    Timer timer("Pattern Combination");
    Logger& logger = Logger::getInstance();
    
    logger.log("INFO", "Creating vehicles using factory pattern");
    
    auto carFactory = std::make_unique<CarFactory>();
    auto car = carFactory->createVehicle();
    
    logger.log("INFO", "Vehicle created: " + car->getType());
    
    // 使用命令模式控制车辆
    class VehicleStartCommand : public Command {
    private:
        Vehicle* vehicle;
    public:
        VehicleStartCommand(Vehicle* v) : vehicle(v) {}
        void execute() override { vehicle->start(); }
        void undo() override { vehicle->stop(); }
    };
    
    RemoteControl vehicleRemote;
    vehicleRemote.setCommand(1, std::make_unique<VehicleStartCommand>(car.get()));
    vehicleRemote.pressButton(1);
    
    logger.log("INFO", "Pattern combination demo completed");
}

int main() {
    try {
        std::cout << "🎨 Design Patterns Comprehensive Demo 🎨" << std::endl;
        std::cout << "===========================================" << std::endl;
        
        demonstrateCreationalPatterns();
        demonstrateStructuralPatterns();
        demonstrateBehavioralPatterns();
        demonstrateModernPatterns();
        demonstratePatternCombination();
        
        std::cout << "\n===========================================" << std::endl;
        std::cout << "✅ All design patterns demonstrated successfully!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
