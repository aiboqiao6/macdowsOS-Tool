#pragma once
#include<bits/stdc++.h>
#include"WindowControl.h"
#include<windows.h>
#include"LogSystem.h"
#include"FilesSystem.h"
#include"All.h"
#include <AccCtrl.h>
#include <AclAPI.h>
#include <string>
#pragma comment(lib, "Advapi32.lib")
using namespace std;
/*
imageres.dll
imagesp1.dll
zipfldr.dll
C:\Windows\System32
C:\Windows\SysWOW64
*/
void Icon_Install() {
    INFO_(L"[图标安装组件]启动");
    //需替换的文件
    wstring res1 = L"AppData/Icon/Icon_res/imageres.dll";
    wstring res2 = L"AppData/Icon/Icon_res/imagesp1.dll";
    wstring res3 = L"AppData/Icon/Icon_res/zipfldr.dll";
    //原文件
    wstring res5 = L"C:\\Windows\\System32\\imageres.dll";
    wstring res6 = L"C:\\Windows\\System32\\imagesp1.dll";
    wstring res7 = L"C:\\Windows\\System32\\zipfldr.dll";

    wstring res8 = L"C:\\Windows\\SysWOW64\\imageres.dll";
    wstring res9 = L"C:\\Windows\\SysWOW64\\imagesp1.dll";
    wstring res10 = L"C:\\Windows\\SysWOW64\\zipfldr.dll";
    //系统目录
    wstring to1 = L"C:\\Windows\\System32";
    wstring to2 = L"C:\\Windows\\SysWOW64";
    //备份目录
    wstring backup1 = L"AppData/Icon/Icon_backup/System32";
    wstring backup2 = L"AppData/Icon/Icon_backup/SysWOW64";
    INFO_(L"[图标安装组件]备份");
    //
    copyPath(res5, backup1);
    copyPath(res6, backup1);
    copyPath(res7, backup1);
    //
    copyPath(res8, backup2);
    copyPath(res9, backup2);
    copyPath(res10, backup2);
    INFO_(L"[图标安装组件]关闭文件资源管理器");
    system("taskkill /f /im explorer.exe");
    INFO_(L"[图标安装组件]替换文件");
    copyPath(res1, to1);
    copyPath(res2, to1);
    copyPath(res3, to1);
    //
    copyPath(res1, to2);
    copyPath(res2, to2);
    copyPath(res3, to2);
    INFO_(L"[图标安装组件]启动文件资源管理器");
    system("start explorer");
    INFO_(L"[图标安装组件]刷新图标缓存");
    RunBatchSilentlyNative(L"AppData\\Icon\\Icon.bat");
   // system("start AppData\\Icon\\Icon.bat");
    return;
}