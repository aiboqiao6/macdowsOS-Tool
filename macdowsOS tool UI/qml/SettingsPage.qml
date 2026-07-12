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
            Label { text: "设置"; color: "#1D1D1F"; font { pixelSize: 28; weight: Font.DemiBold } }
            Label { text: "应用信息与界面状态。"; color: "#6E6E73"; font.pixelSize: 13 }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.maximumWidth: 620
            Layout.preferredHeight: 96
            radius: 10
            color: "#FFFFFF"
            border.color: "#E5E5EA"
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.margins: 18
                spacing: 12

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 4
                    Label { text: "macdowsOS Tool UI"; color: "#1D1D1F"; font { pixelSize: 15; weight: Font.DemiBold } }
                    Label { text: "雾蓝回针 MistBlueSt"; color: "#6E6E73"; font.pixelSize: 13 }
                    Label { text: "版本 1.30 · 20260712130"; color: "#86868B"; font.pixelSize: 12 }
                }
            }
        }

        Item { Layout.fillHeight: true }
    }
}
