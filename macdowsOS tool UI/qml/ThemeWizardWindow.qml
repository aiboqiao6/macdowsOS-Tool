// ============================================================
// ThemeWizardWindow.qml — 安装向导窗口（主窗口风格统一版）
// 玻璃质感 + #2F6EED 主题色 + WindowControls
// ============================================================
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Effects
import Backend 1.0

Window {
    id: wizardWindow
    title: "安装 macdowsOS"
    width: 640
    height: 500
    minimumWidth: 560
    minimumHeight: 440

    // 无边框窗口
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowSystemMenuHint

    color: "transparent"

    // 窗口居中显示
    Component.onCompleted: {
        setX(Screen.width / 2 - width / 2)
        setY(Screen.height / 2 - height / 2)
    }

    // 向导状态
    property int step: 0
    property int selectedVersion: 10
    property int installProgress: 0
    property int installTotal: 0

    // 重置所有状态
    function resetWizard() {
        step = 0
        installProgress = 0
        installTotal = 0
    }

    // 关闭窗口
    function closeWizard() {
        resetWizard()
        wizardWindow.close()
    }

    // ============================================================
    // 圆角蒙版 — 与主窗口相同的 ShaderEffectSource 方案
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
        hideSource: true
    }

    // ============================================================
    // 主内容 — 使用 layer + mask 裁剪圆角
    // ============================================================
    Item {
        id: rootContent
        anchors.fill: parent
        layer.enabled: true
        layer.effect: MultiEffect {
            maskEnabled: true
            maskSource: maskTexture
        }

        // ——— 纯白背景 ———
        Rectangle {
            id: bgWhite
            anchors.fill: parent
            color: "#FFFFFF"
        }

        // ============================================================
        // 内容区域（覆盖在背景之上）
        // ============================================================
        ColumnLayout {
            anchors.fill: parent
            spacing: 0

            // ---- 标题栏 (与主窗口风格一致) ----
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 48
                color: "transparent"

                // 窗口控制按钮（复用 WindowControls 组件）
                WindowControls {
                    anchors.left: parent.left
                    anchors.leftMargin: 16
                    anchors.verticalCenter: parent.verticalCenter
                    onCloseClicked: closeWizard()
                    onMinimizeClicked: wizardWindow.showMinimized()
                    onMaximizeClicked: {
                        wizardWindow.visibility === Window.Maximized
                            ? wizardWindow.showNormal()
                            : wizardWindow.showMaximized()
                    }
                }

                // 拖动区域
                MouseArea {
                    anchors.left: parent.left
                    anchors.leftMargin: 80
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    cursorShape: Qt.OpenHandCursor
                    property variant clickPos: "0,0"
                    onPressed: { clickPos = Qt.point(mouse.x, mouse.y) }
                    onPositionChanged: {
                        var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)
                        wizardWindow.setX(wizardWindow.x + delta.x)
                        wizardWindow.setY(wizardWindow.y + delta.y)
                    }
                }

                // 居中标题
                Label {
                    anchors.centerIn: parent
                    text: "安装 macdowsOS"
                    color: "#1D1D1F"
                    font { pixelSize: 14; weight: Font.Medium }
                    opacity: 0.85
                }
            }

            // ---- 步骤指示器 (仅步骤 0-2 显示) ----
            Row {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 14
                Layout.bottomMargin: 6
                spacing: 8
                visible: step < 3

                Repeater {
                    model: 4
                    delegate: Rectangle {
                        width: 8; height: 8
                        radius: 4
                        color: step === index ? "#2F6EED" : "#C7C7CC"
                        Behavior on color { ColorAnimation { duration: 200 } }
                    }
                }
            }

            // ---- 内容区域 ----
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: 40

                // 步骤 0: 欢迎页
                Column {
                    visible: step === 0
                    anchors.centerIn: parent
                    spacing: 18

                    Item { width: 1; height: 24 }

                    Image {
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: "qrc:/res/WindowsToMac.png"
                        width: 88; height: 88
                        fillMode: Image.PreserveAspectFit
                    }

                    Label {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "欢迎使用 macdowsOS"
                        font { pixelSize: 26; weight: Font.Bold }
                        color: "#1D1D1F"
                    }

                    Label {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "此向导将帮助您把 Windows 界面\n转换为 macOS 风格"
                        font.pixelSize: 14
                        color: "#6E6E73"
                        horizontalAlignment: Text.AlignHCenter
                        lineHeight: 1.6
                    }

                    Item { width: 1; height: 10 }

                    Rectangle {
                        anchors.horizontalCenter: parent.horizontalCenter
                        width: parent.width - 40
                        height: 44
                        color: "#F5F5F7"
                        radius: 10
                        border { color: "#D2D2D7"; width: 1 }

                        Row {
                            anchors.centerIn: parent
                            spacing: 8

                            Label {
                                text: "?"
                                font { pixelSize: 15; bold: true }
                                color: "#2F6EED"
                                anchors.verticalCenter: parent.verticalCenter
                            }

                            Label {
                                text: "建议先创建系统还原点"
                                font.pixelSize: 14
                                color: "#2F6EED"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: Backend.createRestorePoint("macdowsOS 美化前备份")
                        }
                    }
                }

                // 步骤 1: 选择版本
                Column {
                    visible: step === 1
                    anchors.centerIn: parent
                    spacing: 22

                    Label {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "选择您的 Windows 版本"
                        font { pixelSize: 20; weight: Font.Bold }
                        color: "#1D1D1F"
                    }

                    Label {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "选择后将自动安装对应的 macOS 风格主题"
                        font.pixelSize: 13
                        color: "#86868B"
                    }

                    Item { width: 1; height: 6 }

                    Row {
                        spacing: 18
                        anchors.horizontalCenter: parent.horizontalCenter

                        Rectangle {
                            width: 210; height: 190
                            radius: 14
                            color: selectedVersion === 10 ? "#E8F0FE" : "#FFFFFF"
                            border {
                                color: selectedVersion === 10 ? "#2F6EED" : "#D2D2D7"
                                width: selectedVersion === 10 ? 2 : 1
                            }
                            Behavior on border.color { ColorAnimation { duration: 150 } }

                            Column {
                                anchors.centerIn: parent
                                spacing: 14

                                Rectangle {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    width: 52; height: 52
                                    radius: 26
                                    color: selectedVersion === 10 ? "#2F6EED" : "#F5F5F7"
                                    Label {
                                        anchors.centerIn: parent
                                        text: "10"
                                        font { pixelSize: 22; weight: Font.Bold }
                                        color: selectedVersion === 10 ? "#FFFFFF" : "#86868B"
                                    }
                                }

                                Label {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    text: "Windows 10"
                                    font { pixelSize: 17; weight: Font.Bold }
                                    color: "#1D1D1F"
                                }

                                Label {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    text: "macOS Big Sur 风格"
                                    font.pixelSize: 13
                                    color: "#86868B"
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: selectedVersion = 10
                            }
                        }

                        Rectangle {
                            width: 210; height: 190
                            radius: 14
                            color: selectedVersion === 11 ? "#E8F0FE" : "#FFFFFF"
                            border {
                                color: selectedVersion === 11 ? "#2F6EED" : "#D2D2D7"
                                width: selectedVersion === 11 ? 2 : 1
                            }
                            Behavior on border.color { ColorAnimation { duration: 150 } }

                            Column {
                                anchors.centerIn: parent
                                spacing: 14

                                Rectangle {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    width: 52; height: 52
                                    radius: 26
                                    color: selectedVersion === 11 ? "#2F6EED" : "#F5F5F7"
                                    Label {
                                        anchors.centerIn: parent
                                        text: "11"
                                        font { pixelSize: 22; weight: Font.Bold }
                                        color: selectedVersion === 11 ? "#FFFFFF" : "#86868B"
                                    }
                                }

                                Label {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    text: "Windows 11"
                                    font { pixelSize: 17; weight: Font.Bold }
                                    color: "#1D1D1F"
                                }

                                Label {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    text: "macOS Ventura 风格"
                                    font.pixelSize: 13
                                    color: "#86868B"
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: selectedVersion = 11
                            }
                        }
                    }
                }

                // 步骤 2: 安装过程
                Column {
                    visible: step === 2
                    anchors.centerIn: parent
                    spacing: 18
                    width: parent.width

                    Label {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "正在安装..."
                        font { pixelSize: 20; weight: Font.Bold }
                        color: "#1D1D1F"
                    }

                    Item { width: 1; height: 10 }

                    Rectangle {
                        anchors.horizontalCenter: parent.horizontalCenter
                        width: parent.width - 40
                        height: 6
                        radius: 3
                        color: "#E8E8ED"

                        Rectangle {
                            id: progressBar
                            height: parent.height
                            radius: 3
                            color: "#2F6EED"
                            width: (installTotal > 0 ? installProgress / installTotal : 0) * parent.width
                            Behavior on width { NumberAnimation { duration: 300 } }
                        }
                    }

                    Item { width: 1; height: 6 }

                    Label {
                        id: statusLabel
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "准备中..."
                        font.pixelSize: 15
                        color: "#6E6E73"
                    }

                    Label {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "请勿关闭此窗口"
                        font.pixelSize: 13
                        color: "#A1A1A6"
                    }

                    // macOS 风格菊花加载
                    Item {
                        anchors.horizontalCenter: parent.horizontalCenter
                        width: 24; height: 24

                        Repeater {
                            model: 12
                            Rectangle {
                                x: 11; y: 2
                                width: 2; height: 6
                                radius: 1
                                color: "#2F6EED"
                                opacity: 0.15 + 0.85 * (index / 12)
                                transform: Rotation {
                                    origin.x: 1
                                    origin.y: 10
                                    angle: index * 30
                                }
                            }
                        }
                    }
                }

                // 步骤 3: 完成
                Column {
                    visible: step === 3
                    anchors.centerIn: parent
                    spacing: 18

                    Rectangle {
                        anchors.horizontalCenter: parent.horizontalCenter
                        width: 60; height: 60
                        radius: 30
                        color: "#E8F5E9"

                        Label {
                            anchors.centerIn: parent
                            text: "✓"
                            font { pixelSize: 30; weight: Font.Bold }
                            color: "#34C759"
                        }
                    }

                    Label {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "安装完成"
                        font { pixelSize: 24; weight: Font.Bold }
                        color: "#1D1D1F"
                    }

                    Label {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "系统美化已成功应用\n重启资源管理器后可看到效果"
                        font.pixelSize: 14
                        color: "#6E6E73"
                        horizontalAlignment: Text.AlignHCenter
                        lineHeight: 1.5
                    }

                    Item { width: 1; height: 6 }

                    Button {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "重启资源管理器"
                        implicitWidth: 190; implicitHeight: 38
                        background: Rectangle {
                            radius: 10
                            color: parent.down ? "#1E5ACC" : parent.hovered ? "#1E5ACC" : "#2F6EED"
                        }
                        contentItem: Label {
                            text: "重启资源管理器"
                            color: "#FFFFFF"
                            font.pixelSize: 14
                            font.weight: Font.Medium
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        onClicked: Backend.restartExplorer()
                    }
                }
            }

            // ---- 底部按钮栏 (与主窗口风格一致) ----
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 60
                color: "transparent"

                // 顶部分隔线
                Rectangle {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 1
                    color: "#E0E0E0"
                }

                Row {
                    anchors {
                        left: parent.left; leftMargin: 24
                        right: parent.right; rightMargin: 24
                        verticalCenter: parent.verticalCenter
                    }
                    spacing: 14

                    Button {
                        text: "后退"
                        visible: step > 0 && step < 2
                        implicitWidth: 84; implicitHeight: 34
                        background: Rectangle {
                            radius: 10
                            color: parent.down ? "#D0D0D5" : parent.hovered ? "#DCDCE0" : "#E8E8ED"
                        }
                        contentItem: Label {
                            text: "后退"
                            color: "#1D1D1F"
                            font.pixelSize: 13
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        onClicked: step--
                    }

                    Item { Layout.fillWidth: true; height: 1; width: 1 }

                    Button {
                        id: actionButton
                        text: step === 0 ? "继续" :
                              step === 1 ? "开始安装" :
                              step === 2 ? "安装中..." :
                              step === 3 ? "完成" : ""
                        enabled: step !== 2
                        implicitWidth: step === 0 ? 84 : 130
                        implicitHeight: 38

                        background: Rectangle {
                            radius: 10
                            color: {
                                if (!parent.enabled) return "#C7C7CC"
                                if (parent.down) return "#1E5ACC"
                                return "#2F6EED"
                            }
                        }

                        contentItem: Label {
                            text: parent.text
                            color: "#FFFFFF"
                            font { pixelSize: 14; weight: Font.Medium }
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        onClicked: {
                            if (step === 0) {
                                step = 1
                            } else if (step === 1) {
                                step = 2
                                runInstallation()
                            } else if (step === 3) {
                                closeWizard()
                            }
                        }
                    }
                }
            }
        }

        // ——— 窗口边缘边框 ———
        Rectangle {
            anchors.fill: parent
            color: "transparent"
            radius: 16
            border.color: "#70FFFFFF"
            border.width: 8
            z: 80
        }

        // ——— 外描边 ———
        Rectangle {
            anchors.fill: parent
            color: "transparent"
            radius: 16
            border.color: "#1A000000"
            border.width: 1
            z: 79
        }
    }

    // ============================================
    // 安装流程
    // ============================================
    function runInstallation() {
        var steps
        if (selectedVersion === 10) {
            steps = [
                { name: "安装 macOS 主题", action: function() { Backend.installThemeWin10(); } },
                { name: "安装主题路径补丁", action: function() { Backend.patchThemePath(); } },
                { name: "安装 myDockFinder", action: function() { Backend.installMyDockFinder(); } },
                { name: "安装 OldNewExplorer", action: function() { Backend.installOldNewExplorer(); } },
                { name: "安装系统图标", action: function() { Backend.installIcons(); } },
                { name: "执行最终设置", action: function() { Backend.finalSetting(); } }
            ]
        } else {
            steps = [
                { name: "安装 macOS 主题", action: function() { Backend.installThemeWin11(); } },
                { name: "安装主题路径补丁", action: function() { Backend.patchThemePath(); } },
                { name: "安装 myDockFinder", action: function() { Backend.installMyDockFinder(); } },
                { name: "安装 OldNewExplorer", action: function() { Backend.installOldNewExplorer(); } },
                { name: "安装 StartAllBack", action: function() { Backend.installStartAllBack(); } },
                { name: "安装系统图标", action: function() { Backend.installIcons(); } },
                { name: "执行最终设置", action: function() { Backend.finalSetting(); } }
            ]
        }
        installTotal = steps.length
        installProgress = 0
        runNextStep(steps, 0)
    }

    function runNextStep(steps, index) {
        if (index >= steps.length) {
            step = 3
            return
        }
        statusLabel.text = steps[index].name
        installProgress = index + 1
        Qt.callLater(function() {
            steps[index].action()
            runNextStep(steps, index + 1)
        })
    }
}
