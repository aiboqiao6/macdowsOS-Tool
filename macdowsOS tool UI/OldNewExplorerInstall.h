#pragma once
#include"FilesSystem.h"
#include"WindowControl.h"
#include"All.h"
#include<Windows.h>
#include <string>
#include <chrono>
#include <thread>
namespace {
    // 内部辅助结构体（此命名空间外部不可见）
    struct ControlHandles {
        HWND hEdit;
        HWND hOkBtn;
    };
        // 递归：找到第一个符合要求的编辑框HWND，未找到返回NULL
        HWND FindEditControlRecursive(HWND hParent) {

            // 2. 枚举当前父窗口的直接子控件（GW_CHILD获取第一个子控件）
            HWND hChild = GetWindow(hParent, GW_CHILD);
            while (hChild) {
                // 3. 获取当前子控件的类名，判断是否为编辑框（EDIT类）
                wchar_t className[256] = { 0 };
                GetClassNameW(hChild, className, 256);

                // 标准匹配逻辑：如果是EDIT类则找到，直接返回
                if (wcscmp(className, L"Edit") == 0|| wcscmp(className, L"EDIT")==0) {
                    return hChild;
                }

                // 4. 递归查找当前子控件的子控件（深度优先，先深入层级）
                HWND hFound = FindEditControlRecursive(hChild);
                if (hFound) {
                    return hFound;
                }

                // 5. 继续查找下一个兄弟控件
                hChild = GetWindow(hChild, GW_HWNDNEXT);
            }

            // 6. 所有子控件遍历完仍未找到，返回NULL
            return NULL;
        }
    // 核心函数：在运行对话框中执行指定命令
    bool ExecuteInRunDialog(const std::wstring& command){
        // 1. 模拟按下Win+R打开运行窗口（优化确认按下和释放顺序以确保稳定性）
        INPUT inputs[4] = { 0 };
        // 按下Win键
        inputs[0].type = INPUT_KEYBOARD;
        inputs[0].ki.wVk = VK_LWIN;
        // 按下R键
        inputs[1].type = INPUT_KEYBOARD;
        inputs[1].ki.wVk = 'R';
        // 释放R键
        inputs[2].type = INPUT_KEYBOARD;
        inputs[2].ki.wVk = 'R';
        inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;
        // 释放Win键
        inputs[3].type = INPUT_KEYBOARD;
        inputs[3].ki.wVk = VK_LWIN;
        inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(_countof(inputs), inputs, sizeof(INPUT));
        // 2. 等待运行窗口出现（添加获取窗口延时以增加稳定性，先缓冲再查找，避免异常）
        INFO_(L"[OldNewExplorer安装工具]等待运行窗口出现");
        HWND hRunDialog = FindWindowW(NULL, L"运行");
        while (hRunDialog == NULL) {
            hRunDialog = FindWindowW(NULL, L"运行");
        }
        INFO_(L"[OldNewExplorer安装工具]运行窗口出现");
        // 3. 枚举运行窗口的子控件，找到输入编辑框和确定按钮
        // 结构体初始化采用现代格式避免语法问题
        ControlHandles handles;
        handles.hEdit = nullptr;
        handles.hOkBtn = nullptr;

        INFO_(L"[OldNewExplorer安装工具]等待查找编辑框成功");
        handles.hEdit = FindEditControlRecursive(hRunDialog);
        while (handles.hEdit == NULL) {
            handles.hEdit = FindEditControlRecursive(hRunDialog);
        }
        INFO_(L"[OldNewExplorer安装工具]编辑框已找到");
        
        // 4. 控件修改优先级确保输入框获得焦点以提高稳定性
        SetForegroundWindow(hRunDialog); // 置顶运行窗口
        SetFocus(handles.hEdit);         // 设置输入框焦点
        // 5. 在编辑框中输入文本的改良步骤，确保数据稳定
        SendMessageW(handles.hEdit, EM_SETSEL, 0, -1); // 选中所有文本
        SendMessageW(handles.hEdit, WM_CLEAR, 0, 0);    // 清除选中内容
        // 修改1：逐字符输入（模拟真实输入，使数据更稳定）
        for (wchar_t c : command){
            // 发送字符消息（直接SetText可能有稳定性问题，使用模拟输入）
            SendMessageW(handles.hEdit, WM_CHAR, static_cast<WPARAM>(c), 0);
        }
        // 修改2：发送EN_CHANGE通知促使界面更新，确保数据稳定
        SendMessageW(hRunDialog, WM_COMMAND,MAKEWPARAM(GetDlgCtrlID(handles.hEdit), EN_CHANGE),reinterpret_cast<LPARAM>(handles.hEdit));
       
        enter_key();
        

        return true;
    }

}
inline void OldNewExplorer_install() {
    INFO_(L"[OldNewExplorer安装工具]开始安装");
    INFO_(L"[OldNewExplorer安装工具]复制应用文件");
    copyPath(L"./AppData/OldNewExplorer", L"C:/");

    INFO_(L"[OldNewExplorer安装工具]打开配置程序");
    ExecuteInRunDialog(L"C:\\OldNewExplorer\\OldNewExplorerCfg.exe");
    INFO_(L"[OldNewExplorer安装工具]等待配置窗口出现");
    HWND hWnd_Window= FindWindowW(NULL, L"致美化 OldNewExplorer 配置");
    while (hWnd_Window == NULL) {
        hWnd_Window = FindWindowW(NULL, L"致美化 OldNewExplorer 配置");
    }
    INFO_(L"[OldNewExplorer安装工具]配置窗口出现");
    SetForegroundWindow(hWnd_Window);
    SetWindowPos(hWnd_Window, NULL, 0, 0, 0, 0, SWP_NOSIZE);

    INFO_(L"[OldNewExplorer安装工具]开始配置设置");
    TurnOffOn(true , hWnd_Window, L"OldNewExplorer安装工具", L"使用经典驱动器分组");
    TurnOffOn(false, hWnd_Window, L"OldNewExplorer安装工具", L"使用库、并隐藏顶部文件夹");

    TurnOffOn(true , hWnd_Window, L"OldNewExplorer安装工具", L"使用工具栏代替功能区（必选）");
    
    TurnOffOn(true, hWnd_Window, L"OldNewExplorer安装工具", L"隐藏标题栏小图标（可选）");
    TurnOffOn(true, hWnd_Window, L"OldNewExplorer安装工具", L"隐藏标题栏文字（可选）");
    TurnOffOn(true, hWnd_Window, L"OldNewExplorer安装工具", L"隐藏返回上级目录按钮（必选）");
    TurnOffOn(false, hWnd_Window, L"OldNewExplorer安装工具", L"开启导航栏特效（部分主题需要勾选）");
    TurnOffOn(false, hWnd_Window, L"OldNewExplorer安装工具", L"不要勾选我");

    TurnOffOn(false, hWnd_Window, L"OldNewExplorer安装工具", L"显示底部状态栏（不用勾选）");
    TurnOffOn(false, hWnd_Window, L"OldNewExplorer安装工具", L"显示底部详细信息窗格（大部分情况下都不用勾选）");

    ClickButtonMode1(hWnd_Window, L"OldNewExplorer安装工具", L"安装");
    
    INFO_(L"[OldNewExplorer安装工具]配置完成关闭程序");
    killapp(L"OldNewExplorerCfg.exe");
    INFO_(L"[OldNewExplorer安装工具]配置结束 退出");
    return;
}
