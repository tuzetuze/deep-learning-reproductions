/**
  ******************************************************************************
  * @file    motor_control.c
  * @brief   电机控制示例程序
  ******************************************************************************
  */

/*
使用方法：
将此文件内容复制到 Core/Src/main.c 即可运行此示例

功能：
- 控制两个直流电机
- 演示正转、反转、调速、停止
- 通过 UART 输出状态信息

硬件连接：
- PA6:  TIM3_CH1 (电机1 正转)
- PA7:  TIM3_CH2 (电机1 反转)
- PB0:  TIM3_CH3 (电机2 正转)
- PB1:  TIM3_CH4 (电机2 反转)
- PA9:  USART1 TX
- PA10: USART1 RX

电机驱动板连接：
- STM32 PWM → 驱动板 IN1/IN2
- 驱动板 OUT1/OUT2 → 电机
- 共地连接
*/

#include "stm32f1xx.h"
#include "system_stm32f1xx.h"
#include "gpio.h"
#include "uart.h"
#include "delay.h"
#include "pwm.h"

int main(void)
{
    int16_t speed;
    
    /* 系统初始化 */
    SystemInit();
    
    /* 使能时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    
    /* 配置 UART */
    GPIO_Init(GPIOA, GPIO_PIN_9, GPIO_MODE_OUTPUT_50MHZ, GPIO_CNF_AF_PP);
    GPIO_Init(GPIOA, GPIO_PIN_10, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOATING);
    
    /* 初始化外设 */
    Delay_Init();
    UART_Init(USART1, 115200);
    Motor_Init();
    
    /* 发送欢迎信息 */
    UART_SendString(USART1, "\r\n");
    UART_SendString(USART1, "========================================\r\n");
    UART_SendString(USART1, "  STM32 电机控制示例\r\n");
    UART_SendString(USART1, "========================================\r\n\r\n");
    
    /* 主循环 */
    while(1)
    {
        /* 测试序列 1: 电机1 逐渐加速 */
        UART_SendString(USART1, "[测试1] 电机1 正转加速...\r\n");
        for(speed = 0; speed <= 100; speed += 10)
        {
            Motor_SetSpeed(1, speed);
            UART_Printf(USART1, "  电机1 速度: %d%%\r\n", speed);
            Delay_Ms(500);
        }
        
        /* 测试序列 2: 电机1 逐渐减速 */
        UART_SendString(USART1, "[测试2] 电机1 减速...\r\n");
        for(speed = 100; speed >= 0; speed -= 10)
        {
            Motor_SetSpeed(1, speed);
            UART_Printf(USART1, "  电机1 速度: %d%%\r\n", speed);
            Delay_Ms(500);
        }
        
        Delay_Ms(1000);
        
        /* 测试序列 3: 电机1 反转 */
        UART_SendString(USART1, "[测试3] 电机1 反转...\r\n");
        for(speed = 0; speed >= -100; speed -= 10)
        {
            Motor_SetSpeed(1, speed);
            UART_Printf(USART1, "  电机1 速度: %d%%\r\n", speed);
            Delay_Ms(500);
        }
        
        Motor_Stop(1);
        Delay_Ms(1000);
        
        /* 测试序列 4: 双电机同时运行 */
        UART_SendString(USART1, "[测试4] 双电机同时运行...\r\n");
        Motor_SetSpeed(1, 80);
        Motor_SetSpeed(2, 80);
        UART_SendString(USART1, "  电机1: 80%, 电机2: 80%\r\n");
        Delay_Ms(3000);
        
        /* 测试序列 5: 双电机不同速度 */
        UART_SendString(USART1, "[测试5] 双电机不同速度...\r\n");
        Motor_SetSpeed(1, 50);
        Motor_SetSpeed(2, 100);
        UART_SendString(USART1, "  电机1: 50%, 电机2: 100%\r\n");
        Delay_Ms(3000);
        
        /* 停止所有电机 */
        UART_SendString(USART1, "[停止] 所有电机停止\r\n");
        Motor_Stop(1);
        Motor_Stop(2);
        
        Delay_Ms(2000);
        UART_SendString(USART1, "\r\n========================================\r\n");
        UART_SendString(USART1, "循环重新开始...\r\n\r\n");
    }
}

