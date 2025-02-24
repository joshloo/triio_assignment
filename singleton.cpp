class LCDHandler {
    public:
        static LCDHandler& getInstance() { // Single Instance
            static LCDHandler instance;
            return instance;
        }
    
        void printLine(uint8_t row, const char* message) {
            printf("LCD[%d]: %s\n", row, message);
        }
    
    private:
        LCDHandler() {}  // Private constructor
        LCDHandler(const LCDHandler&) = delete;
        void operator=(const LCDHandler&) = delete;
    };
    
    // Usage:
    LCDHandler::getInstance().printLine(0, "Hello, Singleton!");
    