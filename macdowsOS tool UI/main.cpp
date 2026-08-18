#define _CRT_SECURE_NO_WARNINGS

// ============================================================
// main.cpp — macdowsOS Tool UI 主入口
//
//  使用固定的浅色材质模拟低透明度毛玻璃，不采样桌面内容。
// ============================================================

#include <QtGui/QGuiApplication>
#include <QtGui/QIcon>
#include <QtGui/QFontDatabase>
#include <QtGui/QSurfaceFormat>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlError>
#include <QtQml/QJSEngine>
#include <QtCore/QUrl>
#include <QtQuick/QQuickWindow>
#include <QtQuick/QSGRendererInterface>
#include <QtQuickControls2/QQuickStyle>

#include <Windows.h>

#include "Backend.h"

#ifdef Q_OS_WIN
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
    MessageBoxW(NULL, L"项目涉及的部分主题、图标和桌面组件来自第三方，其著作权与许可归原作者所有。用户应自行确认相关资源的授权范围并遵守对应许可协议。本项目仅用于技术研究、学习和合法授权的软件交互，不支持盗版或商业侵权用途。开发者不对第三方文件的合法性、兼容性以及因使用本程序造成的数据丢失、系统异常或版权纠纷承担责任。使用本程序即表示你理解并接受相关风险。如果本项目侵犯到了您的权益 请通过邮箱联系 有关内容将会在核实后删除",
        L"提示", MB_OK);
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
            HWND hwnd = (HWND)window->winId();

#ifdef Q_OS_WIN
            applyNativeRoundedCorners(hwnd);
            auto updateRoundedCorners = [hwnd]() {
                applyNativeRoundedCorners(hwnd);
            };
            QObject::connect(window, &QQuickWindow::widthChanged,
                             window, updateRoundedCorners);
            QObject::connect(window, &QQuickWindow::heightChanged,
                             window, updateRoundedCorners);
#endif
        }
    }

    return app.exec();
}
