#pragma once

#include <Windows.h>
#include <srrestoreptapi.h>
#include <string>

#include "LogSystem.h"

#pragma comment(lib, "SrClient.lib")

namespace {

bool EnableSystemRestore() {
    wchar_t systemDirectory[MAX_PATH]{};
    if (!GetSystemDirectoryW(systemDirectory,
                             static_cast<UINT>(_countof(systemDirectory)))) {
        MESSAGE_(L"[还原工具]无法获取系统目录，错误码: ", GetLastError());
        return false;
    }

    const std::wstring powershell =
        std::wstring(systemDirectory) +
        L"\\WindowsPowerShell\\v1.0\\powershell.exe";
    std::wstring command =
        L"\"" + powershell +
        L"\" -NoLogo -NoProfile -NonInteractive -ExecutionPolicy Bypass "
        L"-Command \"Enable-ComputerRestore -Drive 'C:\\' -ErrorAction Stop\"";

    STARTUPINFOW startupInfo{};
    startupInfo.cb = sizeof(startupInfo);
    PROCESS_INFORMATION processInfo{};
    if (!CreateProcessW(nullptr, command.data(), nullptr, nullptr, FALSE,
                        CREATE_NO_WINDOW, nullptr, nullptr, &startupInfo,
                        &processInfo)) {
        MESSAGE_(L"[还原工具]无法启动系统保护启用程序，错误码: ",
                 GetLastError());
        return false;
    }

    const DWORD waitResult = WaitForSingleObject(processInfo.hProcess, 120000);
    DWORD exitCode = ERROR_GEN_FAILURE;
    if (waitResult == WAIT_OBJECT_0) {
        GetExitCodeProcess(processInfo.hProcess, &exitCode);
    } else {
        TerminateProcess(processInfo.hProcess, ERROR_TIMEOUT);
        exitCode = ERROR_TIMEOUT;
    }

    CloseHandle(processInfo.hThread);
    CloseHandle(processInfo.hProcess);

    if (exitCode != ERROR_SUCCESS) {
        MESSAGE_(L"[还原工具]启用系统保护失败，退出码: ", exitCode);
        return false;
    }

    INFO_(L"[还原工具]系统盘保护已启用");
    return true;
}

DWORD BeginRestorePoint(const std::wstring& description,
                        RESTOREPOINTINFOW& restorePoint,
                        STATEMGRSTATUS& status) {
    restorePoint = {};
    restorePoint.dwEventType = BEGIN_SYSTEM_CHANGE;
    restorePoint.dwRestorePtType = APPLICATION_INSTALL;
    wcsncpy_s(restorePoint.szDescription,
              _countof(restorePoint.szDescription), description.c_str(),
              _TRUNCATE);

    status = {};
    SetLastError(ERROR_SUCCESS);
    if (!SRSetRestorePointW(&restorePoint, &status))
        return status.nStatus != ERROR_SUCCESS ? status.nStatus
                                                : GetLastError();
    return status.nStatus;
}

bool CreateSystemRestorePoint(const std::wstring& description) {
    if (description.empty()) {
        ERROR_(L"[还原工具]还原点描述不能为空");
        return false;
    }

    RESTOREPOINTINFOW beginInfo{};
    STATEMGRSTATUS beginStatus{};
    DWORD error = BeginRestorePoint(description, beginInfo, beginStatus);
    if (error == ERROR_SERVICE_DISABLED) {
        INFO_(L"[还原工具]检测到系统保护已关闭，正在启用系统盘保护");
        if (EnableSystemRestore())
            error = BeginRestorePoint(description, beginInfo, beginStatus);
    }

    if (error != ERROR_SUCCESS) {
        MESSAGE_(L"[还原工具]开始创建失败，错误码: ", error);
        return false;
    }

    if (beginStatus.llSequenceNumber <= 0) {
        ERROR_(L"[还原工具]系统未返回有效序列号，可能受到还原点创建频率限制");
        return false;
    }

    RESTOREPOINTINFOW endInfo{};
    endInfo.dwEventType = END_SYSTEM_CHANGE;
    endInfo.dwRestorePtType = APPLICATION_INSTALL;
    endInfo.llSequenceNumber = beginStatus.llSequenceNumber;
    wcsncpy_s(endInfo.szDescription, _countof(endInfo.szDescription),
              description.c_str(), _TRUNCATE);

    STATEMGRSTATUS endStatus{};
    SetLastError(ERROR_SUCCESS);
    if (!SRSetRestorePointW(&endInfo, &endStatus) ||
        endStatus.nStatus != ERROR_SUCCESS) {
        const DWORD error = endStatus.nStatus != ERROR_SUCCESS
            ? endStatus.nStatus
            : GetLastError();
        MESSAGE_(L"[还原工具]完成创建失败，错误码: ", error);

        RESTOREPOINTINFOW cancelInfo = endInfo;
        cancelInfo.dwRestorePtType = CANCELLED_OPERATION;
        STATEMGRSTATUS cancelStatus{};
        SRSetRestorePointW(&cancelInfo, &cancelStatus);
        return false;
    }

    MESSAGE_(L"[还原工具]创建成功，序列号: ",
             beginStatus.llSequenceNumber);
    return true;
}

}
