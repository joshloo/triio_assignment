class TemperatureSensor {
    public:
        static uint8_t instanceCount;
    
        TemperatureSensor() { instanceCount++; }
    
        static uint8_t getInstanceCount() { return instanceCount; }
    };
    
    uint8_t TemperatureSensor::instanceCount = 0; // Define outside class
    
    // Usage:
    TemperatureSensor s1, s2, s3;
    printf("Total Sensors: %d\n", TemperatureSensor::getInstanceCount()); 
    // Output: Total Sensors: 3
    