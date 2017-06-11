#include <stdio.h>
#include "stm32f4xx.h"
#include "leds.h"
#include "uart.h"

static uint32_t blink_counter;

// Initialize the blink data structures.
void blink_init(void)
{
  // Simple initialization of the blink counter.
  blink_counter = 0;
}

// Process one iteration of the LED blinking.
void blink_process(void)
{
  // Update the blink counter.
  ++blink_counter;

  // Blink the LEDs.
  leds_set_value(LED_GREEN, blink_counter & 1 ? 1 : 0);
  leds_set_value(LED_RED, blink_counter & 1 ? 0 : 1);
}

int main(void)
{
  int count = 0;

  // At this stage the microcontroller clock setting is already configured, 
  // this is done through SystemInit() function which is called from startup
  // file (startup_stm32f40_41xxx.s) before to branch to application main. 
  // To reconfigure the default setting of SystemInit() function, refer to
  // system_stm32f4xx.c file.

  // Update the system clocks.
  SystemCoreClockUpdate();

  // LED initialization.
  leds_init();

  // Debugging UART initialization.
  uart_debug_init();

  // Loop forever.
  while (1)
  {
    int i;

    // Waste some time counting.
    for (i = 0; i < 8000000; ++i);

    printf("%d\n", count++);

    // Blink the LEDs.
    blink_process();
  }
}

#ifdef  USE_FULL_ASSERT
// Reports the name of the source file and the source line number
// where the assert_param error has occurred.
void assert_failed(uint8_t* file, uint32_t line)
{ 
  // User can add his own implementation to report the file name and line number,
  // ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line)

  // Infinite loop.
  for (;;);
}
#endif

