#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <mutex>
#include <fstream>

class Logger {
public:
    ~Logger();
    explicit Logger(const std::string& filePath);
    void log(const std::string& message);
    static std::string generateDailyLogFilePath(const std::string& directory);
    std::string getCurrentTimeForLog();
    static std::string getCurrentTimeForFileName();

private:
    std::ofstream logFile;
    std::mutex logMutex;
    static std::tm getLocalTime();
    static std::string getFormattedTime(const std::string &format);
};

#endif // LOGGER_H
