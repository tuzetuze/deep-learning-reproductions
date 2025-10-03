/**
  ******************************************************************************
  * @file    comprehensive_demo.c
  * @brief   综合示例程序 - 集成多个功能模块
  ******************************************************************************
  */

/*
使用方法：
将此文件内容复制到 Core/Src/main.c 即可运行此示例

功能：
- LCD1602 显示屏
- ADC 采集（电位器控制）
- PWM 电机控制
- UART 调试输出
- 多任务协同工作

硬件连接：
LCD1602:
  - PB12-14: RS, RW, EN
  - PB8-11: D4-D7

ADC:
  - PA0: 电位器输入

电机:
  - PA6-7: 电机1 PWM
  
UART:
  - PA9-10: TX/RX

应用场景：
通过电位器调节电机速度，LCD 显示速度值，UART 输出调试信息
*/

#include "stm32f1xx.h"
#include "system_stm32f1xx.h"
#include "gpio.h"
#include "uart.h"
#include "delay.h"
#include "adc.h"
#include "pwm.h"
#include "lcd1602.h"

/* 自定义字符：速度表图标 */
uint8_t speed_icon[] = {
    0x00, 0x0E, 0x11, 0x11,
    0x11, 0x0E, 0x04, 0x00
};

int main(void)
{
    uint16_t adc_value;
    int16_t motor_speed;
    float voltage;
    uint32_t update_counter = 0;
    
    /* 系统初始化 */
    SystemInit();
    
    /* 使能时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    
    /* 配置 UART */
    GPIO_Init(GPIOA, GPIO_PIN_9, GPIO_MODE_OUTPUT_50MHZ, GPIO_CNF_AF_PP);
    GPIO_Init(GPIOA, GPIO_PIN_10, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOATING);
    
    /* 初始化所有外设 */
    Delay_Init();
    UART_Init(USART1, 115200);
    ADC_Init();
    Motor_Init();
    LCD1602_Init();
    
    /* 创建自定义字符 */
    LCD1602_CreateChar(0, speed_icon);
    
    /* LCD 欢迎界面 */
    LCD1602_Clear();
    LCD1602_Printf(0, 0, " STM32 Control ");
    LCD1602_Printf(1, 0, "  System v1.0  ");
    Delay_Ms(2000);
    
    /* UART 欢迎信息 */
    UART_SendString(USART1, "\r\n");
    UART_SendString(USART1, "╔════════════════════════════════════════╗\r\n");
    UART_SendString(USART1, "║   STM32F103 综合控制系统               ║\r\n");
    UART_SendString(USART1, "╠════════════════════════════════════════╣\r\n");
    UART_SendString(USART1, "║ 功能:                                  ║\r\n");
    UART_SendString(USART1, "║  • LCD1602 实时显示                    ║\r\n");
    UART_SendString(USART1, "║  • ADC 电位器采集                      ║\r\n");
    UART_SendString(USART1, "║  • PWM 电机速度控制                    ║\r\n");
    UART_SendString(USART1, "║  • UART 数据监控                       ║\r\n");
    UART_SendString(USART1, "╚════════════════════════════════════════╝\r\n");
    UART_SendString(USART1, "\r\n系统已启动！\r\n\r\n");
    
    /* LCD 主界面 */
    LCD1602_Clear();
    LCD1602_Printf(0, 0, "Speed:");
    LCD1602_SetCursor(0, 15);
    LCD1602_PrintChar(0);  /* 速度图标 */
    LCD1602_Printf(1, 0, "ADC:     V:    ");
    
    /* 主循环 */
    while(1)
    {
        /* 读取 ADC 值 (电位器) */
        adc_value = ADC_ReadAverage(ADC_CHANNEL_0, 5);
        voltage = ADC_ReadVoltage(ADC_CHANNEL_0);
        
        /* 计算电机速度 (-100 到 +100) */
        /* ADC 0-2047: 反转, 2048-4095: 正转 */
        if(adc_value < 2048)
        {
            motor_speed = -(int16_t)((2048 - adc_value) * 100 / 2048);
        }
        else
        {
            motor_speed = (int16_t)((adc_value - 2048) * 100 / 2047);
        }
        
        /* 死区处理 (±5%) */
        if(motor_speed > -5 && motor_speed < 5)
        {
            motor_speed = 0;
        }
        
        /* 设置电机速度 */
        Motor_SetSpeed(1, motor_speed);
        
        /* 更新 LCD 显示 */
        LCD1602_Printf(0, 7, "%4d%%", motor_speed);
        LCD1602_Printf(1, 4, "%4u", adc_value);
        LCD1602_Printf(1, 11, "%.2f", voltage);
        
        /* 每秒通过 UART 输出一次状态 */
        update_counter++;
        if(update_counter >= 10)  /* 100ms * 10 = 1s */
        {
            update_counter = 0;
            
            UART_SendString(USART1, "┌─────────────────────────────────────┐\r\n");
            UART_Printf(USART1,     "│ ADC值: %-4u  电压: %.2fV         │\r\n", 
                        adc_value, voltage);
            UART_Printf(USART1,     "│ 电机速度: %+4d%%                   │\r\n", 
                        motor_speed);
            
            if(motor_speed > 0)
                UART_SendString(USART1, "│ 状态: 正转 →                       │\r\n");
            else if(motor_speed < 0)
                UART_SendString(USART1, "│ 状态: 反转 ←                       │\r\n");
            else
                UART_SendString(USART1, "│ 状态: 停止 ■                       │\r\n");
            
            UART_SendString(USART1, "└─────────────────────────────────────┘\r\n");
        }
        
        /* 延时 100ms */
        Delay_Ms(100);
    }
}

