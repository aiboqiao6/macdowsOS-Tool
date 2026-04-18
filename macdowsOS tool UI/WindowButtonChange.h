#pragma once
#include<bits/stdc++.h>
#include<Windows.h>
//SFML
#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>
using namespace std;

void test() {
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 800;
    sf::RenderWindow window(sf::VideoMode({ 300, 70 }), L"macdowsOS Tool test", sf::Style::None, sf::State::Windowed, settings);
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
    HWND hWnd_Window;

    int top=0, right=0;
    LPCTSTR changeapp = L"QQ";
    hWnd_Window = FindWindowW(NULL, changeapp);
    while (hWnd_Window == NULL) {
        hWnd_Window = FindWindowW(NULL, changeapp);
    }

    while (window.isOpen())
    {
        //位置
       SetWindowPos(hWnd, hWnd_Window, right - 210, top + 10, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
       SetWindowPos(hWnd_Window, hWnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

       hWnd_Window = FindWindowW(NULL, changeapp);
       int outtime = 0;
       while (hWnd_Window == NULL||(top == windowRect.top && right == windowRect.right)) {
           //10s强制同步一次
           if (outtime == 2000) {
               SetWindowPos(hWnd, hWnd_Window, right - 210, top + 15, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
               SetWindowPos(hWnd_Window, hWnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
               outtime = 0;
           }
           else {
               outtime++;
           }
           Sleep(1);
           hWnd_Window = FindWindowW(NULL, changeapp);
           GetWindowRect(hWnd_Window, &windowRect);
           while (const optional event = window.pollEvent()) {}
       }
       GetWindowRect(hWnd_Window, &windowRect);
       
       top = windowRect.top;
       right = windowRect.right;
       
       
       
    }
}