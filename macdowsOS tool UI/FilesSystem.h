#pragma once
#include <filesystem>
#include <iostream>
#include <string>
#include <shobjidl.h>
#include <chrono>
#include <thread>
#include <Windows.h>
#include <vector>
#include <shlwapi.h>
#include <psapi.h>
#include <restartmanager.h>
#include <tlhelp32.h>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <cwchar>
#include <AclAPI.h>
#include <sddl.h>
#include <memory>
#include <format>
#include <stdexcept>
#include <span>
#include <utility>
#include "LogSystem.h"
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "Rstrtmgr.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "psapi.lib")

namespace fs = std::filesystem;
using namespace std;

// ===================== 工具函数：char* 转 wstring =====================
inline std::wstring CharToWstring(const char* str) {
    if (str == nullptr) return L"";
    int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
    std::wstring wstr(len, 0);
    MultiByteToWideChar(CP_UTF8, 0, str, -1, &wstr[0], len);
    return wstr;
}

// ===================== 工具函数：通过进程ID获取进程名（不依赖RM_PROCESS_INFO） =====================
inline std::wstring GetProcessNameById(DWORD dwProcessId) {
    std::wstring procName = L"unknown_process";

    // 打开进程（仅获取信息权限）
    HANDLE hProcess = OpenProcess(
        PROCESS_QUERY_LIMITED_INFORMATION,
        FALSE,
        dwProcessId
    );

    if (hProcess != NULL) {
        WCHAR szImagePath[MAX_PATH] = { 0 };
        DWORD dwSize = MAX_PATH;

        // 获取进程完整路径
        if (QueryFullProcessImageNameW(
            hProcess,
            0,
            szImagePath,
            &dwSize
        )) {
            // 提取文件名（如 C:\Windows\explorer.exe → explorer.exe）
            fs::path path(szImagePath);
            procName = path.filename().wstring();
        }
        CloseHandle(hProcess);
    }

    return procName;
}

// ===================== 工具函数：检查是否以管理员身份运行 =====================
bool IsRunAsAdmin() {
    BOOL bElevated = FALSE;
    HANDLE hToken = NULL;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION elevation;
        DWORD dwSize = sizeof(TOKEN_ELEVATION);
        if (GetTokenInformation(hToken, TokenElevation, &elevation, dwSize, &dwSize)) {
            bElevated = elevation.TokenIsElevated;
        }
        CloseHandle(hToken);
    }
    return bElevated == TRUE;
}

// ===================== 工具函数：重启程序并提权 =====================
void RelaunchAsAdmin() {
    WCHAR szPath[MAX_PATH] = { 0 };
    if (GetModuleFileNameW(NULL, szPath, ARRAYSIZE(szPath))) {
        SHELLEXECUTEINFOW sei = { sizeof(sei) };
        sei.lpVerb = L"runas";          // 以管理员身份运行
        sei.lpFile = szPath;            // 当前程序路径
        sei.hwnd = NULL;
        sei.nShow = SW_SHOWNORMAL;      // 正常显示窗口

        if (!ShellExecuteExW(&sei)) {
            DWORD dwErr = GetLastError();
            ERROR_(L"提权失败，错误码: " + to_wstring(dwErr));
            if (dwErr == ERROR_CANCELLED) {
                ERROR_(L"用户拒绝了管理员权限请求");
            }
        }
    }
}

// ===================== 工具函数：启用指定的系统特权 =====================
bool EnablePrivilege(const wstring& privilegeName) {
    HANDLE hToken = NULL;
    TOKEN_PRIVILEGES tp = { 0 };

    // 打开进程令牌
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        ERROR_(L"OpenProcessToken失败，错误码: " + to_wstring(GetLastError()));
        return false;
    }

    // 获取特权LUID
    if (!LookupPrivilegeValueW(NULL, privilegeName.c_str(), &tp.Privileges[0].Luid)) {
        ERROR_(L"LookupPrivilegeValueW失败，特权: " + privilegeName + L", 错误码: " + to_wstring(GetLastError()));
        CloseHandle(hToken);
        return false;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    // 启用特权
    BOOL bResult = AdjustTokenPrivileges(
        hToken,
        FALSE,
        &tp,
        sizeof(TOKEN_PRIVILEGES),
        NULL,
        NULL
    );

    DWORD dwErr = GetLastError();
    CloseHandle(hToken);

    if (!bResult || dwErr != ERROR_SUCCESS) {
        ERROR_(L"AdjustTokenPrivileges失败，特权: " + privilegeName + L", 错误码: " + to_wstring(dwErr));
        return false;
    }

    INFO_(L"成功启用特权: " + privilegeName);
    return true;
}

// ===================== 工具函数：解除文件占用（完全移除strProcessName依赖） =====================
bool ReleaseFileLock(const wstring& filePath) {
    DWORD dwSessionHandle = 0;
    WCHAR szSessionKey[CCH_RM_SESSION_KEY + 1] = { 0 };

    // 创建重启管理器会话
    DWORD dwResult = RmStartSession(&dwSessionHandle, 0, szSessionKey);
    if (dwResult != ERROR_SUCCESS) {
        ERROR_(L"RmStartSession失败，错误码: " + to_wstring(dwResult));
        return false;
    }

    // 添加要释放的文件
    PCWSTR pszFilePaths[] = { filePath.c_str() };
    dwResult = RmRegisterResources(dwSessionHandle, 1, pszFilePaths, 0, NULL, 0, NULL);
    if (dwResult != ERROR_SUCCESS) {
        ERROR_(L"RmRegisterResources失败，错误码: " + to_wstring(dwResult));
        RmEndSession(dwSessionHandle);
        return false;
    }

    // 获取占用文件的进程
    UINT nProcInfoNeeded = 0;
    UINT nProcInfo = 0;
    RM_PROCESS_INFO* pProcessInfo = NULL;
    dwResult = RmGetList(dwSessionHandle, &nProcInfoNeeded, &nProcInfo, NULL, NULL);

    if (dwResult == ERROR_MORE_DATA) {
        pProcessInfo = (RM_PROCESS_INFO*)LocalAlloc(LPTR, nProcInfoNeeded * sizeof(RM_PROCESS_INFO));
        if (!pProcessInfo) {
            ERROR_(L"LocalAlloc失败，无法获取进程信息");
            RmEndSession(dwSessionHandle);
            return false;
        }

        nProcInfo = nProcInfoNeeded;
        dwResult = RmGetList(dwSessionHandle, &nProcInfoNeeded, &nProcInfo, pProcessInfo, NULL);

        if (dwResult == ERROR_SUCCESS) {
            // 终止占用文件的非系统关键进程（仅终止explorer.exe）
            for (UINT i = 0; i < nProcInfo; i++) {
                // 核心修复：完全通过进程ID获取进程名，不访问RM_PROCESS_INFO的任何进程名成员
                DWORD dwPid = pProcessInfo[i].Process.dwProcessId;
                std::wstring procName = GetProcessNameById(dwPid);

                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwPid);
                if (hProcess) {
                    // 只终止 explorer.exe（文件资源管理器）
                    if (procName.find(L"explorer.exe") != wstring::npos) {
                        INFO_(L"终止占用文件的进程: " + procName + L" (PID: " + to_wstring(dwPid) + L")");
                        TerminateProcess(hProcess, 0);
                        WaitForSingleObject(hProcess, 3000); // 等待3秒
                    }
                    CloseHandle(hProcess);
                }
            }
        }
        LocalFree(pProcessInfo);
    }

    RmEndSession(dwSessionHandle);
    return dwResult == ERROR_SUCCESS;
}

// ===================== 核心复制函数：修复权限处理 + 提权逻辑 =====================
void copyPath(const std::wstring& A, const std::wstring& B) {
    namespace fs = std::filesystem;
    HANDLE hToken = nullptr;
    PTOKEN_USER pTokenUser = nullptr;
    PACL pNewDACL = nullptr;
    PSID pSid = nullptr;
    wchar_t* pDstPath = nullptr;
    PVOID OldValue = nullptr;
    bool bWow64RedirectDisabled = false;

    // ========== 核心工具：标准化路径（修复C盘根目录解析问题） ==========
    auto normalizePath = [](const std::wstring& rawPath) -> fs::path {
        std::wstring tempPath = rawPath;
        std::replace(tempPath.begin(), tempPath.end(), L'/', L'\\');

        // 关键修复1：保留盘符后的根目录反斜杠（处理 C: → C:\）
        if (tempPath.size() == 2 && tempPath[1] == L':' && tempPath[0] >= L'A' && tempPath[0] <= L'Z') {
            tempPath += L'\\'; // C: → C:\
                    }
                    // 处理 C:/ → C:\
                    else if (tempPath.size() >= 3 && tempPath[1] == L':' && tempPath[2] == L'\\') {
                        // 已是根目录，无需修改
        }
        // 处理 C:\xxx → 保留结构
        else if (tempPath.size() >= 3 && tempPath[1] == L':' && tempPath[2] != L'\\') {
            // 非根目录，正常处理
        }

        // 合并连续反斜杠（避免 C:\\\\xxx → C:\xxx）
        std::wstring normalized;
        bool lastIsSlash = false;
        for (wchar_t c : tempPath) {
            if (c == L'\\') {
                if (!lastIsSlash) {
                    normalized += c;
                    lastIsSlash = true;
                }
            }
            else {
                normalized += c;
                lastIsSlash = false;
            }
        }

        // 关键修复2：根目录不删除末尾反斜杠（C:\ → 保留，而非 C:）
        bool isRootDir = false;
        if (normalized.size() == 3 && normalized[1] == L':' && normalized[2] == L'\\') {
            isRootDir = true; // 匹配 C:\、D:\ 等根目录
        }
        // 非根目录才删除末尾反斜杠
        if (!isRootDir && !normalized.empty() && normalized.back() == L'\\') {
            normalized.pop_back();
        }

        // 处理开头多余反斜杠（仅非根目录时）
        if (!isRootDir && !normalized.empty() && normalized[0] == L'\\' &&
            !(normalized.size() >= 2 && normalized[1] == L':')
            ) {
            normalized = normalized.substr(1);
        }

        fs::path stdPath(normalized);
        stdPath = stdPath.lexically_normal();

        INFO_(L"路径标准化完成 - 原始: " + rawPath + L", 标准化: " + stdPath.wstring());
        return stdPath;
        };

    try {
        // ========== 第一步：检查并提权 ==========
        if (!IsRunAsAdmin()) {
            INFO_(L"当前进程无管理员权限，尝试提权...");
            RelaunchAsAdmin();
            return; // 提权后重启程序，当前调用退出
        }

        // 启用修改系统文件必需的特权
        EnablePrivilege(SE_BACKUP_NAME);
        EnablePrivilege(SE_RESTORE_NAME);
        EnablePrivilege(SE_TAKE_OWNERSHIP_NAME);
        EnablePrivilege(SE_SECURITY_NAME);

        // ========== 第二步：标准化路径 ==========
        fs::path srcPath = normalizePath(A);
        fs::path dstDirPath = normalizePath(B);

        // ========== 禁用WOW64重定向（记录状态，确保后续恢复） ==========
        if (Wow64DisableWow64FsRedirection(&OldValue)) {
            bWow64RedirectDisabled = true;
        }

        // ========== 转为绝对路径 ==========
        if (srcPath.is_relative()) {
            srcPath = fs::absolute(srcPath);
            INFO_(L"源路径转为绝对路径: " + srcPath.wstring());
        }
        if (dstDirPath.is_relative()) {
            dstDirPath = fs::absolute(dstDirPath);
            INFO_(L"目标文件夹转为绝对路径: " + dstDirPath.wstring());
        }

        // 前置检查：源文件存在
        if (!fs::exists(srcPath)) {
            wstring errMsg = L"源路径不存在: " + srcPath.wstring();
            ERROR_(errMsg);
            throw runtime_error("Source path does not exist");
        }

        // ========== 拼接目标完整路径 ==========
        fs::path dstFullPath = dstDirPath / srcPath.filename();
        if (!fs::exists(dstDirPath)) {
            fs::create_directories(dstDirPath);
            INFO_(L"目标文件夹创建成功: " + dstDirPath.wstring());
        }

        INFO_(L"copyPath函数启动 - 源路径: " + srcPath.wstring() + L", 目标文件夹: " + dstDirPath.wstring());
        INFO_(L"最终目标路径（正确拼接）: " + dstFullPath.wstring());

        // ========== 第三步：处理目标文件（删除/改权限） ==========
        if (fs::exists(dstFullPath)) {
            INFO_(L"目标路径已存在，尝试删除: " + dstFullPath.wstring());
            BOOL bDeleteSuccess = FALSE;

            try {
                // 先尝试普通删除
                if (fs::is_directory(dstFullPath)) {
                    fs::remove_all(dstFullPath);
                }
                else {
                    fs::remove(dstFullPath);
                }
                bDeleteSuccess = TRUE;
                INFO_(L"目标路径删除成功: " + dstFullPath.wstring());
            }
            catch (const fs::filesystem_error& e) {
                // 修复：char* 转 wstring
                if (e.code().value() == ERROR_ACCESS_DENIED || e.code().value() == ERROR_SHARING_VIOLATION) {
                    INFO_(L"删除失败（权限/占用），尝试：1.解除文件占用 2.修改权限");

                    // 第一步：解除文件占用
                    ReleaseFileLock(dstFullPath.wstring());

                    // 第二步：修改文件所有权
                    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
                        DWORD dwErr = GetLastError();
                        ERROR_(L"OpenProcessToken失败，错误码: " + to_wstring(dwErr));
                        throw runtime_error("OpenProcessToken failed");
                    }

                    DWORD dwSize = 0;
                    GetTokenInformation(hToken, TokenUser, nullptr, 0, &dwSize);
                    pTokenUser = (PTOKEN_USER)LocalAlloc(LPTR, dwSize);
                    if (!pTokenUser) {
                        ERROR_(L"LocalAlloc分配TokenUser内存失败");
                        CloseHandle(hToken);
                        hToken = nullptr;
                        throw runtime_error("LocalAlloc failed for TOKEN_USER");
                    }

                    if (!GetTokenInformation(hToken, TokenUser, pTokenUser, dwSize, &dwSize)) {
                        DWORD dwErr = GetLastError();
                        ERROR_(L"GetTokenInformation失败，错误码: " + to_wstring(dwErr));
                        LocalFree(pTokenUser);
                        pTokenUser = nullptr;
                        CloseHandle(hToken);
                        hToken = nullptr;
                        throw runtime_error("GetTokenInformation failed");
                    }
                    pSid = pTokenUser->User.Sid;
                    CloseHandle(hToken);
                    hToken = nullptr;
                    INFO_(L"成功获取当前进程SID");

                    // 修改所有权（关键：系统文件需要先获取所有权）
                    pDstPath = const_cast<wchar_t*>(dstFullPath.c_str());
                    DWORD dwResult = SetNamedSecurityInfoW(
                        pDstPath,
                        SE_FILE_OBJECT,
                        OWNER_SECURITY_INFORMATION | WRITE_OWNER, // 增加WRITE_OWNER标志
                        pSid,
                        nullptr,
                        nullptr,
                        nullptr
                    );
                    if (dwResult != ERROR_SUCCESS) {
                        ERROR_(L"SetNamedSecurityInfoW(所有权)失败，错误码: " + to_wstring(dwResult));
                        LocalFree(pTokenUser);
                        pTokenUser = nullptr;
                        throw runtime_error("SetNamedSecurityInfoW failed (Owner)");
                    }
                    INFO_(L"成功修改目标路径所有权: " + dstFullPath.wstring());

                    // 设置完全控制权限
                    EXPLICIT_ACCESS ea = { 0 };
                    ea.grfAccessPermissions = GENERIC_ALL;
                    ea.grfAccessMode = SET_ACCESS;
                    ea.grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
                    ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
                    ea.Trustee.TrusteeType = TRUSTEE_IS_USER;
                    ea.Trustee.ptstrName = const_cast<LPWSTR>(reinterpret_cast<LPCWSTR>(pSid));

                    dwResult = SetEntriesInAclW(1, &ea, nullptr, &pNewDACL);
                    if (dwResult != ERROR_SUCCESS) {
                        ERROR_(L"SetEntriesInAclW失败，错误码: " + to_wstring(dwResult));
                        LocalFree(pTokenUser);
                        pTokenUser = nullptr;
                        throw runtime_error("SetEntriesInAclW failed");
                    }

                    dwResult = SetNamedSecurityInfoW(
                        pDstPath,
                        SE_FILE_OBJECT,
                        DACL_SECURITY_INFORMATION | WRITE_DAC, // 增加WRITE_DAC标志
                        nullptr,
                        nullptr,
                        pNewDACL,
                        nullptr
                    );
                    if (dwResult != ERROR_SUCCESS) {
                        ERROR_(L"SetNamedSecurityInfoW(ACL)失败，错误码: " + to_wstring(dwResult));
                        LocalFree(pNewDACL);
                        pNewDACL = nullptr;
                        LocalFree(pTokenUser);
                        pTokenUser = nullptr;
                        throw runtime_error("SetNamedSecurityInfoW failed (DACL)");
                    }
                    INFO_(L"成功设置目标路径完全控制权限: " + dstFullPath.wstring());

                    // 重新删除
                    if (fs::is_directory(dstFullPath)) {
                        fs::remove_all(dstFullPath);
                    }
                    else {
                        fs::remove(dstFullPath);
                    }
                    if (!fs::exists(dstFullPath)) {
                        bDeleteSuccess = TRUE;
                        INFO_(L"修改权限后目标路径删除成功: " + dstFullPath.wstring());
                    }
                    else {
                        ERROR_(L"修改权限后仍无法删除目标路径");
                        LocalFree(pNewDACL);
                        pNewDACL = nullptr;
                        LocalFree(pTokenUser);
                        pTokenUser = nullptr;
                        throw runtime_error("Failed to delete path after permission change");
                    }

                    // 释放内存
                    LocalFree(pNewDACL);
                    pNewDACL = nullptr;
                    LocalFree(pTokenUser);
                    pTokenUser = nullptr;
                }
                else {
                    // 修复：char* 转 wstring
                    std::wstring errWhat = CharToWstring(e.what());
                    ERROR_(L"删除失败（非权限问题）: " + errWhat + L", 错误码: " + to_wstring(e.code().value()));
                    throw;
                }
            }

            if (!bDeleteSuccess) {
                ERROR_(L"目标路径删除最终失败: " + dstFullPath.wstring());
                throw runtime_error("Failed to delete target path");
            }
        }
        else {
            INFO_(L"目标路径不存在，无需删除: " + dstFullPath.wstring());
        }

        // ========== 第四步：执行复制 ==========
        INFO_(L"开始复制 - 源: " + srcPath.wstring() + L", 目标: " + dstFullPath.wstring());
        if (fs::is_directory(srcPath)) {
            fs::copy(srcPath, dstFullPath, fs::copy_options::recursive | fs::copy_options::overwrite_existing | fs::copy_options::copy_symlinks);
            MESSAGE_(L"文件夹复制完成", srcPath.wstring());
        }
        else {
            fs::copy(srcPath, dstFullPath, fs::copy_options::overwrite_existing | fs::copy_options::copy_symlinks);
            MESSAGE_(L"文件复制完成", srcPath.wstring());
        }
        INFO_(L"复制成功: " + srcPath.wstring() + L" -> " + dstFullPath.wstring());

    }
    catch (const std::exception& e) {
        // 修复：char* 转 wstring
        std::wstring errWhat = CharToWstring(e.what());
        wstring errMsg = L"copyPath执行异常: " + errWhat;
        ERROR_(errMsg);
    }
    catch (...) {
        ERROR_(L"copyPath发生未知异常（无类型），可能是内存错误或API调用失败");
    }

    // ========== 最终清理：必做 ==========
    // 恢复WOW64重定向
    if (bWow64RedirectDisabled) {
        Wow64RevertWow64FsRedirection(OldValue);
        bWow64RedirectDisabled = false;
        INFO_(L"WOW64重定向已恢复");
    }
    // 释放资源
    if (hToken) CloseHandle(hToken);
    if (pTokenUser) LocalFree(pTokenUser);
    if (pNewDACL) LocalFree(pNewDACL);

    INFO_(L"copyPath函数执行完成");
}

// ===================== 弹窗选择文件夹 =====================
wstring BrowseFolder(const wstring& title) {
    wstring result;
    INFO_(L"[文件操作组件]组件运行");

    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) {
        ERROR_(L"[文件操作组件]无法初始化 退出，错误码: " + to_wstring(hr));
        return result;
    }

    IFileDialog* pFileDialog = nullptr;
    INFO_(L"[文件操作组件]创建对话框");

    hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_IFileDialog, reinterpret_cast<void**>(&pFileDialog));

    if (SUCCEEDED(hr) && pFileDialog) {
        INFO_(L"[文件操作组件]设置对话框信息");
        DWORD options;
        pFileDialog->GetOptions(&options);
        pFileDialog->SetOptions(options | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM); // 仅选择文件系统文件夹

        pFileDialog->SetTitle(title.c_str());
        INFO_(L"[文件操作组件]显示对话框");

        hr = pFileDialog->Show(nullptr);
        if (SUCCEEDED(hr)) {
            IShellItem* pShellItem = nullptr;
            hr = pFileDialog->GetResult(&pShellItem);
            if (SUCCEEDED(hr) && pShellItem) {
                wchar_t* pszFilePath = nullptr;
                hr = pShellItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
                if (SUCCEEDED(hr) && pszFilePath) {
                    result = pszFilePath;
                    CoTaskMemFree(pszFilePath);
                    INFO_(L"[文件操作组件]选择的文件夹: " + result);
                }
                pShellItem->Release();
            }
        }
        else {
            ERROR_(L"[文件操作组件]对话框显示失败，错误码: " + to_wstring(hr));
        }
        pFileDialog->Release();
    }
    else {
        ERROR_(L"[文件操作组件]创建对话框失败，错误码: " + to_wstring(hr));
    }

    INFO_(L"[文件操作组件]回传路径 退出");
    CoUninitialize();
    return result;
}