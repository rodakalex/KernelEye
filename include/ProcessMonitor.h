#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H

#include "Logger.h"
#include <string>
#include <windows.h>
#include <atomic>


class ProcessMonitor {
public:
    explicit ProcessMonitor(Logger& logger);
    void start();
    void stop();

private:
    Logger& logger;
    std::atomic<bool> running;
    void monitorProcesses();
    std::string getProcessName(DWORD processId);
    DWORD getParentProcessId(DWORD processId);
    void logLoadedModules(DWORD processId);
};

#endif // PROCESSMONITOR_H
