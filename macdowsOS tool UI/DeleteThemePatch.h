#pragma once
#include <cstdlib>
#include<windows.h>
#include"WindowControl.h"
#include"LogSystem.h"

namespace {
    void theme_path_uninstall() {
        INFO_(L"[主题补丁工具]开始");
        HWND temp, temp1;
        //卸载
        INFO_(L"[主题补丁卸载工具]开始卸载程序");
        system("start C:\\Program&Files&(x86)\\UltraUXThemePatcher\\Uninstall.exe");
        //等待应用程序&&查找-卸载
        INFO_(L"[主题补丁卸载工具]等待应用程序");
        HWND hWnd_Window = FindWindow_(L"UltraUXThemePatcher Uninstall", L"主题补丁卸载工具");

        INFO_(L"[主题补丁卸载工具]已找到窗口");
        //前置
        SetForegroundWindow(hWnd_Window);
        SetWindowPos(hWnd_Window, NULL, 0, 0, 0, 0, SWP_NOSIZE);

        ClickButtonMode3(hWnd_Window, L"主题补丁卸载工具", L"&Uninstall");
        ClickButtonMode1(hWnd_Window, L"主题补丁卸载工具", L"&Next >");

        INFO_(L"[主题补丁卸载工具]查找 稍后重启");
        //稍后重启-查找
        temp = FindChildControlExact(hWnd_Window, L"I want to manually reboot later");
        temp1 = FindChildControlExact(hWnd_Window, L"Donation via Paypal");
        while (temp == NULL) {
            temp = FindChildControlExact(hWnd_Window, L"I want to manually reboot later");
            temp1 = FindChildControlExact(hWnd_Window, L"Donation via Paypal");
            if (temp1 != NULL) {
                INFO_(L"[主题补丁卸载工具]已找到 捐赠按钮");
                INFO_(L"[主题补丁卸载工具]主题补丁已卸载或不兼容 退出");
                //前置
                SetForegroundWindow(hWnd_Window);
                enter_key();
                return;
            }

        }
        INFO_(L"[主题补丁卸载工具]已找到 稍后重启");
        //前置
        SetForegroundWindow(hWnd_Window);
        //选中
        SendMessageW(temp, BM_CLICK, 0, 0);


        ClickButtonMode3(hWnd_Window, L"主题补丁卸载工具", L"&Finish");
        INFO_(L"[主题补丁卸载工具]退出");
        return;
    }

}
