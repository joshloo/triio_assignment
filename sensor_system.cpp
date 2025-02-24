#include <iostream>
#include <cstdlib>  // For rand()
#include <ctime>    // For seeding rand()
#include <vector>

// ========== Singleton Pattern (LCD Simulated Output) ==========
class LCDHandler {
public:
    static LCDHandler& getInstance() {
        static LCDHandler instance;
        return instance;
    }

    void init() {
        printLine(0, "System Ready...");
    }

    void printLine(int row, const std::string& message) {
        std::cout << "LCD[" << row << "]: " << message << std::endl;
    }

private:
    LCDHandler() {}
    LCDHandler(const LCDHandler&) = delete;
    void operator=(const LCDHandler&) = delete;
};

// ========== Abstract Sensor Class ==========
class Sensor {
public:
    virtual void initialize() = 0;
    virtual void readSensor() = 0;
    virtual void processData() = 0;
    virtual ~Sensor() = default; // Ensure proper cleanup

protected:
    float sensorValue = 0.0;
};

// ========== Temperature Sensor (Random Simulation) ==========
class TemperatureSensor : public Sensor {
public:
    static int instanceCount;

    TemperatureSensor() { instanceCount++; }

    void initialize() override {
        LCDHandler::getInstance().printLine(0, "Temp Sensor Init");
    }

    void readSensor() override {
        sensorValue = 20.0f + (rand() % 1500) / 100.0f; // Generates 20.0 - 35.0°C
    }

    void processData() override {
        LCDHandler::getInstance().printLine(1, "Temp: " + std::to_string(sensorValue) + " C");
    }
};

// ========== Humidity Sensor (Random Simulation) ==========
class HumiditySensor : public Sensor {
public:
    static int instanceCount;

    HumiditySensor() { instanceCount++; }

    void initialize() override {
        LCDHandler::getInstance().printLine(0, "Hum Sensor Init");
    }

    void readSensor() override {
        sensorValue = 40.0f + (rand() % 4000) / 100.0f; // Generates 40.0 - 80.0% humidity
    }

    void processData() override {
        LCDHandler::getInstance().printLine(1, "Humidity: " + std::to_string(sensorValue) + "%");
    }
};

// Define static members
int TemperatureSensor::instanceCount = 0;
int HumiditySensor::instanceCount = 0;

// ========== Factory Pattern ==========
class SensorFactory {
public:
    static Sensor* createSensor(int type) {
        if (type == 0) return new TemperatureSensor();
        if (type == 1) return new HumiditySensor();
        return nullptr;
    }
};

// ========== Simulation ==========
int main() {
    srand(static_cast<unsigned>(time(0))); // Seed random generator

    LCDHandler::getInstance().init(); // Initialize LCD

    std::vector<Sensor*> sensors;
    sensors.push_back(SensorFactory::createSensor(0)); // Create Temperature Sensor
    sensors.push_back(SensorFactory::createSensor(1)); // Create Humidity Sensor

    for (Sensor* sensor : sensors) {
        sensor->initialize();
    }

    // Simulate 10 sensor readings
    for (int i = 0; i < 10; i++) {
        std::cout << "\nCycle " << i + 1 << ":\n";
        for (Sensor* sensor : sensors) {
            sensor->readSensor();
            sensor->processData();
        }
    }

    // Cleanup
    for (Sensor* sensor : sensors) {
        delete sensor;
    }

    return 0;
}
