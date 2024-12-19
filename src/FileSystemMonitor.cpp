#include "FileSystemMonitor.h"
#include <windows.h>
#include <wincrypt.h>
#include <fstream>
#include <algorithm>
#include <iostream>

FileSystemMonitor::FileSystemMonitor(Logger& logger) : logger(logger) {}

std::vector<FileSystemMonitor::FileData> FileSystemMonitor::takeSnapshot(const std::string& directory) {
    std::vector<FileData> snapshot;

    // Конвертируем std::string в std::wstring
    std::wstring wideDirectory = std::wstring(directory.begin(), directory.end()) + L"\\*";

    WIN32_FIND_DATAW findFileData;
    HANDLE hFind = FindFirstFileW(wideDirectory.c_str(), &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        logger.log("Error: Unable to access directory " + directory);
        return snapshot;
    }

    do {
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            continue; // Пропускаем каталоги
        }

        std::wstring wideFileName = findFileData.cFileName;
        std::wstring wideFilePath = std::wstring(directory.begin(), directory.end()) + L"\\" + wideFileName;
        std::string filePath = std::string(wideFilePath.begin(), wideFilePath.end());
        std::string fileHash = calculateHash(filePath);
        snapshot.push_back({filePath, fileHash});
        logger.log("File: " + filePath + " | Hash: " + fileHash);
    } while (FindNextFileW(hFind, &findFileData) != 0);

    FindClose(hFind);
    return snapshot;
}

void FileSystemMonitor::compareSnapshots(const std::vector<FileData>& before, const std::vector<FileData>& after) {
    logger.log("Comparing filesystem snapshots...");

    for (const auto& fileAfter : after) {
        auto it = std::find_if(before.begin(), before.end(), [&fileAfter](const FileData& fileBefore) {
            return fileBefore.filePath == fileAfter.filePath;
        });

        if (it == before.end()) {
            logger.log("Added: " + fileAfter.filePath);
        } else if (it->hash != fileAfter.hash) {
            logger.log("Modified: " + fileAfter.filePath);
        }
    }

    for (const auto& fileBefore : before) {
        auto it = std::find_if(after.begin(), after.end(), [&fileBefore](const FileData& fileAfter) {
            return fileAfter.filePath == fileBefore.filePath;
        });

        if (it == after.end()) {
            logger.log("Deleted: " + fileBefore.filePath);
        }
    }
}

std::string FileSystemMonitor::calculateHash(const std::string& filePath) {
    // Конвертируем std::string в std::wstring для использования CreateFileW
    std::wstring wideFilePath = std::wstring(filePath.begin(), filePath.end());

    HANDLE file = CreateFileW(wideFilePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        return "Error";
    }

    HCRYPTPROV hProv;
    HCRYPTHASH hHash;
    BYTE buffer[4096];
    DWORD bytesRead;
    BYTE hash[32];
    DWORD hashSize = sizeof(hash);
    std::string hashString;

    if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT) &&
        CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
        while (ReadFile(file, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0) {
            CryptHashData(hHash, buffer, bytesRead, 0);
        }
        CryptGetHashParam(hHash, HP_HASHVAL, hash, &hashSize, 0);

        for (DWORD i = 0; i < hashSize; i++) {
            char hex[3];
            sprintf_s(hex, "%02x", hash[i]);
            hashString.append(hex);
        }

        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
    }

    CloseHandle(file);
    return hashString;
}
