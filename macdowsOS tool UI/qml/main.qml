// ============================================================
// main.qml — 主窗口文件
// 采用 ShaderEffectSource 蒙版方案实现圆角窗口
// 完全基于纯 QML 实现，无 Win32/DWM 依赖
// 整体布局：左（半透明玻璃侧边栏） | 右（白色内容区）
// ============================================================

// QtQuick — 基础 QML 模块，提供 Item、Rectangle、MouseArea 等核心类型
import QtQuick
// QtQuick.Controls — 提供 Label、ItemDelegate 等标准控件
import QtQuick.Controls
// QtQuick.Layouts — 提供 ColumnLayout、Row 等布局管理器
import QtQuick.Layouts
// QtQuick.Window — 提供 Window 类型，用于创建顶级窗口
import QtQuick.Window
// QtQuick.Effects — 提供 MultiEffect，用于实现圆角蒙版裁剪效果
import QtQuick.Effects

// === 主应用程序窗口 ===
// 无边框透明窗口，通过蒙版实现自定义圆角
Window {
    id: appWindow
    title: "macdowsOS Tool"
    width: 780
    height: 520
    minimumWidth: 680
    minimumHeight: 480

    // 窗口标志：
    //   Qt.Window                  — 标准窗口
    //   Qt.FramelessWindowHint     — 无系统标题栏，实现自定义标题栏
    //   Qt.WindowSystemMenuHint    — 保留系统菜单（右键任务栏）
    //   Qt.WindowMinMaxButtonsHint — 保留最小化/最大化按钮行为
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowSystemMenuHint | Qt.WindowMinMaxButtonsHint
    color: "transparent"   // 窗口背景透明，由内部内容提供背景色
    visible: true

    // ——— 属性定义 ———
    property var wizardWindow: null                    // 主题向导窗口实例引用
    property var pageMap: [0, 1, 4]                    // 导航索引 → 页面索引映射表
    property int sidebarWidth: 222                     // 侧边栏宽度，可被分隔条拖动调整

    // ——— 函数：打开主题向导窗口 ———
    // 接受版本参数（10=Win10, 11=Win11）并传递给向导
    function openThemeWizard(version) {
        // 如果向导窗口已存在，直接激活并更新版本
        if (wizardWindow !== null && wizardWindow !== undefined) {
            wizardWindow.selectedVersion = version
            wizardWindow.show()
            wizardWindow.raise()
            wizardWindow.requestActivate()
            return
        }

        // 动态创建 ThemeWizardWindow 组件并传入版本参数
        let component = Qt.createComponent("qrc:/qml/ThemeWizardWindow.qml")
        if (component.status === Component.Ready) {
            wizardWindow = component.createObject(null, { selectedVersion: version })
            if (wizardWindow) {
                wizardWindow.show()
                wizardWindow.raise()
                wizardWindow.requestActivate()
                // 窗口关闭时释放组件并清除引用，避免内存泄漏
                wizardWindow.closing.connect(function() {
                    wizardWindow.destroy()
                    wizardWindow = null
                })
            }
        } else {
            console.error("ThemeWizardWindow 加载失败:", component.errorString())
        }
    }

    // ============================================================
    // 圆角蒙版实现
    // 1. cornerMask — 白色圆角矩形作为蒙版源
    // 2. maskTexture — ShaderEffectSource 捕获蒙版的亮度纹理
    // 3. rootContent — 使用 MultiEffect.maskSource 引用该纹理实现裁剪
    // ============================================================
    Rectangle {
        id: cornerMask
        anchors.fill: parent
        color: "white"
        radius: 16           // 圆角半径 16px，匹配 macOS 风格
        visible: true
    }

    ShaderEffectSource {
        id: maskTexture
        sourceItem: cornerMask
        hideSource: true     // 蒙版本身不可见，但其纹理实时更新供 maskSource 引用
    }

    // ============================================================
    // 根内容层
    // 所有 UI 元素都放在这里，通过 layer + MultiEffect 裁剪为圆角
    // ============================================================
    Item {
        id: rootContent
        anchors.fill: parent
        layer.enabled: true
        layer.effect: MultiEffect {
            maskEnabled: true
            maskSource: maskTexture   // 使用蒙版纹理裁剪圆角
        }

        // ——— 取色背景 ———
        // 通过 C++ 端 Backend 对象提取窗口背后平均颜色，实现类似 macOS 的取色效果
        Rectangle {
            id: bgRect
            anchors.fill: parent
            color: typeof Backend !== 'undefined' ? Backend.bgColor : "#D0ECF0F5"
        }

        // ——— 左侧半透明玻璃覆盖层 ———
        // 覆盖在背景之上，模拟 macOS 侧边栏的毛玻璃视觉效果
        Rectangle {
            id: leftBg
            x: 0; y: 0
            width: appWindow.sidebarWidth
            height: parent.height
            color: "#D0ECF0F5"    // 基础半透明白色

            // 渐变层：从上到下施加渐变透明度，增加玻璃质感
            Rectangle {
                anchors.fill: parent
                gradient: Gradient {
                    orientation: Gradient.Vertical
                    GradientStop { position: 0.00; color: "#30FFFFFF" }
                    GradientStop { position: 0.35; color: "#08FFFFFF" }
                    GradientStop { position: 0.70; color: "#00000000" }
                    GradientStop { position: 1.00; color: "#1CD0D8E0" }
                }
            }

            // 内侧高光边框：在玻璃层边缘添加微弱白色描边
            Rectangle {
                anchors.fill: parent
                color: "transparent"
                border.color: "#18FFFFFF"
                border.width: 1
            }
        }

        // ——— 右侧白色背景 ———
        // 内容区域使用纯白色背景，与左侧玻璃效果形成对比
        Rectangle {
            id: rightBg
            x: appWindow.sidebarWidth
            y: 0
            width: parent.width - appWindow.sidebarWidth
            height: parent.height
            color: "#FFFFFF"
        }

        // ——— 侧边栏拖拽分隔条 ———
        // 允许用户通过拖拽调整左侧侧边栏宽度
        MouseArea {
            id: splitter
            x: appWindow.sidebarWidth - 3   // 定位在侧边栏右边缘
            y: 0
            width: 6
            height: parent.height
            z: 100
            cursorShape: Qt.SizeHorCursor   // 水平缩放光标
            hoverEnabled: true

            // 拖拽时动态更新侧边栏宽度（限制范围 160~400px）
            onPositionChanged: function(mouse) {
                let newWidth = appWindow.sidebarWidth + mouse.x - 3
                if (newWidth >= 160 && newWidth <= 400)
                    appWindow.sidebarWidth = newWidth
            }

            // 悬停指示线：鼠标悬停或拖拽时显示蓝色竖线
            Rectangle {
                anchors.centerIn: parent
                width: 2
                height: parent.height
                color: splitter.containsMouse ? "#2F6EED" : "transparent"
                opacity: splitter.containsMouse ? 0.6 : 0
                Behavior on opacity { NumberAnimation { duration: 150 } }
            }
        }

        // ——— 左侧内容区域 ———
        // 包含窗口控制按钮和导航侧边栏
        Item {
            x: 0; y: 0
            width: appWindow.sidebarWidth
            height: parent.height
            z: 10

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                // ——— 顶部标题栏 ———
                // 放置 macOS 风格的"红绿灯"窗口控制按钮 + 窗口拖拽区域
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    color: "transparent"

                    // 红绿灯按钮：关闭/最小化/最大化
                    WindowControls {
                        anchors.left: parent.left
                        anchors.leftMargin: 16
                        anchors.verticalCenter: parent.verticalCenter
                        onMinimizeClicked: appWindow.showMinimized()
                        onMaximizeClicked: {
                            // 切换最大化/正常状态
                            appWindow.visibility === Window.Maximized
                                ? appWindow.showNormal()
                                : appWindow.showMaximized()
                        }
                        onCloseClicked: appWindow.close()
                    }

                    // 窗口拖拽区域：按住可拖动整个无边框窗口
                    MouseArea {
                        anchors.left: parent.left
                        anchors.leftMargin: 80
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        cursorShape: Qt.OpenHandCursor
                        onPressed: appWindow.startSystemMove()
                    }
                }

                // ——— 导航侧边栏 ———
                NavigationSidebar {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    // 处理导航请求：根据 navIndex 切换到对应页面
                    onNavigationRequested: function(navIndex) {
                        if (navIndex >= 0 && navIndex < pageMap.length) {
                            let target = pageMap[navIndex]
                            if (target === -1) openThemeWizard()   // 打开主题向导（独立窗口）
                            else contentStack.currentIndex = target  // 切换主内容区页面
                        } else if (navIndex === pageMap.length) {
                            contentStack.currentIndex = 3  // 切换到设置页面（SettingsPage）
                        }
                    }
                }
            }
        }

        // ——— 右侧内容区域 ———
        Item {
            id: rightContentPanel
            x: appWindow.sidebarWidth + 1
            y: 0
            width: parent.width - appWindow.sidebarWidth - 1
            height: parent.height
            z: 10

            // ——— 右侧顶部拖拽区域 ———
            // 覆盖在右侧内容区顶部 48px 区域，方便用户拖拽窗口
            Rectangle {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: 48
                color: "transparent"
                z: 10
                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.OpenHandCursor
                    onPressed: appWindow.startSystemMove()
                }
            }

            // ——— 页面内容容器 ———
            // 使用 StackLayout 实现多页面切换
            // 边距：上 48px（避开拖拽区）、左/右 30px、下 32px
            Item {
                anchors {
                    top: parent.top; topMargin: 48
                    left: parent.left; leftMargin: 30
                    right: parent.right; rightMargin: 30
                    bottom: parent.bottom; bottomMargin: 32
                }

                StackLayout {
                    id: contentStack
                    anchors.fill: parent
                    currentIndex: 0          // 默认显示首页

                    // === 页面栈（按索引排列） ===
                    // 0: HomePage          — 首页/欢迎页
                    // 1: BeautificationHub — 系统美化中心
                    // 2: CeleCraftPage     — CeleCraft 页面
                    // 3: SettingsPage      — 设置页面
                    // 4: WindowsFixPage    — Windows 系统修复页
                    HomePage {}
                    BeautificationHub {}
                    CeleCraftPage {}
                    SettingsPage {}
                    //WindowRoundPage {}     // 窗口圆角页（暂时注释禁用）
                    WindowsFixPage {}
                }
            }
        }

        // ——— 窗口边缘高光边框 ———
        // 在内侧添加半透明白色宽边框，增强圆角区域的层次感和立体感
        Rectangle {
            anchors.fill: parent
            color: "transparent"
            radius: 16
            border.color: "#70FFFFFF"
            border.width: 8
            z: 80
        }

        // ——— 外描边 ———
        // 在窗口最外缘添加暗色细线，衬托亮边框，使窗口轮廓更清晰
        Rectangle {
            anchors.fill: parent
            color: "transparent"
            radius: 16
            border.color: "#1A000000"
            border.width: 1
            z: 79
        }
    } // rootContent

    // === 窗口边缘缩放区域 ===
    // 8 个 MouseArea 分别覆盖窗口的四个边缘和四个角
    // 当鼠标悬停时显示对应缩放光标，按下时触发系统窗口缩放
    // z:99 确保始终位于最上层，不会被其他 UI 阻挡

    // 左边缘：水平缩放
    MouseArea { z:99; anchors.left: parent.left; anchors.top: parent.top; anchors.bottom: parent.bottom; width: 5; cursorShape: Qt.SizeHorCursor; onPressed: appWindow.startSystemResize(Qt.LeftEdge) }
    // 右边缘：水平缩放
    MouseArea { z:99; anchors.right: parent.right; anchors.top: parent.top; anchors.bottom: parent.bottom; width: 5; cursorShape: Qt.SizeHorCursor; onPressed: appWindow.startSystemResize(Qt.RightEdge) }
    // 上边缘：垂直缩放
    MouseArea { z:99; anchors.left: parent.left; anchors.right: parent.right; anchors.top: parent.top; height: 5; cursorShape: Qt.SizeVerCursor; onPressed: appWindow.startSystemResize(Qt.TopEdge) }
    // 下边缘：垂直缩放
    MouseArea { z:99; anchors.left: parent.left; anchors.right: parent.right; anchors.bottom: parent.bottom; height: 5; cursorShape: Qt.SizeVerCursor; onPressed: appWindow.startSystemResize(Qt.BottomEdge) }
    // 左上角：对角线缩放
    MouseArea { z:99; anchors.left: parent.left; anchors.top: parent.top; width: 5; height: 5; cursorShape: Qt.SizeFDiagCursor; onPressed: appWindow.startSystemResize(Qt.TopEdge | Qt.LeftEdge) }
    // 右上角：反对角线缩放
    MouseArea { z:99; anchors.right: parent.right; anchors.top: parent.top; width: 5; height: 5; cursorShape: Qt.SizeBDiagCursor; onPressed: appWindow.startSystemResize(Qt.TopEdge | Qt.RightEdge) }
    // 左下角：反对角线缩放
    MouseArea { z:99; anchors.left: parent.left; anchors.bottom: parent.bottom; width: 5; height: 5; cursorShape: Qt.SizeBDiagCursor; onPressed: appWindow.startSystemResize(Qt.BottomEdge | Qt.LeftEdge) }
    // 右下角：对角线缩放
    MouseArea { z:99; anchors.right: parent.right; anchors.bottom: parent.bottom; width: 5; height: 5; cursorShape: Qt.SizeFDiagCursor; onPressed: appWindow.startSystemResize(Qt.BottomEdge | Qt.RightEdge) }
}
