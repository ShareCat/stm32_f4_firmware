#ifndef __LEDS_H__
#define __LEDS_H__

#include <stdint.h>

enum
{
  LED_GREEN = 0,
  LED_ORANGE,
  LED_RED,
  LED_BLUE,
  LED_COUNT
};


void leds_init(void);
void leds_set_value(uint32_t led_index, uint32_t led_value);

#endif /* __LEDS_H__ */
