#include "ProcessMonitor.h"
#include <windows.h>
#include <TlHelp32.h>
#include <unordered_set>
#include <psapi.h>


ProcessMonitor::ProcessMonitor(Logger& logger) : logger(logger) {}


void ProcessMonitor::start() {
    logger.log("Starting process monitor...");
    monitorProcesses();
}


void ProcessMonitor::stop() {
    logger.log("Stopping process monitor...");
    running = false;
}


void ProcessMonitor::logLoadedModules(DWORD processId) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
    if (!hProcess) {
        logger.log("Error: Cannot open process for PID=" + std::to_string(processId));
        return;
    }

    HMODULE hMods[1024];
    DWORD cbNeeded;

    if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
        logger.log("Modules loaded for PID=" + std::to_string(processId) + ":");
        for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
            char szModName[MAX_PATH];

            if (GetModuleFileNameExA(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(char))) {
                logger.log("  " + std::string(szModName));
            }
        }
    } else {
        logger.log("Error: Cannot enumerate modules for PID=" + std::to_string(processId));
    }

    CloseHandle(hProcess);
}


void ProcessMonitor::monitorProcesses() {
    logger.log("Starting process monitoring...");
    std::unordered_set<DWORD> knownProcesses;

    while (running) {
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE) {
            logger.log("Error: Cannot create process snapshot.");
            continue;
        }

        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(hSnapshot, &pe32)) {
            do {
                if (knownProcesses.find(pe32.th32ProcessID) == knownProcesses.end()) {
                    knownProcesses.insert(pe32.th32ProcessID);

                    std::string processName = getProcessName(pe32.th32ProcessID);
                    std::string parentProcessName = getProcessName(pe32.th32ParentProcessID);

                    logger.log("New process detected: PID=" + std::to_string(pe32.th32ProcessID) +
                               ", Name=" + processName +
                               ", ParentPID=" + std::to_string(pe32.th32ParentProcessID) +
                               ", ParentName=" + parentProcessName);
                    
                    logLoadedModules(pe32.th32ProcessID);
                }                
            } while (Process32Next(hSnapshot, &pe32));
        }

        CloseHandle(hSnapshot);
        Sleep(1000);
    }
}



std::string ProcessMonitor::getProcessName(DWORD processId) {
    if (processId == 0) return "System Idle Process";
    if (processId == 4) return "System Process";

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, processId);
    if (hProcess) {
        char processName[MAX_PATH];
        DWORD size = MAX_PATH;
        if (QueryFullProcessImageNameA(hProcess, 0, processName, &size)) {
            CloseHandle(hProcess);
            return std::string(processName);
        }
        CloseHandle(hProcess);
    }
    return "Unknown";
}


DWORD ProcessMonitor::getParentProcessId(DWORD processId) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe32 = { sizeof(PROCESSENTRY32) };

    if (Process32First(hSnapshot, &pe32)) {
        do {
            if (pe32.th32ProcessID == processId) {
                CloseHandle(hSnapshot);
                return pe32.th32ParentProcessID;
            }
        } while (Process32Next(hSnapshot, &pe32));
    }
    CloseHandle(hSnapshot);
    return 0;
}
