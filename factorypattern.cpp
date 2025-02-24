class Sensor {
    public:
        virtual void initialize() = 0;
    };
    
    class TemperatureSensor : public Sensor {
    public:
        void initialize() override { printf("Temperature Sensor Initialized\n"); }
    };
    
    class HumiditySensor : public Sensor {
    public:
        void initialize() override { printf("Humidity Sensor Initialized\n"); }
    };
    
    // Factory Class
    class SensorFactory {
    public:
        static Sensor* createSensor(int type) {
            if (type == 0) return new TemperatureSensor();
            if (type == 1) return new HumiditySensor();
            return nullptr;
        }
    };
    
    // Usage:
    Sensor* temp = SensorFactory::createSensor(0);
    temp->initialize(); // Output: Temperature Sensor Initialized
    