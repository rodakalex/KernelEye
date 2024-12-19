#include "ProcessMonitor.h"
#include "FileSystemMonitor.h"
#include "Logger.h"
#include <iostream>
#include <stdexcept>


void printUsage() {
    std::cout << "Usage: KernelEye [--process] [--filesystem] [--dir <directory>]\n"
              << "Options:\n"
              << "  --process       Monitor system processes and their DLLs.\n"
              << "  --filesystem    Compare filesystem state before and after updates.\n"
              << "  --dir <path>    Specify the directory to monitor (for filesystem analysis).\n";
}


int main(int argc, char* argv[]) {
    try {
        std::string dailyLogPath = Logger::generateDailyLogFilePath("logs");
        Logger logger(dailyLogPath);
        logger.log("KernelEye started...");

        bool enableProcessMonitor = false;
        bool enableFileSystemMonitor = false;
        std::string directoryToMonitor = "C:\\Windows\\System32";

        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "--process") {
                enableProcessMonitor = true;
            } else if (arg == "--filesystem") {
                enableFileSystemMonitor = true;
            } else if (arg == "--dir" && i + 1 < argc) {
                directoryToMonitor = argv[++i];
            } else {
                printUsage();
                return 0;
            }
        }

        if (enableProcessMonitor) {
            logger.log("Starting ProcessMonitor...");
            ProcessMonitor processMonitor(logger);
            processMonitor.start();
        }

        if (enableFileSystemMonitor) {
            logger.log("Starting FileSystemMonitor...");
            FileSystemMonitor fsMonitor(logger);

            logger.log("Taking filesystem snapshot (before)...");
            auto beforeSnapshot = fsMonitor.takeSnapshot(directoryToMonitor);

            std::cout << "Simulate update installation. Press Enter to continue...";
            std::cin.get();

            auto afterSnapshot = fsMonitor.takeSnapshot(directoryToMonitor);

            logger.log("Comparing snapshots...");
            fsMonitor.compareSnapshots(beforeSnapshot, afterSnapshot);
        }

        logger.log("KernelEye finished.");

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
