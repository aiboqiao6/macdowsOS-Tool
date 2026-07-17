[English Guide](README-English.md) | [简体中文使用文档](README-Chinese.md)

<div align="center">

# macdowsOS Tool UI

用于 Windows 10/11 的自动化桌面美化、组件安装与系统修复工具

[![Platform](https://img.shields.io/badge/Platform-Windows%2010%20%7C%2011-0078D4?style=flat-square&logo=windows)](https://github.com/aiboqiao6/macdowsOS-Tool)
[![Language](https://img.shields.io/badge/Language-C%2B%2B20-00599C?style=flat-square&logo=cplusplus)](https://github.com/aiboqiao6/macdowsOS-Tool)
[![Framework](https://img.shields.io/badge/Framework-Qt%206%20Quick-41CD52?style=flat-square&logo=qt)](https://github.com/aiboqiao6/macdowsOS-Tool)
[![Release](https://img.shields.io/badge/Release-1.30-F5A623?style=flat-square)](https://github.com/aiboqiao6/macdowsOS-Tool/releases)

[下载发布版](https://github.com/aiboqiao6/macdowsOS-Tool/releases) · [中文文档](README-Chinese.md) · [English Guide](README-English.md) · [问题反馈](https://github.com/aiboqiao6/macdowsOS-Tool/issues)

</div>

## 项目简介

macdowsOS Tool UI主要基于自动操作技术实现的全自动Windows仿macOS


## 系统要求

| 状态 | 操作系统 | 架构 | 版本 |
|:---:|---|---|
| ✅ | Windows 11 | x64，不支持 ARM64 | 建议使用24H2及以上版本 |
| ✅ | Windows 10 | x64，不支持ARM64  | 建议使用21H2及以上版本 |
| ❗ | 精简版 Windows | 不支持过度精简或缺失系统组件的版本 | - |

自动操作技术目前依赖于组件标题名匹配 ，当前仅支持 **简体中文**。

## 下载与运行

1. 前往 [Releases](https://github.com/aiboqiao6/macdowsOS-Tool/releases) 下载最新发布包。
2. 完整解压压缩包，不要直接在压缩软件中运行程序。
3. 右键 `macdowsOS Tool UI.exe`，选择 **以管理员身份运行**。
4. 按照界面指引选择需要的美化、安装或修复功能。

> 不建议普通用户直接使用开发分支构建。开发中的自动化步骤可能为测试而临时调整，请优先使用 Releases 页面提供的完整发布包。

## 使用前必读

> [!WARNING]
> 部分功能会修改系统文件、主题和资源管理器组件。请先创建系统还原点并备份重要数据。第三方组件可能与特定 Windows 版本不兼容，所有操作风险由使用者自行承担。

- 必须以管理员身份运行，否则修改系统文件时可能因权限不足而失败。
- 自动化操作可能被安全软件拦截；请先核对发布来源，不要盲目关闭系统安全防护。
- 操作期间不要移动、关闭或手动操作正在被程序控制的安装窗口。
- 如果安装后资源管理器异常，请参阅[中文使用文档](README-Chinese.md#三问题排查)。

## 文档与反馈

- [简体中文使用文档](README-Chinese.md)
- [English User Guide](README-English.md)
- [GitHub Issues](https://github.com/aiboqiao6/macdowsOS-Tool/issues)
- QQ 交流群：`942644281`
- 开发者联系邮箱：`aiboqiao250@outlook.com`

提交问题时，请附上 Windows 版本、程序版本、错误截图以及程序目录中的 `log.txt`（如有）。请勿在公开 Issue 中上传账号、令牌或其他敏感信息。

## 第三方资源与免责声明

项目涉及的部分主题、图标和桌面组件来自第三方，其著作权与许可归原作者所有。用户应自行确认相关资源的授权范围并遵守对应许可协议。本项目仅用于技术研究、学习和合法授权的软件交互，不支持盗版或商业侵权用途。开发者不对第三方文件的合法性、兼容性以及因使用本程序造成的数据丢失、系统异常或版权纠纷承担责任。使用本程序即表示你理解并接受相关风险。如果本项目侵犯到了您的权益 请通过邮箱联系 有关内容将会在核实后删除
