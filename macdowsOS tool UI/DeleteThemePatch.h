#pragma once
#include <cstdlib>
#include<windows.h>
#include"WindowControl.h"
#include"LogSystem.h"
#include"All.h"

namespace {
    void theme_path_uninstall() {
        INFO_(L"[主题补丁工具]开始");
        HWND temp, temp1;

        std::wstring path = L"C:\\Program Files (x86)\\UltraUXThemePatcher\\Uninstall.exe";
        INFO_(L"[主题补丁卸载工具]检查文件是否存在");
        if (!fs::exists(path)) {
            ERROR_(L"[主题补丁卸载工具]未找到OldNewExplorer主程序 OldNewExplorer卸载进程退出");
            return;
        }
        //卸载
        INFO_(L"[主题补丁卸载工具]开始卸载程序");
        STARTUPINFOW startupInfo{ sizeof(startupInfo) };
        PROCESS_INFORMATION processInfo{};
        if (!CreateProcessW(path.c_str(), nullptr, nullptr, nullptr, FALSE,
            0, nullptr, nullptr, &startupInfo, &processInfo)) {
            ERROR_(L"[主题补丁卸载工具]启动卸载程序失败，错误码: " +
                std::to_wstring(GetLastError()));
            return;
        }
        CloseHandle(processInfo.hThread);
        CloseHandle(processInfo.hProcess);
        //等待应用程序&&查找-卸载
        INFO_(L"[主题补丁卸载工具]等待应用程序");
        HWND hWnd_Window = FindWindow_(L"UltraUXThemePatcher Uninstall", L"主题补丁卸载工具");

        INFO_(L"[主题补丁卸载工具]已找到窗口");
        //前置
        SetForegroundWindow(hWnd_Window);
        SetWindowPos(hWnd_Window, NULL, 0, 0, 0, 0, SWP_NOSIZE);

        ClickButtonMode2(hWnd_Window, L"主题补丁卸载工具", L"&Uninstall");

        
        INFO_(L"[主题补丁卸载工具]继续-查找");
        //继续-查找
        temp = FindChildControlExact(hWnd_Window, L"Uninstallation Complete");
        temp1 = FindChildControlExact(hWnd_Window, L"&Next >");
        while (temp == NULL || temp1==NULL) {
            temp = FindChildControlExact(hWnd_Window, L"Uninstallation Complete");
            temp1 = FindChildControlExact(hWnd_Window, L"&Next >");
        }
        /*INFO_(L"[主题补丁卸载工具]查找--");
        //前置
        SetForegroundWindow(hWnd_Window);
        INFO_(L"[主题补丁卸载工具]查找---");
        //选中
        SendMessageW(temp1, BM_CLICK, 0, 0);
        INFO_(L"[主题补丁卸载工具]卸载已完成");
        */
        killapp(L"Un.exe");
        
        INFO_(L"[主题补丁卸载工具]退出");
        return;
    }

}
