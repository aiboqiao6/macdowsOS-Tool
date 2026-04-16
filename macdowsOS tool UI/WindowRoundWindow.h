#pragma once
//UI
//c++
#include<bits/stdc++.h>
#include<Windows.h>
//SFML
#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>
//winAPI
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
//log
#include"LogSystem.h"
//macdowsOS tool
#include"WindowConfig.h"
#include"WindowControl.h"
using namespace std;

void WinroundWindow_() {
	INFO_(L"[WindowRound]启动");
// 窗口
	sf::RenderWindow WinRound_window(sf::VideoMode({ 800, 500 }), L"WindowRound设置", sf::Style::None);
	int Window_x = 0, Window_y = 0;
	//
	INFO_(L"[WindowRound]设置垂直同步");
	//垂直同步
	WinRound_window.setVerticalSyncEnabled(ChuiZhiTongBu);

	INFO_(L"[WindowRound]设置字体");
//字体
	sf::Font font;
	if (!font.openFromFile("pingfang0.ttf")) {
		ERROR_(L"[WindowRound]字体文件无法使用");
		MessageBox(NULL, (LPCTSTR)L" 字体文件不存在或不可用,请检查字体文件完整性和可用性", (LPCTSTR)L" macdowsOS tool 工具箱组件", MB_OK | MB_ICONERROR);
		MessageBox(NULL, (LPCTSTR)L" UI界面无法加载 程序即将退出", (LPCTSTR)L" macdowsOS tool 工具箱组件", MB_OK | MB_ICONERROR);
		exit(0);
	}
	INFO_(L"[WindowRound]设置组件");
//注意事项
	MessageBox(NULL, (LPCTSTR)L" 当前功能为测试阶段 兼容性 稳定性较低 如果你只是想改圆角而不是R角的话请使用兼容性更好 稳定性高的MicaForEveryone", (LPCTSTR)L" macdowsOS tool 工具箱组件", MB_OK);
//文字
	//标题
	sf::Text title(font);
	title.setString(wstring(L"请在配置文件中输入窗口标题以开启窗口R角修改 仅在主\n窗口运行时会进行修改(若没有窗口标题则输入程序名称)"));
	title.setCharacterSize(30);
	title.setFillColor(sf::Color::Color(100, 100, 100));
	title.setPosition({ 25,100 });
	
	sf::Text buttontext(font);
	buttontext.setString(wstring(L"打开配置文件"));
	buttontext.setCharacterSize(50);
	buttontext.setFillColor(sf::Color::Color(100, 100, 100));
	buttontext.setPosition({ 250,415 });

	sf::RectangleShape buttonbackground({ 900, 100 });
	buttonbackground.setPosition({ 0,400 });
	buttonbackground.setFillColor(sf::Color::Color(210, 210, 210));
//红绿灯
	//红
	sf::CircleShape op_button_red(15);
	op_button_red.setFillColor(sf::Color(255, 0, 0));
	op_button_red.setPosition({ 30,30 });
	// set a 10-pixel wide orange outline
	op_button_red.setOutlineThickness(0.2);
	op_button_red.setOutlineColor(sf::Color(0, 0, 0));
	
//状态变量
	//拖动
	bool isDragging = false;
	sf::Vector2i dragOffset;
	//
	bool buttonbackgound = false;
	//窗口循环
	while (WinRound_window.isOpen()) {
		//缩放
		if (Window_xx == 1) {
			Window_x = 400;
			Window_y = 250;
			WinRound_window.setSize(sf::Vector2u(Window_x, Window_y));
		}
		else if (Window_xx == 2) {
			Window_x = 600;
			Window_y = 375;
			WinRound_window.setSize(sf::Vector2u(Window_x, Window_y));
		}
		else if (Window_xx == 3) {
			Window_x = 800;
			Window_y = 500;
			WinRound_window.setSize(sf::Vector2u(Window_x, Window_y));
		}
		else {
			Window_x = 1200;
			Window_y = 750;
			WinRound_window.setSize(sf::Vector2u(Window_x, Window_y));
		}
	//阴影和圆角
		// 获取句柄
		HWND hwnd_setting = WinRound_window.getNativeHandle();
		//设置窗口圆角
		HRGN hRgn = CreateRoundRectRgn(0, 0, Window_x, Window_y, 90, 90);
		SetWindowRgn(hwnd_setting, hRgn, TRUE);
		DeleteObject(hRgn); // 记得删除区域对象[citation:8]
		// 启用窗口阴影
		const MARGINS shadow = { 100, 100, 100, 100 };
		DwmExtendFrameIntoClientArea(hwnd_setting, &shadow);
		//垂直同步
		WinRound_window.setVerticalSyncEnabled(ChuiZhiTongBu);
		while (const std::optional event = WinRound_window.pollEvent()) {
			sf::Vector2f worldPos = WinRound_window.mapPixelToCoords(sf::Mouse::getPosition(WinRound_window));
			int mouse_x = worldPos.x;
			int mouse_y = worldPos.y;
			//日志
			if (open_debug_window) {
				MESSAGE_(L"[WindowRound]鼠标坐标 X:", mouse_x);
				MESSAGE_(L"[WindowRound]鼠标坐标 Y:", mouse_y);
				MESSAGE_(L"[WindowRound]拖动状态", isDragging);
			}
			
		//鼠标拖动窗口
			//检测按下鼠标
			if (event->is<sf::Event::MouseButtonPressed>() && mouse_y < 80) {
				isDragging = true;
				dragOffset = WinRound_window.getPosition() - sf::Mouse::getPosition();
			}
			//检测松开鼠标
			if (event->is<sf::Event::MouseButtonReleased>() || !(0 <= mouse_y && mouse_y <= 900) || !(0 <= mouse_x && mouse_x <= 600)) {
				isDragging = false;
			}
			//移动窗口
			if (event->is<sf::Event::MouseMoved>() && isDragging) {
				WinRound_window.setPosition(sf::Mouse::getPosition() + dragOffset);
			}
		//红绿灯
			//按下鼠标
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
				//y坐标
				if (30 <= mouse_y && mouse_y <= 60) {
					//红
					if (30 <= mouse_x && mouse_x <= 60) {
						INFO_(L"[WindowRound]退出");
						WinRound_window.close();
					}
				}
			}
		//按钮
			//按下鼠标
			//y坐标
			if (400 <= mouse_y && mouse_y <= 500 && 0 <= mouse_x && mouse_x <= 800) {
				buttonbackgound = true;
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					INFO_(L"[WindowRound]打开配置文件");
					HINSTANCE result = ShellExecuteW(NULL, L"open", L"config\\WindowRound.txt", NULL, NULL, SW_SHOWNORMAL);
					// 检查执行结果
					if ((DWORD)result <= 32) {
						ERROR_(L"[WindowRound]无法打开配置文件");
						MessageBox(NULL, (LPCTSTR)L"无法打开配置文件 请检查config文件夹完整性和可用性", (LPCTSTR)L" macdowsOS tool WindowRound设置", MB_OK | MB_ICONERROR);
					}
				}
			}
			else {
				buttonbackgound = false;
			}

		}

		WinRound_window.clear(sf::Color::Color(245, 245, 245));
		//红绿灯
		WinRound_window.draw(op_button_red);
		//
		WinRound_window.draw(title);
		if(buttonbackgound)WinRound_window.draw(buttonbackground);
		WinRound_window.draw(buttontext);
		

		WinRound_window.display();
	}
	return;
}