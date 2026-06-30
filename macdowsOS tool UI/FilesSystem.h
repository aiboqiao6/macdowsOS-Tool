#pragma once
// ============================================================
// FilesSystem.h — 文件系统工具函数
// 提供文件复制、文件夹选择、文件存在性检查等功能
// ============================================================

#include <string>
#include <vector>
#include <windows.h>
#include <shlobj_core.h>
#include <shobjidl.h>
#include <fileapi.h>

// 文件系统命名空间
namespace fs {
    inline bool exists(const std::wstring& path) {
        DWORD attr = GetFileAttributesW(path.c_str());
        return attr != INVALID_FILE_ATTRIBUTES;
    }
}

// 递归创建目录路径中所有不存在的目录
inline bool createDirectories(const std::wstring& path) {
    if (path.empty()) return false;
    // 检查是否已存在
    DWORD attr = GetFileAttributesW(path.c_str());
    if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY))
        return true;

    // 找到上级目录，递归创建
    size_t pos = path.find_last_of(L"/\\");
    if (pos != std::wstring::npos) {
        std::wstring parent = path.substr(0, pos);
        // 跳过根目录（如 D:）
        if (parent.length() > 2 && parent.back() != L':') {
            createDirectories(parent);
        }
    }
    return CreateDirectoryW(path.c_str(), NULL) != 0 ||
           GetLastError() == ERROR_ALREADY_EXISTS;
}

// 内部递归复制 — 直接将 src 内容复制到 dst 下，不额外添加文件夹名
static inline void copyPathImpl(const std::wstring& src, const std::wstring& dst) {
    DWORD srcAttr = GetFileAttributesW(src.c_str());
    if (srcAttr == INVALID_FILE_ATTRIBUTES) return;

    if (srcAttr & FILE_ATTRIBUTE_DIRECTORY) {
        createDirectories(dst);
        WIN32_FIND_DATAW findData;
        HANDLE hFind = FindFirstFileW((src + L"\\*").c_str(), &findData);
        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                if (wcscmp(findData.cFileName, L".") == 0 ||
                    wcscmp(findData.cFileName, L"..") == 0) continue;
                copyPathImpl(src + L"\\" + findData.cFileName,
                             dst + L"\\" + findData.cFileName);
            } while (FindNextFileW(hFind, &findData));
            FindClose(hFind);
        }
    } else {
        // 确保目标文件的父目录存在
        size_t pos = dst.find_last_of(L"/\\");
        if (pos != std::wstring::npos) {
            createDirectories(dst.substr(0, pos));
        }
        CopyFileW(src.c_str(), dst.c_str(), FALSE);
    }
}

// 复制文件/目录到目标路径
// - 若 src 是目录，将整个目录复制到 dst 下（保留文件夹名）
// - 若 src 是文件，将文件复制到 dst 下（保留文件名）
//   例如 copyPath(L"C:\\windows", L"D:\\")   → D:\\windows\\...
//   例如 copyPath(L"C:\\file.txt", L"D:\\") → D:\\file.txt
inline void copyPath(const std::wstring& src, const std::wstring& dst) {
    MESSAGE_(L"[文件操作组件]复制文件从", src);
    MESSAGE_(L"[文件操作组件]到", dst);

    DWORD srcAttr = GetFileAttributesW(src.c_str());
    if (srcAttr == INVALID_FILE_ATTRIBUTES) return;

    if (srcAttr & FILE_ATTRIBUTE_DIRECTORY) {
        // 提取源文件夹名，拼接到目标路径下，形成 dst\folderName
        size_t pos = src.find_last_of(L"/\\");
        std::wstring name = (pos != std::wstring::npos) ? src.substr(pos + 1) : src;
        std::wstring targetDir = dst + L"\\" + name;
        copyPathImpl(src, targetDir);
    } else {
        // 提取源文件名，拼接到目标路径下，形成 dst\fileName
        size_t pos = src.find_last_of(L"/\\");
        std::wstring name = (pos != std::wstring::npos) ? src.substr(pos + 1) : src;
        std::wstring targetFile = dst + L"\\" + name;
        copyPathImpl(src, targetFile);
    }
}

// 现代文件夹选择对话框 (IFileOpenDialog)
inline std::wstring BrowseFolder(const std::wstring& title) {
    std::wstring result;

    HRESULT hrCom = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    IFileOpenDialog *pFileOpen = NULL;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                                  IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

    if (SUCCEEDED(hr)) {
        DWORD dwOptions;
        pFileOpen->GetOptions(&dwOptions);
        pFileOpen->SetOptions(dwOptions | FOS_PICKFOLDERS);
        pFileOpen->SetTitle(title.c_str());

        hr = pFileOpen->Show(NULL);
        if (SUCCEEDED(hr)) {
            IShellItem *pItem;
            hr = pFileOpen->GetResult(&pItem);
            if (SUCCEEDED(hr)) {
                PWSTR pszPath;
                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszPath);
                if (SUCCEEDED(hr)) {
                    result = pszPath;
                    CoTaskMemFree(pszPath);
                }
                pItem->Release();
            }
        }
        pFileOpen->Release();
    }

    if (hrCom == S_OK) CoUninitialize();
    return result;
}
