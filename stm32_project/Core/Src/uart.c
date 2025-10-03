/**
  ******************************************************************************
  * @file    uart.c
  * @brief   UART driver implementation
  ******************************************************************************
  */

#include "uart.h"
#include "system_stm32f1xx.h"
#include <stdarg.h>
#include <stdio.h>

/**
  * @brief  Initialize UART
  * @param  USARTx: USART peripheral (USART1, USART2)
  * @param  baudrate: Baud rate (e.g., 9600, 115200)
  * @retval None
  */
void UART_Init(USART_TypeDef *USARTx, uint32_t baudrate)
{
    uint32_t apbclock;
    uint32_t mantissa;
    uint32_t fraction;
    
    /* Determine APB clock frequency */
    if(USARTx == USART1)
    {
        apbclock = SystemCoreClock; /* APB2 = 72MHz */
    }
    else
    {
        apbclock = SystemCoreClock / 2; /* APB1 = 36MHz */
    }
    
    /* Calculate baud rate register value */
    uint32_t divider = (apbclock * 25) / (4 * baudrate);
    mantissa = divider / 100;
    fraction = ((divider - (mantissa * 100)) * 16 + 50) / 100;
    
    USARTx->BRR = (mantissa << 4) | (fraction & 0x0F);
    
    /* Enable USART, Transmitter and Receiver */
    USARTx->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}

/**
  * @brief  Send a character via UART
  * @param  USARTx: USART peripheral
  * @param  ch: Character to send
  * @retval None
  */
void UART_SendChar(USART_TypeDef *USARTx, char ch)
{
    /* Wait until transmit data register is empty */
    while(!(USARTx->SR & USART_SR_TXE));
    
    /* Send data */
    USARTx->DR = ch;
    
    /* Wait for transmission to complete */
    while(!(USARTx->SR & USART_SR_TC));
}

/**
  * @brief  Send a string via UART
  * @param  USARTx: USART peripheral
  * @param  str: String to send
  * @retval None
  */
void UART_SendString(USART_TypeDef *USARTx, const char *str)
{
    while(*str)
    {
        UART_SendChar(USARTx, *str++);
    }
}

/**
  * @brief  Receive a character via UART
  * @param  USARTx: USART peripheral
  * @retval Received character
  */
char UART_ReceiveChar(USART_TypeDef *USARTx)
{
    /* Wait until data is received */
    while(!(USARTx->SR & USART_SR_RXNE));
    
    /* Read and return data */
    return (char)(USARTx->DR & 0xFF);
}

/**
  * @brief  Printf-style UART output
  * @param  USARTx: USART peripheral
  * @param  format: Format string
  * @retval None
  */
void UART_Printf(USART_TypeDef *USARTx, const char *format, ...)
{
    char buffer[128];
    va_list args;
    
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    UART_SendString(USARTx, buffer);
}

