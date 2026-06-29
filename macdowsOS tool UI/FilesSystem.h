#pragma once
// ============================================================
// FilesSystem.h — 文件系统工具函数
// 提供文件复制、文件夹选择、文件存在性检查等功能
// ============================================================

#include <string>
#include <vector>
#include <windows.h>
#include <shlobj_core.h>
#include <fileapi.h>

// 文件系统命名空间
namespace fs {
    inline bool exists(const std::wstring& path) {
        DWORD attr = GetFileAttributesW(path.c_str());
        return attr != INVALID_FILE_ATTRIBUTES;
    }
}

// 递归复制目录/文件
inline void copyPath(const std::wstring& src, const std::wstring& dst) {
    // 检查源路径是否存在
    DWORD srcAttr = GetFileAttributesW(src.c_str());
    if (srcAttr == INVALID_FILE_ATTRIBUTES) return;

    // 如果是目录
    if (srcAttr & FILE_ATTRIBUTE_DIRECTORY) {
        // 创建目标目录
        CreateDirectoryW(dst.c_str(), NULL);

        // 枚举源目录中的文件
        WIN32_FIND_DATAW findData;
        HANDLE hFind = FindFirstFileW((src + L"\\*").c_str(), &findData);
        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                if (wcscmp(findData.cFileName, L".") == 0 ||
                    wcscmp(findData.cFileName, L"..") == 0) continue;

                std::wstring subSrc = src + L"\\" + findData.cFileName;
                std::wstring subDst = dst + L"\\" + findData.cFileName;
                copyPath(subSrc, subDst);
            } while (FindNextFileW(hFind, &findData));
            FindClose(hFind);
        }
    } else {
        // 复制文件
        CopyFileW(src.c_str(), dst.c_str(), FALSE);
    }
}

// 浏览文件夹对话框
inline std::wstring BrowseFolder(const std::wstring& title) {
    std::wstring result;
    BROWSEINFOW bi = { 0 };
    bi.lpszTitle = title.c_str();
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

    LPITEMIDLIST pidl = SHBrowseForFolderW(&bi);
    if (pidl != NULL) {
        wchar_t path[MAX_PATH];
        if (SHGetPathFromIDListW(pidl, path)) {
            result = path;
        }
        CoTaskMemFree(pidl);
    }
    return result;
}
