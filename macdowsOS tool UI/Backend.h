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
#include "FinalSetting.h"
#include "Icon_install.h"
#include "iconback.h"
#include "LogSystem.h"
#include "OldNewExplorerInstall.h"
#include "StartAllBack_Install.h"
#include "Theme_Install_win10.h"
#include "Theme_Install_win11.h"
#include "Theme_patch_install.h"
#include "WindowControl.h"
#include "WindowsFix.h"
#include "mydockfinder_Install.h"
#include "restart.h"

class Backend : public QObject {
    Q_OBJECT
    Q_PROPERTY(QColor bgColor READ bgColor NOTIFY bgColorChanged)

public:
    explicit Backend(QObject* parent = nullptr) : QObject(parent) {}

    ~Backend() override {
        if (m_installThread && m_installThread->isRunning())
            m_installThread->wait();
    }

    QColor bgColor() const { return m_bgColor; }

    void setBgColor(const QColor& color) {
        if (m_bgColor == color)
            return;
        m_bgColor = color;
        emit bgColorChanged();
    }

    Q_INVOKABLE void installThemeWin10() {
        INFO_(L"[Backend] 安装 Win10 主题");
        theme_install_win10();
    }

    Q_INVOKABLE void installThemeWin11() {
        INFO_(L"[Backend] 安装 Win11 主题");
        theme_install_win11();
    }

    Q_INVOKABLE void patchThemePath() {
        INFO_(L"[Backend] 安装主题补丁");
        theme_path_install();
    }

    Q_INVOKABLE void installIcons() {
        INFO_(L"[Backend] 安装图标");
        Icon_Install();
    }

    Q_INVOKABLE void restoreIcons() {
        INFO_(L"[Backend] 恢复系统图标");
        Icon_Back();
    }

    Q_INVOKABLE void installMyDockFinder() {
        INFO_(L"[Backend] 安装 myDockFinder");
        mydockfinder_install();
    }

    Q_INVOKABLE void deleteTheme() {
        INFO_(L"[Backend] 删除主题");
        Delete_theme();
    }

    Q_INVOKABLE void deleteMyDockFinder() {
        INFO_(L"[Backend] 删除 MyDockFinder");
        DeleteMyDockFinder();
    }

    Q_INVOKABLE void uninstallThemePatch() {
        INFO_(L"[Backend] 删除主题补丁");
        theme_path_uninstall();
    }

    Q_INVOKABLE void installStartAllBack() {
        INFO_(L"[Backend] 安装 StartAllBack");
        StartAllBack_SetupRun();
    }

    Q_INVOKABLE void installOldNewExplorer() {
        INFO_(L"[Backend] 安装 OldNewExplorer");
        OldNewExplorer_install();
    }

    Q_INVOKABLE void finalSetting() {
        INFO_(L"[Backend] 执行最终设置");
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
        restart_();
    }

    Q_INVOKABLE void restartNoNotice() {
        INFO_(L"[Backend] 无提示重启");
        restart_nonotice();
    }

    Q_INVOKABLE bool createRestorePoint(const QString& description) {
        INFO_(L"[Backend] 创建系统还原点");
        return CreateSystemRestorePoint(description.toStdWString());
    }

    Q_INVOKABLE bool startInstallation(int version) {
        if (m_installThread || (version != 10 && version != 11))
            return false;

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

private:
    QColor m_bgColor{236, 240, 245};
    QThread* m_installThread = nullptr;
};
