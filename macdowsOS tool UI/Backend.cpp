// ============================================================
// Backend.cpp — C++ 后端桥接层实现
// 描述：Backend 类的方法实现
//       Q_OBJECT 元数据由 Qt VS Tools 自动处理
// ============================================================

#include "Backend.h"

#include "All.h"
#include "LogSystem.h"
#include "WindowsFix.h"
//#include "WindowRound.h"
#include "CreateSafePoint.h"
#include "Icon_install.h"
#include "Theme_Install_win10.h"
#include "Theme_Install_win11.h"
#include "Theme_patch_install.h"
#include "DeleteTheme.h"
#include "StartAllBack_Install.h"
#include "OldNewExplorerInstall.h"
#include "mydockfinder_Install.h"
#include "FinalSetting.h"
#include "restart.h"
#include "WindowControl.h"

Backend::Backend(QObject* parent) : QObject(parent) {}

void Backend::setBgColor(const QColor &color) {
    if (m_bgColor != color) {
        m_bgColor = color;
        emit bgColorChanged();
    }
}

void Backend::installThemeWin10() {
    INFO_(L"[Backend] 安装 Win10 主题");
    theme_install_win10();
    return;
}

void Backend::installThemeWin11() {
    INFO_(L"[Backend] 安装 Win11 主题");
    theme_install_win11();
    return;
}

void Backend::patchThemePath() {
    INFO_(L"[Backend] 安装主题补丁");
    theme_path_install();
    return;
}

void Backend::installIcons() {
    INFO_(L"[Backend] 安装图标");
    Icon_Install();
    return;
}

void Backend::installMyDockFinder() {
    INFO_(L"[Backend] 安装 myDockFinder");
    mydockfinder_install();
    return;
}

void Backend::deleteTheme() {
    INFO_(L"[Backend] 删除主题");
    Delete_theme();
    return;
}

void Backend::installStartAllBack() {
    INFO_(L"[Backend] 安装 StartAllBack");
    StartAllBack_SetupRun();
    return;
}

void Backend::installOldNewExplorer() {
    INFO_(L"[Backend] 安装 OldNewExplorer");
    OldNewExplorer_install();
    return;
}

void Backend::finalSetting() {
    INFO_(L"[Backend] 执行最终设置");
    FinalSetting();
    return;
}

void Backend::runWindowsFix() {
    INFO_(L"[Backend] 运行系统修复");
    WindowsFix();
    return;
}

void Backend::runWindowsFixNoWindow() {
    INFO_(L"[Backend] 运行无弹窗系统修复");
    WindowsFix_nowindow();
    return;
}

void Backend::restart() {
    INFO_(L"[Backend] 重启");
    restart_();
    return;
}

void Backend::restartNoNotice() {
    INFO_(L"[Backend] 无提示重启");
    restart_nonotice();
    return;
}

bool Backend::createRestorePoint(const QString& description) {
    INFO_(L"[Backend] 创建系统还原点");
    INT64 seq = Start_CreateSafePoint(
        reinterpret_cast<const wchar_t*>(description.utf16()));
    if (seq < 0) return false;
    return Finish_CreateSafePoint(seq);
}

void Backend::killProcess(const QString& appName) {
    killapp(reinterpret_cast<const wchar_t*>(appName.utf16()));
    return;
}

void Backend::addAutoStart(const QString& targetPath, const QString& shortcutName) {
    AddAutoStart(
        reinterpret_cast<const wchar_t*>(targetPath.utf16()),
        reinterpret_cast<const wchar_t*>(shortcutName.utf16()));
    return;
}

void Backend::setDesktopIcons(bool show) {
    SetDesktopIconsPermanent(show);
    return;
}

void Backend::log(const QString& message) {
    INFO_(reinterpret_cast<const wchar_t*>(message.utf16()));
    return;
}
