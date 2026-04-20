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
#include"AboutWindow.h"
// 需要链接 NtDll.lib
#pragma comment(lib, "NtDll.lib")
using namespace std;

void SettingWindow_() {
	INFO_(L"[设置]启动");
// 窗口
	sf::RenderWindow setting_window(sf::VideoMode({ 600, 900 }), L"macdowsOS tool 设置", sf::Style::None);
	int Window_x = 0, Window_y = 0;
	//
	INFO_(L"[设置]设置垂直同步");
//垂直同步
	setting_window.setVerticalSyncEnabled(ChuiZhiTongBu);
	INFO_(L"[设置]设置字体");
//字体
	sf::Font font;
	if (!font.openFromFile("pingfang0.ttf")) {
		ERROR_(L"[设置]字体文件无法使用");
		MessageBox(NULL, (LPCTSTR)L" 字体文件不存在或不可用,请检查字体文件完整性和可用性", (LPCTSTR)L" macdowsOS tool 设置", MB_OK | MB_ICONERROR);
		MessageBox(NULL, (LPCTSTR)L" UI界面无法加载 程序即将退出", (LPCTSTR)L" macdowsOS tool 设置", MB_OK | MB_ICONERROR);
		exit(0);
	}
	INFO_(L"[设置]设置组件");
//红绿灯
	//红3
	sf::CircleShape op_button_red(15);
	op_button_red.setFillColor(sf::Color(255, 0, 0));
	op_button_red.setPosition({ 30,30 });
	// set a 10-pixel wide orange outline
	op_button_red.setOutlineThickness(0.2);
	op_button_red.setOutlineColor(sf::Color(0, 0, 0));
//标题
	sf::Text title(font);
	title.setString(L"macdowsOS tool 设置");
	title.setCharacterSize(40);
	title.setFillColor(sf::Color::Color(80, 80, 80));
	title.setPosition({ 110,20 });
//项目文字
	sf::Text program_text(font);
	program_text.setString(L"开启垂直同步\n\n\n开启窗口循环日志输出\n\n\n窗口缩放大小");
	program_text.setCharacterSize(40);
	program_text.setFillColor(sf::Color::Color(80, 80, 80));
	program_text.setPosition({ 20,110 });

	sf::Text program_minitext(font);
	program_minitext.setString(L"关闭此项可能导致窗口严重撕裂 若没有出现显示异常请勿关闭此\n项\n\n\n\n\n此项仅建议在UI窗口出现bug时开启,开启此项可能会使占用增加\n\n\n\n\n\n如果macdowsOS Tool的UI界面不适合你的电脑屏幕 可以修改此\n选项调整UI");
	program_minitext.setCharacterSize(20);
	program_minitext.setFillColor(sf::Color::Color(80, 80, 80));
	program_minitext.setPosition({ 20,160 });
//项目状态
	sf::Text program_nowtext1(font);
	program_nowtext1.setString(L"--");
	program_nowtext1.setCharacterSize(40);
	program_nowtext1.setFillColor(sf::Color::Color(80, 80, 80));
	program_nowtext1.setPosition({ 500,110 });

	sf::Text program_nowtext2(font);
	program_nowtext2.setString(L"--");
	program_nowtext2.setCharacterSize(40);
	program_nowtext2.setFillColor(sf::Color::Color(80, 80, 80));
	program_nowtext2.setPosition({ 500,270 });

	sf::Text program_nowtext3(font);
	program_nowtext3.setString(L"--");
	program_nowtext3.setCharacterSize(40);
	program_nowtext3.setFillColor(sf::Color::Color(80, 80, 80));
	program_nowtext3.setPosition({ 500,430 });
//关于
	sf::Text abouttext(font);
	abouttext.setString(L"关于macdowsOS Tool");
	abouttext.setCharacterSize(40);
	abouttext.setFillColor(sf::Color::Color(80, 80, 80));
	abouttext.setPosition({ 100,810 });
	//背景
	sf::RectangleShape aboutbackground({ 600, 125 });
	aboutbackground.setPosition({ 0,775 });
	aboutbackground.setFillColor(sf::Color::Color(225, 225, 225));
//状态变量
    //拖动
	bool isDragging = false;
	sf::Vector2i dragOffset;
	//背景
	bool aboutback = false;
//窗口循环
	while (setting_window.isOpen()){
		//缩放
		if (Window_xx == 1) {
			Window_x = 300;
			Window_y = 450;
			setting_window.setSize(sf::Vector2u(Window_x, Window_y));
		}
		else if (Window_xx == 2) {
			Window_x = 450;
			Window_y = 675;
			setting_window.setSize(sf::Vector2u(Window_x, Window_y));
		}
		else if (Window_xx == 3) {
			Window_x = 600;
			Window_y = 900;
			setting_window.setSize(sf::Vector2u(Window_x, Window_y));
		}
		else {
			Window_x = 900;
			Window_y = 1350;
			setting_window.setSize(sf::Vector2u(Window_x, Window_y));
		}
	//阴影和圆角
		// 获取句柄
		HWND hwnd_setting = setting_window.getNativeHandle();
		//设置窗口圆角
		HRGN hRgn = CreateRoundRectRgn(0, 0, Window_x, Window_y, 90, 90);
		SetWindowRgn(hwnd_setting, hRgn, TRUE);
		DeleteObject(hRgn); // 记得删除区域对象[citation:8]
		// 启用窗口阴影
		const MARGINS shadow = { 100, 100, 100, 100 };
		DwmExtendFrameIntoClientArea(hwnd_setting, &shadow);
		//垂直同步
		setting_window.setVerticalSyncEnabled(ChuiZhiTongBu);
		while (const std::optional event = setting_window.pollEvent()) {
			sf::Vector2f worldPos = setting_window.mapPixelToCoords(sf::Mouse::getPosition(setting_window));
			int mouse_x = worldPos.x;
			int mouse_y = worldPos.y;
			//项目状态
			//1
			if (ChuiZhiTongBu) {
				program_nowtext1.setString(L"开");
			}
			else {
				program_nowtext1.setString(L"关");
			}
			//2
			if (open_debug_window == 1) {
				program_nowtext2.setString(L"开");
			}
			else {
				program_nowtext2.setString(L"关");
			}
			//3
			if (Window_xx == 1) {
				program_nowtext3.setString(L"1x");
			}
			else if (Window_xx == 2) {
				program_nowtext3.setString(L"2x");
			}
			else if (Window_xx == 3) {
				program_nowtext3.setString(L"3x");
			}
			else {
				program_nowtext3.setString(L"4x");
			}
			//日志
			if (open_debug_window) {
				MESSAGE_(L"[设置]鼠标坐标 X:", mouse_x);
				MESSAGE_(L"[设置]鼠标坐标 Y:", mouse_y);
				MESSAGE_(L"[设置]拖动状态", isDragging);
			}
				
		//鼠标拖动窗口
				//检测按下鼠标
				if (event->is<sf::Event::MouseButtonPressed>() && mouse_y < 80) {
					isDragging = true;
					dragOffset = setting_window.getPosition() - sf::Mouse::getPosition();
				}
				//检测松开鼠标
				if (event->is<sf::Event::MouseButtonReleased>() || !(0 <= mouse_y && mouse_y <= 900) || !(0 <= mouse_x && mouse_x <= 600)) {
					isDragging = false;
				}
				//移动窗口
				if (event->is<sf::Event::MouseMoved>() && isDragging) {
					setting_window.setPosition(sf::Mouse::getPosition() + dragOffset);
				}
				
		//红绿灯
			//按下鼠标
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
				//y坐标
				if (30 <= mouse_y && mouse_y <= 60) {
					//红
					if (30 <= mouse_x && mouse_x <= 60) {
						INFO_(L"[设置]退出");
						setting_window.close();
					}
				}
			}
			//操作按钮
			
			//1
			if (0 <= mouse_x && mouse_x <= 600) {
				if (110 <= mouse_y && mouse_y <= 150) {
					if (event->is<sf::Event::MouseButtonPressed>()) {
						INFO_(L"[设置]点击 项目1");
						if (ChuiZhiTongBu) {
							ChuiZhiTongBu = false;
						}
						else {
							ChuiZhiTongBu = true;
						}
					}
				}
				else if (270 <= mouse_y && mouse_y <= 310) {
					if (event->is<sf::Event::MouseButtonPressed>()) {
						INFO_(L"[设置]点击 项目2");
						if (open_debug_window == 1) {
							open_debug_window = 0;
						}
						else {
							open_debug_window = 1;
						}
					}
				}
				else if (430 <= mouse_y && mouse_y <= 470) {
					if (event->is<sf::Event::MouseButtonPressed>()) {
						INFO_(L"[设置]点击 项目3");
						Window_xx++;
						if (Window_xx >= 5) {
							Window_xx = 1;
						}
					}
				}
			}
			//关于
			if (0 <= mouse_x && mouse_x <= 600 && 775 <= mouse_y && mouse_y <= 900) {
				aboutback = true;
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					INFO_(L"[设置]点击 关于");
					isDragging = false;
					setting_window.setSize({ 1,1 });
					AboutWindow_();
					setting_window.setSize(sf::Vector2u(Window_x, Window_y));
					isDragging = false;
					INFO_(L"[设置]运行");
				}
			}
			else {
				aboutback = false;
			}



		}

		setting_window.clear(sf::Color::Color(245, 245, 245));
		//红绿灯
		setting_window.draw(op_button_red);
		//标题
		setting_window.draw(title);
		//项目文字
		setting_window.draw(program_text);
		setting_window.draw(program_minitext);
		//项目状态
		setting_window.draw(program_nowtext1);
		setting_window.draw(program_nowtext2);
		setting_window.draw(program_nowtext3);
		//关于
		if(aboutback) setting_window.draw(aboutbackground);
		setting_window.draw(abouttext);
		

		setting_window.display();
	}
	return;
}