#include <stdio.h>

#include "SEGGER_RTT.h"

int fputc(int character, FILE *stream) {
  (void)stream;
  SEGGER_RTT_PutChar(0, (char)character);
  return character;
}
