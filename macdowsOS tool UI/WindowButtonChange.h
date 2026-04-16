#pragma once
#include<bits/stdc++.h>
#include<Windows.h>
//SFML
#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>
using namespace std;

void test() {
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    
    sf::RenderWindow window(sf::VideoMode({ 200, 40 }), L"macdowsOS Tool test", sf::Style::None);
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
    //黄
    sf::CircleShape op_button_yellow(15);
    op_button_yellow.setFillColor(sf::Color(255, 215, 0));
    op_button_yellow.setOutlineThickness(0.2);
    op_button_yellow.setOutlineColor(sf::Color(0, 0, 0));
    //绿
    sf::CircleShape op_button_green(15);
    op_button_green.setFillColor(sf::Color(0, 245, 0));
    op_button_green.setOutlineThickness(0.2);
    op_button_green.setOutlineColor(sf::Color(0, 0, 0));
    
    op_button_red.setPosition({ 1, 1 });
    op_button_yellow.setPosition({ 70, 1 });
    op_button_green.setPosition({ 140, 1 });


    window.clear(sf::Color::Black);
    // 绘制所有内容，和普通SFML程序无差异
    window.draw(op_button_red);
    window.draw(op_button_yellow);
    window.draw(op_button_green);
    // 提交帧
    window.display();


    
    
    RECT windowRect;
    HWND hWnd_Window;

    int top=0, right=0;
    // 4. 主循环，完全兼容SFML原生绘制API
    while (window.isOpen())
    {
       hWnd_Window = FindWindowW(NULL, L"设置");
       while (hWnd_Window == NULL||(top == windowRect.top && right == windowRect.right)) {
           Sleep(1);
           hWnd_Window = FindWindowW(NULL, L"设置");
           GetWindowRect(hWnd_Window, &windowRect);
           while (const optional event = window.pollEvent()) {}
       }
       GetWindowRect(hWnd_Window, &windowRect);
       
       top = windowRect.top;
       right = windowRect.right;
       //SetWindowLongPtr(hWnd, -8, (LONG_PTR)hWnd_Window);
       SetWindowPos(hWnd, hWnd_Window, right -200,  top+10, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
    }
}