import QtQuick
import Backend 1.0
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
            Label { text: "系统修复工具"; color: "#1D1D1F"; font { pixelSize: 28; weight: Font.DemiBold } }
            Label { text: "使用系统工具检查和修复 Windows 文件。"; color: "#6E6E73"; font.pixelSize: 13 }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.maximumWidth: 620
            Layout.preferredHeight: 84
            radius: 10
            color: "#FFF6F6"
            border.color: "#FFE0E0"
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.margins: 18
                spacing: 12
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 4

                    Label {
                        text: "重要提示"
                        color: "#D70015"
                        font { pixelSize: 14; weight: Font.DemiBold }
                    }

                    Label {
                        text: "系统修复可能需要重启计算机，建议操作前创建系统还原点。"
                        color: "#D70015"
                        font.pixelSize: 12
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.maximumWidth: 620
            Layout.preferredHeight: 78
            radius: 10
            color: "#FFFFFF"
            border.color: "#E5E5EA"
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.margins: 18
                spacing: 14

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 4

                    Label {
                        text: "运行系统修复"
                        color: "#1D1D1F"
                        font { pixelSize: 14; weight: Font.DemiBold }
                    }

                    Label {
                        text: "执行系统文件检查和修复流程。"
                        color: "#86868B"
                        font.pixelSize: 12
                    }
                }

                Button {
                    id: fixBtn
                    text: "运行"
                    implicitWidth: 82
                    implicitHeight: 32
                    background: Rectangle {
                        radius: 8
                        color: fixBtn.down ? "#0066CC" : fixBtn.hovered ? "#2F7AF8" : "#0A84FF"
                    }
                    contentItem: Label {
                        text: fixBtn.text
                        color: "#FFFFFF"
                        font { pixelSize: 13; weight: Font.DemiBold }
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: Backend.runWindowsFix()
                }
            }
        }

        Item { Layout.fillHeight: true }
    }
}
