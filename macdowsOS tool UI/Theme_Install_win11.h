#pragma once
#include <string>
#include"WindowControl.h"
#include <tlhelp32.h> 
#include <tchar.h>  
#include <shellapi.h> 
#include"LogSystem.h"
#include"FilesSystem.h"
// 将主题设置为开机后自动应用
bool SetThemeToOpenAfterReboot_win11(const std::wstring& themeFilePath) {
    HKEY hKey = nullptr;
    bool success = false;

    // 1. 构建打开主题文件的命令
    // 使用"start"来确保通过默认关联程序打开
    std::wstring command = L"cmd /c start \"\" \"" + themeFilePath + L"\"";

    // 2. 打开当前用户的RunOnce注册表项
    // 使用 KEY_WOW64_64KEY 确保64位系统上访问正确的注册表位置
    LONG result = RegOpenKeyExW(
        HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce",
        0,
        KEY_WRITE | KEY_WOW64_64KEY,
        &hKey
    );

    if (result == ERROR_SUCCESS && hKey != nullptr) {
        // 3. 创建一个唯一的值的名称，使用固定名称
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
inline void theme_install_win11() {
    INFO_(L"[主题安装工具]开始安装");
    INFO_(L"[主题安装工具]复制文件");
    //资源
    std::wstring res = L"AppData/Theme/WIndows 11 Themes/Tahoe";
    std::wstring res1 = L"AppData/Theme/WIndows 11 Themes/Tahoe - Light.theme";
    std::wstring res2 = L"AppData/Theme/WIndows 11 Themes/Tahoe - Night.theme";
    std::wstring topath = L"C:\\Windows\\Resources\\Themes";
    copyPath(res, topath);
    copyPath(res1, topath);
    copyPath(res2, topath);
    //应用主题
    INFO_(L"[主题安装工具]应用主题");
    HINSTANCE result = ShellExecuteW(NULL,L"open",L"C:\\Windows\\Resources\\Themes\\Tahoe - Light.theme",NULL,NULL,SW_SHOWNORMAL);
    // 检查执行结果
    if ((INT_PTR)result <= 32) {
        ERROR_(L"[主题安装工具]无法应用主题");
        MessageBox(NULL, (LPCTSTR)L"无法应用主题 请检查应用是否被安全软件拦截或重启当前系统后再试", (LPCTSTR)L" macdowsOS tool 主题安装工具", MB_OK | MB_ICONERROR);
        HINSTANCE result1 = ShellExecuteW(NULL,L"open",L"ms-settings:themes",NULL,NULL,SW_SHOWNORMAL);
        if ((INT_PTR)result1 <= 32) {
            ERROR_(L"[主题安装工具]无法打开系统设置");
            MessageBox(NULL, (LPCTSTR)L"无法打开系统设置 请检查是否被安全软件拦截或重启系统重试", (LPCTSTR)L" macdowsOS tool 主题安装工具", MB_OK | MB_ICONERROR);
            MessageBox(NULL, (LPCTSTR)L"请手动打开主题应用", (LPCTSTR)L" macdowsOS tool 主题安装工具", MB_OK);
        }
        else {
            Sleep(500);
            INFO_(L"[主题安装工具]已打开系统设置");
            MessageBox(NULL, (LPCTSTR)L"请尝试搜索主题并手动应用主题", (LPCTSTR)L" macdowsOS tool 主题安装工具", MB_OK);
        }
    }
    INFO_(L"[主题安装工具]设置开机自动应用主题");
    SetThemeToOpenAfterReboot_win11(L"C:\\Windows\\Resources\\Themes\\Tahoe - Light.theme");
    INFO_(L"[主题安装工具]退出");
    return;
}
