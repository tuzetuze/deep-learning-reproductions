/**
  ******************************************************************************
  * @file    blink_led.c
  * @brief   LED 闪烁示例程序
  * @note    这是一个独立的示例，展示如何使用 GPIO 驱动控制 LED
  ******************************************************************************
  */

/*
使用方法：
将此文件内容复制到 Core/Src/main.c 即可运行此示例

功能：
- PC13 LED 以 500ms 间隔闪烁
- 最简单的 GPIO 控制示例
*/

#include "stm32f1xx.h"
#include "system_stm32f1xx.h"
#include "gpio.h"
#include "delay.h"

int main(void)
{
    /* 系统初始化 */
    SystemInit();
    
    /* 使能 GPIOC 时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    
    /* 配置 PC13 为输出 */
    GPIO_Init(GPIOC, GPIO_PIN_13, GPIO_MODE_OUTPUT_50MHZ, GPIO_CNF_OUTPUT_PP);
    
    /* 初始化延时 */
    Delay_Init();
    
    /* 主循环 */
    while(1)
    {
        /* 翻转 LED */
        GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        
        /* 延时 500ms */
        Delay_Ms(500);
    }
}

