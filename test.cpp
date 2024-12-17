#include <windows.h>
#include <iostream>

int main() {
    DWORD pid = GetCurrentProcessId();
    std::cout << "Current Process ID: " << pid << std::endl;
    return 0;
}
