#pragma once
// ============================================================
// WindowsFix.h
// 描述：Windows 系统修复工具，提供系统文件检查与修复功能
//      包括使用 DISM 和 SFC 命令修复系统文件的函数
// ============================================================
#include<windows.h>
#include"restart.h"
#include"LogSystem.h"

namespace {
void WindowsFix() {
	INFO_(L"[系统修复工具]进入");
	if (MessageBox(NULL, (LPCTSTR)L"执行修复后将会导致已安装的部分美化软件失效 ", (LPCTSTR)L" macdowsOS tool 系统修复工具", MB_YESNO)==6) {
		INFO_(L"[系统修复工具]系统修复");
		system("Dism.exe /Online /Cleanup-image /CheckHealth");
		system("DISM.exe /Online /Cleanup-image /Scanhealth");
		system("DISM.exe /Online /Cleanup-image /Restorehealth");
		system("sfc /scannow");
		INFO_(L"[系统修复工具]完成");
		restart_();
	}
	INFO_(L"[系统修复工具]退出");
	return;
}

void WindowsFix_nowindow() {
	INFO_(L"[系统修复工具]系统修复");
	system("Dism.exe /Online /Cleanup-image /CheckHealth");
	system("DISM.exe /Online /Cleanup-image /Scanhealth");
	system("DISM.exe /Online /Cleanup-image /Restorehealth");
	system("sfc /scannow");
	INFO_(L"[系统修复工具]完成");
	return;
}

}
