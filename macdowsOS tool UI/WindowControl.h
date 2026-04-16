#pragma once
#include<bits/stdc++.h>
#include<windows.h>
#include"LogSystem.h"
using namespace std;
//回车
void enter_key() {
    INFO_(L"[窗口控制组件]发送回车点击信息");
    // 定义输入事件（按下 + 释放，共2个事件）
    INPUT inputs[2] = {};

    // 1. 模拟回车键按下（VK_RETURN是回车键的虚拟键码）
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_RETURN;
    inputs[0].ki.dwFlags = 0;

    // 2. 模拟回车键释放
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = VK_RETURN;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

    // 发送输入事件
    SendInput(_countof(inputs), inputs, sizeof(INPUT));
}
//查找
HWND FindChildControlExact(HWND hParent, const wchar_t* targetTitle) {
    bool recursive = true;
    if (!hParent || !targetTitle) return NULL;
    // 标准化目标标题（移除&符号和多余空格）
    wchar_t normalizedTarget[256] = { 0 };
    int destIndex = 0;
    for (int i = 0; targetTitle[i] && destIndex < 255; i++) {
        // 跳过单个&（通常用于快捷键标记，如"I &accept"中的&）
        if (targetTitle[i] == L'&' && targetTitle[i + 1] != L'&') continue;
        // 处理空格（连续空格合并为单个，开头/结尾空格去除）
        if (iswspace(targetTitle[i])) {
            if (destIndex > 0 && !iswspace(normalizedTarget[destIndex - 1])) {
                normalizedTarget[destIndex++] = L' ';
            }
            continue;
        }
        normalizedTarget[destIndex++] = targetTitle[i];
    }
    normalizedTarget[destIndex] = L'\0';
    // 遍历父窗口的所有子窗口
    HWND hChild = GetWindow(hParent, GW_CHILD);
    while (hChild) {
        // 获取子窗口标题
        wchar_t childText[256] = { 0 };
        GetWindowTextW(hChild, childText, 256);
        // 标准化子窗口标题（同目标标题处理逻辑）
        wchar_t normalizedChild[256] = { 0 };
        int childIndex = 0;
        for (int i = 0; childText[i] && childIndex < 255; i++) {
            if (childText[i] == L'&' && childText[i + 1] != L'&') continue;
            if (iswspace(childText[i])) {
                if (childIndex > 0 && !iswspace(normalizedChild[childIndex - 1])) {
                    normalizedChild[childIndex++] = L' ';
                }
                continue;
            }
            normalizedChild[childIndex++] = childText[i];
        }
        normalizedChild[childIndex] = L'\0';
        // 标题匹配则返回控件句柄
        if (wcscmp(normalizedChild, normalizedTarget) == 0) return hChild;
        // 递归查找子窗口的子控件（深度优先）
        if (recursive) {
            HWND hFound = FindChildControlExact(hChild, targetTitle);
            if (hFound) return hFound;
        }
        // 查找下一个兄弟窗口
        hChild = GetWindow(hChild, GW_HWNDNEXT);
    }
    return NULL;  // 未找到目标控件
}
//模拟鼠标真实点击
void RealisticClick(HWND hwnd) {
    if (!hwnd) return;

    // 激活窗口
    SetForegroundWindow(GetAncestor(hwnd, GA_ROOT));
    // 获取控件位置和大小
    RECT rc;
    GetWindowRect(hwnd, &rc);
    int x = (rc.left + rc.right) / 2;  // 控件中心x坐标
    int y = (rc.top + rc.bottom) / 2;  // 控件中心y坐标
    MESSAGE_(L"[macdowsOS Tool窗口控制组件]控件x坐标", x);
    MESSAGE_(L"[macdowsOS Tool窗口控制组件]控件y坐标", y);

    // 保存原始鼠标位置，操作完成后恢复
    POINT origPos;
    GetCursorPos(&origPos);
    SetCursorPos(x, y);  // 移动鼠标到控件中心

    // 准备鼠标输入事件
    INPUT inputs[4] = {};
    // 1. 移动鼠标到绝对位置
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dx = static_cast<LONG>(x * (65535.0 / GetSystemMetrics(SM_CXSCREEN)));  // 转换为0-65535范围
    inputs[0].mi.dy = static_cast<LONG>(y * (65535.0 / GetSystemMetrics(SM_CYSCREEN)));
    inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

    // 2. 左键按下
    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    // 3. 左键释放（延迟50ms模拟真实点击）
    inputs[2].type = INPUT_MOUSE;
    inputs[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    inputs[2].mi.time = 50;

    // 4. 恢复鼠标到原始位置
    inputs[3].type = INPUT_MOUSE;
    inputs[3].mi.dx = static_cast<LONG>(origPos.x * (65535.0 / GetSystemMetrics(SM_CXSCREEN)));
    inputs[3].mi.dy = static_cast<LONG>(origPos.y * (65535.0 / GetSystemMetrics(SM_CYSCREEN)));
    inputs[3].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

    // 发送鼠标输入
    SendInput(4, inputs, sizeof(INPUT));
}
//获取最后一个子窗口ID
HWND GetLastChildWindow(HWND hParent) {
    HWND hChild = GetWindow(hParent, GW_CHILD);
    if (!hChild) return NULL;

    HWND hLast = hChild;
    // 遍历到最后一个子窗口
    while ((hChild = GetWindow(hChild, GW_HWNDNEXT)) != NULL) {
        hLast = hChild;
    }
    return hLast;
}

namespace {
    HWND findwindow(HWND hwnd_window, wstring pack_name, wstring button_name) {
        SYSTEMTIME now;
        GetLocalTime(&now);
        INFO_(L"[" + pack_name + L"]" + L"正在查找组件" + button_name);
        const wchar_t* button_ = button_name.data();
        HWND temp = FindChildControlExact(hwnd_window, button_);
        while (temp == NULL) {
            SYSTEMTIME now_temp;
            GetLocalTime(&now_temp);
            if (now_temp.wSecond - now.wSecond >= 20) {
                ERROR_(L"[" + pack_name + L"]" + L"查找组件超时 " + button_name);
                MessageBox(NULL, (LPCTSTR)L" 查找组件超时 窗口可能没有启动或正确加载", (LPCTSTR)L" macdowsOS tool 窗口控制核心组件", MB_OK);
            }
            temp = FindChildControlExact(hwnd_window, button_);
        }
        INFO_(L"[" + pack_name + L"]" + L"成功查找到" + button_name);
        return temp;
    }
}
//直接发送点击消息
void ClickButtonMode1(HWND hwnd_window, wstring pack_name, wstring button_name) {
    HWND temp = findwindow(hwnd_window, pack_name, button_name);
    SetForegroundWindow(hwnd_window);
    SendMessageW(temp, BM_CLICK, 0, 0);
    return;
}
//模拟鼠标点击
void ClickButtonMode2(HWND hwnd_window, wstring pack_name, wstring button_name) {
    HWND temp = findwindow(hwnd_window, pack_name, button_name);
    SetForegroundWindow(hwnd_window);
    RealisticClick(temp);
    return;
}
//回车点击
void ClickButtonMode3(HWND hwnd_window, wstring pack_name, wstring button_name) {
    HWND temp = findwindow(hwnd_window, pack_name, button_name);
    SetForegroundWindow(hwnd_window);
    enter_key();
    return;
}
//开关
void TurnOffOn(bool OffOn, HWND hwnd_window, wstring pack_name, wstring button_name) {
    HWND temp = findwindow(hwnd_window, pack_name, button_name);
    SetForegroundWindow(hwnd_window);
    if ((SendMessageW(temp, BM_GETCHECK, 0, 0) == BST_CHECKED) != OffOn) {
        SendMessageW(GetParent(temp), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(temp), BN_CLICKED), (LPARAM)temp);
    }
    return;
}
