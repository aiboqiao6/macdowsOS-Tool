#pragma once
// ============================================================
// DeleteTheme.h
// 描述：主题删除工具，提供删除已安装的 macOS 主题文件的功能
//      清理 Windows Resources\Themes 目录下的相关文件
// ============================================================
#include<Windows.h>
#include <filesystem>
#include"LogSystem.h"

namespace {
void Delete_theme(){
    INFO_(L"[主题删除工具]开始删除");
    std::filesystem::remove_all("C:\\Windows\\Resources\\Themes\\Big Sur"); INFO_(L"[主题删除工具]删除Big Sur");
    std::filesystem::remove_all("C:\\Windows\\Resources\\Themes\\Tahoe"); INFO_(L"[主题删除工具]删除Tahoe");
    std::filesystem::remove_all("C:\\Windows\\Resources\\Themes\\Big Sur Dark Mode.theme"); INFO_(L"[主题删除工具]删除Big Sur Dark Mode.theme");
    std::filesystem::remove_all("C:\\Windows\\Resources\\Themes\\Big Sur Day.theme"); INFO_(L"[主题删除工具]删除Big Sur Day.theme");
    std::filesystem::remove_all("C:\\Windows\\Resources\\Themes\\Big Sur Night.theme"); INFO_(L"[主题删除工具]删除Big Sur Night.theme");
    std::filesystem::remove_all("C:\\Windows\\Resources\\Themes\\Tahoe - Light.theme"); INFO_(L"[主题删除工具]删除Tahoe - Light.theme");
    std::filesystem::remove_all("C:\\Windows\\Resources\\Themes\\Tahoe - Night.theme"); INFO_(L"[主题删除工具]删除Tahoe - Night.theme");
    return;
}

}
