#include <stdio.h>
#include "cmsis_os.h"
#include "stm32f4xx.h"
#include "event.h"
#include "leds.h"
#include "uart.h"

static void blink_event(void);
static void blink_timer_handler(void const * p_context);
static osTimerId blink_timer_id;
osTimerDef(blink_timer, blink_timer_handler);

static uint32_t blink_counter = 0;

// LED blink timer callback handler.  This is called from the
// context of the timer thread.
static void blink_timer_handler(void const *p_context)
{
  // Schedule a heart beat event.
  event_schedule(blink_event);
}

// Initialize the blink timer.
void blink_init(void)
{
  // Initialize the blink counter.
  blink_counter = 0;

  // Create the blink timer.
  blink_timer_id = osTimerCreate(osTimer(blink_timer), osTimerPeriodic, NULL);

  // Start the blink timer running with a period of 250 ms.
  osTimerStart(blink_timer_id, 250);
}

// A blink event occured.
void blink_event(void)
{
  // Update the blink counter.
  ++blink_counter;

  // Blink the LEDs.
  leds_set_value(LED_GREEN, blink_counter & 1 ? 1 : 0);
  leds_set_value(LED_RED, blink_counter & 1 ? 0 : 1);
}

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

  // Debugging UART initialization.
  uart_debug_init();

  // Initialize the LED blinker.
  blink_init();

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

