import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

Window {
    id: appWindow
    title: "macdowsOS Tool"
    width: 900
    height: 620
    minimumWidth: 780
    minimumHeight: 540
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowSystemMenuHint | Qt.WindowMinMaxButtonsHint
    color: "transparent"
    visible: true

    property var wizardWindow: null
    property var pageMap: [0, 1, 4]
    readonly property int sidebarWidth: 232
    readonly property int titleBarHeight: 52

    function openThemeWizard(version) {
        if (wizardWindow !== null && wizardWindow !== undefined) {
            wizardWindow.selectedVersion = version
            wizardWindow.show()
            wizardWindow.raise()
            wizardWindow.requestActivate()
            return
        }

        let component = Qt.createComponent("qrc:/qml/ThemeWizardWindow.qml")
        if (component.status === Component.Ready) {
            wizardWindow = component.createObject(null, { selectedVersion: version })
            if (wizardWindow) {
                wizardWindow.show()
                wizardWindow.raise()
                wizardWindow.requestActivate()
                wizardWindow.closing.connect(function() {
                    wizardWindow.destroy()
                    wizardWindow = null
                })
            }
        } else {
            console.error("ThemeWizardWindow 加载失败:", component.errorString())
        }
    }

    Rectangle {
        id: rootContent
        anchors.fill: parent
        radius: 18
        clip: true
        color: "#F5F5F7"
        border.color: "#D8D8DE"
        border.width: 1

        RowLayout {
            anchors.fill: parent
            spacing: 0

            Rectangle {
                id: sidebarMaterial
                Layout.preferredWidth: appWindow.sidebarWidth
                Layout.fillHeight: true
                color: "#E9E9EE"

                Rectangle {
                    anchors.fill: parent
                    color: "#28FFFFFF"
                }

                Rectangle {
                    anchors.fill: parent
                    color: "#40FFFFFF"
                    opacity: 0.18
                    gradient: Gradient {
                        orientation: Gradient.Vertical
                        GradientStop { position: 0.0; color: "#55FFFFFF" }
                        GradientStop { position: 0.45; color: "#18FFFFFF" }
                        GradientStop { position: 1.0; color: "#38D7D7E0" }
                    }
                }

                ColumnLayout {
                    id: sidebarColumn
                    anchors.fill: parent
                    spacing: 0

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: appWindow.titleBarHeight
                        color: "transparent"

                        WindowControls {
                            anchors.left: parent.left
                            anchors.leftMargin: 18
                            anchors.verticalCenter: parent.verticalCenter
                            onMinimizeClicked: appWindow.showMinimized()
                            onMaximizeClicked: appWindow.visibility === Window.Maximized ? appWindow.showNormal() : appWindow.showMaximized()
                            onCloseClicked: appWindow.close()
                        }

                        MouseArea {
                            anchors.left: parent.left
                            anchors.leftMargin: 92
                            anchors.right: parent.right
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            cursorShape: Qt.OpenHandCursor
                            onPressed: appWindow.startSystemMove()
                        }
                    }

                    NavigationSidebar {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        onNavigationRequested: function(navIndex) {
                            if (navIndex >= 0 && navIndex < pageMap.length) {
                                contentStack.currentIndex = pageMap[navIndex]
                            } else if (navIndex === pageMap.length) {
                                contentStack.currentIndex = 3
                            }
                        }
                    }
                }

                Rectangle {
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width: 1
                    color: "#D1D1D8"
                }
            }

            ColumnLayout {
                id: contentColumn
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 0
                Rectangle {
                    z: -1
                    anchors.fill: parent
                    color: "#F5F5F7"
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: appWindow.titleBarHeight
                    color: "#F5F5F7"

                    Label {
                        anchors.centerIn: parent
                        text: "macdowsOS Tool"
                        color: "#6E6E73"
                        font { pixelSize: 12; weight: Font.Medium }
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.OpenHandCursor
                        onPressed: appWindow.startSystemMove()
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    StackLayout {
                        id: contentStack
                        anchors.fill: parent
                        anchors.leftMargin: 40
                        anchors.rightMargin: 40
                        anchors.topMargin: 22
                        anchors.bottomMargin: 34
                        currentIndex: 0

                        HomePage {}
                        BeautificationHub {}
                        CeleCraftPage {}
                        SettingsPage {}
                        WindowsFixPage {}
                    }
                }
            }
        }

    }

    MouseArea { z:99; anchors.left: parent.left; anchors.top: parent.top; anchors.bottom: parent.bottom; width: 5; cursorShape: Qt.SizeHorCursor; onPressed: appWindow.startSystemResize(Qt.LeftEdge) }
    MouseArea { z:99; anchors.right: parent.right; anchors.top: parent.top; anchors.bottom: parent.bottom; width: 5; cursorShape: Qt.SizeHorCursor; onPressed: appWindow.startSystemResize(Qt.RightEdge) }
    MouseArea { z:99; anchors.left: parent.left; anchors.right: parent.right; anchors.top: parent.top; height: 5; cursorShape: Qt.SizeVerCursor; onPressed: appWindow.startSystemResize(Qt.TopEdge) }
    MouseArea { z:99; anchors.left: parent.left; anchors.right: parent.right; anchors.bottom: parent.bottom; height: 5; cursorShape: Qt.SizeVerCursor; onPressed: appWindow.startSystemResize(Qt.BottomEdge) }
    MouseArea { z:99; anchors.left: parent.left; anchors.top: parent.top; width: 5; height: 5; cursorShape: Qt.SizeFDiagCursor; onPressed: appWindow.startSystemResize(Qt.TopEdge | Qt.LeftEdge) }
    MouseArea { z:99; anchors.right: parent.right; anchors.top: parent.top; width: 5; height: 5; cursorShape: Qt.SizeBDiagCursor; onPressed: appWindow.startSystemResize(Qt.TopEdge | Qt.RightEdge) }
    MouseArea { z:99; anchors.left: parent.left; anchors.bottom: parent.bottom; width: 5; height: 5; cursorShape: Qt.SizeBDiagCursor; onPressed: appWindow.startSystemResize(Qt.BottomEdge | Qt.LeftEdge) }
    MouseArea { z:99; anchors.right: parent.right; anchors.bottom: parent.bottom; width: 5; height: 5; cursorShape: Qt.SizeFDiagCursor; onPressed: appWindow.startSystemResize(Qt.BottomEdge | Qt.RightEdge) }
}
