# Keil MDK 示例

这个示例面向 Keil MDK 的 MicroLIB，适用于常见 Cortex-M 工程。

## 接入步骤

1. 将仓库根目录的 `SEGGER_RTT` 目录复制到工程。
2. 在 Keil Project 窗口中加入 `SEGGER_RTT.c`。
3. 在 `Options for Target -> C/C++ -> Include Paths` 中加入 `SEGGER_RTT`。
4. 在 `Options for Target -> Target` 中启用 `Use MicroLIB`。
5. 把本目录中的 `main.c` 和 `rtt_retarget.c` 加入工程。
6. 将 `main.c` 中的注释替换为芯片实际初始化和延时函数。

`rtt_retarget.c` 通过实现 `fputc()` 把 `printf()` 的每个字符送入 RTT Terminal 0。

这种方式便于移植，但逐字符输出效率低。高频或大块日志建议直接使用：

```c
SEGGER_RTT_Write(0, data, length);
```

如果工程已经实现了 `fputc()`，应修改原实现，不要同时编译两个同名函数。
