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
            Label {
                text: "macdowsOS Tool UI"
                font { pixelSize: 15; weight: Font.Bold }  // 20px 加粗字体
                color: "#6E6E73"         // 深灰色文字
                bottomPadding: 5
            }
            Label {
                text: "雾蓝回针MistBlueSt"
                font { pixelSize: 15; weight: Font.Bold }  // 20px 加粗字体
                color: "#6E6E73"         // 深灰色文字
                bottomPadding: 5
            }
            Label {
                text: "版本 1.30 RC2 版本号 20260711130C2"
                font { pixelSize: 15; weight: Font.Bold }  // 20px 加粗字体
                color: "#6E6E73"         // 深灰色文字
                bottomPadding: 5
            }

            // 此处可扩展更多设置项（如开机启动、主题设置等）
            
            // === 页面底部留白 ===
            Item { width: 1; height: 24 }
        }
    }
}
