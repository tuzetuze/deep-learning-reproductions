/**
  ******************************************************************************
  * @file    uart.h
  * @brief   UART driver header file
  ******************************************************************************
  */

#ifndef __UART_H
#define __UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx.h"

/* USART Status Register bits */
#define USART_SR_TXE    (1 << 7)  /* Transmit data register empty */
#define USART_SR_RXNE   (1 << 5)  /* Read data register not empty */
#define USART_SR_TC     (1 << 6)  /* Transmission complete */

/* USART Control Register 1 bits */
#define USART_CR1_UE    (1 << 13) /* USART Enable */
#define USART_CR1_TE    (1 << 3)  /* Transmitter Enable */
#define USART_CR1_RE    (1 << 2)  /* Receiver Enable */

/* Function prototypes */
void UART_Init(USART_TypeDef *USARTx, uint32_t baudrate);
void UART_SendChar(USART_TypeDef *USARTx, char ch);
void UART_SendString(USART_TypeDef *USARTx, const char *str);
char UART_ReceiveChar(USART_TypeDef *USARTx);
void UART_Printf(USART_TypeDef *USARTx, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif /* __UART_H */

