/**
  ******************************************************************************
  * @file    adc_sensor.c
  * @brief   ADC 传感器采集示例程序
  ******************************************************************************
  */

/*
使用方法：
将此文件内容复制到 Core/Src/main.c 即可运行此示例

功能：
- ADC 多通道采集
- 电压测量
- 模拟传感器读取（如光敏电阻、电位器、温度传感器等）
- UART 输出采集数据

硬件连接：
- PA0:  ADC1_IN0 (模拟输入通道0)
- PA1:  ADC1_IN1 (模拟输入通道1)
- PA2:  ADC1_IN2 (模拟输入通道2)
- PA3:  ADC1_IN3 (模拟输入通道3)
- PA9:  USART1 TX
- PA10: USART1 RX

传感器连接示例：
- 电位器: VCC → 可调端 → PA0, GND
- 光敏电阻: VCC → 光敏电阻 → PA1 → 10kΩ → GND
- LM35温度: VCC → LM35 → PA2, GND
*/

#include "stm32f1xx.h"
#include "system_stm32f1xx.h"
#include "gpio.h"
#include "uart.h"
#include "delay.h"
#include "adc.h"

int main(void)
{
    uint16_t adc_value[4];
    float voltage[4];
    float temperature;
    
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
    ADC_Init();
    
    /* 发送欢迎信息 */
    UART_SendString(USART1, "\r\n");
    UART_SendString(USART1, "========================================\r\n");
    UART_SendString(USART1, "  STM32 ADC 传感器采集示例\r\n");
    UART_SendString(USART1, "========================================\r\n");
    UART_SendString(USART1, "ADC 分辨率: 12位 (0-4095)\r\n");
    UART_SendString(USART1, "参考电压: 3.3V\r\n");
    UART_SendString(USART1, "采样频率: 1Hz\r\n");
    UART_SendString(USART1, "========================================\r\n\r\n");
    
    /* 主循环 */
    while(1)
    {
        /* 读取多个 ADC 通道 */
        adc_value[0] = ADC_ReadAverage(ADC_CHANNEL_0, 10);  /* 10次平均，降噪 */
        adc_value[1] = ADC_ReadAverage(ADC_CHANNEL_1, 10);
        adc_value[2] = ADC_ReadAverage(ADC_CHANNEL_2, 10);
        adc_value[3] = ADC_ReadAverage(ADC_CHANNEL_3, 10);
        
        /* 转换为电压 */
        voltage[0] = ADC_ReadVoltage(ADC_CHANNEL_0);
        voltage[1] = ADC_ReadVoltage(ADC_CHANNEL_1);
        voltage[2] = ADC_ReadVoltage(ADC_CHANNEL_2);
        voltage[3] = ADC_ReadVoltage(ADC_CHANNEL_3);
        
        /* 计算温度 (假设使用 LM35，10mV/°C) */
        temperature = voltage[2] * 100.0f;  /* LM35: 每°C = 10mV */
        
        /* 输出数据 */
        UART_SendString(USART1, "╔════════════════════════════════════════╗\r\n");
        UART_SendString(USART1, "║        ADC 采集数据                    ║\r\n");
        UART_SendString(USART1, "╠════════════════════════════════════════╣\r\n");
        
        /* 通道 0 - 电位器 */
        UART_Printf(USART1, "║ CH0 (电位器):                          ║\r\n");
        UART_Printf(USART1, "║   ADC值: %-4u  电压: %.2fV         ║\r\n", 
                    adc_value[0], voltage[0]);
        UART_Printf(USART1, "║   百分比: %d%%                         ║\r\n", 
                    (int)((adc_value[0] * 100) / 4095));
        UART_SendString(USART1, "╟────────────────────────────────────────╢\r\n");
        
        /* 通道 1 - 光敏电阻 */
        UART_Printf(USART1, "║ CH1 (光敏电阻):                        ║\r\n");
        UART_Printf(USART1, "║   ADC值: %-4u  电压: %.2fV         ║\r\n", 
                    adc_value[1], voltage[1]);
        if(adc_value[1] > 3000)
            UART_SendString(USART1, "║   光照: 强光                           ║\r\n");
        else if(adc_value[1] > 1500)
            UART_SendString(USART1, "║   光照: 中等                           ║\r\n");
        else
            UART_SendString(USART1, "║   光照: 弱光                           ║\r\n");
        UART_SendString(USART1, "╟────────────────────────────────────────╢\r\n");
        
        /* 通道 2 - 温度传感器 */
        UART_Printf(USART1, "║ CH2 (温度传感器):                      ║\r\n");
        UART_Printf(USART1, "║   ADC值: %-4u  电压: %.2fV         ║\r\n", 
                    adc_value[2], voltage[2]);
        UART_Printf(USART1, "║   温度: %.1f°C                        ║\r\n", temperature);
        UART_SendString(USART1, "╟────────────────────────────────────────╢\r\n");
        
        /* 通道 3 - 通用传感器 */
        UART_Printf(USART1, "║ CH3 (通用):                            ║\r\n");
        UART_Printf(USART1, "║   ADC值: %-4u  电压: %.2fV         ║\r\n", 
                    adc_value[3], voltage[3]);
        UART_SendString(USART1, "╚════════════════════════════════════════╝\r\n");
        UART_SendString(USART1, "\r\n");
        
        /* 延时 1 秒 */
        Delay_Ms(1000);
    }
}

