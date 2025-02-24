#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <map>

// ========== Singleton Pattern (LCD Output) ==========
class LCDHandler {
public:
    static LCDHandler& getInstance() {
        static LCDHandler instance;
        return instance;
    }

    void printLine(int row, const std::string& message) {
        std::cout << "LCD[" << row << "]: " << message << std::endl;
    }

private:
    LCDHandler() {}
    LCDHandler(const LCDHandler&) = delete;
    void operator=(const LCDHandler&) = delete;
};

// ========== Observer Pattern (Notification System) ==========
class Observer {
public:
    virtual void update(float value) = 0;
};

// ========== Sensor Base Class ==========
class Sensor {
protected:
    float sensorValue = 0.0;
    std::vector<Observer*> observers;
public:
    virtual void initialize() = 0;
    virtual void readSensor() = 0;
    virtual void processData() = 0;
    virtual void run() = 0;
    void attach(Observer* obs) { observers.push_back(obs); }
    void notify() {
        for (Observer* obs : observers) {
            obs->update(sensorValue);
        }
    }
};

// ========== State Pattern (Sensor Modes) ==========
class SensorState {
public:
    virtual void handle(Sensor* sensor) = 0;
};

class IdleState : public SensorState {
public:
    void handle(Sensor* sensor) override {
        LCDHandler::getInstance().printLine(0, "Sensor is IDLE");
    }
};

class ActiveState : public SensorState {
public:
    void handle(Sensor* sensor) override {
        sensor->readSensor();
        sensor->processData();
    }
};

class ErrorState : public SensorState {
public:
    void handle(Sensor* sensor) override {
        LCDHandler::getInstance().printLine(0, "Sensor ERROR! Check connections.");
    }
};

// ========== Strategy Pattern (Sensor Data Processing) ==========
class ProcessingStrategy {
public:
    virtual void process(float value) = 0;
};

class SimpleProcessing : public ProcessingStrategy {
public:
    void process(float value) override {
        LCDHandler::getInstance().printLine(1, "Processed: " + std::to_string(value));
    }
};

class AdvancedProcessing : public ProcessingStrategy {
public:
    void process(float value) override {
        LCDHandler::getInstance().printLine(1, "Advanced Processed: " + std::to_string(value * 1.1));
    }
};

// ========== Concrete Sensors ==========
class TemperatureSensor : public Sensor {
private:
    SensorState* state;
    ProcessingStrategy* strategy;
public:
    TemperatureSensor(SensorState* initialState, ProcessingStrategy* strat) : state(initialState), strategy(strat) {}

    void setState(SensorState* newState) { state = newState; }
    void setStrategy(ProcessingStrategy* strat) { strategy = strat; }

    void initialize() override {
        LCDHandler::getInstance().printLine(0, "Temp Sensor Init");
    }

    void readSensor() override {
        sensorValue = 20.0f + (rand() % 1500) / 100.0f; // Simulated temperature
        notify();
    }

    void processData() override {
        strategy->process(sensorValue);
    }

    void run() {
        state->handle(this);
    }
};

// ========== Concrete Observers ==========
class LCDDisplay : public Observer {
public:
    void update(float value) override {
        LCDHandler::getInstance().printLine(1, "LCD Display: Temp = " + std::to_string(value) + "°C");
    }
};

class Logger : public Observer {
public:
    void update(float value) override {
        std::cout << "Logger: Temperature recorded: " << value << "°C\n";
    }
};

// ========== Factory Pattern (Sensor Creation) ==========
class SensorFactory {
public:
    static Sensor* createSensor(SensorState* state, ProcessingStrategy* strategy) {
        return new TemperatureSensor(state, strategy);
    }
};

// ========== Command Pattern (User Actions) ==========
class Command {
public:
    virtual void execute() = 0;
};

class SetTemperatureCommand : public Command {
private:
    float temperature;
public:
    SetTemperatureCommand(float temp) : temperature(temp) {}
    void execute() override {
        LCDHandler::getInstance().printLine(0, "Set Temp to " + std::to_string(temperature) + "°C");
    }
};

class TurnOnLEDCommand : public Command {
public:
    void execute() override {
        LCDHandler::getInstance().printLine(0, "LED Turned ON");
    }
};

// ========== Command Processor ==========
class CommandProcessor {
private:
    std::map<std::string, Command*> commands;
public:
    void registerCommand(const std::string& name, Command* command) {
        commands[name] = command;
    }

    void executeCommand(const std::string& name) {
        if (commands.find(name) != commands.end()) {
            commands[name]->execute();
        } else {
            std::cout << "Unknown command: " << name << std::endl;
        }
    }
};

// ========== Main Simulation ==========
int main() {
    srand(static_cast<unsigned>(time(0)));

    LCDHandler::getInstance().printLine(0, "System Booting...");

    // Create States
    IdleState idle;
    ActiveState active;
    ErrorState error;

    // Create Processing Strategies
    SimpleProcessing simple;
    AdvancedProcessing advanced;

    // Create Sensor using Factory
    Sensor* tempSensor = SensorFactory::createSensor(&idle, &simple);

    // Attach Observers
    LCDDisplay lcd;
    Logger logger;
    tempSensor->attach(&lcd);
    tempSensor->attach(&logger);

    // Create Command Processor
    CommandProcessor processor;
    processor.registerCommand("SET_TEMP", new SetTemperatureCommand(25.0));
    processor.registerCommand("LED_ON", new TurnOnLEDCommand());

    // Run Simulation
    tempSensor->run(); // Initially IDLE
    static_cast<TemperatureSensor*>(tempSensor)->setState(&active);
    
    for (int i = 0; i < 5; i++) {
        tempSensor->run(); // Read sensor and process
    }

    // Execute commands
    processor.executeCommand("SET_TEMP");
    processor.executeCommand("LED_ON");
    processor.executeCommand("INVALID"); // Unknown command

    delete tempSensor;
    return 0;
}
