#include "ProcessMonitor.h"
#include "Logger.h"
#include <iostream>


int main() {
    try {
        Logger logger("logs/processes.log");
        logger.log("KernelEye started...");

        ProcessMonitor monitor(logger);
        monitor.start();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
