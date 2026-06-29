// ============================================================
// main.qml — ShaderEffectSource 蒙版方案
//   圆角 + 模糊，纯 QML，零 Win32/DWM
// ============================================================
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Effects

Window {
    id: appWindow
    title: "macdowsOS Tool"
    width: 780
    height: 520
    minimumWidth: 680
    minimumHeight: 480

    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowSystemMenuHint | Qt.WindowMinMaxButtonsHint
    color: "transparent"
    visible: true

    property var wizardWindow: null
    property var pageMap: [0, 1, 4]
    property int sidebarWidth: 222

    function openThemeWizard() {
        if (wizardWindow === null || wizardWindow === undefined) {
            wizardWindow = Qt.createComponent("qrc:/qml/ThemeWizardWindow.qml").createObject()
        }
        wizardWindow.show()
        wizardWindow.raise()
        wizardWindow.requestActivate()
        wizardWindow.closing.connect(function() {
            wizardWindow = null
        })
    }

    // ============================================================
    // 圆角蒙版 — 白色圆角矩形，ShaderEffectSource 捕获其亮度纹理
    // ============================================================
    Rectangle {
        id: cornerMask
        anchors.fill: parent
        color: "white"
        radius: 16
        visible: true
    }

    ShaderEffectSource {
        id: maskTexture
        sourceItem: cornerMask
        hideSource: true      // 蒙版不可见，但纹理实时更新供 maskSource 使用
    }

    // ============================================================
    // 根内容 — 所有 UI 放在这里，用 layer + mask 裁剪圆角
    // ============================================================
    Item {
        id: rootContent
        anchors.fill: parent
        layer.enabled: true
        layer.effect: MultiEffect {
            maskEnabled: true
            maskSource: maskTexture
        }

        // ——— 取色背景（C++ 提取窗口背后平均颜色） ———
        Rectangle {
            id: bgRect
            anchors.fill: parent
            color: Backend.bgColor
        }

        // ——— 左侧半透明玻璃覆盖 ———
        Rectangle {
            id: leftBg
            x: 0; y: 0
            width: appWindow.sidebarWidth
            height: parent.height
            color: "#D0ECF0F5"

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

            Rectangle {
                anchors.fill: parent
                color: "transparent"
                border.color: "#18FFFFFF"
                border.width: 1
            }
        }

        // ——— 右侧白色背景 ———
        Rectangle {
            id: rightBg
            x: appWindow.sidebarWidth
            y: 0
            width: parent.width - appWindow.sidebarWidth
            height: parent.height
            color: "#FFFFFF"
        }

        // ——— 左侧/右侧拖拽分隔条 ———
        MouseArea {
            id: splitter
            x: appWindow.sidebarWidth - 3
            y: 0
            width: 6
            height: parent.height
            z: 100
            cursorShape: Qt.SizeHorCursor
            hoverEnabled: true
            onPositionChanged: function(mouse) {
                var newWidth = appWindow.sidebarWidth + mouse.x - 3
                if (newWidth >= 160 && newWidth <= 400)
                    appWindow.sidebarWidth = newWidth
            }

            Rectangle {
                anchors.centerIn: parent
                width: 2
                height: parent.height
                color: splitter.containsMouse ? "#2F6EED" : "transparent"
                opacity: splitter.containsMouse ? 0.6 : 0
                Behavior on opacity { NumberAnimation { duration: 150 } }
            }
        }

        // ——— 左侧内容 ———
        Item {
            x: 0; y: 0
            width: appWindow.sidebarWidth
            height: parent.height
            z: 10

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    color: "transparent"

                    WindowControls {
                        anchors.left: parent.left
                        anchors.leftMargin: 16
                        anchors.verticalCenter: parent.verticalCenter
                        onMinimizeClicked: appWindow.showMinimized()
                        onMaximizeClicked: {
                            appWindow.visibility === Window.Maximized
                                ? appWindow.showNormal()
                                : appWindow.showMaximized()
                        }
                        onCloseClicked: appWindow.close()
                    }

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

                NavigationSidebar {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    onNavigationRequested: function(navIndex) {
                        // navModel.count = 设置项
                        if (navIndex >= 0 && navIndex < pageMap.length) {
                            var target = pageMap[navIndex]
                            if (target === -1) openThemeWizard()
                            else contentStack.currentIndex = target
                        } else if (navIndex === pageMap.length) {
                            contentStack.currentIndex = 3  // SettingsPage
                        }
                    }
                }
            }
        }

        // ——— 右侧内容 ———
        Item {
            id: rightContentPanel
            x: appWindow.sidebarWidth + 1
            y: 0
            width: parent.width - appWindow.sidebarWidth - 1
            height: parent.height
            z: 10

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
                    currentIndex: 0
                    HomePage {}
                    BeautificationHub {}
                    CeleCraftPage {}
                    SettingsPage {}
                    //WindowRoundPage {}
                    WindowsFixPage {}
                }
            }
        }

        // ——— 窗口边缘边框（加深层级感） ———
        Rectangle {
            anchors.fill: parent
            color: "transparent"
            radius: 16
            border.color: "#70FFFFFF"
            border.width: 8
            z: 80
        }

        // ——— 外描边（暗色细线衬托亮边框） ———
        Rectangle {
            anchors.fill: parent
            color: "transparent"
            radius: 16
            border.color: "#1A000000"
            border.width: 1
            z: 79
        }
    } // rootContent

    // ——— 窗口边缘缩放 ———
    MouseArea { z:99; anchors.left: parent.left; anchors.top: parent.top; anchors.bottom: parent.bottom; width: 5; cursorShape: Qt.SizeHorCursor; onPressed: appWindow.startSystemResize(Qt.LeftEdge) }
    MouseArea { z:99; anchors.right: parent.right; anchors.top: parent.top; anchors.bottom: parent.bottom; width: 5; cursorShape: Qt.SizeHorCursor; onPressed: appWindow.startSystemResize(Qt.RightEdge) }
    MouseArea { z:99; anchors.left: parent.left; anchors.right: parent.right; anchors.top: parent.top; height: 5; cursorShape: Qt.SizeVerCursor; onPressed: appWindow.startSystemResize(Qt.TopEdge) }
    MouseArea { z:99; anchors.left: parent.left; anchors.right: parent.right; anchors.bottom: parent.bottom; height: 5; cursorShape: Qt.SizeVerCursor; onPressed: appWindow.startSystemResize(Qt.BottomEdge) }
    MouseArea { z:99; anchors.left: parent.left; anchors.top: parent.top; width: 5; height: 5; cursorShape: Qt.SizeFDiagCursor; onPressed: appWindow.startSystemResize(Qt.TopEdge | Qt.LeftEdge) }
    MouseArea { z:99; anchors.right: parent.right; anchors.top: parent.top; width: 5; height: 5; cursorShape: Qt.SizeBDiagCursor; onPressed: appWindow.startSystemResize(Qt.TopEdge | Qt.RightEdge) }
    MouseArea { z:99; anchors.left: parent.left; anchors.bottom: parent.bottom; width: 5; height: 5; cursorShape: Qt.SizeBDiagCursor; onPressed: appWindow.startSystemResize(Qt.BottomEdge | Qt.LeftEdge) }
    MouseArea { z:99; anchors.right: parent.right; anchors.bottom: parent.bottom; width: 5; height: 5; cursorShape: Qt.SizeFDiagCursor; onPressed: appWindow.startSystemResize(Qt.BottomEdge | Qt.RightEdge) }
}
