#pragma once
#include <string>
#include <filesystem>
#include <cstdlib>
#include<Windows.h>
#include"LogSystem.h"
#include"WindowControl.h"
#include"All.h"
bool StartAllBack_Config(int mode) {
    INFO_(L"[StartAllBack配置器]开始");
    INFO_(L"[StartAllBack配置器]启动配置程序");
    if (mode == 1) {
        ShellExecuteW(NULL, L"open", L"C:\\Program Files\\StartAllBack\\StartAllBackCfg.exe", NULL, NULL, SW_SHOWNORMAL);
    }
    else if (mode == 2) {
        //获取用户名
        wchar_t username[10000];
        DWORD username_len = 10000;
        GetUserName(username, &username_len);
        std::wstring user_name = username;
        MESSAGE_(L"[StartAllBack配置器]检测到用户名", user_name);

        std::wstring configprogrampath = L"C:\\Users\\" + user_name + L"\\AppData\\Local\\StartAllBack\\StartAllBackCfg.exe";
        const wchar_t* path_temp = configprogrampath.data();
        //
        ShellExecuteW(NULL, L"open", path_temp, NULL, NULL, SW_SHOWNORMAL);
    }
    else {
        MessageBox(NULL, (LPCTSTR)L"检测安装模式时出现错误，请将安装日志发送给开发者处理 StartAllBack配置程序退出", (LPCTSTR)L" StartAllBack配置器", MB_OK | MB_ICONERROR);
        return false;
    }

    HWND hWnd_Window = FindWindowW(NULL, L"配置 StartAllBack");
    while (hWnd_Window == NULL) {
        hWnd_Window = FindWindowW(NULL, L"配置 StartAllBack");
    }
    INFO_(L"[StartAllBack配置器]已找到窗口");
    SetForegroundWindow(hWnd_Window);
    SetWindowPos(hWnd_Window, NULL, 0, 0, 0, 0, SWP_NOSIZE);

    ClickButtonMode1(hWnd_Window, L"StartAllBack配置器", L"Windows 7 资源管理器样式");
    TurnOffOn(false, hWnd_Window, L"StartAllBack配置器", L"显示开始按钮");
    TurnOffOn(false, hWnd_Window, L"StartAllBack配置器", L"增强型任务管理器");
    TurnOffOn(true , hWnd_Window, L"StartAllBack配置器", L"恢复传统右键菜单");
    TurnOffOn(false, hWnd_Window, L"StartAllBack配置器", L"选中时显示强调色");
    TurnOffOn(true , hWnd_Window, L"StartAllBack配置器", L"资源管理器式右键菜单");
    TurnOffOn(false, hWnd_Window, L"StartAllBack配置器", L"禁用大图标资源管理器");
    TurnOffOn(true , hWnd_Window, L"StartAllBack配置器", L"全新关机图标");
    TurnOffOn(false, hWnd_Window, L"StartAllBack配置器", L"底部详细信息栏");
    TurnOffOn(false, hWnd_Window, L"StartAllBack配置器", L"右侧详细信息栏");
    TurnOffOn(true , hWnd_Window, L"StartAllBack配置器", L"启用搜索字母效果");

    INFO_(L"[StartAllBack配置器]配置完成 重启文件资源管理器");
    system("taskkill /f /im explorer.exe");
    system("start explorer");
    return true;
}
bool StartAllBack_Install(int mode) {
	INFO_(L"[StartAllBack安装器]开始");
	INFO_(L"[StartAllBack安装器]开始安装程序");
	system("start AppData\\StartAllBack\\Install.exe");

    //等待应用程序
    INFO_(L"[StartAllBack安装器]等待应用程序");
    HWND hWnd_Window = FindWindowW(NULL, L"安装 StartAllBack");
    while (hWnd_Window == NULL) {
        hWnd_Window = FindWindowW(NULL, L"安装 StartAllBack");
    }
    INFO_(L"[StartAllBack安装器]已找到窗口");
    //前置
    SetForegroundWindow(hWnd_Window);
    SetWindowPos(hWnd_Window, NULL, 0, 0, 0, 0, SWP_NOSIZE);


    if (mode == 1) {
        ClickButtonMode1(hWnd_Window, L"StartAllBack安装器", L"为所有用户安装");
    }
    else if (mode == 2) {//
        ClickButtonMode1(hWnd_Window, L"StartAllBack安装器", L"为当前用户安装");
    }
    else {
        MessageBox(NULL, (LPCTSTR)L"检测安装模式时出现错误，请将安装日志发送给开发者处理 StartAllBack安装器退出", (LPCTSTR)L" StartAllBack安装器", MB_OK | MB_ICONERROR);
        return false;
    }
    


    INFO_(L"[StartAllBack安装器]等待安装完成");
    //等待应用程序
    INFO_(L"[StartAllBack安装器]等待安装完成");
    hWnd_Window = FindWindowW(NULL, L"安装 StartAllBack");
    HWND hWnd_ERROR = FindWindowW(NULL, L"错误");
    while (hWnd_Window != NULL) {
        hWnd_Window = FindWindowW(NULL, L"安装 StartAllBack");
        hWnd_ERROR = FindWindowW(NULL, L"错误");
        if (hWnd_ERROR != NULL) {
            INFO_(L"[StartAllBack安装器]已找到错误窗口");
            HWND errormain=FindChildControlExact(hWnd_ERROR, L"StartAllBack 不兼容当前 Windows 系统版本");
            if (errormain != NULL) {
                ERROR_(L"[StartAllBack安装器]StartAllBack不兼容当前系统");
                SetForegroundWindow(hWnd_ERROR);
                enter_key();
                MessageBox(NULL, (LPCTSTR)L"StartAllBack不兼容当前系统 安装程序退出", (LPCTSTR)L" StartAllBack安装器", MB_OK | MB_ICONERROR);
                return false;
            }
        }
    }
    INFO_(L"[StartAllBack安装器]安装显示完成");
    INFO_(L"[StartAllBack安装器]安装完成");
    if (!StartAllBack_Config(mode)) {
        return false;
    }
    else {
        return true;
    }
}
inline void StartAllBack_SetupRun(){
    INFO_(L"[StartAllBack执行安装器]开始");
    //获取用户名
    wchar_t username[10000];
    DWORD username_len=10000;
    GetUserName(username, &username_len);
    std::wstring user_name = username;
    MESSAGE_(L"[StartAllBack执行安装器]检测到用户名", user_name);
    if (std::filesystem::exists(L"C:/Program Files/StartAllBack/StartAllBackCfg.exe")) {
        INFO_(L"[StartAllBack执行安装器]检测到已安装StartAllBack全局");
        if (!StartAllBack_Install(1)) {
            ERROR_(L"[StartAllBack执行安装器]安装时出现错误");
            return;
        }
    }
    else if (std::filesystem::exists(L"C:/Users/"+ user_name + L"/AppData/Local/StartAllBack/StartAllBackCfg.exe")) {
        INFO_(L"[StartAllBack执行安装器]检测到已安装StartAllBack当前用户");
        if (!StartAllBack_Install(2)) {
            ERROR_(L"[StartAllBack执行安装器]安装时出现错误");
            return;
        }
    }
    else {
        INFO_(L"[StartAllBack执行安装器]未检测到已安装的StartAllBack");
        if (!StartAllBack_Install(1)) {
            ERROR_(L"[StartAllBack执行安装器]安装时出现错误");
            return;
        }
    }
    INFO_(L"[StartAllBack执行安装器]退出");
    return;
}
