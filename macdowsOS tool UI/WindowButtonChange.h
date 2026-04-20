#pragma once
#include<bits/stdc++.h>
#include<Windows.h>
#include <fstream>
#include <mutex>
#include <unordered_set>
//SFML
#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>
using namespace std;

void buttonchangemain() {

    // 获取桌面窗口的第一个子窗口（即第一个顶层窗口）
    HWND hWnd = GetWindow(GetDesktopWindow(), GW_CHILD);

    while (hWnd != nullptr) {
        // 1. 检查窗口是否可见
        if (IsWindowVisible(hWnd)) {
            // 2. 获取扩展样式，排除工具窗口
            const DWORD exStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
            if (!(exStyle & WS_EX_TOOLWINDOW)) {
                // 3. 排除具有所有者窗口的弹窗
                if (GetWindow(hWnd, GW_OWNER) == nullptr) {
                    // -- - 核心修改：为这个窗口单独启动一个线程-- -
                        // 删文件
                    
                    //hwnd写入文件
                    std::mutex g_fileMutex;
                    const wchar_t* SAVE_FILE = L"temp.tmp";
                    std::lock_guard<std::mutex> lock(g_fileMutex);
                    std::wofstream file(SAVE_FILE);
                    if (!file.is_open() || !IsWindow(hWnd)) return;

                    file << std::hex << reinterpret_cast<uintptr_t>(hWnd) << std::endl;
                    file.close();
                    //run
                    system("start ButtonChanger.exe");
                    while (fs::exists("temp.tmp")) {
                        ;
                    }
                    Sleep(1000);
                }
            }
        }
        // 获取下一个窗口
        hWnd = GetNextWindow(hWnd, GW_HWNDNEXT);
    }
    Sleep(1100000000000);
}