#define _CRT_SECURE_NO_WARNINGS

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
#include <QtCore/QTimer>
#include <QtQuick/QQuickWindow>
#include <QtQuick/QSGRendererInterface>
#include <QtQuickControls2/QQuickStyle>

#include <Windows.h>

#include "Backend.h"

#ifdef Q_OS_WIN
// Windows 10/11 Acrylic blur behind.  Qt Quick 的 MultiEffect 只能模糊 Qt 场景内的
// Item，不能模糊窗口背后的桌面内容；真实毛玻璃需要交给 DWM 处理。
enum AccentState {
    ACCENT_DISABLED = 0,
    ACCENT_ENABLE_GRADIENT = 1,
    ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
    ACCENT_ENABLE_BLURBEHIND = 3,
    ACCENT_ENABLE_ACRYLICBLURBEHIND = 4,
    ACCENT_ENABLE_HOSTBACKDROP = 5
};

struct AccentPolicy {
    int AccentState;
    int AccentFlags;
    int GradientColor;
    int AnimationId;
};

struct WindowCompositionAttributeData {
    int Attribute;
    void *Data;
    size_t SizeOfData;
};

static void enableAcrylicBlur(HWND hwnd) {
    using SetWindowCompositionAttributeFunc = BOOL(WINAPI *)(HWND, WindowCompositionAttributeData *);
    HMODULE user32 = GetModuleHandleW(L"user32.dll");
    auto setWindowCompositionAttribute = reinterpret_cast<SetWindowCompositionAttributeFunc>(
        GetProcAddress(user32, "SetWindowCompositionAttribute"));

    if (!setWindowCompositionAttribute)
        return;

    // AABBGGRR。较低 alpha 可以让背景纹理更明显；QML 再叠一层浅色材质。
    AccentPolicy accent = {};
    accent.AccentState = ACCENT_ENABLE_ACRYLICBLURBEHIND;
    accent.AccentFlags = 2;
    accent.GradientColor = 0x66F7F7F7;

    WindowCompositionAttributeData data = {};
    data.Attribute = 19; // WCA_ACCENT_POLICY
    data.Data = &accent;
    data.SizeOfData = sizeof(accent);
    setWindowCompositionAttribute(hwnd, &data);
}

static void applyNativeRoundedCorners(HWND hwnd) {
    HMODULE dwmapi = LoadLibraryW(L"dwmapi.dll");
    if (dwmapi) {
        using DwmSetWindowAttributeFunc = HRESULT(WINAPI *)(HWND, DWORD, LPCVOID, DWORD);
        auto dwmSetWindowAttribute = reinterpret_cast<DwmSetWindowAttributeFunc>(
            GetProcAddress(dwmapi, "DwmSetWindowAttribute"));
        if (dwmSetWindowAttribute) {
            constexpr DWORD DWMWA_WINDOW_CORNER_PREFERENCE = 33;
            constexpr DWORD DWMWCP_ROUND = 2;
            DWORD preference = DWMWCP_ROUND;
            dwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE,
                                  &preference, sizeof(preference));
        }
        FreeLibrary(dwmapi);
    }

    RECT clientRect{};
    if (!GetClientRect(hwnd, &clientRect))
        return;

    const int width = clientRect.right - clientRect.left;
    const int height = clientRect.bottom - clientRect.top;
    if (width <= 0 || height <= 0)
        return;

    UINT dpi = 96;
    HMODULE user32 = GetModuleHandleW(L"user32.dll");
    if (user32) {
        using GetDpiForWindowFunc = UINT(WINAPI *)(HWND);
        auto getDpiForWindow = reinterpret_cast<GetDpiForWindowFunc>(
            GetProcAddress(user32, "GetDpiForWindow"));
        if (getDpiForWindow)
            dpi = getDpiForWindow(hwnd);
    }

    const int radius = MulDiv(16, static_cast<int>(dpi), 96);
    HRGN roundedRegion = CreateRoundRectRgn(0, 0, width + 1, height + 1,
                                           radius * 2, radius * 2);
    SetWindowRgn(hwnd, roundedRegion, TRUE);
}
#endif

// ============================================================
int main(int argc, char *argv[]) {
    freopen("log.txt", "w", stdout);
    (void)argc; (void)argv;
    QGuiApplication app(argc, nullptr);

    // 设置 Quick Controls 样式为非原生样式，以支持 background/contentItem 自定义
    QQuickStyle::setStyle(QStringLiteral("Basic"));

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

#ifdef Q_OS_WIN
            enableAcrylicBlur(hwnd);
            applyNativeRoundedCorners(hwnd);
#endif

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

            auto onWindowChanged = [dragThrottle, dragStop, hwnd]() {
#ifdef Q_OS_WIN
                applyNativeRoundedCorners(hwnd);
#endif
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
