cmake_minimum_required(VERSION 3.10)

project(KernelEye)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

set(SOURCES
    src/main.cpp
    src/ProcessMonitor.cpp
    src/Logger.cpp
    src/SystemUtils.cpp
    src/FileSystemMonitor.cpp
)

add_executable(KernelEye ${SOURCES})


if(WIN32)
    target_link_libraries(KernelEye PRIVATE Psapi)
endif()
