#pragma once
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
using namespace std;

void AboutWindow_() {
	INFO_(L"[关于]启动");
// 窗口
	sf::RenderWindow About_window(sf::VideoMode({ 600, 500 }), L"关于 macdowsOS Tool", sf::Style::None);
	int Window_x = 0, Window_y = 0;
	//
	INFO_(L"[关于]设置垂直同步");
	//垂直同步
	About_window.setVerticalSyncEnabled(ChuiZhiTongBu);
	INFO_(L"[关于]设置字体");
//字体
	sf::Font font;
	if (!font.openFromFile("pingfang0.ttf")) {
		ERROR_(L"[关于]字体文件无法使用");
		MessageBox(NULL, (LPCTSTR)L" 字体文件不存在或不可用,请检查字体文件完整性和可用性", (LPCTSTR)L" macdowsOS tool 工具箱组件", MB_OK | MB_ICONERROR);
		MessageBox(NULL, (LPCTSTR)L" UI界面无法加载 程序即将退出", (LPCTSTR)L" macdowsOS tool 工具箱组件", MB_OK | MB_ICONERROR);
		exit(0);
	}
	INFO_(L"[关于]设置组件");
//文字
	//标题
	sf::Text title(font);
	title.setString(wstring(L"版本:1.21 版本号:1211.20260307"));
	title.setCharacterSize(30);
	title.setFillColor(sf::Color::Color(100, 100, 100));
	title.setPosition({ 105,200 });
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
	while (About_window.isOpen()) {
		//缩放
		if (Window_xx == 1) {
			Window_x = 300;
			Window_y = 250;
			About_window.setSize(sf::Vector2u(Window_x, Window_y));
		}
		else if (Window_xx == 2) {
			Window_x = 450;
			Window_y = 375;
			About_window.setSize(sf::Vector2u(Window_x, Window_y));
		}
		else if (Window_xx == 3) {
			Window_x = 600;
			Window_y = 500;
			About_window.setSize(sf::Vector2u(Window_x, Window_y));
		}
		else {
			Window_x = 900;
			Window_y = 750;
			About_window.setSize(sf::Vector2u(Window_x, Window_y));
		}
	//阴影和圆角
		// 获取句柄
		HWND hwnd_setting = About_window.getNativeHandle();
		//设置窗口圆角
		HRGN hRgn = CreateRoundRectRgn(0, 0, Window_x, Window_y, 90, 90);
		SetWindowRgn(hwnd_setting, hRgn, TRUE);
		DeleteObject(hRgn); // 记得删除区域对象[citation:8]
		// 启用窗口阴影
		const MARGINS shadow = { 100, 100, 100, 100 };
		DwmExtendFrameIntoClientArea(hwnd_setting, &shadow);
		//垂直同步
		About_window.setVerticalSyncEnabled(ChuiZhiTongBu);
		while (const std::optional event = About_window.pollEvent()) {
			sf::Vector2f worldPos = About_window.mapPixelToCoords(sf::Mouse::getPosition(About_window));
			int mouse_x = worldPos.x;
			int mouse_y = worldPos.y;
			//日志
			if (open_debug_window) {
				MESSAGE_(L"[关于]鼠标坐标 X:", mouse_x);
				MESSAGE_(L"[关于]鼠标坐标 Y:", mouse_y);
				MESSAGE_(L"[关于]拖动状态", isDragging);
			}
		//鼠标拖动窗口
			//检测按下鼠标
			if (event->is<sf::Event::MouseButtonPressed>() && mouse_y < 80) {
				isDragging = true;
				dragOffset = About_window.getPosition() - sf::Mouse::getPosition();
			}
			//检测松开鼠标
			if (event->is<sf::Event::MouseButtonReleased>() || !(0 <= mouse_y && mouse_y <= 900) || !(0 <= mouse_x && mouse_x <= 600)) {
				isDragging = false;
			}
			//移动窗口
			if (event->is<sf::Event::MouseMoved>() && isDragging) {
				About_window.setPosition(sf::Mouse::getPosition() + dragOffset);
			}
			//红绿灯
				//按下鼠标
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
				//y坐标
				if (30 <= mouse_y && mouse_y <= 60) {
					//红
					if (30 <= mouse_x && mouse_x <= 60) {
						INFO_(L"[关于]退出");
						About_window.close();
						return;
					}
				}
			}
		}

		About_window.clear(sf::Color::Color(245, 245, 245));
		//红绿灯
		About_window.draw(op_button_red);
		//
		About_window.draw(title);


		About_window.display();
	}
	return;
}