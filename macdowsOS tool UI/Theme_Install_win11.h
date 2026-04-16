#pragma once
#include<bits/stdc++.h>
#include"WindowControl.h"
#include <tlhelp32.h> 
#include <tchar.h>  
#include <shellapi.h> 
#include"LogSystem.h"
#include"FilesSystem.h"
using namespace std;
// 静默设置主题文件在重启后打开一次
bool SetThemeToOpenAfterReboot_win11(const std::wstring& themeFilePath) {
    HKEY hKey = nullptr;
    bool success = false;

    // 1. 构建打开主题文件的命令
    // 使用"start"命令确保通过默认关联程序打开
    std::wstring command = L"cmd /c start \"\" \"" + themeFilePath + L"\"";

    // 2. 打开当前用户的RunOnce注册表键
    // 使用 KEY_WOW64_64KEY 确保在64位系统上访问正确的注册表位置
    LONG result = RegOpenKeyExW(
        HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce",
        0,
        KEY_WRITE | KEY_WOW64_64KEY,
        &hKey
    );

    if (result == ERROR_SUCCESS && hKey != nullptr) {
        // 3. 创建一个唯一的值名称（使用固定名称）
        const wchar_t* valueName = L"ApplyThemeOnStartup";

        // 4. 将命令写入注册表
        result = RegSetValueExW(
            hKey,
            valueName,
            0,
            REG_SZ,
            reinterpret_cast<const BYTE*>(command.c_str()),
            static_cast<DWORD>((command.length() + 1) * sizeof(wchar_t))
        );

        if (result == ERROR_SUCCESS) {
            success = true;
        }

        RegCloseKey(hKey);
    }

    return success;
}
void theme_install_win11() {
    INFO_(L"[主题安装组件]运行组件");
    INFO_(L"[主题安装组件]复制文件");
    //复制
    wstring res = L"AppData/Theme/WIndows 11 Themes/Tahoe";
    wstring res1 = L"AppData/Theme/WIndows 11 Themes/Tahoe - Light.theme";
    wstring res2 = L"AppData/Theme/WIndows 11 Themes/Tahoe - Night.theme";
    wstring topath = L"C:\\Windows\\Resources\\Themes";
    copyPath(res, topath);
    copyPath(res1, topath);
    copyPath(res2, topath);
    //应用主题
    INFO_(L"[主题安装组件]应用主题");
    HINSTANCE result = ShellExecuteW(NULL,L"open",L"C:\\Windows\\Resources\\Themes\\Tahoe - Light.theme",NULL,NULL,SW_SHOWNORMAL);
    // 检查执行结果
    if ((DWORD)result <= 32) {
        ERROR_(L"[主题安装组件]无法应用主题");
        MessageBox(NULL, (LPCTSTR)L"无法应用主题 请检查主题应用操作是否被杀毒软件拦截或检查当前系统兼容性和完整性", (LPCTSTR)L" macdowsOS tool 主题安装组件", MB_OK | MB_ICONERROR);
        HINSTANCE result1 = ShellExecuteW(NULL,L"open",L"ms-settings:themes",NULL,NULL,SW_SHOWNORMAL);
        if ((INT_PTR)result1 <= 32) {
            ERROR_(L"[主题安装组件]无法打开系统设置");
            MessageBox(NULL, (LPCTSTR)L"无法打开系统设置 请检查操作是否被杀毒软件拦截或检查系统完整性", (LPCTSTR)L" macdowsOS tool 主题安装组件", MB_OK | MB_ICONERROR);
            MessageBox(NULL, (LPCTSTR)L"请手动打开设置应用主题", (LPCTSTR)L" macdowsOS tool 主题安装组件", MB_OK);
        }
        else {
            Sleep(500);
            INFO_(L"[主题安装组件]打开系统设置");
            MessageBox(NULL, (LPCTSTR)L"请尝试在设置中手动应用主题", (LPCTSTR)L" macdowsOS tool 主题安装组件", MB_OK);
        }
    }
    INFO_(L"[主题安装组件]设置重启后打开");
    SetThemeToOpenAfterReboot_win11(L"C:\\Windows\\Resources\\Themes\\Tahoe - Light.theme");
    INFO_(L"[主题安装组件]退出");
    return;
}

