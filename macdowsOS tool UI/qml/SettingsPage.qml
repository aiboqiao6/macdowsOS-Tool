// ============================================================
// SettingsPage.qml — 设置页面（macOS 27 风格）
// macOS "系统设置" 面板风格
// 功能：提供应用的基本配置选项，如桌面图标显示/隐藏等
// ============================================================

// QtQuick 模块：提供 QML 核心类型（Rectangle、Item、Label 等基础组件）
import QtQuick
// Backend 模块：C++ 后端接口，提供设置操作的实际功能调用
import Backend 1.0
// QtQuick.Controls 模块：提供标准界面控件（Button、ScrollView 等）
import QtQuick.Controls

// === 根元素：透明的矩形容器，作为页面背景 ===
Rectangle {
    id: root
    color: "transparent"  // 背景透明，由父容器控制背景色

    // === 滚动视图：内容超出可视区域时支持滚动 ===
    ScrollView {
        anchors.fill: parent
        clip: true  // 裁剪超出部分，防止内容溢出

        // 隐藏垂直滚动条（保持 macOS 简洁风格）
        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AlwaysOff
        }

        // === 垂直列布局：排列所有设置项 ===
        Column {
            spacing: 14           // 子元素垂直间距 14px
            width: parent.width

            // === 页面标题："设置" ===
            Label {
                text: "设置"
                font { pixelSize: 20; weight: Font.Bold }  // 20px 加粗字体
                color: "#1D1D1F"         // 深灰色文字
                bottomPadding: 10
            }

            // === 桌面图标设置项 ===
            // 提供"隐藏"和"显示"两个按钮，控制 Windows 桌面图标的可见性
            Rectangle {
                width: parent.width
                height: 56       // 设置项行高 56px

                // === 水平行布局：左侧文字描述 + 右侧操作按钮 ===
                Row {
                    anchors {
                        left: parent.left; leftMargin: 18
                        right: parent.right; rightMargin: 18
                        verticalCenter: parent.verticalCenter
                    }
                    spacing: 10

                    // === 左侧：设置项名称和说明 ===
                    Column {
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 2
                        // 设置项名称
                        Label {
                            text: "桌面图标"
                            font { pixelSize: 14; weight: Font.Medium }
                            color: "#1D1D1F"
                        }
                        // 设置项说明
                        Label {
                            text: "显示或隐藏桌面图标"
                            font.pixelSize: 12
                            color: "#86868B"     // 浅灰色辅助文字
                        }
                    }

                    // 弹性填充项：将左右两侧推到两端
                    Item { height: 1; width: 1 }

                    // === "隐藏" 按钮：点击后隐藏桌面图标 ===
                    Button {
                        id: hideBtn
                        text: "隐藏"
                        implicitWidth: 64; implicitHeight: 30
                        anchors.verticalCenter: parent.verticalCenter
                        // 浅灰色圆角按钮，支持按下和悬停状态变色
                        background: Rectangle { radius: 10; color: hideBtn.down ? "#D0D0D5" : hideBtn.hovered ? "#DCDCE0" : "#E8E8ED" }
                        contentItem: Label { text: "隐藏"; color: "#1D1D1F"; font.pixelSize: 12; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                        // 调用后端接口，传入 false 表示隐藏桌面图标
                        onClicked: Backend.setDesktopIcons(false)
                    }

                    // === "显示" 按钮：点击后显示桌面图标 ===
                    Button {
                        id: showBtn
                        text: "显示"
                        implicitWidth: 64; implicitHeight: 30
                        anchors.verticalCenter: parent.verticalCenter
                        background: Rectangle { radius: 10; color: showBtn.down ? "#D0D0D5" : showBtn.hovered ? "#DCDCE0" : "#E8E8ED" }
                        contentItem: Label { text: "显示"; color: "#1D1D1F"; font.pixelSize: 12; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                        // 调用后端接口，传入 true 表示显示桌面图标
                        onClicked: Backend.setDesktopIcons(true)
                    }
                }
            }

            // 此处可扩展更多设置项（如开机启动、主题设置等）
            
            // === 页面底部留白 ===
            Item { width: 1; height: 24 }
        }
    }
}
