#pragma once

#include <Windows.h>
#include <exception>
#include <sstream>
#include <string>

namespace {

std::wstring GetLogFilePath() {
    wchar_t modulePath[MAX_PATH]{};
    const DWORD length = GetModuleFileNameW(
        nullptr, modulePath, static_cast<DWORD>(_countof(modulePath)));
    std::wstring path(modulePath, length);
    const size_t separator = path.find_last_of(L"\\/");
    if (separator != std::wstring::npos)
        path.resize(separator + 1);
    else
        path.clear();
    return path + L"log.txt";
}

std::wstring GetFormattedTime() {
    SYSTEMTIME now{};
    GetLocalTime(&now);
    std::wstringstream stream;
    stream << L"[" << now.wYear << L"/" << now.wMonth << L"/" << now.wDay
           << L"|" << now.wHour << L":" << now.wMinute << L":"
           << now.wSecond << L":" << now.wMilliseconds << L"]";
    return stream.str();
}

void WriteLog(const wchar_t* level, const std::wstring& message) {
    const std::wstring line = GetFormattedTime() + L"[" + level + L"]" +
                              message + L"\r\n";
    const int byteCount = WideCharToMultiByte(
        CP_UTF8, 0, line.data(), static_cast<int>(line.size()),
        nullptr, 0, nullptr, nullptr);
    if (byteCount <= 0)
        return;

    std::string bytes(static_cast<size_t>(byteCount), '\0');
    WideCharToMultiByte(CP_UTF8, 0, line.data(),
                        static_cast<int>(line.size()), bytes.data(),
                        byteCount, nullptr, nullptr);

    HANDLE file = CreateFileW(
        GetLogFilePath().c_str(), FILE_APPEND_DATA,
        FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE)
        return;

    DWORD written = 0;
    WriteFile(file, bytes.data(), static_cast<DWORD>(bytes.size()),
              &written, nullptr);
    FlushFileBuffers(file);
    CloseHandle(file);
}

void ResetLog() {
    HANDLE file = CreateFileW(
        GetLogFilePath().c_str(), GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file != INVALID_HANDLE_VALUE)
        CloseHandle(file);
}

void ERROR_(const std::wstring& data) {
    WriteLog(L"ERROR", data);
}

void INFO_(const std::wstring& data) {
    WriteLog(L"INFO", data);
}

std::wstring ToWide(const std::string& text) {
    if (text.empty())
        return {};

    const int length = MultiByteToWideChar(
        CP_ACP, 0, text.data(), static_cast<int>(text.size()),
        nullptr, 0);
    std::wstring result(static_cast<size_t>(length), L'\0');
    MultiByteToWideChar(CP_ACP, 0, text.data(),
                        static_cast<int>(text.size()), result.data(), length);
    return result;
}

template<typename T>
void MESSAGE_(const std::wstring& message, const T& value) {
    std::wstringstream stream;
    stream << message << value;
    WriteLog(L"MESSAGE", stream.str());
}

void MESSAGE_(const std::wstring& message, const std::exception& value) {
    WriteLog(L"MESSAGE", message + ToWide(value.what()));
}

void MESSAGE_(const std::wstring& message, const std::string& value) {
    WriteLog(L"MESSAGE", message + ToWide(value));
}

void MESSAGE_(const std::wstring& message, const char* value) {
    if (value)
        WriteLog(L"MESSAGE", message + ToWide(value));
}

void MESSAGE_(const std::wstring& message, const std::wstring& value) {
    WriteLog(L"MESSAGE", message + value);
}

}
