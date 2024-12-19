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


std::tm Logger::getLocalTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    return tm;
}

std::string Logger::getFormattedTime(const std::string& format) {
    std::tm tm = getLocalTime();
    std::ostringstream oss;
    oss << std::put_time(&tm, format.c_str());
    return oss.str();
}


std::string Logger::getCurrentTimeForLog() {
    return getFormattedTime("[%Y-%m-%d %H:%M:%S]");
}


std::string Logger::getCurrentTimeForFileName() {
    return getFormattedTime("%Y-%m-%d %H-%M-%S");
}


std::string Logger::generateDailyLogFilePath(const std::string& directory) {
    std::string currentTimeForFileName = Logger::getCurrentTimeForFileName();
    std::ostringstream oss;
    oss << directory << "/processes_" << currentTimeForFileName << ".log";
    return oss.str();
}


void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    logFile << getCurrentTimeForLog() << " " << message << std::endl;
}
