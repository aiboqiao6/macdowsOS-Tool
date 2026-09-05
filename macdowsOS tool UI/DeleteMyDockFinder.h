#pragma once
#include <string>
#include "LogSystem.h"
#include "FilesSystem.h"
#include "mydockfinder_Install.h"
#include <Windows.h>
#include "All.h"

namespace {

void DeleteMyDockFinder() {
	std::wifstream file(L"C:\\Windows\\macdowsOStool\\MyDockFinderPath.txt");
	std::wstring installPath;
	std::getline(file, installPath);
    if (installPath[installPath.size() - 1]=='\\') {
        installPath += L"MyDockFinder";
    }
    else {
        installPath += L"\\MyDockFinder";
    }
    file.close();
    
    
    INFO_(L"[mydockfinder安装器]关闭mydockfinder相关");
    killapp(L"trayico.exe");
    killapp(L"dockmod64arm_update.exe");
    killapp(L"UiAccess.exe");
    killapp(L"Mydock_update.exe");
    killapp(L"temp_64.exe");
    killapp(L"Temperature.exe");
    killapp(L"temp.exe");
    killapp(L"ScreenRound.exe");
    killapp(L"Dock_64.exe");
    killapp(L"Dockmod_update.exe");
    killapp(L"Dockmod64_update.exe");
    killapp(L"dock.exe");
    killapp(L"ApplicationFrameHost.exe");

    INFO_(L"[mydockfinder卸载组件]删除文件");
    //删文件
    TakeOwnershipAndGrantFullControl(installPath);

    std::filesystem::remove_all(installPath);
    INFO_(L"[mydockfinder卸载组件]删除配置文件");
    //删配置
    TakeOwnershipAndGrantFullControl(L"C:\\Windows\\macdowsOStool\\MyDockFinderPath.txt");

    std::filesystem::remove_all(L"C:\\Windows\\macdowsOStool\\MyDockFinderPath.txt");
    INFO_(L"[mydockfinder卸载组件]删除自启动项");
    PWSTR startupPath = nullptr;
    if (SUCCEEDED(SHGetKnownFolderPath(
        FOLDERID_Startup, 0, nullptr, &startupPath))) {
        std::wstring shortcut =
            std::wstring(startupPath) + L"\\MyDockFinder.lnk";
        TakeOwnershipAndGrantFullControl(shortcut);

        DeleteFileW(shortcut.c_str());
        CoTaskMemFree(startupPath);
    }

    INFO_(L"[mydockfinder卸载组件]删除自启动项");
    HKEY key = nullptr;
    if (RegOpenKeyExW(
        HKEY_LOCAL_MACHINE,
        L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\PersonalizationCSP",
        0, KEY_WRITE, &key) == ERROR_SUCCESS) {
        RegDeleteValueW(key, L"LockScreenImagePath");
        RegDeleteValueW(key, L"LockScreenImageStatus");
        RegCloseKey(key);
    }
    INFO_(L"[mydockfinder卸载组件]结束");
    return;
}

}
