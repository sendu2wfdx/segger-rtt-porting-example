#include "SEGGER_RTT.h"

int _write(int file, char *data, int length) {
  (void)file;

  if ((data == 0) || (length <= 0)) {
    return 0;
  }

  SEGGER_RTT_Write(0, data, (unsigned int)length);
  return length;
}
