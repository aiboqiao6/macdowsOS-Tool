// ============================================================
// CeleCraftPage.qml — 选择性安装页面（macOS 27 风格）
// macOS 偏好设置面板风格
// 功能：列出所有可单独安装的美化组件，用户可逐一安装
// ============================================================

// QtQuick 模块：提供 QML 核心类型（Rectangle、Item、Label 等基础组件）
import QtQuick
// Backend 模块：C++ 后端接口，提供实际的安装/修复/配置功能
import Backend 1.0
// QtQuick.Controls 模块：提供标准界面控件（Button、ScrollView 等）
import QtQuick.Controls

// === 根元素：透明的矩形容器，作为页面背景 ===
Rectangle {
    id: root
    color: "transparent"  // 背景透明，由父容器控制背景色

    // === 滚动视图：当内容超出可视区域时支持滚动 ===
    ScrollView {
        anchors.fill: parent
        clip: true  // 裁剪超出部分，防止内容溢出

        // 隐藏垂直滚动条（保持 macOS 简洁风格）
        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AlwaysOff
        }

        // === 垂直列布局：排列标题、说明和安装项列表 ===
        Column {
            spacing: 14           // 子元素垂直间距 14px
            width: parent.width

            // === 页面标题："选择安装" ===
            Label {
                text: "选择安装"
                font { pixelSize: 20; weight: Font.Bold }
                color: "#1D1D1F"
                bottomPadding: 6
            }

            // === 页面副标题/说明文字 ===
            Label {
                text: "单独安装某项美化组件"          // 提示用户本页用途
                font.pixelSize: 13
                color: "#86868B"                     // 浅灰色辅助文字
                wrapMode: Text.WordWrap              // 自动换行
                width: parent.width
                bottomPadding: 10
            }

            // === 安装项目列表 — macOS 偏好设置风格 ===
            // 使用 Repeater 根据 model 数据动态生成多个安装项
            Repeater {
                // model 定义了每个安装项的名称、描述和对应的后端操作函数
                model: [
                    { name: "安装 myDockFinder", desc: "macOS 风格 Dock 栏", action: function() { Backend.installMyDockFinder(); } },
                    { name: "安装系统图标", desc: "替换为 macOS 风格图标", action: function() { Backend.installIcons(); } },
                    { name: "安装 Win11 主题", desc: "安装 macOS 风格主题（Win11）", action: function() { Backend.installThemeWin11(); } },
                    { name: "安装 Win10 主题", desc: "安装 macOS 风格主题（Win10）", action: function() { Backend.installThemeWin10(); } },
                    { name: "安装主题路径补丁", desc: "确保主题文件正确加载", action: function() { Backend.patchThemePath(); } },
                    { name: "删除已安装主题", desc: "恢复 Windows 默认主题", action: function() { Backend.deleteTheme(); } },
                    { name: "安装 StartAllBack", desc: "Win11 开始菜单增强", action: function() { Backend.installStartAllBack(); } },
                    { name: "安装 OldNewExplorer", desc: "资源管理器增强", action: function() { Backend.installOldNewExplorer(); } },
                    { name: "重启资源管理器", desc: "使更改生效", action: function() { Backend.restartExplorer(); } }
                ]

                // === 每个安装项的委托（delegate）模板 ===
                delegate: Rectangle {
                    width: parent.width
                    height: 60     // 每行固定高度 60px

                    // === 水平行布局：左侧描述 + 右侧操作按钮 ===
                    Row {
                        anchors {
                            left: parent.left; leftMargin: 18
                            right: parent.right; rightMargin: 18
                            verticalCenter: parent.verticalCenter
                        }
                        spacing: 10

                        // === 左侧：项目名称和描述 ===
                        Column {
                            anchors.verticalCenter: parent.verticalCenter
                            spacing: 3
                            // 项目名称
                            Label {
                                text: modelData.name         // 从 model 中获取名称
                                font { pixelSize: 14; weight: Font.Medium }
                                color: "#1D1D1F"
                            }
                            // 项目描述
                            Label {
                                text: modelData.desc         // 从 model 中获取描述
                                font.pixelSize: 12
                                color: "#86868B"
                            }
                        }

                        // 弹性填充项：将左右两侧推到两端
                        Item { height: 1; width: 1 }

                        // === 右侧：安装/操作按钮 ===
                        Button {
                            id: actionBtn
                            text: "安装"                    // 按钮默认文本
                            implicitWidth: 72
                            implicitHeight: 30
                            anchors.verticalCenter: parent.verticalCenter

                            // 按钮背景：蓝色圆角矩形，支持按下和悬停状态变色
                            background: Rectangle {
                                radius: 10
                                color: actionBtn.down ? "#0066CC" : (actionBtn.hovered ? "#0066CC" : "#007AFF")
                            }
                            // 按钮文字：白色居中
                            contentItem: Label {
                                text: actionBtn.text
                                color: "#FFFFFF"
                                font { pixelSize: 12; weight: Font.Medium }
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }

                            // === 点击处理逻辑 ===
                            // 1. 禁用按钮防止重复点击
                            // 2. 文字改为 "..." 表示正在处理
                            // 3. 使用 Qt.callLater 异步调用后端操作
                            // 4. 操作完成后更新按钮文字为 "完成" 并重新启用
                            onClicked: {
                                actionBtn.enabled = false          // 防止重复触发
                                actionBtn.text = "..."             // 显示加载中状态
                                Qt.callLater(function() {
                                    modelData.action()             // 调用后端对应的安装函数
                                    actionBtn.text = "完成"         // 操作完成后更新按钮文字
                                    actionBtn.enabled = true        // 重新启用按钮
                                })
                            }
                        }
                    }
                }
            }

            // === 列表底部留白 ===
            Item { width: 1; height: 24 }
        }
    }
}
