// ============================================================
// BeautificationHub.qml — 系统美化入口页
//   提供三个功能入口：Win10 美化、Win11 美化、选择安装
// ============================================================

pragma ComponentBehavior: Bound

// QtQuick 模块：提供 QML 核心类型（Item、Rectangle、Text 等基础组件）
import QtQuick
// QtQuick.Controls 模块：提供界面控件（Label、Button 等标准控件）
import QtQuick.Controls

// === 根元素：不可见的容器，作为页面顶层布局容器 ===
Item {
    id: root

    // === 垂直列布局：用于排列页面标题和三个功能选项 ===
    Column {
        anchors {
            top: parent.top; topMargin: 0
            left: parent.left; leftMargin: 0
            right: parent.right; rightMargin: 0
        }
        spacing: 0  // 子元素间距为 0，用 Item 手动控制间隔

        // === 页面标题："系统美化" ===
        Label {
            text: "系统美化"          // 显示的标题文本
            font { pixelSize: 20; weight: Font.Bold }  // 字体：20px 加粗
            color: "#1D1D1F"         // 深灰色文字，接近黑色
            leftPadding: 30           // 左侧内边距 30px
            bottomPadding: 0
            height: 34                // 固定高度
            verticalAlignment: Text.AlignVCenter  // 文字垂直居中
        }

        // === 标题与选项之间的空白间隔 ===
        Item { height: 4; width: 1 }

        // ——— 选项 1：Win10 系统美化 ———
        // 点击后打开主题向导，预设 Windows 10 版本
        Item {
            width: parent.width
            height: 34

            Rectangle {
                anchors {
                    left: parent.left; leftMargin: 24
                    right: parent.right; rightMargin: 24
                }
                height: parent.height
                y: 0
                radius: 8
                color: ma1.containsMouse ? "#D8D8E0" : "transparent"
            }

            Row {
                anchors {
                    left: parent.left; leftMargin: 40
                    verticalCenter: parent.verticalCenter
                }
                spacing: 10

                Image {
                    source: "qrc:/res/WindowsToMac.png"
                    width: 18; height: 18
                    anchors.verticalCenter: parent.verticalCenter
                    fillMode: Image.PreserveAspectFit
                }

                Text {
                    text: "Win10 系统美化"
                    font { pixelSize: 13 }
                    color: "#252525"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            MouseArea {
                id: ma1
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: appWindow.openThemeWizard(10)
            }
        }

        // ——— 选项 2：Win11 系统美化 ———
        // 点击后打开主题向导，预设 Windows 11 版本
        Item {
            width: parent.width
            height: 34

            Rectangle {
                anchors {
                    left: parent.left; leftMargin: 24
                    right: parent.right; rightMargin: 24
                }
                height: parent.height
                y: 0
                radius: 8
                color: ma2.containsMouse ? "#D8D8E0" : "transparent"
            }

            Row {
                anchors {
                    left: parent.left; leftMargin: 40
                    verticalCenter: parent.verticalCenter
                }
                spacing: 10

                Image {
                    source: "qrc:/res/WindowsToMac.png"
                    width: 18; height: 18
                    anchors.verticalCenter: parent.verticalCenter
                    fillMode: Image.PreserveAspectFit
                }

                Text {
                    text: "Win11 系统美化"
                    font { pixelSize: 13 }
                    color: "#252525"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            MouseArea {
                id: ma2
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: appWindow.openThemeWizard(11)
            }
        }

        // ——— 选项 3：选择安装 ———
        // 点击后跳转到组件选择页面，可单独安装某个美化组件
        Item {
            width: parent.width
            height: 34

            Rectangle {
                anchors {
                    left: parent.left; leftMargin: 24
                    right: parent.right; rightMargin: 24
                }
                height: parent.height
                y: 0
                radius: 8
                color: ma3.containsMouse ? "#D8D8E0" : "transparent"
            }

            Row {
                anchors {
                    left: parent.left; leftMargin: 40
                    verticalCenter: parent.verticalCenter
                }
                spacing: 10

                Image {
                    source: "qrc:/res/box.png"
                    width: 18; height: 18
                    anchors.verticalCenter: parent.verticalCenter
                    fillMode: Image.PreserveAspectFit
                }

                Text {
                    text: "选择安装"
                    font { pixelSize: 13 }
                    color: "#252525"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            MouseArea {
                id: ma3
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: contentStack.currentIndex = 2
            }
        }
    }
}
