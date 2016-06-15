#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "cmsis_os.h"
#include "stm32f4xx.h"
#include "uart.h"

// UART Hardware Configuration
//
// GPIO Inputs
//
// PC6         SERIAL_TX
// PC7         SERIAL_RX

#define USART_QUEUE_SIZE         32

// Serial send and receive queues.
osMessageQDef(usart_send_queue, USART_QUEUE_SIZE, uint32_t);
osMessageQDef(usart_recv_queue, USART_QUEUE_SIZE, uint32_t);
osMessageQId usart_send_queue;
osMessageQId usart_recv_queue;

// @brief  Checks whether the specified USART interrupt has occurred or not.
// @param  USARTx: where x can be 1, 2, 3, 4, 5 or 6 to select the USART or 
//         UART peripheral.
// @param  USART_IT: specifies the USART interrupt sources to be checked.
//          This parameter can be one of the following values:
//            @arg USART_IT_CTS:  CTS change interrupt
//            @arg USART_IT_LBD:  LIN Break detection interrupt
//            @arg USART_IT_TXE:  Transmit Data Register empty interrupt
//            @arg USART_IT_TC:   Transmission complete interrupt
//            @arg USART_IT_RXNE: Receive Data register not empty interrupt
//            @arg USART_IT_IDLE: Idle line detection interrupt
//            @arg USART_IT_PE:   Parity Error interrupt
//            @arg USART_IT_ERR:  Error interrupt(Frame error, noise error, overrun error)
// @retval The state of USART_IT (SET or RESET).
ITStatus USART_GetITConfig(USART_TypeDef* USARTx, uint16_t USART_IT)
{
  uint32_t usartreg;
  uint32_t itmask;
  uint32_t usartxbase;
  ITStatus bitstatus = RESET;

  // Check the parameters.
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_CONFIG_IT(USART_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  // The CTS interrupt is not available for UART4 and UART5.
  if (USART_IT == USART_IT_CTS)
  {
    assert_param(IS_USART_1236_PERIPH(USARTx));
  }

  // Get the USART register base.
  usartxbase = (uint32_t) USARTx;

  // Get the USART register index.
  usartreg = (((uint8_t) USART_IT) >> 0x05);

  // Get the interrupt position.
  itmask = (((uint32_t) 0x01) << (USART_IT & ((uint16_t) 0x001F)));

  // Adjust the base to specific register.
  if (usartreg == 0x01)
  {
    // The IT is in CR1 register.
    usartxbase += 0x0C;
  }
  else if (usartreg == 0x02)
  {
    // The IT is in CR2 register.
    usartxbase += 0x10;
  }
  else
  {
    // The IT is in CR3 register.
    usartxbase += 0x14; 
  }

  if (*(__IO uint32_t*)usartxbase & itmask)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }

  return bitstatus;  
}

// Interrupt handler for USART6 port.
void USART6_IRQHandler(void)
{
  uint32_t ch;
  osEvent event;
  bool recv_error;

  // Check if the USART6 transmit empty flag is set.
  if (USART_GetITConfig(USART6, USART_IT_TXE) && USART_GetITStatus(USART6, USART_IT_TXE))
  {
    // Get the next character from the send queue.
    event = osMessageGet(usart_send_queue, 0);

    // Do we have a character to send?
    if (event.status == osEventMessage)
    {
      // Send the character which will clear the interrupt.
      USART_SendData(USART6, (uint16_t) (uint32_t) event.value.p);
    }
    else
    {
      // Disable the transmit empty interrupt.
      USART_ITConfig(USART6, USART_IT_TXE, DISABLE);
    }
  }

  // Check if the USART6 receive not empty flag is set.
  if (USART_GetITConfig(USART6, USART_IT_RXNE) && USART_GetITStatus(USART6, USART_IT_RXNE))
  {
    // Check for various receive errors.
    recv_error = USART_GetFlagStatus(USART6, USART_FLAG_PE) != RESET ? true : false;
    recv_error |= USART_GetFlagStatus(USART6, USART_FLAG_FE) != RESET ? true : false;
    recv_error |= USART_GetFlagStatus(USART6, USART_FLAG_NE) != RESET ? true : false;
    recv_error |= USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET ? true : false;

    // Read from the data register will clear the interrupt.
    ch = (uint32_t) USART_ReceiveData(USART6);

    // Do we have a receive error?
    if (!recv_error)
    {
      // No. Insert the character into the receive queue.
      osMessagePut(usart_recv_queue, ch, 0);
    }
    else
    {
      // Yes. Reset any error flags.
      USART_ClearFlag(USART6, USART_FLAG_FE | USART_FLAG_PE | USART_FLAG_NE | USART_FLAG_ORE);
    }
  }
}


// Initialize UART peripheral.
void uart_init(void)
{
  NVIC_InitTypeDef nvic_init;
  GPIO_InitTypeDef gpio_init;
  USART_InitTypeDef usart_init;

  // Create the serial send and receive queues for the USART port.
  usart_send_queue = osMessageCreate(osMessageQ(usart_send_queue), NULL);
  usart_recv_queue = osMessageCreate(osMessageQ(usart_recv_queue), NULL);
  if ((usart_send_queue == NULL) || (usart_recv_queue == NULL))
  {
    // XXX Handle the error.
  }

  // Enable peripheral clocks.
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

  // Connect GPIO pins to USART peripheral.
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

  // Configure GPIO pin for USART TX.
  gpio_init.GPIO_Pin = GPIO_Pin_6;
  gpio_init.GPIO_OType = GPIO_OType_PP;
  gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
  gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_init.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOC, &gpio_init);

  // Configure GPIO pin for USART RX.
  gpio_init.GPIO_Pin = GPIO_Pin_7;
  gpio_init.GPIO_OType = GPIO_OType_PP;
  gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
  gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_init.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOC, &gpio_init);

  // USARTx configured as follows: 
  // 115200 baud, 8 Bits, 1 Stop, No Parity, 
  // No Flow Control, Receive and Transmit Enabled.
  USART_StructInit(&usart_init);
  usart_init.USART_BaudRate = 115200;
  usart_init.USART_WordLength = USART_WordLength_8b;
  usart_init.USART_StopBits = USART_StopBits_1;
  usart_init.USART_Parity = USART_Parity_No;
  usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  usart_init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART6, &usart_init);

  // Disable USART interrupts for now.
  USART_ITConfig(USART6, USART_IT_CTS, DISABLE);
  USART_ITConfig(USART6, USART_IT_LBD, DISABLE);
  USART_ITConfig(USART6, USART_IT_TXE, DISABLE);
  USART_ITConfig(USART6, USART_IT_TC, DISABLE);
  USART_ITConfig(USART6, USART_IT_RXNE, DISABLE);
  USART_ITConfig(USART6, USART_IT_IDLE, DISABLE);
  USART_ITConfig(USART6, USART_IT_PE, DISABLE);
  USART_ITConfig(USART6, USART_IT_ERR, DISABLE);

  // Configure USART interrupt.
  memset(&nvic_init, 0, sizeof(nvic_init));
  nvic_init.NVIC_IRQChannel = USART6_IRQn;
  nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
  nvic_init.NVIC_IRQChannelSubPriority = 0;
  nvic_init.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic_init);

  // Enable the USART.
  USART_Cmd(USART6, ENABLE);

  // Enable error interrupt.
  USART_ITConfig(USART6, USART_IT_ERR, ENABLE);

  // Enable receiver not empty interrupt.
  USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
}

// Send standard output to the USART.
int fputc(int ch, FILE *f)
{
  // Call recursively to send return character preceedign each newline character.
  if (ch == '\n') fputc('\r', f);

  // Write the byte to send into the send queue.
  osMessagePut(usart_send_queue, (uint32_t) ch, osWaitForever);

  // Enable the transmit empty flag.
  USART_ITConfig(USART6, USART_IT_TXE, ENABLE);

  // No error so return the same character written.
  return ch;
}

// Receive standard input from the USART.
int fgetc(FILE *f)
{
  osEvent event;
  bool done = false;

  // Keep going until we receive a character.
  while (!done)
  {
    // Wait to get the next character from the receive queue.
    event = osMessageGet(usart_recv_queue, osWaitForever);

    // Did we actually receive a character?
    done = (event.status == osEventMessage) ? true : false;
  }

  // Return the character received.
  return (int) event.value.p;
}

