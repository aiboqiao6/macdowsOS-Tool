#pragma once

#include <Windows.h>
#include <tlhelp32.h>
#include <shlobj_core.h>
#include <string>
#include <algorithm>

#include "LogSystem.h"

// 静默运行批处理文件
inline bool RunBatchSilentlyNative(const std::wstring& batchFilePath) {
    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    std::wstring cmdLine = L"cmd /c \"" + batchFilePath + L"\"";
    if (!CreateProcessW(NULL, &cmdLine[0], NULL, NULL, FALSE,
        CREATE_NO_WINDOW | CREATE_NEW_CONSOLE,
        NULL, NULL, &si, &pi)) {
        return false;
    }
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return true;
}

// 终止进程
inline void killapp(const std::wstring& appname) {
    MESSAGE_(L"[macdowsOS Tool] 正在终止进程:", appname);
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        ERROR_(L"[macdowsOS Tool] 创建进程快照时出现错误");
        MessageBoxW(NULL, L" 终止进程时出现错误 错误1", L" macdowsOS tool 进程管理", MB_OK | MB_ICONERROR);
        return;
    }
    PROCESSENTRY32W pe32{};
    pe32.dwSize = sizeof(PROCESSENTRY32W);
    if (!Process32FirstW(hSnapshot, &pe32)) {
        ERROR_(L"[macdowsOS Tool] 进程信息为空");
        MessageBoxW(NULL, L" 终止进程时出现错误 错误2", L" macdowsOS tool 进程管理", MB_OK | MB_ICONERROR);
        CloseHandle(hSnapshot);
        return;
    }
    do {
        std::wstring processName(pe32.szExeFile);
        if (processName == appname) {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
            if (hProcess == nullptr) continue;
            TerminateProcess(hProcess, 0);
            CloseHandle(hProcess);
        }
    } while (Process32NextW(hSnapshot, &pe32));
    CloseHandle(hSnapshot);
    INFO_(L"[macdowsOS Tool] 终止完成");
}

// 添加开机自启动
inline void AddAutoStart(const std::wstring& targetPath, const std::wstring& shortcutName) {
    std::wstring normalizedTarget = targetPath;
    std::replace(normalizedTarget.begin(), normalizedTarget.end(), L'/', L'\\');

    PWSTR startupPath = nullptr;
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_Startup, 0, nullptr, &startupPath);
    if (FAILED(hr) || !startupPath) {
        ERROR_(L"[macdowsOS Tool] 添加开机自启动时出现错误 无法获取startup文件夹路径");
        MessageBoxW(NULL, L" 添加开机自启动时出现错误 错误1", L" macdowsOS tool 进程管理", MB_OK | MB_ICONERROR);
        return;
    }
    std::wstring shortcutPath = startupPath;
    shortcutPath += L"\\";
    shortcutPath += shortcutName;
    shortcutPath += L".lnk";
    std::replace(shortcutPath.begin(), shortcutPath.end(), L'/', L'\\');
    CoTaskMemFree(startupPath);

    hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(hr) && hr != RPC_E_CHANGED_MODE) {
        ERROR_(L"[macdowsOS Tool] 添加开机自启动时出现错误 无法初始化COM");
        MessageBoxW(NULL, L" 添加开机自启动时出现错误 错误2", L" macdowsOS tool 进程管理", MB_OK | MB_ICONERROR);
        return;
    }
    bool success = false;
    IShellLinkW* pShellLink = nullptr;
    IPersistFile* pPersistFile = nullptr;
    do {
        hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER,
            IID_IShellLinkW, reinterpret_cast<LPVOID*>(&pShellLink));
        if (FAILED(hr) || !pShellLink) break;

        hr = pShellLink->SetPath(normalizedTarget.c_str());
        if (FAILED(hr)) break;

        size_t lastSlash = normalizedTarget.find_last_of(L'\\');
        if (lastSlash != std::wstring::npos) {
            std::wstring workingDir = normalizedTarget.substr(0, lastSlash);
            pShellLink->SetWorkingDirectory(workingDir.c_str());
        }
        hr = pShellLink->QueryInterface(IID_IPersistFile, reinterpret_cast<LPVOID*>(&pPersistFile));
        if (FAILED(hr) || !pPersistFile) break;

        hr = pPersistFile->Save(shortcutPath.c_str(), TRUE);
        success = SUCCEEDED(hr);
    } while (false);

    if (pPersistFile) pPersistFile->Release();
    if (pShellLink) pShellLink->Release();
    if (hr == S_OK) CoUninitialize();
}

// 设置桌面图标可见性
inline void SetDesktopIconsPermanent(bool show) {
    HKEY hKey = NULL;
    DWORD dwValue = show ? 0 : 1;
    DWORD dwSize = sizeof(DWORD);
    LONG result = RegCreateKeyExW(HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced",
        0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
    if (result != ERROR_SUCCESS) {
        ERROR_(L"[macdowsOS Tool] 修改注册表时出现错误 无法打开注册表项");
        return;
    }
    result = RegSetValueExW(hKey, L"HideIcons", 0, REG_DWORD,
        (const BYTE*)&dwValue, dwSize);
    RegCloseKey(hKey);
    if (result != ERROR_SUCCESS) {
        ERROR_(L"[macdowsOS Tool] 修改注册表时出现错误 无法修改注册表值");
    }
}
