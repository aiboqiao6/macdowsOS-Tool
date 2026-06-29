#pragma once
// ============================================================
// Icon_install.h
// 描述：图标替换安装工具，提供替换 Windows 系统图标的安装功能
//      包括图标文件备份、替换、资源管理器重启及缓存刷新
// ============================================================
#include <string>
#include <cstdlib>
#include<windows.h>
#include"LogSystem.h"
#include"FilesSystem.h"
#include"All.h"
#pragma comment(lib, "Advapi32.lib")
/*
imageres.dll
imagesp1.dll
zipfldr.dll
C:\Windows\System32
C:\Windows\SysWOW64
*/
inline void Icon_Install() {
    INFO_(L"[图标安装工具]开始");
    //替换用文件
    std::wstring res1 = L"AppData/Icon/Icon_res/imageres.dll";
    std::wstring res2 = L"AppData/Icon/Icon_res/imagesp1.dll";
    std::wstring res3 = L"AppData/Icon/Icon_res/zipfldr.dll";
    //源文件
    std::wstring res5 = L"C:\\Windows\\System32\\imageres.dll";
    std::wstring res6 = L"C:\\Windows\\System32\\imagesp1.dll";
    std::wstring res7 = L"C:\\Windows\\System32\\zipfldr.dll";

    std::wstring res8 = L"C:\\Windows\\SysWOW64\\imageres.dll";
    std::wstring res9 = L"C:\\Windows\\SysWOW64\\imagesp1.dll";
    std::wstring res10 = L"C:\\Windows\\SysWOW64\\zipfldr.dll";
    //系统目录
    std::wstring to1 = L"C:\\Windows\\System32";
    std::wstring to2 = L"C:\\Windows\\SysWOW64";
    //备份目录
    std::wstring backup1 = L"AppData/Icon/Icon_backup/System32";
    std::wstring backup2 = L"AppData/Icon/Icon_backup/SysWOW64";
    INFO_(L"[图标安装工具]备份");
    //
    copyPath(res5, backup1);
    copyPath(res6, backup1);
    copyPath(res7, backup1);
    //
    copyPath(res8, backup2);
    copyPath(res9, backup2);
    copyPath(res10, backup2);
    INFO_(L"[图标安装工具]关闭文件资源管理器");
    system("taskkill /f /im explorer.exe");
    INFO_(L"[图标安装工具]替换文件");
    copyPath(res1, to1);
    copyPath(res2, to1);
    copyPath(res3, to1);
    //
    copyPath(res1, to2);
    copyPath(res2, to2);
    copyPath(res3, to2);
    INFO_(L"[图标安装工具]启动文件资源管理器");
    system("start explorer");
    INFO_(L"[图标安装工具]刷新图标缓存");
    RunBatchSilentlyNative(L"AppData\\Icon\\Icon.bat");
   // system("start AppData\\Icon\\Icon.bat");
    return;
}
