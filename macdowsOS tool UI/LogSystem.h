#pragma once
// ============================================================
// LogSystem.h
// 描述：日志系统工具，提供程序运行时的日志记录功能
//      包含 INFO_、ERROR_、MESSAGE_ 等日志输出函数
// ============================================================

#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>

namespace detail {

// 获取格式化时间字符串
inline std::wstring GetFormattedTime() {
    SYSTEMTIME now;
    GetLocalTime(&now);
    std::wstringstream ws;
    ws << L"[" << now.wYear << L"/" << now.wMonth << L"/" << now.wDay
       << L"|" << now.wHour << L":" << now.wMinute << L":" << now.wSecond
       << L":" << now.wMilliseconds << L"]";
    return ws.str();
}

} // namespace detail

// 日志函数 - 参数改为 const& 避免拷贝
inline void ERROR_(const std::wstring& data) {
    std::wcout << detail::GetFormattedTime()
               << L"[ERROR]" << data << std::endl;
}

inline void INFO_(const std::wstring& data) {
    std::wcout << detail::GetFormattedTime()
               << L"[INFO]" << data << std::endl;
}

// 将窄字符串转为宽字符串
inline std::wstring ToWide(const std::string& str) {
    if (str.empty()) return {};
    int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
    std::wstring wide(static_cast<size_t>(len) - 1, L'\0');
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &wide[0], len);
    return wide;
}

// MESSAGE_ 模板 - 中文信息 + 值
template<typename T>
void MESSAGE_(const std::wstring& chinese, const T& value) {
    std::wcout << detail::GetFormattedTime()
               << L"[MESSAGE]" << chinese << value << std::endl;
}

inline void MESSAGE_(const std::wstring& chinese, const std::exception& value) {
    std::wcout << detail::GetFormattedTime()
               << L"[MESSAGE]" << chinese << ToWide(value.what()) << std::endl;
}

inline void MESSAGE_(const std::wstring& chinese, const std::string& value) {
    std::wcout << detail::GetFormattedTime()
               << L"[MESSAGE]" << chinese << ToWide(value) << std::endl;
}

inline void MESSAGE_(const std::wstring& chinese, const char* value) {
    if (value) {
        std::wcout << detail::GetFormattedTime()
                   << L"[MESSAGE]" << chinese << ToWide(value) << std::endl;
    }
}

inline void MESSAGE_(const std::wstring& chinese, const std::wstring& value) {
    std::wcout << detail::GetFormattedTime()
               << L"[MESSAGE]" << chinese << value << std::endl;
}
