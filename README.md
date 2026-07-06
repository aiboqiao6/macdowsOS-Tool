[English](README-English.md) | [简体中文](README-Chinese.md)

<div align="center">

# macdowsOS Tool UI

**Automated Windows-to-macOS theming & beautification tool**

[![Platform](https://img.shields.io/badge/platform-Windows%2010%2F11-blue?style=flat-square&logo=windows)](https://github.com/aiboqiao6/macdowsOS-Tool)
[![Language](https://img.shields.io/badge/language-C%2B%2B20-purple?style=flat-square&logo=c%2B%2B)](https://github.com/aiboqiao6/macdowsOS-Tool)
[![Framework](https://img.shields.io/badge/framework-Qt%206.11%20Quick-41CD52?style=flat-square&logo=qt)](https://github.com/aiboqiao6/macdowsOS-Tool)
[![Version](https://img.shields.io/badge/version-1.30%20RC-orange?style=flat-square)](https://github.com/aiboqiao6/macdowsOS-Tool/releases)

</div>

---

## 目录

- [简介](#简介)
- [提示](#提示)
- [系统支持](#系统支持)
- [免责声明](#免责声明)

---

## 简介

欢迎大家使用 macdowsOStool

使用 C++ 编写的自动化 Windows 11 仿 macOS 工具

原理上使用了 `SendMessage` 函数和其他模拟用户操作的方法实现了自动化操作

---

## 提示

macdowsOS Tool UI 安装功能高度依赖窗口/组件名称查找，请确保系统语言为 **简体·中文**，避免安装时因语言不同导致工具无法正常控制安装程序。目前暂时没有适配其他语言的计划。

> **请务必在安装前创建好还原点，作者不保证安装的所有组件完美兼容所有系统！！**

**如有需要可加入交流群 942644281（QQ）**

某些功能在测试时我可能会代码进行一些调整方便测试，不建议手动构建使用 macdowsOSTool

---

## 系统支持

| 状态 | 操作系统 | 说明 |
|:----:|----------|------|
| ✅ | Windows 11 x64 | 不支持 ARM64 |
| ✅ | Windows 10 x64 | 21H2 及以上版本，不支持 ARM64 和 x32 |
| ❌ | 精简版 Windows | 不支持过度精简的系统 |

---

## 免责声明

> 用户应自行确保所使用的所有组件均符合版权法规。开发者不对用户使用的任何第三方文件的合法性负责，本程序旨在研究或教育用途，不支持盗版行为。用户需遵守软件许可协议。本程序仅用于技术研究、学习或合法授权的软件交互，禁止用于商业侵权用途。开发者不对用户滥用本程序导致的版权纠纷负责。使用本程序即表示用户同意自行承担相关风险。
