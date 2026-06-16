# ld_ncm

基于 Qt/C++ 开发的网易云音乐桌面客户端。

## 技术栈

- **Qt 框架** (Core, GUI, Widgets, Network, Multimedia)
- **C++11**
- **OpenSSL** 用于 HTTPS 支持

## 功能

- 网易云账号二维码登录
- 音乐搜索
- 音乐播放与进度控制
- 用户信息展示

## 项目结构

| 文件 | 说明 |
|------|------|
| `main.cpp` | 程序入口 |
| `mainwindow.{h,cpp,ui}` | 主窗口，包含搜索、播放和登录功能 |
| `ncm_login.{h,cpp,ui}` | 二维码登录对话框 |
| `network.{h,cpp}` | 网易云后端 API 封装 |
| `httpclient.{h,cpp}` | 底层 HTTP 客户端 |
| `userdata.{h,cpp}` | 用户数据管理（Cookie、个人信息） |
| `music_list_item.{h,cpp,ui}` | 音乐列表项组件 |
| `common.{h,cpp}` | 通用工具函数 |

## 构建

### 环境要求

- Qt 5.x（需包含 Widgets、Network、Multimedia 模块）
- Qt Creator 或 qmake
- OpenSSL 1.1.x（已附带 DLL：`libssl-1_1-x64.dll`、`libcrypto-1_1-x64.dll`）

### 编译步骤

```bash
qmake ld_ncm.pro
make
```

或直接使用 Qt Creator 打开 `ld_ncm.pro` 进行构建。

## API 配置

程序连接至网易云后端 API：
- 默认 API 地址：`https://ncm-api.prod.gbclstudio.cn`
- 可通过 `network::setApi_url()` 自定义

## 许可

本项目仅供学习使用。
