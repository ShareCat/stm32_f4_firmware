#include <stdio.h>
#include "cmsis_os.h"
#include "stm32f4xx.h"
#include "event.h"
#include "leds.h"
#include "blink.h"
#include "uart.h"
#include "shell.h"

int main(void)
{
  // At this stage the microcontroller clock setting is already configured, 
  // this is done through SystemInit() function which is called from startup
  // file (startup_stm32f40_41xxx.s) before to branch to application main. 
  // To reconfigure the default setting of SystemInit() function, refer to
  // system_stm32f4xx.c file.

  // Update the system clocks.
  SystemCoreClockUpdate();

  // Event initialization.
  event_init();

  // LED initialization.
  leds_init();

  // UART initialization.
  uart_init();

  // Initialize the LED blinker.
  blink_init();

  // Initialize the interactive shell.
  shell_init();

  // Process events.
  event_loop();
}

#ifdef  USE_FULL_ASSERT
// Reports the name of the source file and the source line number
// where the assert_param error has occurred.
void assert_failed(uint8_t* file, uint32_t line)
{ 
  // User can add his own implementation to report the file name and line number,
  // ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  // Infinite loop.
  while (1)
  {
  }
}
#endif

