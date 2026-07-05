#pragma once
#include <string>
#include"WindowControl.h"
#include <tlhelp32.h> 
#include <tchar.h>  
#include <shellapi.h> 
#include <objbase.h>
#include"LogSystem.h"
#include"FilesSystem.h"

// ====== 启用 Windows 主题服务（Themes / UxSms）======
// 先将服务设置为自动启动，再尝试启动（已运行则 sc start 会自动跳过）
// 程序已获得管理员权限，无需提权
inline void EnableThemeService_win10() {
    INFO_(L"[主题安装工具]设置主题服务为自动启动");
    system("sc config Themes start= auto");
    INFO_(L"[主题安装工具]尝试启动主题服务（如已运行则自动跳过）");
    system("sc start Themes");
    INFO_(L"[主题安装工具]主题服务启用完成");
}
// 将主题设置为开机后自动应用
// 通过 RunOnce + CurrentTheme 双重机制确保主题在重启后正确加载
bool SetThemeToOpenAfterReboot_win10(const std::wstring& themeFilePath) {
    bool success = true;
    HKEY hKey = nullptr;

    // ====== 1. 设置 RunOnce：开机后直接打开 .theme 文件触发主题引擎加载 ======
    // 注意：直接使用文件路径（加引号），不使用 cmd /c start
    // RunOnce 会通过文件关联自动调用 Personalization 控制面板应用主题
    std::wstring runOnceCmd = L"\"" + themeFilePath + L"\"";
    LONG result = RegCreateKeyExW(
        HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce",
        0, NULL, REG_OPTION_NON_VOLATILE,
        KEY_WRITE | KEY_WOW64_64KEY,
        NULL, &hKey, NULL
    );

    if (result == ERROR_SUCCESS && hKey != nullptr) {
        result = RegSetValueExW(
            hKey,
            L"ApplyThemeOnStartup",
            0,
            REG_SZ,
            reinterpret_cast<const BYTE*>(runOnceCmd.c_str()),
            static_cast<DWORD>((runOnceCmd.length() + 1) * sizeof(wchar_t))
        );

        if (result != ERROR_SUCCESS) {
            success = false;
            MESSAGE_(L"[主题安装工具]RunOnce注册失败，错误码:", result);
        }
        RegCloseKey(hKey);
    }
    else {
        success = false;
        MESSAGE_(L"[主题安装工具]无法打开 RunOnce 注册表，错误码:", result);
    }

    // ====== 2. 设置 CurrentTheme：让 Windows 登录时自动识别当前主题 ======
    HKEY hKeyTheme = nullptr;
    result = RegOpenKeyExW(
        HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes",
        0,
        KEY_WRITE | KEY_WOW64_64KEY,
        &hKeyTheme
    );

    if (result == ERROR_SUCCESS && hKeyTheme != nullptr) {
        result = RegSetValueExW(
            hKeyTheme,
            L"CurrentTheme",
            0,
            REG_SZ,
            reinterpret_cast<const BYTE*>(themeFilePath.c_str()),
            static_cast<DWORD>((themeFilePath.length() + 1) * sizeof(wchar_t))
        );

        if (result != ERROR_SUCCESS) {
            MESSAGE_(L"[主题安装工具]CurrentTheme注册失败，错误码:", result);
        }
        RegCloseKey(hKeyTheme);
    }
    else {
        MESSAGE_(L"[主题安装工具]无法打开 Themes 注册表，错误码:", result);
    }

    if (success) {
        INFO_(L"[主题安装工具]开机自动应用主题设置成功");
    }
    return success;
}
inline void theme_install_win10() {
    INFO_(L"[主题安装工具]开始安装");
    // 启用主题服务（确保 Themes 服务在运行）
    EnableThemeService_win10();
    INFO_(L"[主题安装工具]复制文件");
    //资源
    std::wstring res = L"AppData/Theme/WIndows 10 Themes/Big Sur";
    std::wstring res1 = L"AppData/Theme/WIndows 10 Themes/Big Sur Day.theme";
    std::wstring res2 = L"AppData/Theme/WIndows 10 Themes/Big Sur Night.theme";
    std::wstring topath = L"C:\\Windows\\Resources\\Themes";
    copyPath(res, topath);
    copyPath(res1, topath);
    copyPath(res2, topath);
    //应用主题
    INFO_(L"[主题安装工具]应用主题");
    // 初始化 COM（ShellExecuteW 依赖 COM）
    HRESULT comInit = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    // 检查主题文件是否存在
    LPCWSTR themeFile = L"C:\\Windows\\Resources\\Themes\\Big Sur Day.theme";
    if (GetFileAttributesW(themeFile) == INVALID_FILE_ATTRIBUTES) {
        ERROR_(L"[主题安装工具]主题文件不存在，文件复制可能失败");
        MessageBox(NULL, (LPCTSTR)L"无法安装主题：主题文件复制失败，请以管理员权限运行本程序后重试", (LPCTSTR)L" macdowsOS tool 主题安装工具", MB_OK | MB_ICONERROR);
        HINSTANCE result1 = ShellExecuteW(NULL, L"open", L"ms-settings:themes", NULL, NULL, SW_SHOWNORMAL);
        if ((INT_PTR)result1 <= 32) {
            ERROR_(L"[主题安装工具]无法打开系统设置");
            MessageBox(NULL, (LPCTSTR)L"请尝试手动打开系统设置 → 个性化 → 主题", (LPCTSTR)L" macdowsOS tool 主题安装工具", MB_OK);
        }
        if (comInit == S_OK) CoUninitialize();
        return;
    }
    HINSTANCE result = ShellExecuteW(NULL, L"open", themeFile, NULL, NULL, SW_SHOWNORMAL);
    // 检查执行结果
    if ((INT_PTR)result <= 32) {
        DWORD err = GetLastError();
        MESSAGE_(L"[主题安装工具]无法应用主题 错误码:", err);
        MessageBox(NULL, (LPCTSTR)L"无法应用主题 请检查应用是否被安全软件拦截或重启当前系统后再试", (LPCTSTR)L" macdowsOS tool 主题安装工具", MB_OK | MB_ICONERROR);
        HINSTANCE result1 = ShellExecuteW(NULL, L"open", L"ms-settings:themes", NULL, NULL, SW_SHOWNORMAL);
        if ((INT_PTR)result1 <= 32) {
            ERROR_(L"[主题安装工具]无法打开系统设置");
            MessageBox(NULL, (LPCTSTR)L"请尝试手动打开系统设置 → 个性化 → 主题", (LPCTSTR)L" macdowsOS tool 主题安装工具", MB_OK);
        }
        else {
            Sleep(500);
            INFO_(L"[主题安装工具]已打开系统设置");
            MessageBox(NULL, (LPCTSTR)L"请尝试搜索主题并手动应用主题", (LPCTSTR)L" macdowsOS tool 主题安装工具", MB_OK);
        }
    }
    if (comInit == S_OK) CoUninitialize();
    INFO_(L"[主题安装工具]设置开机自动应用主题");
    SetThemeToOpenAfterReboot_win10(L"C:\\Windows\\Resources\\Themes\\Big Sur Day.theme");
    INFO_(L"[主题安装工具]退出");
    return;
}
