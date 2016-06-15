// Main Interrupt Service Routines.
// This file provides template for all exceptions handler and 
// peripherals interrupt service routine.

#include "stm32f4xx_it.h"

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

// This function handles NMI exception.
void NMI_Handler(void)
{
}

// This function handles Hard Fault exception.
void HardFault_Handler(void)
{
  // Go to infinite loop when Hard Fault exception occurs.
  while (1)
  {
  }
}

// This function handles Memory Manage exception.
void MemManage_Handler(void)
{
  // Go to infinite loop when Memory Manage exception occurs.
  while (1)
  {
  }
}

// This function handles Bus Fault exception.
void BusFault_Handler(void)
{
  // Go to infinite loop when Bus Fault exception occurs.
  while (1)
  {
  }
}

// This function handles Usage Fault exception.
void UsageFault_Handler(void)
{
  // Go to infinite loop when Usage Fault exception occurs.
  while (1)
  {
  }
}

// This function handles Debug Monitor exception.
void DebugMon_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f40xx.s/startup_stm32f427x.s).                         */
/******************************************************************************/

// This function handles PPP interrupt request.
/*void PPP_IRQHandler(void)
{
}*/
