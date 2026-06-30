// ============================================================
// WindowControls.qml — 仿 macOS 经典"红绿灯"窗口控制按钮组件
// 提供 关闭(红)、最小化(黄)、最大化(绿) 三个圆形按钮
// 悬停时显示对应操作图标，14px 标准尺寸适配 macOS 风格
// ============================================================

// QtQuick — 基础 QML 模块，提供核心类型如 Item、Rectangle、MouseArea 等
import QtQuick

// === 窗口控制按钮行容器 ===
// 使用 Row 水平排列三个按钮，间距 8px
Row {
    id: root
    spacing: 8

    // ——— 信号定义 ———
    signal minimizeClicked()    // 最小化按钮被点击时触发
    signal maximizeClicked()    // 最大化按钮被点击时触发
    signal closeClicked()       // 关闭按钮被点击时触发

    // ——— 属性定义 ———
    property bool showDivider: false   // 是否显示分隔线（预留，暂未使用）
    property bool showMinimize: true   // 是否显示最小化按钮
    property bool showMaximize: true   // 是否显示最大化按钮

    // ---- 关闭按钮（红色 #EC6765） ----
    // macOS 风格红绿灯中最左侧的红色圆点
    Item {
        width: 14
        height: 14

        Rectangle {
            anchors.fill: parent
            radius: width / 2        // 圆形：半径 = 宽度的一半
            color: "#EC6765"         // macOS 标准红色

            // 悬停时显示的 "×" 图标 — 由两条旋转线条组成
            // 第一条线：45° 斜线（左上到右下）
            Rectangle {
                anchors.centerIn: parent
                width: 9; height: 1.5
                color: "#773432"
                radius: 0.5
                rotation: 45
                opacity: closeMouse.containsMouse ? 0.8 : 0   // 仅鼠标悬停时可见
                Behavior on opacity { NumberAnimation { duration: 100 } }  // 淡入/淡出动画
            }
            // 第二条线：-45° 斜线（右上到左下），与上一条组成 "×"
            Rectangle {
                anchors.centerIn: parent
                width: 9; height: 1.5
                color: "#773432"
                radius: 0.5
                rotation: -45
                opacity: closeMouse.containsMouse ? 0.8 : 0
                Behavior on opacity { NumberAnimation { duration: 100 } }
            }
        }

        // 鼠标交互区域：处理悬停状态和点击事件
        MouseArea {
            id: closeMouse
            anchors.fill: parent
            hoverEnabled: true           // 启用悬停检测，用于控制图标显隐
            cursorShape: Qt.PointingHandCursor  // 鼠标悬停时显示手型指针
            onClicked: root.closeClicked()      // 点击时向外发射关闭信号
        }
    }

    // ---- 最小化按钮（黄色 #F2CA44） ----
    // macOS 风格红绿灯中间的黄色圆点
    Item {
        visible: root.showMinimize
        width: 14
        height: 14

        Rectangle {
            anchors.fill: parent
            radius: width / 2
            color: "#F2CA44"           // macOS 标准黄色

            // 悬停时显示的 "—" 图标 — 一条短横线
            Rectangle {
                anchors.centerIn: parent
                width: 9; height: 1.5
                color: "#7A6522"
                radius: 0.5
                opacity: minMouse.containsMouse ? 0.8 : 0
                Behavior on opacity { NumberAnimation { duration: 100 } }
            }
        }

        MouseArea {
            id: minMouse
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: root.minimizeClicked()   // 点击时向外发射最小化信号
        }
    }

    // ---- 最大化按钮（绿色 #65C466） ----
    // macOS 风格红绿灯最右侧的绿色圆点
    Item {
        visible: root.showMaximize
        width: 14
        height: 14

        Rectangle {
            anchors.fill: parent
            radius: width / 2
            color: "#65C466"            // macOS 标准绿色

            // 悬停时显示的 "⤢" 图标 — 一个空心正方形
            Rectangle {
                anchors.centerIn: parent
                width: 9; height: 9
                color: "transparent"
                border { color: "#336233"; width: 1.5 }   // 边框正方形
                radius: 1.5
                opacity: maxMouse.containsMouse ? 0.8 : 0
                Behavior on opacity { NumberAnimation { duration: 100 } }
            }
        }

        MouseArea {
            id: maxMouse
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: root.maximizeClicked()   // 点击时向外发射最大化信号
        }
    }
}
