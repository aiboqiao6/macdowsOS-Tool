import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import Backend 1.0

Window {
    id: appWindow
    title: "macdowsOS Tool"
    width: 900
    height: 620
    minimumWidth: 780
    minimumHeight: 540
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowSystemMenuHint | Qt.WindowMinMaxButtonsHint
    color: "transparent"
    visible: true

    property var wizardWindow: null
    property var pageMap: [0, 1, 4]
    property bool startupUpdateCheckPending: true
    readonly property int sidebarWidth: 232
    readonly property int titleBarHeight: 52

    // 仿 macOS 更新全屏遮挡窗口（自动化安装/卸载期间覆盖安装器弹窗）
    UpdateOverlayWindow {
        id: overlayWindow
    }

    Connections {
        target: Backend
        // 单组件操作（OverlayGuard）触发的显示/隐藏；
        // 全流程安装期间以内层子操作不重置进度、不中途隐藏
        function onOverlayRequested(show) {
            if (show) {
                if (!overlayWindow.visible)
                    overlayWindow.begin(1)
            } else if (!Backend.installationRunning && !Backend.restartPending) {
                // 重启待定时永不隐藏：保持"正在准备重新启动…"直至 Windows 接管
                overlayWindow.visible = false
            }
        }
        // 完整安装流程：由 installation* 信号驱动
        function onInstallationStarted(totalSteps) {
            overlayWindow.begin(totalSteps)
        }
        function onInstallationStepChanged(stepName) {
            // 步骤名仅写入日志，界面保持 Apple 原生样式（不显示步骤文字）
        }
        function onInstallationProgressChanged(completedSteps, totalSteps) {
            overlayWindow.updateProgress("", completedSteps, totalSteps)
        }
        function onInstallationFinished(success, errorMessage) {
            if (success) {
                // 最后一步即将重启系统，遮挡层保持显示直至关机
                overlayWindow.finish(true)
            } else {
                overlayWindow.finish(false)
            }
        }
        // 重启待定（安装/卸载流程任一路径）：统一切换到"正在准备重新启动…"终态，
        // 遮挡层此后永不隐藏，直至 Windows 接管重启
        function onRestartPendingChanged() {
            if (Backend.restartPending && overlayWindow.visible
                    && !overlayWindow.restarting) {
                overlayWindow.restarting = true
                overlayWindow.displayRatio = 1
                overlayWindow.timeLabel.text = "正在准备重新启动…"
            }
        }
    }

    function openThemeWizard(version) {
        if (wizardWindow !== null && wizardWindow !== undefined) {
            wizardWindow.selectedVersion = version
            wizardWindow.show()
            wizardWindow.raise()
            wizardWindow.requestActivate()
            return
        }

        let component = Qt.createComponent("qrc:/qml/ThemeWizardWindow.qml")
        if (component.status === Component.Ready) {
            wizardWindow = component.createObject(null, { selectedVersion: version })
            if (wizardWindow) {
                wizardWindow.show()
                wizardWindow.raise()
                wizardWindow.requestActivate()
                wizardWindow.closing.connect(function() {
                    wizardWindow.destroy()
                    wizardWindow = null
                })
            }
        } else {
            console.error("ThemeWizardWindow 加载失败:", component.errorString())
        }
    }

    Rectangle {
        id: rootContent
        anchors.fill: parent
        radius: 18
        clip: true
        color: "#F5F5F7"
        border.color: "#D8D8DE"
        border.width: 1

        RowLayout {
            anchors.fill: parent
            spacing: 0

            Rectangle {
                id: sidebarMaterial
                Layout.preferredWidth: appWindow.sidebarWidth
                Layout.fillHeight: true
                color: "#E9E9EE"

                Rectangle {
                    anchors.fill: parent
                    color: "#28FFFFFF"
                }

                Rectangle {
                    anchors.fill: parent
                    color: "#40FFFFFF"
                    opacity: 0.18
                    gradient: Gradient {
                        orientation: Gradient.Vertical
                        GradientStop { position: 0.0; color: "#55FFFFFF" }
                        GradientStop { position: 0.45; color: "#18FFFFFF" }
                        GradientStop { position: 1.0; color: "#38D7D7E0" }
                    }
                }

                ColumnLayout {
                    id: sidebarColumn
                    anchors.fill: parent
                    spacing: 0

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: appWindow.titleBarHeight
                        color: "transparent"

                        WindowControls {
                            anchors.left: parent.left
                            anchors.leftMargin: 18
                            anchors.verticalCenter: parent.verticalCenter
                            onMinimizeClicked: appWindow.showMinimized()
                            onMaximizeClicked: appWindow.visibility === Window.Maximized ? appWindow.showNormal() : appWindow.showMaximized()
                            onCloseClicked: appWindow.close()
                        }

                        MouseArea {
                            anchors.left: parent.left
                            anchors.leftMargin: 92
                            anchors.right: parent.right
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            cursorShape: Qt.OpenHandCursor
                            onPressed: appWindow.startSystemMove()
                        }
                    }

                    NavigationSidebar {
                        id: navigationSidebar
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        onNavigationRequested: function(navIndex) {
                            if (navIndex >= 0 && navIndex < pageMap.length) {
                                contentStack.currentIndex = pageMap[navIndex]
                            } else if (navIndex === pageMap.length) {
                                contentStack.currentIndex = 3
                            }
                        }
                    }
                }

                Rectangle {
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width: 1
                    color: "#D1D1D8"
                }
            }

            ColumnLayout {
                id: contentColumn
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 0
                Rectangle {
                    z: -1
                    anchors.fill: parent
                    color: "#F5F5F7"
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: appWindow.titleBarHeight
                    color: "#F5F5F7"

                    Label {
                        anchors.centerIn: parent
                        text: "macdowsOS Tool"
                        color: "#6E6E73"
                        font { pixelSize: 12; weight: Font.Medium }
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.OpenHandCursor
                        onPressed: appWindow.startSystemMove()
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    StackLayout {
                        id: contentStack
                        anchors.fill: parent
                        anchors.leftMargin: 40
                        anchors.rightMargin: 40
                        anchors.topMargin: 22
                        anchors.bottomMargin: 34
                        currentIndex: 0

                        HomePage {}
                        BeautificationHub {}
                        CeleCraftPage {}
                        SettingsPage {}
                        WindowsFixPage {}
                    }
                }
            }
        }

    }

    Connections {
        target: Backend.updateChecker

        function onUpdateStateChanged() {
            if (!appWindow.startupUpdateCheckPending)
                return

            const state = Backend.updateChecker.updateState
            if (state !== "available" && state !== "up-to-date" && state !== "error")
                return

            appWindow.startupUpdateCheckPending = false
            if (state === "available") {
                contentStack.currentIndex = 3
                navigationSidebar.selectedIndex = 3
            }
        }
    }

    Component.onCompleted: Backend.updateChecker.checkForUpdates()

    MouseArea { z:99; anchors.left: parent.left; anchors.top: parent.top; anchors.bottom: parent.bottom; width: 5; cursorShape: Qt.SizeHorCursor; onPressed: appWindow.startSystemResize(Qt.LeftEdge) }
    MouseArea { z:99; anchors.right: parent.right; anchors.top: parent.top; anchors.bottom: parent.bottom; width: 5; cursorShape: Qt.SizeHorCursor; onPressed: appWindow.startSystemResize(Qt.RightEdge) }
    MouseArea { z:99; anchors.left: parent.left; anchors.right: parent.right; anchors.top: parent.top; height: 5; cursorShape: Qt.SizeVerCursor; onPressed: appWindow.startSystemResize(Qt.TopEdge) }
    MouseArea { z:99; anchors.left: parent.left; anchors.right: parent.right; anchors.bottom: parent.bottom; height: 5; cursorShape: Qt.SizeVerCursor; onPressed: appWindow.startSystemResize(Qt.BottomEdge) }
    MouseArea { z:99; anchors.left: parent.left; anchors.top: parent.top; width: 5; height: 5; cursorShape: Qt.SizeFDiagCursor; onPressed: appWindow.startSystemResize(Qt.TopEdge | Qt.LeftEdge) }
    MouseArea { z:99; anchors.right: parent.right; anchors.top: parent.top; width: 5; height: 5; cursorShape: Qt.SizeBDiagCursor; onPressed: appWindow.startSystemResize(Qt.TopEdge | Qt.RightEdge) }
    MouseArea { z:99; anchors.left: parent.left; anchors.bottom: parent.bottom; width: 5; height: 5; cursorShape: Qt.SizeBDiagCursor; onPressed: appWindow.startSystemResize(Qt.BottomEdge | Qt.LeftEdge) }
    MouseArea { z:99; anchors.right: parent.right; anchors.bottom: parent.bottom; width: 5; height: 5; cursorShape: Qt.SizeFDiagCursor; onPressed: appWindow.startSystemResize(Qt.BottomEdge | Qt.RightEdge) }
}
