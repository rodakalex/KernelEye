#include "ProcessMonitor.h"
#include "Logger.h"
#include <iostream>
#include <stdexcept>

int main() {
    try {
        std::string dailyLogPath = Logger::generateDailyLogFilePath("logs");
        Logger logger(dailyLogPath);
        logger.log("KernelEye started...");

        ProcessMonitor monitor(logger);
        monitor.start();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
