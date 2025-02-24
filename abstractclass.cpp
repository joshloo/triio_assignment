class Sensor {
    public:
        virtual void initialize() = 0;  // Must be implemented by derived classes
        virtual void readSensor() = 0;
        virtual void processData() = 0;
    };
    
    // Inheritance
    class TemperatureSensor : public Sensor {
    public:
        void initialize() override { printf("Temperature Sensor Initialized\n"); }
        void readSensor() override { printf("Reading Temperature\n"); }
        void processData() override { printf("Processing Temperature Data\n"); }
    };
    
    // Usage:
    Sensor* tempSensor = new TemperatureSensor();
    tempSensor->initialize();
    tempSensor->readSensor();
    tempSensor->processData();
    