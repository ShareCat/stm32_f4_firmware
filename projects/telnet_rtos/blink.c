#include <stdint.h>
#include "cmsis_os.h"
#include "event.h"
#include "leds.h"

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

// A blink event occured.
static void blink_event(void)
{
  // Update the blink counter.
  ++blink_counter;

  // Blink the LEDs.
  leds_set_value(LED_GREEN, blink_counter & 1 ? 1 : 0);
  leds_set_value(LED_RED, blink_counter & 1 ? 0 : 1);
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

