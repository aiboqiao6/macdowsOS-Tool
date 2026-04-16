#pragma once
#include<bits/stdc++.h>
#include"FilesSystem.h"
#include"WindowControl.h"
#include<Windows.h>
#include <string>
#include <chrono>
#include <thread>
using namespace std;
namespace {
    // 提前声明结构体（解决未声明标识符问题）
    struct ControlHandles {
        HWND hEdit;
        HWND hOkBtn;
    };
        // 返回：第一个找到的输入框控件HWND，未找到返回NULL
        HWND FindEditControlRecursive(HWND hParent) {

            // 2. 遍历当前父窗口的直接子控件（GW_CHILD获取第一个子控件）
            HWND hChild = GetWindow(hParent, GW_CHILD);
            while (hChild) {
                // 3. 获取当前子控件的类名，判断是否为输入框（EDIT类）
                wchar_t className[256] = { 0 };
                GetClassNameW(hChild, className, 256);

                // 精准匹配标准输入框类名（EDIT），找到则直接返回
                if (wcscmp(className, L"Edit") == 0|| wcscmp(className, L"EDIT")==0) {
                    return hChild;
                }

                // 4. 递归查找当前子控件的子控件（深度优先，优先查深层级）
                HWND hFound = FindEditControlRecursive(hChild);
                if (hFound) {
                    return hFound;
                }

                // 5. 查找下一个兄弟控件
                hChild = GetWindow(hChild, GW_HWNDNEXT);
            }

            // 6. 所有子控件遍历完毕未找到，返回NULL
            return NULL;
        }
    // 核心函数：控制运行窗口执行指定命令
    bool ExecuteInRunDialog(const std::wstring& command){
        // 1. 模拟按下Win+R打开运行窗口（优化：确保按键释放顺序正确）
        INPUT inputs[4] = { 0 };
        // 按下Win键
        inputs[0].type = INPUT_KEYBOARD;
        inputs[0].ki.wVk = VK_LWIN;
        // 按下R键
        inputs[1].type = INPUT_KEYBOARD;
        inputs[1].ki.wVk = 'R';
        // 松开R键
        inputs[2].type = INPUT_KEYBOARD;
        inputs[2].ki.wVk = 'R';
        inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;
        // 松开Win键
        inputs[3].type = INPUT_KEYBOARD;
        inputs[3].ki.wVk = VK_LWIN;
        inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(_countof(inputs), inputs, sizeof(INPUT));
        // 2. 等待运行窗口创建，并获取其句柄（兼容多语言：优先按类名查找，不依赖标题）
        INFO_(L"[OldNewExplorer安装组件]等待运行窗口启动");
        HWND hRunDialog = FindWindowW(NULL, L"运行");
        while (hRunDialog == NULL) {
            hRunDialog = FindWindowW(NULL, L"运行");
        }
        INFO_(L"[OldNewExplorer安装组件]运行窗口启动");
        // 3. 枚举运行窗口的子控件，找到输入编辑框和确定按钮
        // 修正结构体初始化方式（解决语法错误）
        ControlHandles handles;
        handles.hEdit = nullptr;
        handles.hOkBtn = nullptr;

        INFO_(L"[OldNewExplorer安装组件]等待输入框加载成功");
        handles.hEdit = FindEditControlRecursive(hRunDialog);
        while (handles.hEdit == NULL) {
            handles.hEdit = FindEditControlRecursive(hRunDialog);
        }
        INFO_(L"[OldNewExplorer安装组件]运行窗口启动");
        
        // 4. 关键修复：先激活输入框，确保焦点在输入框上
        SetForegroundWindow(hRunDialog); // 将运行窗口置顶
        SetFocus(handles.hEdit);         // 给输入框设置焦点
        // 5. 向编辑框发送文本（分步骤，触发内容变更）
        SendMessageW(handles.hEdit, EM_SETSEL, 0, -1); // 选中所有文本
        SendMessageW(handles.hEdit, WM_CLEAR, 0, 0);    // 清除选中内容
        // 核心修复1：逐字符发送输入（模拟真实键盘输入，触发内容变更）
        for (wchar_t c : command){
            // 发送字符消息（替代直接SetText，更贴近真实输入）
            SendMessageW(handles.hEdit, WM_CHAR, static_cast<WPARAM>(c), 0);
        }
        // 核心修复2：主动发送EN_CHANGE通知，告诉父窗口输入框内容变了
        SendMessageW(hRunDialog, WM_COMMAND,MAKEWPARAM(GetDlgCtrlID(handles.hEdit), EN_CHANGE),reinterpret_cast<LPARAM>(handles.hEdit));
       
        enter_key();
        

        return true;
    }

}
void OldNewExplorer_Install() {
    INFO_(L"[OldNewExplorer安装组件]组件运行");
    INFO_(L"[OldNewExplorer安装组件]拷贝程序文件");
    copyPath(L"./AppData/OldNewExplorer", L"C:/");

    INFO_(L"[OldNewExplorer安装组件]运行配置程序");
    ExecuteInRunDialog(L"C:\OldNewExplorer\OldNewExplorerCfg.exe");
    INFO_(L"[OldNewExplorer安装组件]等待配置程序启动");
    HWND hWnd_Window= FindWindowW(NULL, L"致美化 OldNewExplorer 配置");
    while (hWnd_Window == NULL) {
        hWnd_Window = FindWindowW(NULL, L"致美化 OldNewExplorer 配置");
    }
    INFO_(L"[OldNewExplorer安装组件]配置程序启动");
    SetForegroundWindow(hWnd_Window);
    SetWindowPos(hWnd_Window, NULL, 0, 0, 0, 0, SWP_NOSIZE);

    INFO_(L"[OldNewExplorer安装组件]开始操控配置程序");
    TurnOffOn(true , hWnd_Window, L"OldNewExplorer安装组件", L"使用经典驱动器分组");
    TurnOffOn(false, hWnd_Window, L"OldNewExplorer安装组件", L"使用库、并隐藏顶部文件夹");

    TurnOffOn(true , hWnd_Window, L"OldNewExplorer安装组件", L"使用工具栏代替功能区（必选）");
    TurnOffOn(false, hWnd_Window, L"OldNewExplorer安装组件", L"不要勾选我");
    TurnOffOn(false, hWnd_Window, L"OldNewExplorer安装组件", L"隐藏返回上级目录按钮（必选）");
    TurnOffOn(false, hWnd_Window, L"OldNewExplorer安装组件", L"开启导航栏特效（部分主题需要勾选）");
    TurnOffOn(true , hWnd_Window, L"OldNewExplorer安装组件", L"隐藏标题栏小图标（可选）");
    TurnOffOn(true , hWnd_Window, L"OldNewExplorer安装组件", L"隐藏标题栏文字（可选）");
    
    TurnOffOn(false, hWnd_Window, L"OldNewExplorer安装组件", L"显示底部详细信息窗格（大部分情况下都不用勾选）");
    TurnOffOn(false, hWnd_Window, L"OldNewExplorer安装组件", L"显示底部状态栏（不用勾选）");

    ClickButtonMode1(hWnd_Window, L"OldNewExplorer安装组件", L"安装");
    
    INFO_(L"[OldNewExplorer安装组件]结束配置程序进程");
    killapp(L"OldNewExplorerCfg.exe");
    INFO_(L"[OldNewExplorer安装组件]配置结束 退出");
    return;
}