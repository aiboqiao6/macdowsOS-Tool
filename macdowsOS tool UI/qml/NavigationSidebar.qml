// ============================================================
// NavigationSidebar.qml — 仿 macOS 15 Sequoia 风格侧边栏导航组件
// 支持分组显示（通用/工具）、选中态高亮、悬浮高亮
// "设置"项固定在底部，不随内容分组滚动
// ============================================================

// QtQuick — 基础 QML 模块，提供核心类型
import QtQuick
// QtQuick.Controls — 提供 ItemDelegate、Label 等标准控件
import QtQuick.Controls
// QtQuick.Layouts — 提供 ColumnLayout、Row 等布局管理器
import QtQuick.Layouts

// === 侧边栏根容器 ===
Rectangle {
    id: root
    color: "transparent"   // 透明背景，由外层叠加玻璃效果

    // ——— 属性定义 ———
    property int currentIndex: 0            // 当前选中项的索引，供外部使用
    signal navigationRequested(int index)   // 导航请求信号，父容器需处理以切换页面

    // === 导航数据模型 ===
    ListModel {
        id: navModel
        ListElement { name: "首页"; iconSource: "qrc:/res/home.png"; section: "general" }
        ListElement { name: "系统美化"; iconSource: "qrc:/res/WindowsToMac.png"; section: "general" }
        ListElement { name: "系统修复"; iconSource: "qrc:/res/WindowsFix.png"; section: "tools" }
    }

    // ——— 辅助函数 ———
    function sectionIndices(sectionName) {
        var items = []
        for (var i = 0; i < navModel.count; i++) {
            if (navModel.get(i).section === sectionName) items.push(i)
        }
        return items
    }

    // ——— 属性 ———
    property int selectedIndex: 0   // 当前选中的导航项索引（设置项固定为 navModel.count）

    // === 主布局：垂直排列 ===
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
                required property int modelData

                Layout.fillWidth: true
                Layout.preferredHeight: 38
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
                required property int modelData

                Layout.fillWidth: true
                Layout.preferredHeight: 38
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

        // ---- 弹性空间 ----
        Item { Layout.fillHeight: true }

        // ---- 设置 ----
        ItemDelegate {
            id: settingsDelegate
            Layout.fillWidth: true
            Layout.preferredHeight: 38
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
