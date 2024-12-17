#include "Logger.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <filesystem>


Logger::Logger(const std::string& filePath) {
    std::filesystem::create_directories(std::filesystem::path(filePath).parent_path());
    logFile.open(filePath, std::ios::app);
    if (!logFile.is_open()) {
        throw std::runtime_error("Failed to open log file: " + filePath);
    }
}


Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}


void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    logFile << getCurrentTime() << " " << message << std::endl;

}

std::string Logger::getCurrentTime() {
    std::time_t now = std::time(nullptr);
    std::tm localTime;
    localtime_s(&localTime, &now);

    std::ostringstream oss;
    oss << std::put_time(&localTime, "[%Y-%m-%d %H:%M:%S]");
    return oss.str();
}
