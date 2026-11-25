#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QThread>
#include <QtGui/QColor>

#include <stdexcept>

#include "CreateSafePoint.h"
#include "DeleteMyDockFinder.h"
#include "DeleteTheme.h"
#include "DeleteThemePatch.h"
#include "DeleteStartAllBack.h"
#include "FinalSetting.h"
#include "Icon_install.h"
#include "iconback.h"
#include "LogSystem.h"
#include "OldNewExplorerInstall.h"
#include "StartAllBack_Install.h"
#include "Theme_Install_win10.h"
#include "Theme_Install_win11.h"
#include "Theme_patch_install.h"
#include "UpdateChecker.h"
#include "WindowControl.h"
#include "WindowsFix.h"
#include "mydockfinder_Install.h"
#include "restart.h"

//全屏遮挡层 RAII 守卫：构造/析构通过 Backend 的深度计数控制遮挡显示，
//嵌套流程（组合卸载内部调用子操作）不会导致遮挡层中途闪烁隐藏。
//成员函数在 Backend 类定义之后实现（需要完整类型）。
class Backend;

class OverlayGuard {
public:
    explicit OverlayGuard(Backend* backend);
    ~OverlayGuard();
    OverlayGuard(const OverlayGuard&) = delete;
    OverlayGuard& operator=(const OverlayGuard&) = delete;
private:
    Backend* m_backend;
};

class Backend : public QObject {
    Q_OBJECT
    Q_PROPERTY(QColor bgColor READ bgColor NOTIFY bgColorChanged)
    Q_PROPERTY(UpdateChecker* updateChecker READ updateChecker CONSTANT)
    Q_PROPERTY(bool installationRunning READ installationRunning NOTIFY installationRunningChanged)
    Q_PROPERTY(bool restartPending READ restartPending NOTIFY restartPendingChanged)

public:
    explicit Backend(QObject* parent = nullptr)
        : QObject(parent), m_updateChecker(this) {}

    ~Backend() override {
        if (m_installThread && m_installThread->isRunning())
            m_installThread->wait();
    }

    QColor bgColor() const { return m_bgColor; }
    UpdateChecker* updateChecker() { return &m_updateChecker; }
    bool installationRunning() const { return m_installationRunning; }
    bool restartPending() const { return m_restartPending; }

    //遮挡深度计数：仅 0→1 显示、1→0 隐藏，避免嵌套流程闪烁
    void pushOverlay() {
        if (++m_overlayDepth == 1) {
            g_overlayActive = true;
            emit overlayRequested(true);
        }
    }
    void popOverlay() {
        if (m_overlayDepth > 0 && --m_overlayDepth == 0) {
            g_overlayActive = false;
            emit overlayRequested(false);
        }
    }

    void setBgColor(const QColor& color) {
        if (m_bgColor == color)
            return;
        m_bgColor = color;
        emit bgColorChanged();
    }

    Q_INVOKABLE void installThemeWin10() {
        INFO_(L"[Backend] 安装 Win10 主题");
        OverlayGuard guard(this);
        theme_install_win10();
    }

    Q_INVOKABLE void installThemeWin11() {
        INFO_(L"[Backend] 安装 Win11 主题");
        OverlayGuard guard(this);
        theme_install_win11();
    }

    Q_INVOKABLE void patchThemePath() {
        INFO_(L"[Backend] 安装主题补丁");
        OverlayGuard guard(this);
        theme_path_install();
    }

    Q_INVOKABLE void installIcons() {
        INFO_(L"[Backend] 安装图标");
        OverlayGuard guard(this);
        Icon_Install();
    }

    Q_INVOKABLE void restoreIcons() {
        INFO_(L"[Backend] 恢复系统图标");
        OverlayGuard guard(this);
        Icon_Back();
    }

    Q_INVOKABLE void installMyDockFinder() {
        INFO_(L"[Backend] 安装 myDockFinder");
        mydockfinder_install();
    }

    Q_INVOKABLE void deleteTheme() {
        INFO_(L"[Backend] 删除主题");
        OverlayGuard guard(this);
        Delete_theme();
    }

    Q_INVOKABLE void deleteMyDockFinder() {
        INFO_(L"[Backend] 删除 MyDockFinder");
        DeleteMyDockFinder();
    }

    Q_INVOKABLE void uninstallThemePatch() {
        INFO_(L"[Backend] 删除主题补丁");
        OverlayGuard guard(this);
        theme_path_uninstall();
    }

    Q_INVOKABLE void uninstallStartAllBack() {
        INFO_(L"[Backend] 卸载 StartAllBack");
        OverlayGuard guard(this);
        UninstallStartAllBack();
    }

    Q_INVOKABLE void uninstallOldNewExplorer() {
        INFO_(L"[Backend] 卸载 OldNewExplorer");
        OverlayGuard guard(this);
        OldNewExplorer_install();
    }

    Q_INVOKABLE void uninstallWindows10() {
        INFO_(L"[Backend] 开始卸载 Windows 10 组件");
        OverlayGuard guard(this);
        deleteMyDockFinder();
        deleteTheme();
        uninstallThemePatch();
        restoreIcons();
        INFO_(L"[Backend] 恢复最终系统设置");
        FinalSetting_back();
        uninstallOldNewExplorer();
        INFO_(L"[Backend] Windows 10 组件卸载完成，准备无提示重启");
        restartNoNotice();
    }

    Q_INVOKABLE void uninstallWindows11() {
        INFO_(L"[Backend] 开始卸载 Windows 11 组件");
        OverlayGuard guard(this);
        deleteMyDockFinder();
        deleteTheme();
        uninstallThemePatch();
        restoreIcons();
        INFO_(L"[Backend] 恢复最终系统设置");
        FinalSetting_back();
        uninstallStartAllBack();
        INFO_(L"[Backend] Windows 11 组件卸载完成，准备无提示重启");
        restartNoNotice();
    }

    Q_INVOKABLE void installStartAllBack() {
        INFO_(L"[Backend] 安装 StartAllBack");
        OverlayGuard guard(this);
        StartAllBack_SetupRun();
    }

    Q_INVOKABLE void installOldNewExplorer() {
        INFO_(L"[Backend] 安装 OldNewExplorer");
        OverlayGuard guard(this);
        OldNewExplorer_install();
    }

    Q_INVOKABLE void finalSetting() {
        INFO_(L"[Backend] 执行最终设置");
        OverlayGuard guard(this);
        FinalSetting();
    }

    Q_INVOKABLE void runWindowsFix() {
        INFO_(L"[Backend] 运行系统修复");
        WindowsFix();
    }

    Q_INVOKABLE void runWindowsFixNoWindow() {
        INFO_(L"[Backend] 运行无弹窗系统修复");
        WindowsFix_nowindow();
    }

    Q_INVOKABLE void restart() {
        INFO_(L"[Backend] 重启");
        m_restartPending = true;
        emit restartPendingChanged();
        restart_();
    }

    Q_INVOKABLE void restartNoNotice() {
        INFO_(L"[Backend] 无提示重启");
        // 标记重启待定：QML 据此让遮挡层永不自动隐藏，
        // 保持"正在准备重新启动…"画面直至 Windows 接管重启，实现无缝过渡
        m_restartPending = true;
        emit restartPendingChanged();
        restart_nonotice();
    }

    Q_INVOKABLE bool createRestorePoint(const QString& description) {
        INFO_(L"[Backend] 创建系统还原点");
        return CreateSystemRestorePoint(description.toStdWString());
    }

    Q_INVOKABLE bool startInstallation(int version) {
        if (m_installThread || (version != 10 && version != 11))
            return false;

        //全流程模式：QML 据此忽略内层子操作触发的 overlayRequested(false)，
        //遮挡层完全由 installation* 信号驱动
        m_installationRunning = true;
        emit installationRunningChanged();

        constexpr int totalSteps = 9;
        m_installThread = QThread::create([this, version, totalSteps]() {
            int completedSteps = 0;

            try {
                emit installationStarted(totalSteps);

                auto runStep = [this, &completedSteps, totalSteps](
                                   const QString& name, auto action) {
                    emit installationStepChanged(name);
                    action();
                    emit installationProgressChanged(++completedSteps,
                                                     totalSteps);
                };

                runStep(QStringLiteral("创建系统还原点"), [this]() {
                    if (!createRestorePoint(
                            QStringLiteral("macdowsOS 安装前还原点"))) {
                        throw std::runtime_error("restore point creation failed");
                    }
                });
                runStep(QStringLiteral("安装 MyDockFinder"),
                        [this]() { installMyDockFinder(); });
                runStep(QStringLiteral("安装主题补丁"),
                        [this]() { patchThemePath(); });
                runStep(QStringLiteral("清理旧主题"),
                        [this]() { deleteTheme(); });

                if (version == 10) {
                    runStep(QStringLiteral("安装 Windows 10 主题"),
                            [this]() { installThemeWin10(); });
                } else {
                    runStep(QStringLiteral("安装 Windows 11 主题"),
                            [this]() { installThemeWin11(); });
                }

                runStep(QStringLiteral("安装系统图标"),
                        [this]() { installIcons(); });

                if (version == 10) {
                    runStep(QStringLiteral("安装 OldNewExplorer"),
                            [this]() { installOldNewExplorer(); });
                } else {
                    runStep(QStringLiteral("安装 StartAllBack"),
                            [this]() { installStartAllBack(); });
                }

                runStep(QStringLiteral("应用最终设置"),
                        [this]() { finalSetting(); });
                runStep(QStringLiteral("准备重启系统"),
                        [this]() { restartNoNotice(); });
                emit installationFinished(true, QString());
            } catch (const std::exception& error) {
                ERROR_(L"[Backend] 安装流程已停止");
                emit installationFinished(
                    false, QString::fromLocal8Bit(error.what()));
            } catch (...) {
                ERROR_(L"[Backend] 安装流程发生未知错误");
                emit installationFinished(
                    false, QStringLiteral("unknown installation error"));
            }
        });

        QThread* installThread = m_installThread;
        connect(installThread, &QThread::finished, this,
                [this, installThread]() {
                    if (m_installThread == installThread)
                        m_installThread = nullptr;
                    installThread->deleteLater();
                    m_installationRunning = false;
                    emit installationRunningChanged();
                });
        installThread->start();
        return true;
    }

    Q_INVOKABLE void restartExplorer() {
        INFO_(L"[Backend] 重启资源管理器");
        killapp(L"explorer.exe");
    }

    Q_INVOKABLE void killProcess(const QString& appName) {
        killapp(reinterpret_cast<const wchar_t*>(appName.utf16()));
    }

    Q_INVOKABLE void addAutoStart(const QString& targetPath,
                                  const QString& shortcutName) {
        AddAutoStart(
            reinterpret_cast<const wchar_t*>(targetPath.utf16()),
            reinterpret_cast<const wchar_t*>(shortcutName.utf16()));
    }

    Q_INVOKABLE void setDesktopIcons(bool show) {
        SetDesktopIconsPermanent(show);
    }

    Q_INVOKABLE void log(const QString& message) {
        INFO_(reinterpret_cast<const wchar_t*>(message.utf16()));
    }

signals:
    void bgColorChanged();
    void installationStarted(int totalSteps);
    void installationStepChanged(const QString& stepName);
    void installationProgressChanged(int completedSteps, int totalSteps);
    void installationFinished(bool success, const QString& errorMessage);
    void installationRunningChanged();
    void restartPendingChanged();
    void overlayRequested(bool show);

private:
    UpdateChecker m_updateChecker;
    QColor m_bgColor{236, 240, 245};
    QThread* m_installThread = nullptr;
    int m_overlayDepth = 0;
    bool m_installationRunning = false;
    bool m_restartPending = false;
};

// OverlayGuard 成员函数实现（Backend 已完整定义）
inline OverlayGuard::OverlayGuard(Backend* backend) : m_backend(backend) {
    m_backend->pushOverlay();
}
inline OverlayGuard::~OverlayGuard() {
    m_backend->popOverlay();
}
