// ============================================================
// NavigationSidebar.qml — macOS 15 Sequoia 风格侧边栏
// 统一选中态 + 悬浮高亮 + 分组分隔，设置固定底部
// ============================================================
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    color: "transparent"

    property int currentIndex: 0
    signal navigationRequested(int index)

    // 导航数据（设置不在此处，单独固定在底部）
    ListModel {
        id: navModel
        ListElement { name: "首页"; iconSource: "qrc:/res/home.png"; section: "general" }
        ListElement { name: "系统美化"; iconSource: "qrc:/res/WindowsToMac.png"; section: "general" }
        //ListElement { name: "窗口圆角"; iconSource: "qrc:/res/tool/WindowRound.png"; section: "tools" }
        ListElement { name: "系统修复"; iconSource: "qrc:/res/WindowsFix.png"; section: "tools" }
    }

    // 获取指定分组的原始索引列表
    function sectionIndices(sectionName) {
        var items = []
        for (var i = 0; i < navModel.count; i++) {
            if (navModel.get(i).section === sectionName) items.push(i)
        }
        return items
    }

    // 选中项的原始索引（设置固定为最后一个）
    property int selectedIndex: 0

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // ---- "通用" 分组标签 ----
        Label {
            text: "通用"
            color: "#252525"
            font { pixelSize: 11; weight: Font.Bold }
            leftPadding: 18
            topPadding: 8
            bottomPadding: 4
        }

        // ---- 通用分组项 ----
        Repeater {
            model: sectionIndices("general")

            delegate: ItemDelegate {
                id: genDelegate
                Layout.fillWidth: true
                Layout.preferredHeight: 38
                leftPadding: 0
                rightPadding: 0
                topPadding: 0
                bottomPadding: 0
                implicitHeight: 38

                readonly property bool isSelected: root.selectedIndex === modelData

                background: Rectangle {
                    anchors {
                        left: parent.left; leftMargin: 6
                        right: parent.right; rightMargin: 6
                    }
                    height: parent.height
                    y: 0
                    radius: 8
                    color: genDelegate.isSelected ? "#2F6EED" :
                           genDelegate.hovered ? "#D8D8E0" :
                           "transparent"
                }

                contentItem: Row {
                    anchors {
                        left: parent.left; leftMargin: 16
                        verticalCenter: parent.verticalCenter
                    }
                    spacing: 10

                    Image {
                        source: navModel.get(modelData).iconSource
                        width: 18; height: 18
                        anchors.verticalCenter: parent.verticalCenter
                        fillMode: Image.PreserveAspectFit
                    }

                    Label {
                        text: navModel.get(modelData).name
                        color: genDelegate.isSelected ? "#FFFFFF" : "#252525"
                        font { pixelSize: 13; weight: genDelegate.isSelected ? Font.DemiBold : Font.Normal }
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                onClicked: {
                    root.selectedIndex = modelData
                    root.currentIndex = modelData
                    root.navigationRequested(modelData)
                }
            }
        }

        // ---- "工具" 分组标签 ----
        Label {
            text: "工具"
            color: "#252525"
            font { pixelSize: 11; weight: Font.Bold }
            leftPadding: 18
            topPadding: 6
            bottomPadding: 4
        }

        // ---- 工具分组项 ----
        Repeater {
            model: sectionIndices("tools")

            delegate: ItemDelegate {
                id: toolDelegate
                Layout.fillWidth: true
                Layout.preferredHeight: 38
                leftPadding: 0
                rightPadding: 0
                topPadding: 0
                bottomPadding: 0
                implicitHeight: 38

                readonly property bool isSelected: root.selectedIndex === modelData

                background: Rectangle {
                    anchors {
                        left: parent.left; leftMargin: 6
                        right: parent.right; rightMargin: 6
                    }
                    height: parent.height
                    y: 0
                    radius: 8
                    color: toolDelegate.isSelected ? "#2F6EED" :
                           toolDelegate.hovered ? "#D8D8E0" :
                           "transparent"
                }

                contentItem: Row {
                    anchors {
                        left: parent.left; leftMargin: 16
                        verticalCenter: parent.verticalCenter
                    }
                    spacing: 10

                    Image {
                        source: navModel.get(modelData).iconSource
                        width: 18; height: 18
                        anchors.verticalCenter: parent.verticalCenter
                        fillMode: Image.PreserveAspectFit
                    }

                    Label {
                        text: navModel.get(modelData).name
                        color: toolDelegate.isSelected ? "#FFFFFF" : "#252525"
                        font { pixelSize: 13; weight: toolDelegate.isSelected ? Font.DemiBold : Font.Normal }
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                onClicked: {
                    root.selectedIndex = modelData
                    root.currentIndex = modelData
                    root.navigationRequested(modelData)
                }
            }
        }

        // ---- 弹性空间：将设置推到最底 ----
        Item { Layout.fillHeight: true }

        // ---- 设置（固定底部） ----
        ItemDelegate {
            id: settingsDelegate
            Layout.fillWidth: true
            Layout.preferredHeight: 38
            leftPadding: 0
            rightPadding: 0
            topPadding: 0
            bottomPadding: 0
            implicitHeight: 38

            readonly property bool isSelected: root.selectedIndex === navModel.count

            background: Rectangle {
                anchors {
                    left: parent.left; leftMargin: 6
                    right: parent.right; rightMargin: 6
                }
                height: parent.height
                y: 0
                radius: 8
                color: settingsDelegate.isSelected ? "#2F6EED" :
                       settingsDelegate.hovered ? "#D8D8E0" :
                       "transparent"
            }

            contentItem: Row {
                anchors {
                    left: parent.left; leftMargin: 16
                    verticalCenter: parent.verticalCenter
                }
                spacing: 10

                Image {
                    source: "qrc:/res/Setting.png"
                    width: 18; height: 18
                    anchors.verticalCenter: parent.verticalCenter
                    fillMode: Image.PreserveAspectFit
                }

                Label {
                    text: "设置"
                    font { pixelSize: 13; weight: settingsDelegate.isSelected ? Font.DemiBold : Font.Normal }
                    color: settingsDelegate.isSelected ? "#FFFFFF" : "#252525"
                    anchors.verticalCenter: parent.verticalCenter
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
