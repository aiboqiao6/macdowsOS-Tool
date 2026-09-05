#pragma once

#include <string>
#include <windows.h>

#include "Icon_install.h"

namespace {

void Icon_Back() {
    INFO_(L"[图标还原组件]启动");
    // 系统源文件
    std::wstring res5 = L"C:\\Windows\\System32\\imageres.dll";
    std::wstring res6 = L"C:\\Windows\\System32\\imagesp1.dll";

    std::wstring res8 = L"C:\\Windows\\SysWOW64\\imageres.dll";
    std::wstring res9 = L"C:\\Windows\\SysWOW64\\imagesp1.dll";

    //系统目录
    std::wstring to1 = L"C:\\Windows\\System32";
    std::wstring to2 = L"C:\\Windows\\SysWOW64";

    // 备份目录
    std::wstring backup1 = L"C:\\Windows\\macdowsOStool\\Icon_backup\\System32\\imageres.dll";
    std::wstring backup2 = L"C:\\Windows\\macdowsOStool\\Icon_backup\\System32\\imagesp1.dll";
    std::wstring backup3 = L"C:\\Windows\\macdowsOStool\\Icon_backup\\SysWOW64\\imageres.dll";
    std::wstring backup4 = L"C:\\Windows\\macdowsOStool\\Icon_backup\\SysWOW64\\imagesp1.dll";

    INFO_(L"[图标还原组件]检测备份文件完整性");
    //检测文件在不在
    if (!fs::exists(backup1) || !fs::exists(backup2)) {
        ERROR_(L"[图标还原组件]图标备份文件缺失 图标还原退出");
        MessageBox(NULL, (LPCTSTR)L"无法还原图标，图标备份文件缺失或损坏", (LPCTSTR)L" macdowsOS tool 图标还原组件", MB_OK | MB_ICONERROR);
        return;
    }
    INFO_(L"[图标还原组件]结束文件资源管理器");
    system("taskkill /f /im explorer.exe");
    
    INFO_(L"[图标还原组件]获得文件所有权");
    //获取所有权
     // System32
    TakeOwnershipAndGrantFullControl(res5);
    TakeOwnershipAndGrantFullControl(res6);
    // SysWOW64
    TakeOwnershipAndGrantFullControl(res8);
    TakeOwnershipAndGrantFullControl(res9);
    //backup
    TakeOwnershipAndGrantFullControl(backup1);
    TakeOwnershipAndGrantFullControl(backup2);
    TakeOwnershipAndGrantFullControl(backup3);
    TakeOwnershipAndGrantFullControl(backup4);

    INFO_(L"[图标还原组件]替换文件");
    // 替换 System32
    copyPath(backup1, to1);
    copyPath(backup2, to1);
    // 替换 SysWOW64
    copyPath(backup3, to2);
    copyPath(backup4, to2);
    INFO_(L"[图标还原组件]删备份");
    TakeOwnershipAndGrantFullControl(L"C:\\Windows\\macdowsOStool\\Icon_backup");
    std::filesystem::remove_all(L"C:\\Windows\\macdowsOStool\\Icon_backup");
    //
    INFO_(L"[图标还原组件]清缓存");
    ClearIconCache();
    system("start explorer");
    return;
}

}
