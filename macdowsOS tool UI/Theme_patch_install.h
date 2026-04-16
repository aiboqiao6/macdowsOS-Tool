#pragma once
#include<bits/stdc++.h>
#include<windows.h>
#include"WindowControl.h"
#include"LogSystem.h"
using namespace std;
void theme_path_install() {
    INFO_(L"[主题补丁安装组件]启动");
    HWND temp,temp1;
    //启动
    INFO_(L"[主题补丁安装组件]运行安装程序");
    system("start AppData\\Theme\\Theme_Patcher.exe");
    //等待应用启动&&继续-查找
    INFO_(L"[主题补丁安装组件]等待窗口启动");
    HWND hWnd_Window = FindWindowW(NULL, L"UltraUXThemePatcher 4.5.0 from https://mhoefs.eu");
    while (hWnd_Window==NULL) {
        hWnd_Window = FindWindowW(NULL, L"UltraUXThemePatcher 4.5.0 from https://mhoefs.eu");
    }
    INFO_(L"[主题补丁安装组件]查找到窗口");
    //前置
    SetForegroundWindow(hWnd_Window);
    SetWindowPos(hWnd_Window, NULL, 0, 0, 0, 0, SWP_NOSIZE);

    ClickButtonMode3(hWnd_Window, L"主题补丁安装组件", L"&Next >");
    ClickButtonMode1(hWnd_Window, L"主题补丁安装组件", L"I &accept the terms of the License Agreement");
    ClickButtonMode3(hWnd_Window, L"主题补丁安装组件", L"&Next >");
    ClickButtonMode3(hWnd_Window, L"主题补丁安装组件", L"I &Agree");
    ClickButtonMode3(hWnd_Window, L"主题补丁安装组件", L"&Install");
    ClickButtonMode3(hWnd_Window, L"主题补丁安装组件", L"Installation Complete");
    ClickButtonMode3(hWnd_Window, L"主题补丁安装组件", L"&Next >");

    INFO_(L"[主题补丁安装组件]查找 稍后重启");
    //稍后重启-查找
    temp = FindChildControlExact(hWnd_Window, L"I want to manually reboot later");
    temp1= FindChildControlExact(hWnd_Window, L"Donation via Paypal");
    while (temp == NULL) {
        temp = FindChildControlExact(hWnd_Window, L"I want to manually reboot later");
        temp1 = FindChildControlExact(hWnd_Window, L"Donation via Paypal");
        if (temp1!=NULL) {
            INFO_(L"[主题补丁安装组件]查找到 捐赠按钮");
            INFO_(L"[主题补丁安装组件]主题补丁可能已安装或不兼容 退出");
            //前置
            SetForegroundWindow(hWnd_Window);
            enter_key();
            return;
        }
        
    }
    INFO_(L"[主题补丁安装组件]查找到 稍后重启");
    //前置
    SetForegroundWindow(hWnd_Window);
    //点击
    SendMessageW(temp, BM_CLICK, 0, 0);

    
    ClickButtonMode3(hWnd_Window, L"主题补丁安装组件", L"&Finish");
    INFO_(L"[主题补丁安装组件]退出");
    return;
}