#pragma once
// ============================================================
// Backend.h — C++ 后端桥接层 (QObject + Q_INVOKABLE)
// 描述：声明 Backend 类，Q_OBJECT 元数据由 MOC 生成
//       实现在 Backend.cpp 中
// ============================================================

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtGui/QColor>

class Backend : public QObject {
    Q_OBJECT
    Q_PROPERTY(QColor bgColor READ bgColor NOTIFY bgColorChanged)
public:
    explicit Backend(QObject* parent = nullptr);

    QColor bgColor() const { return m_bgColor; }
    void setBgColor(const QColor &color);

    Q_INVOKABLE void installThemeWin10();
    Q_INVOKABLE void installThemeWin11();
    Q_INVOKABLE void patchThemePath();
    Q_INVOKABLE void installIcons();
    Q_INVOKABLE void installMyDockFinder();
    Q_INVOKABLE void deleteTheme();
    Q_INVOKABLE void installStartAllBack();
    Q_INVOKABLE void installOldNewExplorer();
    Q_INVOKABLE void finalSetting();
    Q_INVOKABLE void runWindowsFix();
    Q_INVOKABLE void runWindowsFixNoWindow();
    Q_INVOKABLE void restart();
    Q_INVOKABLE void restartNoNotice();
    Q_INVOKABLE bool createRestorePoint(const QString& description);
    Q_INVOKABLE void restartExplorer();
    Q_INVOKABLE void killProcess(const QString& appName);
    Q_INVOKABLE void addAutoStart(const QString& targetPath, const QString& shortcutName);
    Q_INVOKABLE void setDesktopIcons(bool show);
    Q_INVOKABLE void log(const QString& message);

signals:
    void bgColorChanged();

private:
    QColor m_bgColor = QColor(236, 240, 245); // 默认浅灰
};
