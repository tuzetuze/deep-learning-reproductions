/**
  ******************************************************************************
  * @file    uart_echo.c
  * @brief   UART 回显示例程序
  * @note    这是一个独立的示例，展示如何使用 UART 进行通信
  ******************************************************************************
  */

/*
使用方法：
将此文件内容复制到 Core/Src/main.c 即可运行此示例

功能：
- UART1 以 115200 波特率通信
- 接收字符并回显
- 演示基本的串口通信功能

硬件连接：
- PA9:  USART1 TX
- PA10: USART1 RX
*/

#include "stm32f1xx.h"
#include "system_stm32f1xx.h"
#include "gpio.h"
#include "uart.h"
#include "delay.h"

int main(void)
{
    char received_char;
    
    /* 系统初始化 */
    SystemInit();
    
    /* 使能时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   // GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;   // GPIOC (LED)
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // USART1
    
    /* 配置 GPIO */
    GPIO_Init(GPIOC, GPIO_PIN_13, GPIO_MODE_OUTPUT_50MHZ, GPIO_CNF_OUTPUT_PP);
    GPIO_Init(GPIOA, GPIO_PIN_9, GPIO_MODE_OUTPUT_50MHZ, GPIO_CNF_AF_PP);
    GPIO_Init(GPIOA, GPIO_PIN_10, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOATING);
    
    /* 初始化延时 */
    Delay_Init();
    
    /* 初始化 UART */
    UART_Init(USART1, 115200);
    
    /* 发送欢迎信息 */
    UART_SendString(USART1, "\r\n");
    UART_SendString(USART1, "========================================\r\n");
    UART_SendString(USART1, "  STM32F103 UART Echo Example\r\n");
    UART_SendString(USART1, "  请输入字符，将会回显\r\n");
    UART_SendString(USART1, "========================================\r\n\r\n");
    
    /* 主循环 */
    while(1)
    {
        /* 接收字符 */
        received_char = UART_ReceiveChar(USART1);
        
        /* 回显字符 */
        UART_SendChar(USART1, received_char);
        
        /* 如果是回车，添加换行 */
        if(received_char == '\r')
        {
            UART_SendChar(USART1, '\n');
        }
        
        /* LED 闪烁表示活动 */
        GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    }
}

