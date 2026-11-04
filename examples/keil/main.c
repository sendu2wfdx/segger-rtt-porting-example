#include <stdio.h>

#include "SEGGER_RTT.h"

static void demo_delay(void) {
  volatile unsigned int i;

  for (i = 0; i < 1000000U; ++i) {
    /* Replace this demonstration loop with the platform delay API. */
  }
}

int main(void) {
  unsigned long count = 0;

  /* Add the target-specific clock and peripheral initialization here. */
  SEGGER_RTT_Init();
  printf("Keil RTT example started.\r\n");

  while (1) {
    printf("Keil count = %lu\r\n", count++);
    demo_delay();
  }
}
