# triio_assignment

This repository contains task for mini assignment, described below.

**Mock Assignment**:
Simulated Medical Device Sensor System

**Context**:
Imagine you are developing software for a medical type sequencing machine that collects real-time sensor data from a temperature sensor and motor speed controller. The data is processed in the middleware layer and displayed in the application layer.

**Task**:
1. Simulate a Temperature Sensor
○ Create a class TemperatureSensor that generates a random temperature (35-45°C).
○ It should update every 200ms in a separate thread.

3. Simulate a Motor Speed Controller
○ Create a class MotorController that sets and reads a motor’s RPM.
○ It should have a setSpeed() method and update every 500ms.

5. Middleware: Data Exchange
○ Implement a thread-safe message queue (or shared memory buffer) for sensor data.
○ The main process should collect data from both sensors asynchronously.

6. Application Layer: Display Data
Create a simple CLI output that prints:
[Time: 12:30:15] Temperature: 38.7°C | Motor Speed: 1200 RPM
○ Display updates every 1 second.

🚀 Bonus (if time permits, but not required)
● Use C++20 coroutines instead of traditional threads (wherever possible).
● Add basic error handling (e.g., sensor failures)

**Pre-requisite library installation (for YAML file read)**:
sudo apt install libyaml-cpp-dev

**Compilation steps**:
g++ -std=c++20 -Wall -Wextra -o sensor_system sensor_system.cpp -lyaml-cpp

**How to run**:
./sensor_system

**How to change RPM**:
Modify RPM field in config.yaml file
