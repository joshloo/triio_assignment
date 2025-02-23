#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <chrono>
#include <random>

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

    T pop() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !queue.empty(); });
        T value = queue.front();
        queue.pop();
        return value;
    }
};

// Simulated Temperature Sensor
class TemperatureSensor {
public:
    void operator()(MessageQueue<float>& queue) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(35.0, 45.0);
        while (true) {
            float temp = dist(gen);
            queue.push(temp);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
};

// Simulated Motor Controller
class MotorController {
private:
    int speed;
    std::mutex speed_mtx;
public:
    MotorController() : speed(1000) {}
    void setSpeed(int newSpeed) {
        std::lock_guard<std::mutex> lock(speed_mtx);
        speed = newSpeed;
    }
    int getSpeed() {
        std::lock_guard<std::mutex> lock(speed_mtx);
        return speed;
    }
    void operator()(MessageQueue<int>& queue) {
        while (true) {
            queue.push(getSpeed());
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

        float temp = tempQueue.pop();
        int speed = speedQueue.pop();
        
        std::cout << "[Time: " << timeBuffer << "] Temperature: " << temp << "°C | Motor Speed: " << speed << " RPM\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    MessageQueue<float> tempQueue;
    MessageQueue<int> speedQueue;
    
    TemperatureSensor tempSensor;
    MotorController motorController;
    
    std::thread tempThread(&TemperatureSensor::operator(), &tempSensor, std::ref(tempQueue));
    std::thread motorThread(&MotorController::operator(), &motorController, std::ref(speedQueue));
    std::thread displayThread(displayData, std::ref(tempQueue), std::ref(speedQueue));
    
    tempThread.join();
    motorThread.join();
    displayThread.join();
    
    return 0;
}
