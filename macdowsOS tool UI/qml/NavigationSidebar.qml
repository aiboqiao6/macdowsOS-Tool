import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    color: "transparent"

    property int currentIndex: 0
    property int selectedIndex: 0
    signal navigationRequested(int index)

    function sectionIndices(sectionName) {
        var items = []
        for (var i = 0; i < navModel.count; i++) {
            if (navModel.get(i).section === sectionName) items.push(i)
        }
        return items
    }

    ListModel {
        id: navModel
        ListElement { name: "首页"; iconSource: "qrc:/res/home.png"; section: "general" }
        ListElement { name: "系统美化"; iconSource: "qrc:/res/WindowsToMac.png"; section: "general" }
        ListElement { name: "系统修复"; iconSource: "qrc:/res/WindowsFix.png"; section: "tools" }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.leftMargin: 14
        anchors.rightMargin: 14
        anchors.topMargin: 10
        anchors.bottomMargin: 16
        spacing: 0

        Label {
            text: "通用"
            Layout.fillWidth: true
            Layout.topMargin: 8
            Layout.bottomMargin: 4
            leftPadding: 8
            color: "#6E6E73"
            font { pixelSize: 11; weight: Font.DemiBold }
        }

        Repeater {
            model: sectionIndices("general")
            delegate: ItemDelegate {
                id: generalItem
                required property int modelData
                Layout.fillWidth: true
                implicitHeight: 38
                hoverEnabled: true

                readonly property bool itemSelected: root.selectedIndex === modelData

                background: Rectangle {
                    anchors.fill: parent
                    radius: 11
                    color: generalItem.itemSelected ? "#D7E7FF" : generalItem.hovered ? "#E4E4E9" : "transparent"
                }

                contentItem: RowLayout {
                    spacing: 9

                    Image {
                        source: navModel.get(modelData).iconSource
                        Layout.preferredWidth: 18
                        Layout.preferredHeight: 18
                        opacity: generalItem.itemSelected ? 1 : 0.75
                        fillMode: Image.PreserveAspectFit
                    }

                    Label {
                        text: navModel.get(modelData).name
                        Layout.fillWidth: true
                        color: generalItem.itemSelected ? "#0A64D8" : "#1D1D1F"
                        font { pixelSize: 13; weight: generalItem.itemSelected ? Font.DemiBold : Font.Normal }
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                onClicked: {
                    root.selectedIndex = modelData
                    root.currentIndex = modelData
                    root.navigationRequested(modelData)
                }
            }
        }

        Label {
            text: "工具"
            Layout.fillWidth: true
            Layout.topMargin: 16
            Layout.bottomMargin: 4
            leftPadding: 8
            color: "#6E6E73"
            font { pixelSize: 11; weight: Font.DemiBold }
        }

        Repeater {
            model: sectionIndices("tools")
            delegate: ItemDelegate {
                id: toolItem
                required property int modelData
                Layout.fillWidth: true
                implicitHeight: 38
                hoverEnabled: true

                readonly property bool itemSelected: root.selectedIndex === modelData

                background: Rectangle {
                    anchors.fill: parent
                    radius: 11
                    color: toolItem.itemSelected ? "#D7E7FF" : toolItem.hovered ? "#E4E4E9" : "transparent"
                }

                contentItem: RowLayout {
                    spacing: 9

                    Image {
                        source: navModel.get(modelData).iconSource
                        Layout.preferredWidth: 18
                        Layout.preferredHeight: 18
                        opacity: toolItem.itemSelected ? 1 : 0.75
                        fillMode: Image.PreserveAspectFit
                    }

                    Label {
                        text: navModel.get(modelData).name
                        Layout.fillWidth: true
                        color: toolItem.itemSelected ? "#0A64D8" : "#1D1D1F"
                        font { pixelSize: 13; weight: toolItem.itemSelected ? Font.DemiBold : Font.Normal }
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                onClicked: {
                    root.selectedIndex = modelData
                    root.currentIndex = modelData
                    root.navigationRequested(modelData)
                }
            }
        }

        Item { Layout.fillHeight: true }

        ItemDelegate {
            id: settingsItem
            Layout.fillWidth: true
            implicitHeight: 38
            hoverEnabled: true

            readonly property bool itemSelected: root.selectedIndex === navModel.count

            background: Rectangle {
                anchors.fill: parent
                radius: 11
                color: settingsItem.itemSelected ? "#D7E7FF" : settingsItem.hovered ? "#E4E4E9" : "transparent"
            }

            contentItem: RowLayout {
                spacing: 9

                Image {
                    source: "qrc:/res/Setting.png"
                    Layout.preferredWidth: 18
                    Layout.preferredHeight: 18
                    opacity: settingsItem.itemSelected ? 1 : 0.75
                    fillMode: Image.PreserveAspectFit
                }

                Label {
                    text: "设置"
                    Layout.fillWidth: true
                    color: settingsItem.itemSelected ? "#0A64D8" : "#1D1D1F"
                    font { pixelSize: 13; weight: settingsItem.itemSelected ? Font.DemiBold : Font.Normal }
                    verticalAlignment: Text.AlignVCenter
                }
            }

            onClicked: {
                root.selectedIndex = navModel.count
                root.currentIndex = navModel.count
                root.navigationRequested(navModel.count)
            }
        }
    }
}
