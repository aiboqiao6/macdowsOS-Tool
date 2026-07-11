// ============================================================
// HomePage.qml — 首页/欢迎页组件
// 居中布局，仿 macOS "关于本机"(About This Mac) 设计风格
// 显示应用名称、描述、版本号及引导提示
// ============================================================

// QtQuick — 基础 QML 模块，提供 Rectangle、Column 等核心类型
import QtQuick
// QtQuick.Controls — 提供 Label 等标准控件
import QtQuick.Controls

// === 首页根容器 ===
Rectangle {
    id: root
    color: "transparent"   // 透明背景，由右侧白色区域承载

    // === 垂直居中布局 ===
    // 所有内容在父容器中水平和垂直居中排列
    Column {
        anchors.centerIn: parent
        spacing: 10         // 子元素之间的垂直间距

        // ——— 应用主标题 ———
        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "macdowsOS Tool"
            font { pixelSize: 40; weight: Font.Bold }   // 大号加粗标题
            color: "#1D1D1F"                             // macOS 深灰文字色
        }

        // ——— 应用副标题/描述 ———
        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "自动化的Windows仿macOS美化工具"        // 应用功能简述
            font.pixelSize: 15
            color: "#6E6E73"                             // macOS 副文本灰
            horizontalAlignment: Text.AlignHCenter
            lineHeight: 1
        }

        // ——— 分隔线 ———
        // 细横线用于视觉分隔标题区域和版本信息
        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            width: 220
            height: 1
            color: "#D2D2D7"                             // 浅灰色分隔线
        }

        // ——— 版本号 ———
        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "版本 1.30 RC2"
            font.pixelSize: 14
            color: "#86868B"                             // macOS 浅灰色
        }

        // ——— 预留区域（功能按钮位，当前为空） ———
        // 原设计可能在此放置操作按钮，目前为空保留占位
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 14
            topPadding: 12

           
           
        }

        // ——— 底部引导提示 ———
        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "使用左侧导航栏选择要执行的操作"
            font.pixelSize: 13
            color: "#A1A1A6"                             // 最浅的辅助文字色
            topPadding: 12
        }
    }
}
