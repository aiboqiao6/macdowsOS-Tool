#pragma once
// ============================================================
// CreateSafePoint.h
// 描述：创建和完成 Windows 系统还原点，提供系统保护功能
//      包含开始创建还原点和结束创建还原点的函数
// ============================================================
#include <Windows.h>
#include <srrestoreptapi.h>
#include"LogSystem.h"
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "SrClient.lib")
#pragma comment(lib, "Shell32.lib")
// 开始系统还原点创建，返回64位序列号
INT64 Start_CreateSafePoint(LPCWSTR description) {
    INFO_(L"[还原工具]开始创建");
    INFO_(L"[还原工具]初始化");
    RESTOREPOINTINFOW rpInfo{};  // 正确的结构体
    STATEMGRSTATUS smStatus{};
    // 1. 设置事件类型：开始系统更改
    INFO_(L"[还原工具]设置事件类型：开始系统更改");
    rpInfo.dwEventType = BEGIN_SYSTEM_CHANGE;
    // 2. 正确填充描述信息，结构体中szDescription是固定大小数组，需复制字符串
    INFO_(L"[还原工具]填充描述信息");
    wcscpy_s(rpInfo.szDescription, 256, description); 
    // 调用API开始创建
    if (!SRSetRestorePointW(&rpInfo, &smStatus)) {
        ERROR_(L"[还原工具]API失败");
        return -1;
    }
    // 返回系统生成的64位序列号，正确对比llSequenceNumber
    INFO_(L"[还原工具]返回序列号 退出");
    return smStatus.llSequenceNumber;
}
// 结束系统还原点创建，完成创建过程
bool Finish_CreateSafePoint(INT64 sequenceNumber) {
    INFO_(L"[还原工具]开始完成");
    if (sequenceNumber == 0) {
        ERROR_(L"[还原工具]序列号无效");
        return false;
    }
    RESTOREPOINTINFOW rpInfo{};
    STATEMGRSTATUS smStatus{};
    // 1. 设置事件类型：结束系统更改
    INFO_(L"[还原工具]设置事件类型：结束系统更改");
    rpInfo.dwEventType = END_SYSTEM_CHANGE;
    // 2. 传入开始时获取的序列号，关键：绑定开始/结束两个操作
    INFO_(L"[还原工具]传入开始时获取的序列号");
    rpInfo.llSequenceNumber = sequenceNumber;
    // 3. 描述信息可由用户定义，此处沿用开始时相同的描述
    wcscpy_s(rpInfo.szDescription, 256, L"系统保护");
    // 调用API完成还原点创建
    if (!SRSetRestorePointW(&rpInfo, &smStatus)) {
        ERROR_(L"[还原工具]API失败");
        return false;
    }
    INFO_(L"[还原工具]退出");
    return true;
}
