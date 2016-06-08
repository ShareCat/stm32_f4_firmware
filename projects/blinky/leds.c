#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "leds.h"

// LED Hardware Configuration
//
// GPIO Outputs
//
// PD12    LED_GREEN
// PD13    LED_ORANGE
// PD14    LED_RED
// PD15    LED_BLUE

// Pins used to set LEDs on and off.
static const uint16_t led_output_pins[LED_COUNT] = 
{
  GPIO_Pin_12,
  GPIO_Pin_13,
  GPIO_Pin_14,
  GPIO_Pin_15
};

void leds_init(void)
{
  GPIO_InitTypeDef gpio_init;

  // Enable LED related peripheral clocks.
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  // Initialize LEDs to off.
  GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

  // Configure GPIOD for output.
  memset(&gpio_init, 0, sizeof(gpio_init));
  gpio_init.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  gpio_init.GPIO_Mode = GPIO_Mode_OUT;
  gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_init.GPIO_OType = GPIO_OType_PP;
  gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &gpio_init);
}

void leds_set_value(uint32_t led_index, uint32_t led_value)
{
  // Sanity check argument.
  if (led_index >= LED_COUNT) return;

  // Set the LED value.
  GPIO_WriteBit(GPIOD, led_output_pins[led_index], led_value ? Bit_SET : Bit_RESET);
}

