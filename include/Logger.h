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
    std::string getCurrentTime();

private:
    std::ofstream logFile;
    std::mutex logMutex;
};

#endif // LOGGER_H
