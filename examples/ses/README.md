# SEGGER Embedded Studio 示例

这个示例按 SEGGER Embedded Studio 8.30b 的 SEGGER RTL 配置编写。SES 已内置 RTT 版标准库 I/O，通常不需要自定义 `_write()` 或 `fputc()`。

## 推荐方式：使用 SES 原生 Library I/O

1. 打开项目的 `Options`。
2. 选择当前构建配置，例如 `Common`、`Debug` 或 `Release`。
3. 进入 `Code -> Library`。
4. 将 `Library I/O` 设置为 `RTT`。
5. 编译并通过 J-Link 启动程序。
6. 打开 SES 的 Debug Terminal 或 J-Link RTT Viewer 查看 Terminal 0。

这个 GUI 选项对应 `.emProject` 中的属性：

```xml
<configuration Name="Common" LIBRARY_IO_TYPE="RTT" />
```

项目已有其他属性时，把 `LIBRARY_IO_TYPE="RTT"` 合并到现有 `<configuration>`，不要创建重复配置。

## 源文件选择

使用 `Library I/O = RTT` 时，SES 会链接自带的 RTT I/O 实现及 RTT 核心，因此：

- `main.c` 只需包含 `<stdio.h>` 并调用 `printf()`。
- 不需要 `rtt_retarget.c`。
- 不要再编译仓库根目录的 `SEGGER_RTT.c`，避免重复符号。

如果希望完全脱离 SES 内置运行库，才采用仓库根目录的 RTT 核心源码并自行实现与所选 C 库匹配的重定向层。

## 直接调用 RTT API

SES 安装目录自带头文件，可使用：

```c
#include <RTT/SEGGER_RTT.h>

SEGGER_RTT_WriteString(0, "Direct RTT output.\r\n");
```

但仅使用标准 `printf()` 时，本示例不要求显式调用 RTT API。
