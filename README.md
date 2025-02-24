# triio_assignment

This repository demonstrates common design patterns such as singleton, factory, abstract class, static variable

Pattern	Usage in Code
Singleton           :   LCDHandler ensures a single LCD instance.
Factory             :	SensorFactory creates Temperature & Humidity sensors dynamically.
Abstract Class      :   Sensor defines a base class for all sensors.
Static Variable     :   TemperatureSensor::instanceCount tracks instances.
Observer Pattern    :   LCDDisplay & Logger receive sensor updates automatically
Command Pattern     :   Handles user commands like "SET TEMP" & "LED ON"
State Pattern       :   Sensor switches between Idle, Active, and Error states
Strategy Pattern    :   Supports Simple & Advanced processing for sensor data

**Step to compile**:
g++ -o sensor_system sensor_system.cpp