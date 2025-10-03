/**
  ******************************************************************************
  * @file    main.c
  * @brief   Main program body
  * @author  STM32 Project
  ******************************************************************************
  * @description
  * This is a complete STM32F103C8T6 example project featuring:
  * - LED blinking on PC13 (onboard LED)
  * - UART1 communication at 115200 baud
  * - System tick timer for precise delays
  * - Peripheral initialization and configuration
  * 
  * Hardware Setup:
  * - PC13: Onboard LED (active low)
  * - PA9:  USART1 TX
  * - PA10: USART1 RX
  * 
  * Clock Configuration:
  * - HSE: 8MHz external crystal
  * - PLL: x9 multiplier
  * - System Clock: 72MHz
  * - APB1: 36MHz
  * - APB2: 72MHz
  ******************************************************************************
  */

#include "stm32f1xx.h"
#include "system_stm32f1xx.h"
#include "gpio.h"
#include "uart.h"
#include "delay.h"

/* Private function prototypes */
static void RCC_Config(void);
static void GPIO_Config(void);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    uint32_t counter = 0;
    
    /* Initialize system */
    SystemInit();
    
    /* Configure peripherals */
    RCC_Config();
    GPIO_Config();
    Delay_Init();
    
    /* Initialize UART1 at 115200 baud */
    UART_Init(USART1, 115200);
    
    /* Send startup message */
    UART_SendString(USART1, "\r\n");
    UART_SendString(USART1, "========================================\r\n");
    UART_SendString(USART1, "  STM32F103C8T6 Complete Project\r\n");
    UART_SendString(USART1, "  System Clock: 72MHz\r\n");
    UART_SendString(USART1, "  UART Baud Rate: 115200\r\n");
    UART_SendString(USART1, "========================================\r\n");
    UART_SendString(USART1, "System initialized successfully!\r\n\r\n");
    
    /* Main loop */
    while(1)
    {
        /* Toggle LED */
        GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        
        /* Send status via UART */
        UART_Printf(USART1, "[%lu] LED Toggle - System Running OK\r\n", counter++);
        
        /* Delay 500ms */
        Delay_Ms(500);
    }
}

/**
  * @brief  Configure RCC (Reset and Clock Control)
  * @param  None
  * @retval None
  */
static void RCC_Config(void)
{
    /* Enable GPIOA clock (for USART1 TX/RX pins) */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    
    /* Enable GPIOC clock (for LED) */
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    
    /* Enable USART1 clock */
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
}

/**
  * @brief  Configure GPIO pins
  * @param  None
  * @retval None
  */
static void GPIO_Config(void)
{
    /* Configure PC13 as output push-pull for LED (50MHz) */
    GPIO_Init(GPIOC, GPIO_PIN_13, GPIO_MODE_OUTPUT_50MHZ, GPIO_CNF_OUTPUT_PP);
    
    /* Configure PA9 (USART1 TX) as alternate function push-pull */
    GPIO_Init(GPIOA, GPIO_PIN_9, GPIO_MODE_OUTPUT_50MHZ, GPIO_CNF_AF_PP);
    
    /* Configure PA10 (USART1 RX) as input floating */
    GPIO_Init(GPIOA, GPIO_PIN_10, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOATING);
    
    /* Turn off LED initially (PC13 is active low) */
    GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Infinite loop for debugging */
    while(1)
    {
        /* Toggle LED rapidly to indicate fault */
        GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        for(volatile uint32_t i = 0; i < 100000; i++);
    }
}

