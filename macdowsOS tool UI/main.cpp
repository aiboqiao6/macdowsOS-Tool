// ============================================================
// main.cpp — macdowsOS Tool UI 主入口
//
//  取色融合方案：
//    C++ 捕获窗口背后的桌面区域，提取平均颜色
//    传递给 QML 作为窗口背景色，纯色渲染，零模糊零截图传输
// ============================================================

#include <QtGui/QGuiApplication>
#include <QtGui/QIcon>
#include <QtGui/QFontDatabase>
#include <QtGui/QSurfaceFormat>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlError>
#include <QtQml/QJSEngine>
#include <QtCore/QUrl>
#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QtQuick/QQuickWindow>
#include <QtQuick/QSGRendererInterface>

#include <Windows.h>

#include "Backend.h"

// ============================================================
int main(int argc, char *argv[]) {
    QGuiApplication app(argc, nullptr);
    app.setApplicationName(QStringLiteral("macdowsOS Tool"));
    app.setApplicationDisplayName(QStringLiteral("macdowsOS Tool"));
    app.setWindowIcon(QIcon());
    setlocale(LC_ALL, "chs");

    // 加载 PingFang 字体
    int fontId = QFontDatabase::addApplicationFont(QStringLiteral(":/pingfang0.ttf"));
    if (fontId != -1) {
        QStringList families = QFontDatabase::applicationFontFamilies(fontId);
        if (!families.isEmpty()) {
            QFont pingfang(families.first());
            pingfang.setPixelSize(13);
            app.setFont(pingfang);
        }
    }

    QSurfaceFormat fmt;
    fmt.setSwapInterval(0);
    QSurfaceFormat::setDefaultFormat(fmt);

    QQuickWindow::setDefaultAlphaBuffer(true);
    QQuickWindow::setGraphicsApi(QSGRendererInterface::Direct3D11);

    QQmlApplicationEngine engine;

    QObject::connect(&engine, &QQmlApplicationEngine::warnings,
        [](const QList<QQmlError> &warnings) {
            for (const auto &w : warnings)
                OutputDebugStringW((w.toString() + QStringLiteral("\n")).toStdWString().c_str());
        });

    // 创建 Backend 单例并注册到 QML
    Backend *backend = new Backend();
    qmlRegisterSingletonType<Backend>("Backend", 1, 0, "Backend",
        [backend](QQmlEngine *, QJSEngine *) -> QObject * { return backend; });

    QUrl qmlUrl(QStringLiteral("qrc:/qml/main.qml"));
    engine.load(qmlUrl);

    if (engine.rootObjects().isEmpty()) {
        MessageBoxW(NULL, L"QML 界面加载失败\n请查看 VS 输出窗口（调试）获取详细错误信息",
                    L"macdowsOS Tool 错误", MB_OK | MB_ICONERROR);
        return -1;
    }

    // 获取窗口
    auto rootObjs = engine.rootObjects();
    if (!rootObjs.isEmpty()) {
        auto window = qobject_cast<QQuickWindow *>(rootObjs.first());
        if (window) {
            // WDA_EXCLUDEFROMCAPTURE 常量（Windows 10 2004+）
#ifndef WDA_EXCLUDEFROMCAPTURE
#define WDA_EXCLUDEFROMCAPTURE 0x00000011
#endif

            HWND hwnd = (HWND)window->winId();

            // ——— 取色函数：BitBlt 窗口区域缩到 1 像素 = 硬件加速平均色 ———
            auto doCapture = [window, backend, hwnd]() {
                SetWindowDisplayAffinity(hwnd, WDA_EXCLUDEFROMCAPTURE);

                QPoint pos = window->position();
                QSize sz = window->size();

                // 用 GDI BitBlt 截窗口区域并缩到 1×1 = 硬件加速区域平均色
                HDC hdcScreen = GetDC(NULL);
                HDC hdcMem = CreateCompatibleDC(hdcScreen);
                HBITMAP hbm = CreateCompatibleBitmap(hdcScreen, 1, 1);
                SelectObject(hdcMem, hbm);

                SetStretchBltMode(hdcMem, HALFTONE);
                StretchBlt(hdcMem, 0, 0, 1, 1,
                           hdcScreen,
                           pos.x(), pos.y(),
                           sz.width(), sz.height(),
                           SRCCOPY);

                COLORREF cr = GetPixel(hdcMem, 0, 0);

                DeleteObject(hbm);
                DeleteDC(hdcMem);
                ReleaseDC(NULL, hdcScreen);

                SetWindowDisplayAffinity(hwnd, WDA_NONE);

                backend->setBgColor(QColor(GetRValue(cr), GetGValue(cr), GetBValue(cr)));
            };

            // ——— 拖动节流：拖动时每 100ms 取一次色 ———
            QTimer *dragThrottle = new QTimer(window);
            dragThrottle->setInterval(100);
            QObject::connect(dragThrottle, &QTimer::timeout, window, doCapture);

            // 拖动停止检测：300ms 无变化 → 停止节流
            QTimer *dragStop = new QTimer(window);
            dragStop->setSingleShot(true);
            dragStop->setInterval(300);
            QObject::connect(dragStop, &QTimer::timeout, dragThrottle, &QTimer::stop);

            auto onWindowChanged = [dragThrottle, dragStop]() {
                if (!dragThrottle->isActive())
                    dragThrottle->start();
                dragStop->start();
            };
            QObject::connect(window, &QQuickWindow::xChanged, window, onWindowChanged);
            QObject::connect(window, &QQuickWindow::yChanged, window, onWindowChanged);
            QObject::connect(window, &QQuickWindow::widthChanged, window, onWindowChanged);
            QObject::connect(window, &QQuickWindow::heightChanged, window, onWindowChanged);

            // 初始取色
            QTimer::singleShot(0, doCapture);

            // ——— 空闲定时器（每 2 秒刷新一次颜色） ———
            QTimer *idleTimer = new QTimer(window);
            QObject::connect(idleTimer, &QTimer::timeout, window, doCapture);
            idleTimer->start(2000);
        }
    }

    return app.exec();
}
