#include <stdio.h>
#include "stm32f4xx.h"
#include "uart.h"

// UART Hardware Configuration
//
// GPIO Inputs
//
// PC6         DBG_TX
// PC7         DBG_RX

// Initialize UART resources for debugging.
void uart_debug_init(void)
{
  GPIO_InitTypeDef gpio_init_struct;
  USART_InitTypeDef usart_init_struct;

  // Enable peripheral clocks.
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

  // Connect GPIO pins to USART peripheral.
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

  // Configure GPIO pin for USART TX.
  gpio_init_struct.GPIO_Pin = GPIO_Pin_6;
  gpio_init_struct.GPIO_OType = GPIO_OType_PP;
  gpio_init_struct.GPIO_PuPd = GPIO_PuPd_UP;
  gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOC, &gpio_init_struct);

  // Configure GPIO pin for USART RX.
  gpio_init_struct.GPIO_Pin = GPIO_Pin_7;
  gpio_init_struct.GPIO_OType = GPIO_OType_PP;
  gpio_init_struct.GPIO_PuPd = GPIO_PuPd_UP;
  gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOC, &gpio_init_struct);

  // USARTx configured as follows: 
  // 115200 baud, 8 Bits, 1 Stop, No Parity, 
  // No Flow Control, Receive and Transmit Enabled.
  USART_StructInit(&usart_init_struct);
  usart_init_struct.USART_BaudRate = 115200;
  usart_init_struct.USART_WordLength = USART_WordLength_8b;
  usart_init_struct.USART_StopBits = USART_StopBits_1;
  usart_init_struct.USART_Parity = USART_Parity_No;
  usart_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  usart_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART6, &usart_init_struct);

  // Enable USART.
  USART_Cmd(USART6, ENABLE);
}

// Retargets the C library printf function to the USART.
int fputc(int ch, FILE *f)
{
  // Send a CRLF for each LF.
  if (ch == '\n')
  {
    // Send the character.
    USART_SendData(USART6, (uint8_t) '\r');

    // Loop until the end of transmission.
    while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);
  }

  // Send the character.
  USART_SendData(USART6, (uint8_t) ch);

  // Loop until the end of transmission.
  while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);

  return ch;
}

int fgetc(FILE *f)
{
  return 0;
}

