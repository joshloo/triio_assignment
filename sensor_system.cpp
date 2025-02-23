#include <iostream>
#include <coroutine>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <chrono>
#include <random>
#include <optional>
#include <yaml-cpp/yaml.h>
#include <fstream>

// Thread-safe message queue template
template <typename T>
class MessageQueue {
private:
    std::queue<T> queue;
    std::mutex mtx;
    std::condition_variable cv;
public:
    void push(T value) {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(value);
        cv.notify_one();
    }

    std::optional<T> pop() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !queue.empty(); });
        if (queue.empty()) return std::nullopt;
        T value = queue.front();
        queue.pop();
        return value;
    }
};

// Coroutine-based Temperature Sensor
class TemperatureSensor {
public:
    struct Task {
        struct promise_type {
            Task get_return_object() { return {}; }
            std::suspend_never initial_suspend() { return {}; }
            std::suspend_never final_suspend() noexcept { return {}; }
            void return_void() {}
            void unhandled_exception() { std::terminate(); }
        };
    };

    Task operator()(MessageQueue<float>& queue) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(35.0, 45.0);
        while (true) {
            try {
                float temp = dist(gen);
                if (temp < 35.0 || temp > 45.0) throw std::runtime_error("Temperature out of range");
                queue.push(temp);
            } catch (const std::exception& e) {
                std::cerr << "Temperature Sensor Error: " << e.what() << '\n';
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
};

// Coroutine-based Motor Controller
class MotorController {
private:
    int speed;
    std::mutex speed_mtx;
public:
    MotorController() : speed(1000) {} // Add default if YAML read fails.
    void setSpeed(int newSpeed) {
        std::lock_guard<std::mutex> lock(speed_mtx);
        speed = newSpeed;
    }
    int getSpeed() {
        std::lock_guard<std::mutex> lock(speed_mtx);
        return speed;
    }

    void loadSpeedFromYAML(const std::string& filename) {
        try {
            YAML::Node config = YAML::LoadFile(filename);
            if (config["motor"] && config["motor"]["rpm"]) {
                int rpm = config["motor"]["rpm"].as<int>();
                if (rpm < 0) throw std::runtime_error("Invalid RPM in YAML");
                setSpeed(rpm);
                std::cout << "Loaded RPM from YAML: " << rpm << " RPM\n";
            } else {
                throw std::runtime_error("Missing 'motor.rpm' in YAML");
            }
        } catch (const std::exception& e) {
            std::cerr << "Error loading YAML: " << e.what() << "\n";
        }
    }
    struct Task {
        struct promise_type {
            Task get_return_object() { return {}; }
            std::suspend_never initial_suspend() { return {}; }
            std::suspend_never final_suspend() noexcept { return {}; }
            void return_void() {}
            void unhandled_exception() { std::terminate(); }
        };
    };

    Task operator()(MessageQueue<int>& queue) {
        while (true) {
            try {
                int currentSpeed = getSpeed();
                if (currentSpeed < 0) throw std::runtime_error("Invalid motor speed");
                queue.push(currentSpeed);
            } catch (const std::exception& e) {
                std::cerr << "Motor Controller Error: " << e.what() << '\n';
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
};

// Display function
void displayData(MessageQueue<float>& tempQueue, MessageQueue<int>& speedQueue) {
    while (true) {
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm* now_tm = std::localtime(&now);
        char timeBuffer[9];
        strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", now_tm);

        auto temp = tempQueue.pop();
        auto speed = speedQueue.pop();
        
        if (temp && speed) {
            std::cout << "[Time: " << timeBuffer << "] Temperature: " << *temp << "°C | Motor Speed: " << *speed << " RPM\n";
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    MessageQueue<float> tempQueue;
    MessageQueue<int> speedQueue;
    
    TemperatureSensor tempSensor;
    MotorController motorController;
    
    // Load RPM from YAML
    motorController.loadSpeedFromYAML("config.yaml");
    
    std::thread tempThread(&TemperatureSensor::operator(), &tempSensor, std::ref(tempQueue));
    std::thread motorThread(&MotorController::operator(), &motorController, std::ref(speedQueue));
    std::thread displayThread(displayData, std::ref(tempQueue), std::ref(speedQueue));
    
    tempThread.join();
    motorThread.join();
    displayThread.join();
    
    return 0;
}
