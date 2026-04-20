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
    //坐标
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    HWND hwnd= { GetTopWindow(NULL) };

    // 开始循环遍历
    while (hwnd != NULL) {

        // 1. 只看可见窗口
        if (IsWindowVisible(hwnd)) {
            // 2. 检查类名，排除右键菜单 (#32768)
            WCHAR class_name[256];
            GetClassName(hwnd, class_name, 256);
            if (wcscmp(class_name, L"#32768") != 0) {
                // 3. 检查扩展样式，排除工具小窗
                LONG ex_style = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
                if (!(ex_style & WS_EX_TOOLWINDOW)) {
                    // --- 核心修改：为这个窗口单独启动一个线程 ---
                    // 删文件
                    DeleteFileW(L"temp.tmp");
                    //hwnd写入文件
                    std::mutex g_fileMutex;
                    const wchar_t* SAVE_FILE = L"temp.tmp";
                    std::lock_guard<std::mutex> lock(g_fileMutex);
                    std::wofstream file(SAVE_FILE, std::ios::app); // 追加模式，不覆盖原有内容
                    if (!file.is_open() || !IsWindow(hwnd)) return;

                    file << std::hex << reinterpret_cast<uintptr_t>(hwnd) << std::endl;
                    file.close();
                    //run
                    system("start ButtonChanger.exe");
                }
            }
        }

        // --- 过滤逻辑结束 ---

        // 获取下一个窗口，继续循环
        hwnd = GetNextWindow(hwnd, GW_HWNDNEXT);
        
    }
    Sleep(1100000000000);
}