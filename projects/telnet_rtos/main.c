#include "cmsis_os.h"
#include "stm32f4x7_eth.h"
#include "netconf.h"
#include "main.h"
#include "event.h"
#include "leds.h"
#include "blink.h"
#include "uart.h"
#include "tcpip.h"
#include "telnet.h"

// LCD Messages
#define MESSAGE1   "     STM32F4x7      "
#define MESSAGE2   " STM32F-4 Discovery "
#define MESSAGE3   " Telnet Shell Demo  "
#define MESSAGE4   "                    "

// Tasks Priority
#define DHCP_TASK_PRIO   (osPriorityIdle + 2)

void LCD_Init(void);

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

  // Initialize the LED blinker.
  blink_init();

  // UART initialization.
  uart_init();

  // Initialize LCD.
  LCD_Init();

  // Configure ethernet (GPIOs, clocks, MAC, DMA).
  ETH_BSP_Config();

  // Initilaize the LwIP stack.
  LwIP_Init();

  // Initialize telnet shell server.
  telnet_shell_init();

#if USE_DHCP
  // Start DHCP client.
  sys_thread_new("DHCP", LwIP_DHCP_task, NULL, DEFAULT_THREAD_STACKSIZE, DHCP_TASK_PRIO);
#endif

  // Process events.
  event_loop();
}

void LCD_Init(void)
{
#if USE_LCD
  // Initialize the STM324xG-EVAL's LCD.
  STM32f4_Discovery_LCD_Init();

  // Clear the LCD.
  LCD_Clear(Black);

  // Set the LCD Back Color.
  LCD_SetBackColor(Black);

  // Set the LCD Text Color.
  LCD_SetTextColor(White);

  // Display message on the LCD.
  LCD_DisplayStringLine(Line0, (uint8_t *) MESSAGE1);
  LCD_DisplayStringLine(Line1, (uint8_t *) MESSAGE2);
  LCD_DisplayStringLine(Line2, (uint8_t *) MESSAGE3);
  LCD_DisplayStringLine(Line3, (uint8_t *) MESSAGE4); 
#endif
}

#ifdef  USE_FULL_ASSERT
// Reports the name of the source file and the source line number
// where the assert_param error has occurred.
void assert_failed(uint8_t* file, uint32_t line)
{
  // User can add his own implementation to report the file name and line number,
  // ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  for (;;);
}
#endif

/*********** Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
