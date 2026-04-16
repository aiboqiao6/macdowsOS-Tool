#pragma once
#include<bits/stdc++.h>
#include<Windows.h>
#include"LogSystem.h"
#include <shellapi.h> 
#include <tlhelp32.h> 
#include <tchar.h>  
#include <shlobj_core.h>
#include"WindowControl.h"
using namespace std;
//运行
bool RunBatchSilentlyNative(const std::wstring& batchFilePath) {
    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    // 准备命令行：cmd /c "你的批处理文件"
    std::wstring cmdLine = L"cmd /c \"" + batchFilePath + L"\"";

    // 创建进程，但先不显示窗口
    if (!CreateProcessW(NULL, &cmdLine[0], NULL, NULL, FALSE,
        CREATE_NO_WINDOW | CREATE_NEW_CONSOLE,
        NULL, NULL, &si, &pi)) {
        return false;
    }

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return true;
}
// 结束进程
void killapp(wstring appname) {
    MESSAGE_(L"[macdowsOS Tool辅助组件]结束进程",appname);
    //创建进程快照
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        ERROR_(L"[macdowsOS Tool辅助组件]创建进程快照时出现错误");
        MessageBox(NULL, (LPCTSTR)L" 结束进程时出现错误 错误1", (LPCTSTR)L" macdowsOS tool 辅助组件", MB_OK | MB_ICONERROR);
        return;
    }
    PROCESSENTRY32W pe32{};
    pe32.dwSize = sizeof(PROCESSENTRY32W);  // 必须初始化大小
    //获取第一个进程信息
    if (!Process32FirstW(hSnapshot, &pe32)) {
        ERROR_(L"[macdowsOS Tool辅助组件]进程信息为空");
        MessageBox(NULL, (LPCTSTR)L" 结束进程时出现错误 错误2", (LPCTSTR)L" macdowsOS tool 辅助组件", MB_OK | MB_ICONERROR);
        CloseHandle(hSnapshot);  // 释放快照句柄
        return;
    }
    //遍历所有进程，筛选
    do {
        // 进程名转为字符串
        wstring processName(pe32.szExeFile);
        if (processName == appname) {
            //打开进程
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
            if (hProcess == nullptr) {
                continue;
            }
            //终止进程
            if (TerminateProcess(hProcess, 0)) {
                ;
            }
            // 释放进程句柄
            CloseHandle(hProcess);
        }
    } while (Process32NextW(hSnapshot, &pe32));  // 遍历下一个进程
    // 释放快照句柄
    CloseHandle(hSnapshot);
    INFO_(L"[macdowsOS Tool辅助组件]返回");
    return;
}

// 添加自启动项
void AddAutoStart(const std::wstring& targetPath, const std::wstring& shortcutName) {
   // 统一路径分隔符为 Windows 标准格式
    wstring normalizedTarget = targetPath;
    replace(normalizedTarget.begin(), normalizedTarget.end(), L'/', L'\\');
    // 获取 Startup 文件夹路径
    PWSTR startupPath = nullptr;
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_Startup, 0, nullptr, &startupPath);
    if (FAILED(hr) || !startupPath) {
        ERROR_(L"[macdowsOS Tool辅助组件]创建自启动项时出现错误 无法获取startup文件夹路径");
        MessageBox(NULL, (LPCTSTR)L" 创建自启动项时出现错误 错误1", (LPCTSTR)L" macdowsOS tool 辅助组件", MB_OK | MB_ICONERROR);
        return;
    }
    // 构建完整的快捷方式路径
    wstring shortcutPath = startupPath;
    shortcutPath += L"\\";
    shortcutPath += shortcutName;
    shortcutPath += L".lnk";
    // 统一快捷方式路径的分隔符
    replace(shortcutPath.begin(), shortcutPath.end(), L'/', L'\\');
    CoTaskMemFree(startupPath);
    // 初始化 COM（单线程）
    hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(hr) && hr != RPC_E_CHANGED_MODE) {
        ERROR_(L"[macdowsOS Tool辅助组件]创建自启动项时出现错误 无法初始化COM");
        MessageBox(NULL, (LPCTSTR)L" 创建自启动项时出现错误 错误2", (LPCTSTR)L" macdowsOS tool 辅助组件", MB_OK | MB_ICONERROR);
        return;
    }
    bool success = false;
    IShellLink* pShellLink = nullptr;
    IPersistFile* pPersistFile = nullptr;
    do {
        // 创建 IShellLink 对象
        hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER,
            IID_IShellLink, reinterpret_cast<LPVOID*>(&pShellLink));
        if (FAILED(hr) || !pShellLink)
            break;
        // 设置目标路径
        hr = pShellLink->SetPath(normalizedTarget.c_str());
        if (FAILED(hr))
            break;
        // 设置工作目录为目标程序所在目录
        size_t lastSlash = normalizedTarget.find_last_of(L'\\');
        if (lastSlash != std::wstring::npos)
        {
            std::wstring workingDir = normalizedTarget.substr(0, lastSlash);
            pShellLink->SetWorkingDirectory(workingDir.c_str());
        }
        // 获取 IPersistFile 接口
        hr = pShellLink->QueryInterface(IID_IPersistFile, reinterpret_cast<LPVOID*>(&pPersistFile));
        if (FAILED(hr) || !pPersistFile)
            break;
        // 保存快捷方式
        hr = pPersistFile->Save(shortcutPath.c_str(), TRUE);
        success = SUCCEEDED(hr);
    } while (false);
    // 清理资源
    if (pPersistFile)
        pPersistFile->Release();
    if (pShellLink)
        pShellLink->Release();
    CoUninitialize();
    return;
}

// 设置桌面图标永久显示/隐藏
void SetDesktopIconsPermanent(bool show) {
    HKEY hKey = NULL;
    DWORD dwValue = show ? 0 : 1;  // 0=显示，1=隐藏
    DWORD dwSize = sizeof(DWORD);
    // 打开或创建注册表项
    LONG result = RegCreateKeyEx(HKEY_CURRENT_USER,(L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced"),0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&hKey,NULL);
    if (result != ERROR_SUCCESS) {
        ERROR_(L"[macdowsOS Tool辅助组件]修改注册表时出现错误 无法创建注册表项");
        MessageBox(NULL, (LPCTSTR)L" 修改桌面图标可见性时出现错误 错误1", (LPCTSTR)L" macdowsOS tool 辅助组件", MB_OK | MB_ICONERROR);
        return;
    }
    // 设置 HideIcons 值
    result = RegSetValueEx(hKey,(L"HideIcons"),0,REG_DWORD,(const BYTE*)&dwValue,dwSize);
    RegCloseKey(hKey);
    if (result != ERROR_SUCCESS) {
        ERROR_(L"[macdowsOS Tool辅助组件]修改注册表时出现错误 无法修改注册表值");
        MessageBox(NULL, (LPCTSTR)L" 修改桌面图标可见性时出现错误 错误2", (LPCTSTR)L" macdowsOS tool 辅助组件", MB_OK | MB_ICONERROR);
        return;
    }
    return;
}