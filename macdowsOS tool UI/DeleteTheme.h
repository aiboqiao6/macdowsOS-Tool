#pragma once
#include<iostream>
#include<cstdio>
#include <string.h>
#include<Windows.h>
#include <filesystem>
#include"LogSystem.h"
using namespace std;
void Delete_theme(){
    INFO_(L"[主题删除组件]启动组件");
    filesystem::remove_all("C:\\Windows\\Resources\\Themes\\Big Sur"); INFO_(L"[主题删除组件]删除Big Sur");
    filesystem::remove_all("C:\\Windows\\Resources\\Themes\\Tahoe"); INFO_(L"[主题删除组件]删除Tahoe");
    filesystem::remove_all("C:\\Windows\\Resources\\Themes\\Big Sur Dark Mode.theme"); INFO_(L"[主题删除组件]删除Big Sur Dark Mode.theme");
    filesystem::remove_all("C:\\Windows\\Resources\\Themes\\Big Sur Day.theme"); INFO_(L"[主题删除组件]删除Big Sur Day.theme");
    filesystem::remove_all("C:\\Windows\\Resources\\Themes\\Big Sur Night.theme"); INFO_(L"[主题删除组件]删除Big Sur Night.theme");
    filesystem::remove_all("C:\\Windows\\Resources\\Themes\\Tahoe - Light.theme"); INFO_(L"[主题删除组件]删除Tahoe - Light.theme");
    filesystem::remove_all("C:\\Windows\\Resources\\Themes\\Tahoe - Night.theme"); INFO_(L"[主题删除组件]删除Tahoe - Night.theme");
    return;
}