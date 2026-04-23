#pragma once
#include<bits/stdc++.h>
#include<Windows.h>
#include <fstream>
#include <mutex>
#include <unordered_set>
#include <filesystem>
#include <string>
#include <sstream>
//SFML
#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>
using namespace std;


namespace fs = std::filesystem;

void buttonchangemain() {
    MessageBox(NULL, (LPCTSTR)L" 本程序仅供测试 可能不稳定", (LPCTSTR)L" 雾蓝回针MistBlueSt", MB_OK);
    std::mutex fileMutex;
    const fs::path TEMP_DIR = L"temp";
    const fs::path CONFIG_DIR = L"config";
    const fs::path LINK_FILE = TEMP_DIR / L"link.tmp";
    const fs::path LINK1_FILE = TEMP_DIR / L"link1.tmp";
    const fs::path NO_LIST_FILE = CONFIG_DIR / L"ButtonChangerNo.txt";

    // 确保目录存在
    fs::create_directories(TEMP_DIR);
    fs::create_directories(CONFIG_DIR);

    // 加载禁止列表（一次性读取，无需热重载）
    std::unordered_set<uintptr_t> noHwndSet;
    {
        std::wifstream noFile(NO_LIST_FILE);
        if (noFile.is_open()) {
            std::wstring line;
            while (std::getline(noFile, line)) {
                if (line.empty()) continue;
                std::wistringstream iss(line);
                uintptr_t hwndVal;
                if (iss >> std::hex >> hwndVal) noHwndSet.insert(hwndVal);
            }
        }
    }

    HWND hWnd = GetWindow(GetDesktopWindow(), GW_CHILD);
    while (hWnd != nullptr) {
        if (IsWindowVisible(hWnd)) {
            const DWORD exStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
            if (!(exStyle & WS_EX_TOOLWINDOW) && GetWindow(hWnd, GW_OWNER) == nullptr) {
                const uintptr_t hwndVal = reinterpret_cast<uintptr_t>(hWnd);
                // 检查禁止列表
                if (noHwndSet.find(hwndVal) == noHwndSet.end()) {
                    // 1. 写入 temp/link.tmp
                    {
                        std::lock_guard<std::mutex> lock(fileMutex);
                        std::wofstream file(LINK_FILE);
                        if (!file.is_open() || !IsWindow(hWnd)) {
                            hWnd = GetNextWindow(hWnd, GW_HWNDNEXT);
                            continue;
                        }
                        file << std::hex << hwndVal << std::endl;
                    }

                    // 2. 启动程序
                    system("start ButtonChanger.exe");

                    // 3. 等待 temp/link1.tmp 变为 done 并清空
                    while (true) {
                        {
                            std::lock_guard<std::mutex> lock(fileMutex);
                            std::wifstream file(LINK1_FILE);
                            if (file.is_open()) {
                                std::wstring content;
                                std::getline(file, content);
                                if (content == L"done") {
                                    file.close();
                                    // 清空文件
                                    std::wofstream outFile(LINK1_FILE, std::ios::trunc);
                                    break;
                                }
                            }
                        }
                        Sleep(100); // 避免忙等待
                    }
                }
            }
        }
        hWnd = GetNextWindow(hWnd, GW_HWNDNEXT);
    }
    Sleep(1100000000000);
}