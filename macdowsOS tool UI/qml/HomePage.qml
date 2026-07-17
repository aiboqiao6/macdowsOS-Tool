import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    ColumnLayout {
        anchors.fill: parent
        spacing: 18

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4

            Label {
                text: "macdowsOS Tool"
                color: "#1D1D1F"
                font { pixelSize: 30; weight: Font.DemiBold }
            }

            Label {
                text: "自动化的 Windows 仿 macOS 美化工具"
                color: "#6E6E73"
                font.pixelSize: 14
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.maximumWidth: 620
            Layout.preferredHeight: 104
            radius: 10
            color: "#FFFFFF"
            border.color: "#E1E1E6"
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 12

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 5

                    Label {
                        text: "开始使用"
                        color: "#1D1D1F"
                        font { pixelSize: 17; weight: Font.DemiBold }
                    }

                    Label {
                        text: "从左侧选择系统美化、组件安装或系统修复。"
                        color: "#6E6E73"
                        font.pixelSize: 13
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.maximumWidth: 620
            Layout.preferredHeight: 72
            radius: 10
            color: "#FFFFFF"
            border.color: "#E1E1E6"
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 18
                anchors.rightMargin: 18
                spacing: 12

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 4

                    Label { text: "版本"; color: "#1D1D1F"; font { pixelSize: 14; weight: Font.DemiBold } }
                    Label { text: "1.30 RC2"; color: "#86868B"; font.pixelSize: 12 }
                }

                Label {
                    text: "GitHub"
                    color: githubArea.containsMouse ? "#0068D9" : "#007AFF"
                    font.pixelSize: 12
                    font.underline: githubArea.containsMouse

                    MouseArea {
                        id: githubArea
                        anchors.fill: parent
                        anchors.margins: -8
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: Qt.openUrlExternally("https://github.com/aiboqiao6/macdowsOS-Tool")
                    }
                }
            }
        }

        Item { Layout.fillHeight: true }
    }
}
