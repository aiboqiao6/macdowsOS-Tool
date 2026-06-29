// ============================================================
// WindowControls.qml — macOS 经典红绿灯窗口控制按钮
// 14px 标准尺寸
// ============================================================
import QtQuick

Row {
    id: root
    spacing: 8

    signal minimizeClicked()
    signal maximizeClicked()
    signal closeClicked()

    property bool showDivider: false

    // ---- 关闭按钮（红色 #EC6765） ----
    Item {
        width: 14
        height: 14

        Rectangle {
            anchors.fill: parent
            radius: width / 2
            color: "#EC6765"

            // 悬停时显示关闭图标
            Rectangle {
                anchors.centerIn: parent
                width: 9; height: 1.5
                color: "#773432"
                radius: 0.5
                rotation: 45
                opacity: closeMouse.containsMouse ? 0.8 : 0
                Behavior on opacity { NumberAnimation { duration: 100 } }
            }
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

        MouseArea {
            id: closeMouse
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: root.closeClicked()
        }
    }

    // ---- 最小化按钮（黄色 #F2CA44） ----
    Item {
        width: 14
        height: 14

        Rectangle {
            anchors.fill: parent
            radius: width / 2
            color: "#F2CA44"

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
            onClicked: root.minimizeClicked()
        }
    }

    // ---- 最大化按钮（绿色 #65C466） ----
    Item {
        width: 14
        height: 14

        Rectangle {
            anchors.fill: parent
            radius: width / 2
            color: "#65C466"

            Rectangle {
                anchors.centerIn: parent
                width: 9; height: 9
                color: "transparent"
                border { color: "#336233"; width: 1.5 }
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
            onClicked: root.maximizeClicked()
        }
    }
}
