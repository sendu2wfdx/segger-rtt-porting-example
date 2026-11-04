#include "SEGGER_RTT.h"

int main(void) {
  /* Add the target-specific clock and peripheral initialization here. */
  SEGGER_RTT_Init();
  SEGGER_RTT_WriteString(0, "RTT started!\r\n");

  while (1) {
    volatile unsigned int delay;

    SEGGER_RTT_WriteString(0, "RTT is alive.\r\n");
    for (delay = 0; delay < 1000000U; ++delay) {
      /* Demonstration delay only. Replace it with the platform delay API. */
    }
  }
}
