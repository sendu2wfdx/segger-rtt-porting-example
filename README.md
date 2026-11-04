# SEGGER RTT 移植示例

这是一个面向裸机 Cortex-M 工程的 SEGGER Real Time Transfer（RTT）最小移植示例，包含：

- SEGGER RTT 7.50 核心源码
- 不依赖芯片厂商 HAL 的最小输出示例
- GCC/newlib 的完整示例与 `printf()` 重定向
- Keil MicroLIB 的完整示例与 `printf()` 重定向
- SEGGER Embedded Studio 8.30b 原生 RTT Library I/O 示例
- CMake 集成示例

RTT 通过调试器直接访问目标 RAM，在不占用 UART 的情况下实现高速日志输出。目标板需要使用 J-Link 连接。

## 目录结构

```text
.
├── SEGGER_RTT/
│   ├── SEGGER_RTT.c
│   ├── SEGGER_RTT.h
│   └── SEGGER_RTT_Conf.h
├── examples/
│   ├── basic/main.c
│   ├── gcc/
│   │   ├── README.md
│   │   ├── main.c
│   │   └── rtt_retarget.c
│   ├── keil/
│   │   ├── README.md
│   │   ├── main.c
│   │   └── rtt_retarget.c
│   └── ses/
│       ├── README.md
│       └── main.c
├── CMakeLists.txt
└── NOTICE.md
```

## 最小移植步骤

### 1. 添加源文件

把 `SEGGER_RTT` 目录复制到 MCU 工程中：

- 将 `SEGGER_RTT/SEGGER_RTT.c` 加入编译。
- 将 `SEGGER_RTT` 加入头文件搜索路径。
- 在需要输出日志的文件中包含 `SEGGER_RTT.h`。

### 2. 输出第一条日志

```c
#include "SEGGER_RTT.h"

int main(void) {
  /* 在这里完成芯片、时钟和外设初始化。 */
  SEGGER_RTT_Init();
  SEGGER_RTT_WriteString(0, "Hello SEGGER RTT!\r\n");

  while (1) {
  }
}
```

`SEGGER_RTT_Init()` 可以显式调用，便于清晰表达初始化时机。第一次调用 RTT API 时，库也会初始化控制块。

## 三种工具链示例

| 工具链 | 示例 | `printf()` 接入方式 |
| --- | --- | --- |
| GCC/newlib | [`examples/gcc`](examples/gcc) | 实现 `_write()` |
| Keil + MicroLIB | [`examples/keil`](examples/keil) | 实现 `fputc()` |
| SEGGER Embedded Studio 8.30b | [`examples/ses`](examples/ses) | `Library I/O = RTT` |

### GCC/newlib

将 [`examples/gcc/rtt_retarget.c`](examples/gcc/rtt_retarget.c) 加入工程，然后直接使用：

```c
printf("value = %lu\r\n", (unsigned long)value);
```

如果工程已有 `syscalls.c` 或 `_write()`，不要重复加入本例；将原 `_write()` 的输出实现改为 `SEGGER_RTT_Write()` 即可。

完整步骤见 [`examples/gcc/README.md`](examples/gcc/README.md)。

### Keil MicroLIB

启用：

```text
Options for Target -> Target -> Use MicroLIB
```

将 [`examples/keil/rtt_retarget.c`](examples/keil/rtt_retarget.c) 加入工程。这个示例通过 `fputc()` 逐字符输出，简单但不适合高频大日志；高吞吐量场景应直接调用 `SEGGER_RTT_Write()`。

完整步骤见 [`examples/keil/README.md`](examples/keil/README.md)。

### SEGGER Embedded Studio

SES 自带 RTT 版标准库 I/O。在工程选项中设置：

```text
Code -> Library -> Library I/O = RTT
```

对应工程属性为：

```xml
LIBRARY_IO_TYPE="RTT"
```

随后直接调用 `printf()`，不需要另写 `_write()` 或 `fputc()`。使用 SES 原生 RTT Library I/O 时，不要再把本仓库的 `SEGGER_RTT.c` 加入同一目标，否则可能产生重复符号。完整步骤见 [`examples/ses/README.md`](examples/ses/README.md)。

## CMake 集成

可以直接将本仓库作为子目录：

```cmake
add_subdirectory(path/to/segger-rtt-porting-example)
target_link_libraries(your_firmware PRIVATE segger_rtt)
```

如果需要 GCC `printf()` 重定向：

```cmake
target_sources(your_firmware PRIVATE
    path/to/segger-rtt-porting-example/examples/gcc/rtt_retarget.c
)
```

## J-Link RTT Viewer 设置

1. 用 J-Link 连接目标板。
2. 打开 J-Link RTT Viewer。
3. 选择实际 MCU 型号和 SWD 接口。
4. RTT Control Block 使用自动搜索。
5. 连接后查看 Terminal 0。

RTT 没有串口波特率。如果输出异常，优先检查目标芯片、SWD 速度、程序是否运行，以及 RTT 核心是否真正参与链接。GCC/Keil 手工移植时检查 `SEGGER_RTT.c`；SES 原生模式检查 `Library I/O = RTT`。

## 缓冲区配置

默认配置在 `SEGGER_RTT/SEGGER_RTT_Conf.h`：

```c
#define BUFFER_SIZE_UP    (1024)
#define BUFFER_SIZE_DOWN  (16)
```

日志量较大时，可以把 `BUFFER_SIZE_UP` 增加到 4096 或更大。一般建议使用非阻塞输出模式，避免未连接主机时阻塞 MCU。

## 常见问题

### RTT Viewer 没有输出

- GCC/Keil 手工移植时，确认 `SEGGER_RTT.c` 已加入编译和链接；SES 原生模式确认 `Library I/O = RTT`。
- 确认程序执行到了日志调用。
- 确认 J-Link 中选择了正确的芯片型号和接口。
- 降低 SWD 时钟后重试。
- 暂时关闭低功耗、休眠以及可能停止调试端口的代码。

### `_write` 重复定义

工程已经提供了 newlib 系统调用。保留原文件，只修改已有的 `_write()`，不要同时编译本仓库的 GCC 重定向文件。

### 程序因日志卡住

检查 `SEGGER_RTT_Conf.h` 中的默认模式，避免使用阻塞输出；不要在高频中断中大量调用格式化 `printf()`。

## 版权说明

`SEGGER_RTT` 目录中的源码版权属于 SEGGER Microcontroller GmbH，并保留其原始版权和许可声明。示例代码仅演示集成方式，详情见 [NOTICE.md](NOTICE.md)。
