#pragma once
// ============================================================
// Icon_install.h
// 描述：图标替换安装工具，提供替换 Windows 系统图标的安装功能
//      包括图标文件备份、替换、资源管理器重启及缓存刷新
// ============================================================
#include <string>
#include <cstdlib>
#include <windows.h>
#include "LogSystem.h"
#include "FilesSystem.h"
#include "All.h"
#pragma comment(lib, "Advapi32.lib")

namespace {

/*
imageres.dll
imagesp1.dll
zipfldr.dll
C:\Windows\System32
C:\Windows\SysWOW64
*/

// 取得指定文件/目录的所有权并授予 Administrators 完全控制权限
bool TakeOwnershipAndGrantFullControl(const std::wstring& filePath) {
    std::wstring takeownCmd = L"takeown /f \"" + filePath + L"\"";
    std::wstring icaclsCmd = L"icacls \"" + filePath + L"\" /grant Administrators:F";
    int r1 = _wsystem(takeownCmd.c_str());
    int r2 = _wsystem(icaclsCmd.c_str());
    return (r1 == 0 && r2 == 0);
}

// 清理 Windows 图标缓存文件
void ClearIconCache() {
    // 获取 LOCALAPPDATA 路径
    wchar_t localAppData[MAX_PATH] = { 0 };
    if (!SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, localAppData))) {
        // 用备用 API 重试
        PWSTR path = nullptr;
        if (FAILED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &path))) {
            ERROR_(L"[图标安装工具]无法获取 LOCALAPPDATA 路径");
            return;
        }
        wcscpy_s(localAppData, path);
        CoTaskMemFree(path);
    }

    INFO_(L"[图标安装工具]清除图标缓存");

    // 1. 删除 Windows 8/10/11 的图标缓存数据库
    std::wstring explorerCacheDir = std::wstring(localAppData) + L"\\Microsoft\\Windows\\Explorer";
    std::wstring searchPattern = explorerCacheDir + L"\\iconcache_*";
    WIN32_FIND_DATAW findData;
    HANDLE hFind = FindFirstFileW(searchPattern.c_str(), &findData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            std::wstring filePath = explorerCacheDir + L"\\" + findData.cFileName;
            DeleteFileW(filePath.c_str());
            INFO_(L"[图标安装工具]删除缓存: " + filePath);
        } while (FindNextFileW(hFind, &findData));
        FindClose(hFind);
    }

    // 2. 删除缩略图缓存
    searchPattern = explorerCacheDir + L"\\thumbcache_*";
    hFind = FindFirstFileW(searchPattern.c_str(), &findData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            std::wstring filePath = explorerCacheDir + L"\\" + findData.cFileName;
            DeleteFileW(filePath.c_str());
        } while (FindNextFileW(hFind, &findData));
        FindClose(hFind);
    }

    // 3. 删除旧的 IconCache.db (Windows 7)
    std::wstring oldCache = std::wstring(localAppData) + L"\\IconCache.db";
    DeleteFileW(oldCache.c_str());

    INFO_(L"[图标安装工具]图标缓存已清除");
}

void Icon_Install() {
    INFO_(L"[图标安装工具]开始");

    // 替换用文件 (第三方自定义图标)
    std::wstring res1 = L"AppData/Icon/Icon_res/imageres.dll";
    std::wstring res2 = L"AppData/Icon/Icon_res/imagesp1.dll";

    // 系统源文件 (用于备份)
    std::wstring res5 = L"C:\\Windows\\System32\\imageres.dll";
    std::wstring res6 = L"C:\\Windows\\System32\\imagesp1.dll";

    std::wstring res8 = L"C:\\Windows\\SysWOW64\\imageres.dll";
    std::wstring res9 = L"C:\\Windows\\SysWOW64\\imagesp1.dll";

    // 目标目录
    std::wstring to1 = L"C:\\Windows\\System32";
    std::wstring to2 = L"C:\\Windows\\SysWOW64";

    // 备份目录
    std::wstring backup1 = L"AppData/Icon/Icon_backup/System32";
    std::wstring backup2 = L"AppData/Icon/Icon_backup/SysWOW64";

    // ========== 1. 备份系统原始文件 ==========
    INFO_(L"[图标安装工具]备份系统原始文件");
    copyPath(res5, backup1);
    copyPath(res6, backup1);

    copyPath(res8, backup2);
    copyPath(res9, backup2);

    // ========== 2. 关闭资源管理器 ==========
    INFO_(L"[图标安装工具]关闭文件资源管理器");
    system("taskkill /f /im explorer.exe");
    Sleep(500);

    // ========== 3. 清理图标缓存 (explorer 已关闭, 无文件锁) ==========
    ClearIconCache();
    Sleep(200);

    // ========== 4. 获取系统文件所有权并替换 ==========
    INFO_(L"[图标安装工具]获取系统文件所有权并替换");

    // System32
    TakeOwnershipAndGrantFullControl(res5);
    TakeOwnershipAndGrantFullControl(res6);
    // SysWOW64
    TakeOwnershipAndGrantFullControl(res8);
    TakeOwnershipAndGrantFullControl(res9);

    // 替换 System32
    copyPath(res1, to1);
    copyPath(res2, to1);
    // 替换 SysWOW64
    copyPath(res1, to2);
    copyPath(res2, to2);

    // ========== 5. 启动资源管理器 ==========
    INFO_(L"[图标安装工具]启动文件资源管理器");
    system("start explorer");

    INFO_(L"[图标安装工具]安装完成");
    return;
}

}
