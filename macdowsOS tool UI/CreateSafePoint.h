#pragma once
#include <Windows.h>
#include <srrestoreptapi.h>
#include <iostream>
#include <cstring>  // 用于字符串复制
#include"LogSystem.h"
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "SrClient.lib")
#pragma comment(lib, "Shell32.lib")
// 开始系统还原点跟踪（返回64位序列号）
INT64 Start_CreateSafePoint(LPCWSTR description) {
    INFO_(L"[还原点组件]组件运行");
    INFO_(L"[还原点组件]初始化");
    RESTOREPOINTINFOW rpInfo{};  // 正确的结构体
    STATEMGRSTATUS smStatus{};
    // 1. 配置事件类型（开始系统变更）
    INFO_(L"[还原点组件]配置事件类型（开始系统变更）");
    rpInfo.dwEventType = BEGIN_SYSTEM_CHANGE;
    // 2. 正确设置描述信息（结构体中szDescription是固定大小数组，需复制字符串）
    INFO_(L"[还原点组件]设置描述信息");
    wcscpy_s(rpInfo.szDescription, 256, description); 
    // 调用API创建开始标记
    if (!SRSetRestorePointW(&rpInfo, &smStatus)) {
        ERROR_(L"[还原点组件]API报错");
        return -1;
    }
    // 返回系统生成的64位序列号（正确成员是llSequenceNumber）
    INFO_(L"[还原点组件]返回序列号 退出");
    return smStatus.llSequenceNumber;
}
// 结束系统还原点跟踪（完成创建）
bool Finish_CreateSafePoint(INT64 sequenceNumber) {
    INFO_(L"[还原点组件]组件运行");
    if (sequenceNumber == 0) ERROR_(L"[还原点组件]传入序列号无效"); return false;
    RESTOREPOINTINFOW rpInfo{};
    STATEMGRSTATUS smStatus{};
    // 1. 配置事件类型（结束系统变更）
    INFO_(L"[还原点组件]配置事件类型（结束系统变更）");
    rpInfo.dwEventType = END_SYSTEM_CHANGE;
    // 2. 传入开始时获取的序列号（关键：绑定开始/结束操作）
    INFO_(L"[还原点组件]传入开始时获取的序列号");
    rpInfo.llSequenceNumber = sequenceNumber;
    // 3. 描述信息可复用或留空（此处复用开始时的描述）
    wcscpy_s(rpInfo.szDescription, 256, L"完成系统变更");
    // 调用API完成还原点创建
    if (!SRSetRestorePointW(&rpInfo, &smStatus)) {
        ERROR_(L"[还原点组件]API报错");
        return false;
    }
    INFO_(L"[还原点组件]退出");
    return true;
}