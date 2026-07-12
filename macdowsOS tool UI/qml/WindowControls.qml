import QtQuick

Row {
    id: root
    spacing: 8

    signal minimizeClicked()
    signal maximizeClicked()
    signal closeClicked()

    property bool showMinimize: true
    property bool showMaximize: true

    Item {
        width: 13
        height: 13

        Rectangle {
            anchors.fill: parent
            radius: width / 2
            color: "#FF5F57"
            border.color: "#D14E48"
            border.width: 0.5

            Rectangle { anchors.centerIn: parent; width: 7; height: 1.2; radius: 0.6; color: "#7A2724"; rotation: 45; opacity: closeMouse.containsMouse ? 0.85 : 0 }
            Rectangle { anchors.centerIn: parent; width: 7; height: 1.2; radius: 0.6; color: "#7A2724"; rotation: -45; opacity: closeMouse.containsMouse ? 0.85 : 0 }
        }

        MouseArea {
            id: closeMouse
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: root.closeClicked()
        }
    }

    Item {
        visible: root.showMinimize
        width: 13
        height: 13

        Rectangle {
            anchors.fill: parent
            radius: width / 2
            color: "#FFBD2E"
            border.color: "#D6A022"
            border.width: 0.5

            Rectangle { anchors.centerIn: parent; width: 7; height: 1.2; radius: 0.6; color: "#805F12"; opacity: minMouse.containsMouse ? 0.85 : 0 }
        }

        MouseArea {
            id: minMouse
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: root.minimizeClicked()
        }
    }

    Item {
        visible: root.showMaximize
        width: 13
        height: 13

        Rectangle {
            anchors.fill: parent
            radius: width / 2
            color: "#28C840"
            border.color: "#22A735"
            border.width: 0.5

            Rectangle { anchors.centerIn: parent; width: 6; height: 6; radius: 1; color: "transparent"; border.color: "#176C24"; border.width: 1; opacity: maxMouse.containsMouse ? 0.85 : 0 }
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
