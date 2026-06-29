#pragma once
// ============================================================
// restart.h
// 描述：系统重启工具，提供重启计算机的相关函数
//      包含带确认提示和无提示两种重启方式
// ============================================================
#include<windows.h>
#include"LogSystem.h"
inline void restart_() {
    INFO_(L"[重启器]开始");
    if (MessageBoxW(NULL, L"请确认重启计算机系统？", L"macdowsOS tool 重启", MB_OK) == 1) {
        INFO_(L"[重启器]重启");
        system("shutdown -r -t 2");  // 2秒后重启
    }
    INFO_(L"[重启器]退出");
    return;
}
inline void restart_nonotice() {
    INFO_(L"[重启器]开始");
    INFO_(L"[重启器]重启");
    system("shutdown -r -t 0");  // 立即重启
    INFO_(L"[重启器]退出");
    return;
}
