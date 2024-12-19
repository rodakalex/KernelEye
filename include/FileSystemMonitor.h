#ifndef FILESYSTEMMONITOR_H
#define FILESYSTEMMONITOR_H

#include "Logger.h"
#include <vector>
#include <string>


class FileSystemMonitor {
public:
    struct FileData {
        std::string filePath;
        std::string hash;
    };

    FileSystemMonitor(Logger& logger);

    std::vector<FileData> takeSnapshot(const std::string& directory);
    void compareSnapshots(const std::vector<FileData>& before, const std::vector<FileData>& after);


private:
    Logger& logger;

    std::string calculateHash(const std::string& filePath);
};

#endif // FILESYSTEMMONITOR_H
