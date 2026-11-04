# GCC/newlib 示例

适用于 `arm-none-eabi-gcc`、STM32CubeIDE、MCUXpresso 等使用 newlib/newlib-nano 的 Cortex-M 工程。

## 接入步骤

1. 将仓库根目录的 `SEGGER_RTT` 目录复制到工程。
2. 编译 `SEGGER_RTT/SEGGER_RTT.c`。
3. 把 `SEGGER_RTT` 加入编译器头文件路径。
4. 把本目录中的 `main.c` 和 `rtt_retarget.c` 加入工程。
5. 将 `main.c` 中的注释替换为芯片实际初始化和延时函数。

`rtt_retarget.c` 实现 newlib 使用的 `_write()`，因此 `printf()` 最终会调用 `SEGGER_RTT_Write()`。

如果工程已有 `syscalls.c` 或 `_write()`，不要重复编译本文件，只需把已有 `_write()` 改为：

```c
int _write(int file, char *data, int length) {
  (void)file;
  SEGGER_RTT_Write(0, data, (unsigned int)length);
  return length;
}
```

建议链接 newlib-nano，并按项目需要决定是否启用浮点格式化。`%f` 通常需要额外链接选项 `-u _printf_float`，整数和字符串打印不需要。
