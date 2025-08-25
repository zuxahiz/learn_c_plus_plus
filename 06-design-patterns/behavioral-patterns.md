# 行为型模式详解

行为型模式关注对象间的通信和职责分配，描述对象和类的交互以及它们之间的职责分配。

## 📋 目录
1. [观察者模式](#观察者模式)
2. [策略模式](#策略模式)
3. [命令模式](#命令模式)
4. [状态模式](#状态模式)
5. [责任链模式](#责任链模式)
6. [模板方法模式](#模板方法模式)
7. [访问者模式](#访问者模式)
8. [中介者模式](#中介者模式)
9. [备忘录模式](#备忘录模式)
10. [迭代器模式](#迭代器模式)
11. [解释器模式](#解释器模式)

---

## 观察者模式

### 🎯 模式意图
定义对象间的一种一对多的依赖关系，当一个对象的状态发生改变时，所有依赖于它的对象都得到通知并被自动更新。

### 🤔 为什么需要？
- **松耦合通信** - 主题和观察者之间松耦合
- **广播通信** - 支持广播式的通信
- **抽象耦合** - 抽象了更新接口

### 🏗️ 实现示例

```cpp
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

// 前向声明
class Observer;

// 被观察者接口
class Subject {
public:
    virtual ~Subject() = default;
    virtual void attach(std::shared_ptr<Observer> observer) = 0;
    virtual void detach(std::shared_ptr<Observer> observer) = 0;
    virtual void notify() = 0;
};

// 观察者接口
class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(Subject* subject) = 0;
};

// 具体被观察者：天气站
class WeatherStation : public Subject {
private:
    std::vector<std::weak_ptr<Observer>> observers;
    float temperature;
    float humidity;
    float pressure;
    
public:
    void attach(std::shared_ptr<Observer> observer) override {
        observers.push_back(observer);
    }
    
    void detach(std::shared_ptr<Observer> observer) override {
        observers.erase(
            std::remove_if(observers.begin(), observers.end(),
                [&](const std::weak_ptr<Observer>& weak_obs) {
                    return weak_obs.lock() == observer;
                }),
            observers.end()
        );
    }
    
    void notify() override {
        // 清理失效的弱引用
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
                obs->update(this);
            }
        }
    }
    
    void setMeasurements(float temperature, float humidity, float pressure) {
        this->temperature = temperature;
        this->humidity = humidity;
        this->pressure = pressure;
        notify();
    }
    
    float getTemperature() const { return temperature; }
    float getHumidity() const { return humidity; }
    float getPressure() const { return pressure; }
};

// 具体观察者：当前天气显示
class CurrentConditionsDisplay : public Observer {
private:
    std::string name;
    
public:
    CurrentConditionsDisplay(const std::string& name) : name(name) {}
    
    void update(Subject* subject) override {
        if (auto weatherStation = dynamic_cast<WeatherStation*>(subject)) {
            std::cout << name << " - Current conditions: "
                      << weatherStation->getTemperature() << "°C, "
                      << weatherStation->getHumidity() << "% humidity, "
                      << weatherStation->getPressure() << " hPa" << std::endl;
        }
    }
};

// 具体观察者：统计显示
class StatisticsDisplay : public Observer {
private:
    std::string name;
    std::vector<float> temperatures;
    
public:
    StatisticsDisplay(const std::string& name) : name(name) {}
    
    void update(Subject* subject) override {
        if (auto weatherStation = dynamic_cast<WeatherStation*>(subject)) {
            temperatures.push_back(weatherStation->getTemperature());
            
            float sum = 0;
            for (float temp : temperatures) {
                sum += temp;
            }
            float average = sum / temperatures.size();
            
            auto minmax = std::minmax_element(temperatures.begin(), temperatures.end());
            
            std::cout << name << " - Statistics: "
                      << "Avg: " << average << "°C, "
                      << "Min: " << *minmax.first << "°C, "
                      << "Max: " << *minmax.second << "°C" << std::endl;
        }
    }
};
```

---

## 策略模式

### 🎯 模式意图
定义一系列的算法，把它们一个个封装起来，并且使它们可相互替换。

### 🤔 为什么需要？
- **算法切换** - 运行时切换算法
- **避免条件语句** - 消除大量的条件分支语句
- **算法复用** - 相同算法在不同地方使用

### 🏗️ 实现示例

```cpp
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

// 策略接口
template<typename T>
class SortStrategy {
public:
    virtual ~SortStrategy() = default;
    virtual void sort(std::vector<T>& data) = 0;
    virtual std::string getName() const = 0;
};

// 具体策略：快速排序
template<typename T>
class QuickSortStrategy : public SortStrategy<T> {
public:
    void sort(std::vector<T>& data) override {
        if (data.size() <= 1) return;
        quickSort(data, 0, data.size() - 1);
    }
    
    std::string getName() const override {
        return "QuickSort";
    }
    
private:
    void quickSort(std::vector<T>& arr, int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    }
    
    int partition(std::vector<T>& arr, int low, int high) {
        T pivot = arr[high];
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

// 具体策略：归并排序
template<typename T>
class MergeSortStrategy : public SortStrategy<T> {
public:
    void sort(std::vector<T>& data) override {
        if (data.size() <= 1) return;
        mergeSort(data, 0, data.size() - 1);
    }
    
    std::string getName() const override {
        return "MergeSort";
    }
    
private:
    void mergeSort(std::vector<T>& arr, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSort(arr, left, mid);
            mergeSort(arr, mid + 1, right);
            merge(arr, left, mid, right);
        }
    }
    
    void merge(std::vector<T>& arr, int left, int mid, int right) {
        std::vector<T> temp(right - left + 1);
        int i = left, j = mid + 1, k = 0;
        
        while (i <= mid && j <= right) {
            if (arr[i] <= arr[j]) {
                temp[k++] = arr[i++];
            } else {
                temp[k++] = arr[j++];
            }
        }
        
        while (i <= mid) temp[k++] = arr[i++];
        while (j <= right) temp[k++] = arr[j++];
        
        for (int i = 0; i < k; i++) {
            arr[left + i] = temp[i];
        }
    }
};

// 上下文类：排序器
template<typename T>
class Sorter {
private:
    std::unique_ptr<SortStrategy<T>> strategy;
    
public:
    void setStrategy(std::unique_ptr<SortStrategy<T>> newStrategy) {
        strategy = std::move(newStrategy);
    }
    
    void sort(std::vector<T>& data) {
        if (strategy) {
            std::cout << "Using " << strategy->getName() << " strategy" << std::endl;
            strategy->sort(data);
        }
    }
};
```

---

## 命令模式

### 🎯 模式意图
将一个请求封装为一个对象，从而使你可用不同的请求对客户进行参数化；对请求排队或记录请求日志，以及支持可撤销的操作。

### 🏗️ 实现示例

```cpp
#include <iostream>
#include <memory>
#include <stack>
#include <vector>

// 命令接口
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual std::string getDescription() const = 0;
};

// 接收者：文本编辑器
class TextEditor {
private:
    std::string content;
    
public:
    void insertText(const std::string& text, size_t position) {
        if (position <= content.length()) {
            content.insert(position, text);
        }
    }
    
    void deleteText(size_t position, size_t length) {
        if (position < content.length()) {
            content.erase(position, std::min(length, content.length() - position));
        }
    }
    
    std::string getContent() const {
        return content;
    }
    
    void setContent(const std::string& newContent) {
        content = newContent;
    }
    
    size_t getLength() const {
        return content.length();
    }
};

// 具体命令：插入文本
class InsertTextCommand : public Command {
private:
    TextEditor* editor;
    std::string text;
    size_t position;
    
public:
    InsertTextCommand(TextEditor* editor, const std::string& text, size_t position)
        : editor(editor), text(text), position(position) {}
    
    void execute() override {
        editor->insertText(text, position);
    }
    
    void undo() override {
        editor->deleteText(position, text.length());
    }
    
    std::string getDescription() const override {
        return "Insert '" + text + "' at position " + std::to_string(position);
    }
};

// 具体命令：删除文本
class DeleteTextCommand : public Command {
private:
    TextEditor* editor;
    size_t position;
    size_t length;
    std::string deletedText;
    
public:
    DeleteTextCommand(TextEditor* editor, size_t position, size_t length)
        : editor(editor), position(position), length(length) {}
    
    void execute() override {
        std::string content = editor->getContent();
        if (position < content.length()) {
            size_t actualLength = std::min(length, content.length() - position);
            deletedText = content.substr(position, actualLength);
            editor->deleteText(position, actualLength);
        }
    }
    
    void undo() override {
        editor->insertText(deletedText, position);
    }
    
    std::string getDescription() const override {
        return "Delete " + std::to_string(length) + " characters at position " + std::to_string(position);
    }
};

// 调用者：编辑器控制器
class EditorController {
private:
    std::stack<std::unique_ptr<Command>> undoStack;
    std::stack<std::unique_ptr<Command>> redoStack;
    
public:
    void executeCommand(std::unique_ptr<Command> command) {
        command->execute();
        undoStack.push(std::move(command));
        
        // 清空重做栈
        while (!redoStack.empty()) {
            redoStack.pop();
        }
    }
    
    void undo() {
        if (!undoStack.empty()) {
            auto command = std::move(undoStack.top());
            undoStack.pop();
            
            command->undo();
            redoStack.push(std::move(command));
        }
    }
    
    void redo() {
        if (!redoStack.empty()) {
            auto command = std::move(redoStack.top());
            redoStack.pop();
            
            command->execute();
            undoStack.push(std::move(command));
        }
    }
    
    bool canUndo() const {
        return !undoStack.empty();
    }
    
    bool canRedo() const {
        return !redoStack.empty();
    }
};
```

---

## 状态模式

### 🎯 模式意图
允许一个对象在其内部状态改变时改变它的行为。对象看起来似乎修改了它的类。

### 🏗️ 实现示例

```cpp
#include <iostream>
#include <memory>

// 前向声明
class VendingMachine;

// 抽象状态
class State {
public:
    virtual ~State() = default;
    virtual void insertCoin(VendingMachine* machine) = 0;
    virtual void selectProduct(VendingMachine* machine) = 0;
    virtual void dispense(VendingMachine* machine) = 0;
    virtual std::string getStateName() const = 0;
};

// 上下文：自动售货机
class VendingMachine {
private:
    std::unique_ptr<State> currentState;
    int coinCount;
    int productCount;
    
public:
    VendingMachine(int productCount);
    
    void setState(std::unique_ptr<State> state) {
        currentState = std::move(state);
    }
    
    void insertCoin() {
        currentState->insertCoin(this);
    }
    
    void selectProduct() {
        currentState->selectProduct(this);
    }
    
    void dispense() {
        currentState->dispense(this);
    }
    
    void addCoin() { coinCount++; }
    void removeCoin() { if (coinCount > 0) coinCount--; }
    void removeProduct() { if (productCount > 0) productCount--; }
    
    int getCoinCount() const { return coinCount; }
    int getProductCount() const { return productCount; }
    
    std::string getCurrentState() const {
        return currentState ? currentState->getStateName() : "Unknown";
    }
};

// 具体状态：无硬币状态
class NoCoinState : public State {
public:
    void insertCoin(VendingMachine* machine) override;
    void selectProduct(VendingMachine* machine) override;
    void dispense(VendingMachine* machine) override;
    std::string getStateName() const override { return "NoCoin"; }
};

// 具体状态：有硬币状态
class HasCoinState : public State {
public:
    void insertCoin(VendingMachine* machine) override;
    void selectProduct(VendingMachine* machine) override;
    void dispense(VendingMachine* machine) override;
    std::string getStateName() const override { return "HasCoin"; }
};

// 具体状态：售出状态
class SoldState : public State {
public:
    void insertCoin(VendingMachine* machine) override;
    void selectProduct(VendingMachine* machine) override;
    void dispense(VendingMachine* machine) override;
    std::string getStateName() const override { return "Sold"; }
};

// 具体状态：售罄状态
class SoldOutState : public State {
public:
    void insertCoin(VendingMachine* machine) override;
    void selectProduct(VendingMachine* machine) override;
    void dispense(VendingMachine* machine) override;
    std::string getStateName() const override { return "SoldOut"; }
};

// 实现VendingMachine构造函数
VendingMachine::VendingMachine(int productCount) 
    : coinCount(0), productCount(productCount) {
    if (productCount > 0) {
        currentState = std::make_unique<NoCoinState>();
    } else {
        currentState = std::make_unique<SoldOutState>();
    }
}
```

## 模板方法模式

### 🎯 模式意图
定义一个操作中的算法的骨架，而将一些步骤延迟到子类中。

### 🏗️ 实现示例

```cpp
// 抽象基类
class DataProcessor {
public:
    // 模板方法
    void process() {
        readData();
        processData();
        writeData();
    }
    
protected:
    virtual void readData() = 0;
    virtual void processData() = 0;
    virtual void writeData() = 0;
};

// 具体实现
class CSVProcessor : public DataProcessor {
protected:
    void readData() override {
        std::cout << "Reading CSV data" << std::endl;
    }
    
    void processData() override {
        std::cout << "Processing CSV data" << std::endl;
    }
    
    void writeData() override {
        std::cout << "Writing CSV data" << std::endl;
    }
};
```

---

## 责任链模式

### 🎯 模式意图
避免请求发送者与接收者耦合在一起，让多个对象都有可能接收请求。

### 🏗️ 实现示例

```cpp
// 处理器基类
class Handler {
protected:
    std::unique_ptr<Handler> nextHandler;
    
public:
    void setNext(std::unique_ptr<Handler> handler) {
        nextHandler = std::move(handler);
    }
    
    virtual void handleRequest(int request) {
        if (canHandle(request)) {
            processRequest(request);
        } else if (nextHandler) {
            nextHandler->handleRequest(request);
        } else {
            std::cout << "No handler for request: " << request << std::endl;
        }
    }
    
protected:
    virtual bool canHandle(int request) = 0;
    virtual void processRequest(int request) = 0;
};

// 具体处理器
class ConcreteHandler1 : public Handler {
protected:
    bool canHandle(int request) override {
        return request >= 0 && request < 10;
    }
    
    void processRequest(int request) override {
        std::cout << "Handler1 processed request: " << request << std::endl;
    }
};
```

---

## 迭代器模式

### 🎯 模式意图
提供一种方法顺序访问一个聚合对象中各个元素，而又不需暴露该对象的内部表示。

### 🏗️ 实现示例

```cpp
template<typename T>
class Iterator {
public:
    virtual ~Iterator() = default;
    virtual bool hasNext() = 0;
    virtual T& next() = 0;
};

template<typename T>
class ConcreteIterator : public Iterator<T> {
private:
    std::vector<T>& collection;
    size_t position;
    
public:
    ConcreteIterator(std::vector<T>& coll) : collection(coll), position(0) {}
    
    bool hasNext() override {
        return position < collection.size();
    }
    
    T& next() override {
        return collection[position++];
    }
};
```

---

## 中介者模式

### 🎯 模式意图
用一个中介对象来封装一系列的对象交互。

### 🏗️ 实现示例

```cpp
class Mediator {
public:
    virtual ~Mediator() = default;
    virtual void notify(const std::string& sender, const std::string& event) = 0;
};

class Component {
protected:
    Mediator* mediator;
    
public:
    Component(Mediator* m) : mediator(m) {}
};

class Button : public Component {
public:
    Button(Mediator* m) : Component(m) {}
    
    void click() {
        mediator->notify("Button", "click");
    }
};
```

---

## 备忘录模式

### 🎯 模式意图
在不破坏封装性的前提下，捕获一个对象的内部状态。

### 🏗️ 实现示例

```cpp
class Memento {
private:
    std::string state;
    
public:
    Memento(const std::string& state) : state(state) {}
    std::string getState() const { return state; }
};

class Originator {
private:
    std::string state;
    
public:
    void setState(const std::string& state) {
        this->state = state;
    }
    
    std::string getState() const {
        return state;
    }
    
    std::unique_ptr<Memento> saveState() {
        return std::make_unique<Memento>(state);
    }
    
    void restoreState(const Memento& memento) {
        state = memento.getState();
    }
};
```

---

## 访问者模式

### 🎯 模式意图
表示一个作用于某对象结构中的各元素的操作。

### 🏗️ 实现示例

```cpp
class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visit(class ConcreteElementA& element) = 0;
    virtual void visit(class ConcreteElementB& element) = 0;
};

class Element {
public:
    virtual ~Element() = default;
    virtual void accept(Visitor& visitor) = 0;
};

class ConcreteElementA : public Element {
public:
    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
    
    void operationA() {
        std::cout << "Operation A" << std::endl;
    }
};
```

---

## 解释器模式

### 🎯 模式意图
给定一个语言，定义它的文法的一种表示，并定义一个解释器。

### 🏗️ 实现示例

```cpp
class Context {
private:
    std::map<std::string, int> variables;
    
public:
    void setVariable(const std::string& name, int value) {
        variables[name] = value;
    }
    
    int getVariable(const std::string& name) {
        return variables[name];
    }
};

class Expression {
public:
    virtual ~Expression() = default;
    virtual int interpret(Context& context) = 0;
};

class NumberExpression : public Expression {
private:
    int number;
    
public:
    NumberExpression(int num) : number(num) {}
    
    int interpret(Context& context) override {
        return number;
    }
};
```

## 📊 行为型模式总结

这11种行为型模式为我们提供了丰富的对象交互方式：

### 🎯 核心价值
1. **观察者模式** - 对象状态变化通知
2. **策略模式** - 算法封装和切换
3. **命令模式** - 请求封装和参数化
4. **状态模式** - 状态相关行为封装
5. **模板方法模式** - 算法框架定义
6. **责任链模式** - 请求处理链
7. **迭代器模式** - 聚合对象访问
8. **中介者模式** - 对象间通信协调
9. **备忘录模式** - 对象状态保存恢复
10. **访问者模式** - 操作与结构分离
11. **解释器模式** - 语言解释器构建

这些模式帮助我们设计出更灵活、更可维护的对象交互系统！🎭
