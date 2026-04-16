#pragma once
#include<windows.h>
#include<bits/stdc++.h>
#include"LogSystem.h"
using namespace std;
void restart_() {
    INFO_(L"[重启组件]启动");
    if (MessageBoxW(NULL, L"点击确定以重新启动系统", L"macdowsOS tool 重启", MB_OK) == 1) {
        INFO_(L"[重启组件]重启");
        system("shutdown -r -t 2");  // 2秒后重启
    }
    INFO_(L"[重启组件]退出");
    return;
}
void restart_nonotice() {
    INFO_(L"[重启组件]启动");
    INFO_(L"[重启组件]重启");
    system("shutdown -r -t 0");  // 2秒后重启
    INFO_(L"[重启组件]退出");
    return;
}
