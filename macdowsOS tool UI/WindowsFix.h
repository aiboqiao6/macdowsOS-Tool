#pragma once
#include<windows.h>
#include<bits/stdc++.h>
#include"restart.h"
#include"LogSystem.h"
using namespace std;
void WindowsFix() {
	INFO_(L"[系统修复组件]启动");
	if (MessageBox(NULL, (LPCTSTR)L"点击确定以开始修复系统 修复将会导致几乎全部对于系统文件的修改失效", (LPCTSTR)L" macdowsOS tool 系统修复组件", MB_YESNO)==6) {
		INFO_(L"[系统修复组件]系统修复");
		system("Dism.exe /Online /Cleanup-image /CheckHealth");
		system("DISM.exe /Online /Cleanup-image /Scanhealth");
		system("DISM.exe /Online /Cleanup-image /Restorehealth");
		system("sfc /scannow");
		INFO_(L"[系统修复组件]结束");
		restart_();
	}
	INFO_(L"[系统修复组件]退出");
	return;
}
void WindowsFix_nowindow() {
	INFO_(L"[系统修复组件]系统修复");
	system("Dism.exe /Online /Cleanup-image /CheckHealth");
	system("DISM.exe /Online /Cleanup-image /Scanhealth");
	system("DISM.exe /Online /Cleanup-image /Restorehealth");
	system("sfc /scannow");
	INFO_(L"[系统修复组件]结束");
	return;
}