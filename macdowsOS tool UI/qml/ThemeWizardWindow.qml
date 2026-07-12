// ============================================================
// ThemeWizardWindow.qml — 安装向导窗口
//
// 功能说明：
//   一个独立的无边框窗口，提供 macOS 风格的主题安装向导（暗色主题）。
//   包含三个视图：
//     步骤 0 — 安装确认页（大圆形预览图 + macOS Golden Gate 标题 + 继续按钮）
//     步骤 1 — 磁盘选择页（小圆形预览图 + 标题 + 磁盘选择 + 安装按钮）
//     步骤 2 — 安装进行中（小圆形预览图 + 状态文字 + 进度条）
//   窗口支持拖拽移动、最小化/最大化/关闭操作。
//   使用 MultiEffect 实现圆角遮罩效果。
// ============================================================

pragma ComponentBehavior: Bound

// QtQuick 核心模块 — 提供 QML 基础类型
import QtQuick
// QtQuick.Controls 模块 — 提供 Button、Label 等标准控件
import QtQuick.Controls
// QtQuick.Layouts 模块 — 提供 ColumnLayout、RowLayout 等高级布局管理器
import QtQuick.Layouts
// QtQuick.Window 模块 — 提供 Window 窗口类型
import QtQuick.Window
// QtQuick.Effects 模块 — 提供 MultiEffect，用于圆角遮罩等视觉效果
import QtQuick.Effects
// Backend 模块（C++ 后端暴露） — 提供安装主题、打补丁等底层操作接口
import Backend 1.0

// ====== 顶级窗口 ======
// 无边框独立窗口，模拟 macOS 安装向导风格
Window {
    id: wizardWindow
    title: "安装 macdowsOS"                  // 窗口标题
    width: 640                               // 初始宽度
    height: 560                              // 初始高度（增加高度容纳磁盘选择列表）
    minimumWidth: 560                        // 最小宽度限制
    minimumHeight: 480                       // 最小高度限制

    // 窗口标志：普通窗口 | 无边框 | 显示系统菜单
    // Qt.FramelessWindowHint 移除原生标题栏，实现自定义标题栏
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowSystemMenuHint
    color: "transparent"                     // 窗口背景透明（依赖内部内容着色）

    // 窗口初始化完成事件：将窗口居中显示在屏幕中央
    Component.onCompleted: {
        setX(Screen.width / 2 - width / 2)   // 水平居中
        setY(Screen.height / 2 - height / 2) // 垂直居中
    }

    // ====== 自定义属性 ======
    property int step: 0              // 当前步骤（0=安装确认, 1=选择磁盘, 2=安装中）
    property int selectedVersion: 10  // 用户选择的 Windows 版本（10 或 11，由调用方传入）
    property int installProgress: 0   // 当前安装进度（已完成的步骤数）
    property int installTotal: 0      // 安装总步骤数
    property string targetDisk: "C:"  // 用户选择的目标磁盘名称
    property int selectedDiskIndex: 0 // 用户选择的磁盘索引

    // ====== 重置向导 ======
    // 将所有状态恢复至初始值，用于重新开始安装
    function resetWizard() {
        wizardWindow.step = 0
        wizardWindow.installProgress = 0
        wizardWindow.installTotal = 0
        wizardWindow.selectedDiskIndex = 0
    }

    // ====== 关闭向导 ======
    // 重置状态后关闭窗口（无边框窗口下 close() 可能失效，改用 visible 控制）
    function closeWizard() {
        resetWizard()
        wizardWindow.visible = false
    }
    // ============================================================
    // 圆角蒙版
    // 通过 ShaderEffectSource + MultiEffect 实现窗口圆角裁剪
    // ============================================================

    // 圆角可见矩形：作为遮罩的源图，白色填充 + 16px 圆角
    Rectangle {
        id: cornerMask
        anchors.fill: parent
        color: "white"
        radius: 14                                     // 圆角半径 14px
    }

    // 着色器效果源：将 cornerMask 的渲染结果作为纹理提供给 MultiEffect
    ShaderEffectSource {
        id: maskTexture
        sourceItem: cornerMask      // 从 cornerMask 捕获纹理
        hideSource: true            // 隐藏原始源，只使用其纹理
    }

    // ====== 主内容容器 ======
    // 外层容器 + 内层 rootContent 圆角遮罩
    Item {
        id: contentHost
        anchors.fill: parent

        // ====== 主要内容（遮罩层） ======
        // 应用 MultiEffect 圆角遮罩，使内部所有内容按圆角边界裁剪
        Item {
            id: rootContent
            anchors.fill: parent
            layer.enabled: true                          // 启用图层
            layer.effect: MultiEffect {
                maskEnabled: true                        // 仅遮罩，无阴影
                maskSource: maskTexture                  // 使用 maskTexture 作为遮罩纹理
            }

        // 白色背景层（亮色主题）
        Rectangle {
            id: bgWhite
            anchors.fill: parent
            color: "#F2F2F7"
        }

        // ====== 整体垂直布局 ======
        ColumnLayout {
            anchors.fill: parent
            spacing: 0

            // ====== 自定义标题栏 ======
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 52
                color: "#F2F2F7"

                // 独立关闭按钮（不使用公共 WindowControls 组件）
                // 红色圆形 macOS 风格，仅步骤0显示
                Item {
                    visible: wizardWindow.step === 0
                    x: 18
                    anchors.verticalCenter: parent.verticalCenter
                    width: 24
                    height: 24

                    Rectangle {
                        anchors.centerIn: parent
                        width: 13
                        height: 13
                        radius: width / 2
                        color: "#FF5F57"
                        border.color: "#D14E48"
                        border.width: 0.5

                        // 悬停时显示的 "×" 图标
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

                    // 鼠标交互（24x24 可点击区域）
                    MouseArea {
                        id: closeMouse
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: closeWizard()
                    }
                }

                // 鼠标拖拽区域：从左侧 80px 开始（避开红绿灯按钮）
                MouseArea {
                    anchors.left: parent.left
                    anchors.leftMargin: 92
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    cursorShape: Qt.OpenHandCursor
                    property variant clickPos: "0,0"
                    onPressed: function(mouse) {
                        clickPos = Qt.point(mouse.x, mouse.y)
                    }
                    onPositionChanged: function(mouse) {
                        let delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)
                        wizardWindow.setX(wizardWindow.x + delta.x)
                        wizardWindow.setY(wizardWindow.y + delta.y)
                    }
                }

            }

            // ============================================================
            // 内容区域 — 根据 step 值切换显示不同页面
            // ============================================================
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                // ============================================================
                // 步骤 0：安装确认页（暗色主题）
                // 大圆形预览图（icon1.jpg，圆形裁剪 + 加粗白色边框）
                // → "macOS Golden Gate" 粗体标题
                // → 小型"继续"按钮，点击进入磁盘选择页
                // ============================================================
                ColumnLayout {
                    visible: wizardWindow.step === 0
                    anchors.centerIn: parent
                    anchors.verticalCenterOffset: 15   // 整体内容向下偏移
                    spacing: 24

                    // ——— 大圆形图片容器（缩小至 300px） ———
                    // 整体 Item 包含阴影 + 圆形图片 + 白色边框
                    Item {
                        implicitWidth: 300
                        implicitHeight: 300
                        Layout.alignment: Qt.AlignHCenter

                        // 阴影层：暗色圆形略微向下偏移，营造立体感
                        Rectangle {
                            anchors.fill: parent
                            anchors.topMargin: 5
                            anchors.leftMargin: 2
                            anchors.rightMargin: 2
                            radius: width / 2
                            color: "#50000000"
                        }

                        // 遮罩用圆形（白色=可见区域）
                        Rectangle {
                            id: bigCircleMaskShape
                            anchors.fill: parent
                            radius: width / 2
                            color: "white"
                            visible: false
                        }

                        // 捕获遮罩纹理
                        ShaderEffectSource {
                            id: bigCircleMask
                            anchors.fill: parent
                            sourceItem: bigCircleMaskShape
                            hideSource: true
                        }

                        // 背景图片（通过 MultiEffect 遮罩裁剪为圆形）
                        Image {
                            anchors.fill: parent
                            source: "qrc:/res/install/icon1.jpg"
                            fillMode: Image.PreserveAspectCrop

                            layer.enabled: true
                            layer.effect: MultiEffect {
                                maskEnabled: true
                                maskSource: bigCircleMask
                            }
                        }

                        // 加粗白色圆形边框（叠加在图片之上）
                        Rectangle {
                            anchors.fill: parent
                            radius: width / 2
                            border.color: "white"
                            border.width: 10
                            color: "transparent"
                        }
                    }

                    // ——— 主标题（亮色，增大） ———
                    Label {
                        text: "macOS Golden Gate"
                        color: "#1D1D1F"
                        font { pixelSize: 26; weight: Font.DemiBold }
                        Layout.alignment: Qt.AlignHCenter
                        Layout.bottomMargin: -14  // 大幅缩小与副标题间距
                    }

                    // ——— 副标题（加粗加深） ———
                    Label {
                        text: "若要设置安装macOS Golden Gate 请点击继续"
                        color: "#555555"
                        font { pixelSize: 13; weight: Font.Medium }
                        Layout.alignment: Qt.AlignHCenter
                    }

                    // ——— "继续"按钮（缩小版） ———
                    // 点击后切换到步骤 1（磁盘选择页）
                    Button {
                        id: continueBtn
                        text: "继续"
                        implicitWidth: 120
                        implicitHeight: 34
                        Layout.alignment: Qt.AlignHCenter
                        Layout.topMargin: 24             // 向下移动，增加空间感

                        // 背景：蓝色圆角矩形，悬停/按下时颜色加深
                        background: Rectangle {
                            radius: 8
                            color: continueBtn.down ? "#0066CC" : continueBtn.hovered ? "#3478F6" : "#0A84FF"
                        }
                        // 文字：白色粗体居中
                        contentItem: Label {
                            text: "继续"
                            color: "white"
                            font { pixelSize: 14; weight: Font.DemiBold }
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        onClicked: wizardWindow.step = 1
                    }
                }

                // ============================================================
                // 步骤 1：磁盘选择页
                // 小圆形预览图（靠上）→ 大标题 → 副标题 → 磁盘选择列表 → 安装按钮
                // ============================================================
                ColumnLayout {
                    visible: wizardWindow.step === 1
                    anchors.fill: parent
                    anchors.topMargin: 16
                    spacing: 8

                    // ——— 小圆形图片容器 ———
                    // 适中大小，与步骤 0 共用同一张背景图
                    // 使用 ShaderEffectSource + MultiEffect 圆形遮罩
                    Item {
                        id: smallCircle
                        implicitWidth: 150
                        implicitHeight: 150
                        Layout.alignment: Qt.AlignHCenter

                        // 遮罩用圆形
                        Rectangle {
                            id: smallCircleMaskShape
                            anchors.fill: parent
                            radius: width / 2
                            color: "white"
                            visible: false
                        }

                        // 捕获遮罩纹理
                        ShaderEffectSource {
                            id: smallCircleMask
                            anchors.fill: parent
                            sourceItem: smallCircleMaskShape
                            hideSource: true
                        }

                        // 背景图片（MultiEffect 遮罩裁剪为圆形）
                        Image {
                            anchors.fill: parent
                            source: "qrc:/res/install/icon1.jpg"
                            fillMode: Image.PreserveAspectCrop

                            layer.enabled: true
                            layer.effect: MultiEffect {
                                maskEnabled: true
                                maskSource: smallCircleMask
                            }
                        }

                        // 白色圆形边框（加粗）
                        Rectangle {
                            anchors.fill: parent
                            radius: width / 2
                            border.color: "white"
                            border.width: 6
                            color: "transparent"
                        }
                    }

                    // ——— 主标题（亮色） ———
                    Label {
                        text: "macOS Golden Gate"
                        color: "#1D1D1F"
                        font { pixelSize: 29; weight: Font.Bold }
                        Layout.alignment: Qt.AlignHCenter
                        Layout.topMargin: 10
                        Layout.bottomMargin: 2      // 主副标题间距
                    }

                    // ——— 副标题（亮色） ———
                    Label {
                        text: "macOS Golden Gate 将会安装在磁盘 Macintosh HD 上"
                        color: "#1D1D1F"
                        font { pixelSize: 12; weight: Font.Bold }
                        Layout.alignment: Qt.AlignHCenter
                        Layout.bottomMargin: 6
                    }

                    // ——— 磁盘选择列表（亮色主题） ———
                    // 磁盘卡片：图标 + 名称 + 容量信息
                    Rectangle {
                        Layout.alignment: Qt.AlignHCenter
                        width: 200
                        height: 160
                        radius: 12
                        color: "#FFFFFF"
                        border.color: clickedDisk ? "#0A84FF" : "#D1D1D6"
                        border.width: 2

                        // 竖向布局：图标在上 → 文字居中
                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 16
                            spacing: 0

                            // 磁盘图标（Icon2.png），居中
                            Image {
                                source: "qrc:/res/install/Icon2.png"
                                Layout.preferredWidth: 80
                                Layout.preferredHeight: 80
                                Layout.alignment: Qt.AlignHCenter
                                fillMode: Image.PreserveAspectFit
                            }

                            // 磁盘名称 + 容量信息，居中
                            ColumnLayout {
                                spacing: 0
                                Layout.alignment: Qt.AlignHCenter

                                Label {
                                    text: "Macintosh HD"
                                    color: "#1D1D1F"
                                    font { pixelSize: 11; weight: Font.Bold }
                                    Layout.alignment: Qt.AlignHCenter
                                }

                                Label {
                                    text: "总共 1 PB"
                                    color: "#1D1D1F"
                                    font { pixelSize: 10; weight: Font.Bold }
                                    Layout.alignment: Qt.AlignHCenter
                                }
                                Label {
                                    text: "1GB可用"
                                    color: "#1D1D1F"
                                    font { pixelSize: 10; weight: Font.Bold }
                                    Layout.alignment: Qt.AlignHCenter
                                }
                            }
                        }

                        property bool clickedDisk: false

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                parent.clickedDisk = true
                                wizardWindow.selectedDiskIndex = 0
                            }
                        }
                    }

                    // ——— 按钮行（后退 + 安装，居中排列） ———
                    Row {
                        Layout.alignment: Qt.AlignHCenter
                        Layout.topMargin: 16
                        spacing: 20

                        // "后退"按钮
                        Button {
                            id: backBtn
                            text: "后退"
                            visible: wizardWindow.step === 1
                            implicitWidth: 100
                            implicitHeight: 36
                            background: Rectangle {
                            radius: 8
                                color: backBtn.down ? "#D1D1D6" : backBtn.hovered ? "#E5E5EA" : "#EDEDF2"
                            }
                            contentItem: Label {
                                text: "后退"
                                color: "#1D1D1F"
                                font { pixelSize: 13; weight: Font.Medium }
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            onClicked: wizardWindow.step--
                        }

                        // "安装"按钮
                        Button {
                            id: installBtn
                            text: "安装"
                            implicitWidth: 100
                            implicitHeight: 36
                            background: Rectangle {
                                radius: 8
                                color: installBtn.down ? "#0066CC" : installBtn.hovered ? "#3478F6" : "#0A84FF"
                            }
                            contentItem: Label {
                                text: "安装"
                                color: "white"
                                font { pixelSize: 14; weight: Font.DemiBold }
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            onClicked: {
                                // 保存选中的磁盘名称用于安装状态显示
                                wizardWindow.targetDisk = "Macintosh HD"
                                // 切换到安装进行中页面
                                wizardWindow.step = 2
                                // 启动安装流程
                                runInstallation()
                            }
                        }
                    }
                }

            }

            // ====== 底部按钮栏（亮色） ======
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 24
                color: "transparent"
            }
        }

        // ============================================================
        // 步骤 2：安装进行中（全窗口覆盖层）
        // 覆盖整个窗口，不受 ColumnLayout 约束
        // ============================================================
        Item {
            visible: wizardWindow.step === 2
            anchors.fill: parent
            z: 50

            // 拖拽区域（步骤 2 时标题栏被隐藏，单独提供拖拽区域）
            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.OpenHandCursor
                property variant clickPos: "0,0"
                onPressed: function(mouse) {
                    clickPos = Qt.point(mouse.x, mouse.y)
                }
                onPositionChanged: function(mouse) {
                    let delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)
                    wizardWindow.setX(wizardWindow.x + delta.x)
                    wizardWindow.setY(wizardWindow.y + delta.y)
                }
            }

            // ——— 小圆形图片（居中，与步骤1大小一致） ———
            Item {
                width: 200
                height: 200
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: parent.height * 0.10

                // 遮罩用圆形
                Rectangle {
                    id: step2CircleMaskShape
                    anchors.fill: parent
                    radius: width / 2
                    color: "white"
                }

                // 捕获遮罩纹理
                ShaderEffectSource {
                    id: step2CircleMask
                    anchors.fill: parent
                    sourceItem: step2CircleMaskShape
                    hideSource: true
                }

                // 图片
                Image {
                    anchors.fill: parent
                    source: "qrc:/res/install/icon1.jpg"
                   
                    layer.enabled: true
                    layer.effect: MultiEffect {
                        maskEnabled: true
                        maskSource: step2CircleMask
                    }
                }

                // 白色圆形边框
                Rectangle {
                    anchors.fill: parent
                    radius: width / 2
                    border.color: "white"
                    border.width: 6
                    color: "transparent"
                }
            }
             
            // ——— 安装状态文字 ———
            Label {
                text: "正在安装 macOS Golden Gate 在 Macintoch HD上"
                color: "#1D1D1F"
                font { pixelSize: 18; weight: Font.Bold }
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: parent.height * 0.55
            }

            // ——— 进度条 ———
            Rectangle {
                implicitWidth: 320
                implicitHeight: 6
                radius: 3
                color: "#D1D1D6"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: parent.height * 0.80

                Rectangle {
                    width: parent.width * (wizardWindow.installTotal > 0 ? wizardWindow.installProgress / wizardWindow.installTotal : 0)
                    height: parent.height
                    radius: 3
                    color: "#0A84FF"
                    Behavior on width {
                        NumberAnimation { duration: 300; easing.type: Easing.OutCubic }
                    }
                }
            }

            // ——— 进度百分比 ———
            Label {
                text: Math.round(wizardWindow.installTotal > 0 ? wizardWindow.installProgress / wizardWindow.installTotal * 100 : 0) + "%"
                color: "#86868B"
                font.pixelSize: 13
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: parent.height * 0.84
            }
        }

        // ============================================================
        // 窗口固态边框
        // ============================================================

        // 固态边框
        Rectangle {
            anchors.fill: parent
            color: "transparent"
            radius: 14
            border.color: "#30000000"
            border.width: 1
            z: 80
        }
    }
}

    // ============================================================
    // 安装流程
    // 根据 selectedVersion 选择对应的安装步骤序列，
    // 逐个调用后端 C++ 函数执行安装操作。
    // 安装期间 step 保持为 2（安装中），进度由 installProgress 反映。
    // 所有步骤完成后进度条显示 100%，无完成界面。
    // ============================================================

    // ====== 启动安装 ======
    // 根据所选版本构建安装步骤数组，然后依次执行
    function runInstallation() {
        let steps
        if (selectedVersion === 10) {
            // Windows 10 安装步骤（6 步）
            steps = [
              { name: "安装MyDockFinder", action: function() { Backend.installMyDockFinder(); } },
              { name: "安装补丁", action: function() { Backend.patchThemePath(); } },
              { name: "删除主题", action: function() { Backend.deleteTheme(); } },
              { name: "安装主题", action: function() { Backend.installThemeWin10(); } },
              { name: "安装图标", action: function() { Backend.installIcons(); } },
              { name: "安装OldNewExplorer", action: function() { Backend.installOldNewExplorer(); } },
              { name: "执行最终设置", action: function() { Backend.finalSetting(); } },
              { name: "重启", action: function() { Backend.restartNoNotice(); } }
            ]
        } else { 
            // Windows 11 安装步骤（7 步，多一个 StartAllBack）
            steps = [
                { name: "安装MyDockFinder", action: function() { Backend.installMyDockFinder(); } },
                { name: "安装补丁", action: function() { Backend.patchThemePath(); } },
                { name: "删除主题", action: function() { Backend.deleteTheme(); } },
                { name: "安装主题", action: function() { Backend.installThemeWin11(); } },
                { name: "安装图标", action: function() { Backend.installIcons(); } },
                { name: "安装StartAllBack", action: function() { Backend.installStartAllBack(); } },
                { name: "执行最终设置", action: function() { Backend.finalSetting(); } },
                { name: "重启", action: function() { Backend.restartNoNotice(); } }
            ]
        }
        wizardWindow.installTotal = steps.length     // 记录总步骤数
        wizardWindow.installProgress = 0             // 重置进度
        runNextStep(steps, 0)           // 从第 0 步开始执行
    }

    // ====== 执行下一步安装 ======
    // 递归调用，依次执行安装步骤数组中的每个动作
    // 所有步骤在 step === 2 的页面中完成，进度条实时更新
    // 参数：
    //   steps — 安装步骤数组
    //   index — 当前要执行的步骤索引
    function runNextStep(steps, index) {
        if (index >= steps.length) {
            // 所有步骤已完成，停留在当前页面（进度条显示 100%）
            return
        }
        // 更新进度（installProgress / installTotal → 进度条百分比）
        wizardWindow.installProgress = index + 1
        // 使用 Qt.callLater 延迟执行，避免阻塞 UI 线程
        Qt.callLater(function() {
            steps[index].action()               // 调用后端执行当前步骤
            runNextStep(steps, index + 1)       // 递归执行下一步
        })
    }
}
