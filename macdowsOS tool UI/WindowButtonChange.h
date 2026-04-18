#pragma once
#include<bits/stdc++.h>
#include<Windows.h>
//SFML
#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>
using namespace std;
namespace {
    void buttonchange(HWND hWnd_Window) {
        //抗锯齿
        sf::ContextSettings settings;
        settings.antiAliasingLevel = 100;
        sf::RenderWindow window(sf::VideoMode({ 200, 70 }), L"macdowsOS Tool test", sf::Style::None, sf::State::Windowed, settings);
        // 2. 窗口永久置顶
        HWND hWnd = window.getNativeHandle();
        // 3. 配置窗口透明与鼠标穿透
        LONG_PTR exStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
        SetWindowLongPtr(hWnd, GWL_EXSTYLE, exStyle
            | WS_EX_LAYERED    // 分层窗口，支持透明度
            | WS_EX_TRANSPARENT // 鼠标事件穿透到下层
            | WS_EX_TOOLWINDOW  // 隐藏任务栏图标
        );
        // 色键透明：纯黑背景完全透明，可按需调整透明色
        SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
        //

        //红绿灯
        //红
        sf::CircleShape op_button_red(15);
        op_button_red.setFillColor(sf::Color(245, 0, 0));
        op_button_red.setOutlineThickness(0.2);
        op_button_red.setOutlineColor(sf::Color(0, 0, 0));
        op_button_red.setPointCount(10000);
        //黄
        sf::CircleShape op_button_yellow(15);
        op_button_yellow.setFillColor(sf::Color(255, 215, 0));
        op_button_yellow.setOutlineThickness(0.2);
        op_button_yellow.setOutlineColor(sf::Color(0, 0, 0));
        op_button_yellow.setPointCount(10000);
        //绿
        sf::CircleShape op_button_green(15);
        op_button_green.setFillColor(sf::Color(0, 245, 0));
        op_button_green.setOutlineThickness(0.2);
        op_button_green.setOutlineColor(sf::Color(0, 0, 0));
        op_button_green.setPointCount(10000);

        op_button_green.setPosition({ 15, 15 });
        op_button_yellow.setPosition({ 84, 15 });
        op_button_red.setPosition({ 154, 15 });

        //back
        sf::CircleShape button1(30);
        button1.setPointCount(100);
        button1.setPosition({ 1,1 });
        button1.setFillColor(sf::Color(64, 64, 64));
        button1.setPointCount(10000);

        sf::RectangleShape button2({ 139.5,60 });
        button2.setPosition({ 30,1 });
        button2.setFillColor(sf::Color(64, 64, 64));

        sf::CircleShape button3(30);
        button3.setPointCount(100);
        button3.setPosition({ 140,1 });
        button3.setFillColor(sf::Color(64, 64, 64));
        button3.setPointCount(10000);


        window.clear(sf::Color::Black);
        // 绘制所有内容，和普通SFML程序无差异
        window.draw(button1);
        window.draw(button2);
        window.draw(button3);

        window.draw(op_button_red);
        window.draw(op_button_yellow);
        window.draw(op_button_green);
        // 提交帧
        window.display();




        RECT windowRect;

        int top = 0, right = 0;
       /* LPCTSTR changeapp = L"QQ";
        //获取hwnd
        hWnd_Window = FindWindowW(NULL, changeapp);
        while (hWnd_Window == NULL) {
            hWnd_Window = FindWindowW(NULL, changeapp);
        }*/
        //是否被激活
        bool iftopon = (GetForegroundWindow() == hWnd_Window);
        while (window.isOpen()) {
            //获取窗口信息
            GetWindowRect(hWnd_Window, &windowRect);

            top = windowRect.top;
            right = windowRect.right;
            //位置
            SetWindowPos(hWnd, hWnd_Window, right - 210, top + 10, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
            SetWindowPos(hWnd_Window, hWnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

            while ((top == windowRect.top && right == windowRect.right)) {
                //当窗口激活状态更改时刷新
                if (iftopon != (GetForegroundWindow() == hWnd_Window)) {
                    //获取窗口信息
                    GetWindowRect(hWnd_Window, &windowRect);

                    top = windowRect.top;
                    right = windowRect.right;
                    SetWindowPos(hWnd, hWnd_Window, right - 210, top + 10, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
                    SetWindowPos(hWnd_Window, hWnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
                    //
                    iftopon = (GetForegroundWindow() == hWnd_Window);
                }

                Sleep(1);
                //刷新窗口信息
                GetWindowRect(hWnd_Window, &windowRect);
                //防无响应
                while (const optional event = window.pollEvent()) {}
            }

        }
    }
}

void buttonchangemain() {
    //坐标
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    HWND hwnd= { GetTopWindow(NULL) };

    // 开始循环遍历
    while (hwnd != NULL) {

        // 1. 只看可见窗口
        if (IsWindowVisible(hwnd)) {
            // 2. 检查类名，排除右键菜单 (#32768)
            WCHAR class_name[256];
            GetClassName(hwnd, class_name, 256);
            if (wcscmp(class_name, L"#32768") != 0) {
                // 3. 检查扩展样式，排除工具小窗
                LONG ex_style = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
                if (!(ex_style & WS_EX_TOOLWINDOW)) {
                    // --- 核心修改：为这个窗口单独启动一个线程 ---
                    HWND target_hwnd = hwnd;
                    std::thread([target_hwnd]() {
                        buttonchange(target_hwnd);
                        }).detach(); // detach 让线程独立运行，主线程不等待
                }
            }
        }

        // --- 过滤逻辑结束 ---

        // 获取下一个窗口，继续循环
        hwnd = GetNextWindow(hwnd, GW_HWNDNEXT);
        
    }
    Sleep(1100000000000);
}