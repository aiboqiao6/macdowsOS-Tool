#pragma once
// ============================================================
// DeleteTheme.h
// 描述：主题删除工具，提供删除已安装的 macOS 主题文件的功能
//      清理 Windows Resources\Themes 目录下的相关文件
// ============================================================
#include<Windows.h>
#include <filesystem>
#include"LogSystem.h"
#include "Theme_Install_win11.h"

namespace {
void Delete_theme(){
    INFO_(L"[主题删除工具]开始删除");
    std::filesystem::remove_all("C:\\Windows\\Resources\\Themes\\Big Sur"); INFO_(L"[主题删除工具]删除Big Sur");
    std::filesystem::remove_all("C:\\Windows\\Resources\\Themes\\Tahoe"); INFO_(L"[主题删除工具]删除Tahoe");
    std::filesystem::remove_all("C:\\Windows\\Resources\\Themes\\Big Sur Dark Mode.theme"); INFO_(L"[主题删除工具]删除Big Sur Dark Mode.theme");
    std::filesystem::remove_all("C:\\Windows\\Resources\\Themes\\Big Sur Day.theme"); INFO_(L"[主题删除工具]删除Big Sur Day.theme");
    std::filesystem::remove_all("C:\\Windows\\Resources\\Themes\\Big Sur Night.theme"); INFO_(L"[主题删除工具]删除Big Sur Night.theme");
    std::filesystem::remove_all("C:\\Windows\\Resources\\Themes\\Tahoe - Light.theme"); INFO_(L"[主题删除工具]删除Tahoe - Light.theme");
    std::filesystem::remove_all("C:\\Windows\\Resources\\Themes\\Tahoe - Night.theme"); INFO_(L"[主题删除工具]删除Tahoe - Night.theme");
    
    //应用主题
    INFO_(L"[主题删除工具]应用主题");
    // 初始化 COM（ShellExecuteW 依赖 COM）
    HRESULT comInit = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    // 检查主题文件是否存在
    LPCWSTR themeFile = L"C:\\Windows\\Resources\\Themes\\aero.theme";
    
    HINSTANCE result = ShellExecuteW(NULL, L"open", themeFile, NULL, NULL, SW_SHOWNORMAL);
    // 检查执行结果
    if ((INT_PTR)result <= 32) {
        DWORD err = GetLastError();
        MESSAGE_(L"[主题删除工具]无法应用主题 错误码:", err);
        MessageBox(NULL, (LPCTSTR)L"无法应用主题 请检查应用是否被安全软件拦截或重启当前系统后再试", (LPCTSTR)L" macdowsOS tool 主题删除工具", MB_OK | MB_ICONERROR);
        HINSTANCE result1 = ShellExecuteW(NULL, L"open", L"ms-settings:themes", NULL, NULL, SW_SHOWNORMAL);
        if ((INT_PTR)result1 <= 32) {
            ERROR_(L"[主题删除工具]无法打开系统设置");
            MessageBox(NULL, (LPCTSTR)L"请尝试手动打开系统设置 → 个性化 → 主题", (LPCTSTR)L" macdowsOS tool 主题删除工具", MB_OK);
        }
        else {
            Sleep(500);
            INFO_(L"[主题删除工具]已打开系统设置");
            MessageBox(NULL, (LPCTSTR)L"请尝试搜索主题并手动应用主题", (LPCTSTR)L" macdowsOS tool 主题删除工具", MB_OK);
        }
    }
    if (comInit == S_OK) CoUninitialize();
    INFO_(L"[主题删除工具]设置开机自动应用主题");
    SetThemeToOpenAfterReboot_win11(L"C:\\Windows\\Resources\\Themes\\aero.theme");
    INFO_(L"[主题删除工具]退出");
    
    return;
}

}
