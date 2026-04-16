#pragma once
#include <iostream>
#include <sstream>
#include <windows.h>
#include <exception>
#include <type_traits>
using namespace std;

int open_debug_window;

// 内部辅助函数 - 不暴露给外部
namespace internal {
    // 获取格式化的时间字符串
    inline wstring getFormattedTime() {
        SYSTEMTIME now;
        GetLocalTime(&now);
        wstringstream ws;
        ws << L"[" << now.wYear << L"/" << now.wMonth << L"/" << now.wDay
            << L"|" << now.wHour << L":" << now.wMinute << L":" << now.wSecond
            << L":" << now.wMilliseconds << L"]";
        return ws.str();
    }
}

// 原有的三个函数保持相同的外观和调用方式
void ERROR_(wstring data_) {
    wcout << internal::getFormattedTime();
    wcout << L"[ERROR]";
    wcout << data_ << endl;
}

void INFO_(wstring data_ ) {
    wcout << internal::getFormattedTime();
    wcout << L"[INFO]";
    wcout << data_ << endl;
}

// 工具函数：将字符串转换为宽字符串
inline wstring to_wstring(const string& str) {
    return wstring(str.begin(), str.end());
}

// 工具函数：将字符指针转换为宽字符串
inline wstring to_wstring(const char* str) {
    if (str == nullptr) return L"(null)";
    return wstring(str, str + strlen(str));
}

// MESSAGE_函数模板
template<typename T>
void MESSAGE_(wstring chinese, const T& value) {
    wcout << internal::getFormattedTime();
    wcout << L"[MESSAGE]";
    wcout << chinese << value << endl;
}

// MESSAGE_函数重载 - 用于std::exception
void MESSAGE_(wstring chinese, const std::exception& value) {
    wcout << internal::getFormattedTime();
    wcout << L"[MESSAGE]";
    wcout << chinese << to_wstring(value.what()) << endl;
}

// MESSAGE_函数重载 - 用于std::string
void MESSAGE_(wstring chinese, const string& value) {
    wcout << internal::getFormattedTime();
    wcout << L"[MESSAGE]";
    wcout << chinese << to_wstring(value) << endl;
}

// MESSAGE_函数重载 - 用于char* (C字符串)
void MESSAGE_(wstring chinese, const char* value) {
    wcout << internal::getFormattedTime();
    wcout << L"[MESSAGE]";
    wcout << chinese << to_wstring(value) << endl;
}

// MESSAGE_函数重载 - 用于wstring (已存在，但简化了)
void MESSAGE_(wstring chinese, const wstring& value) {
    wcout << internal::getFormattedTime();
    wcout << L"[MESSAGE]";
    wcout << chinese << value << endl;
}