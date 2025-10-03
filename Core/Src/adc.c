/**
  ******************************************************************************
  * @file    adc.c
  * @brief   ADC 模数转换驱动实现
  ******************************************************************************
  */

#include "adc.h"
#include "gpio.h"

/* ADC 寄存器位定义 */
#define ADC_CR2_ADON        (1 << 0)   /* ADC 使能 */
#define ADC_CR2_CAL         (1 << 3)   /* ADC 校准 */
#define ADC_CR2_RSTCAL      (1 << 4)   /* 复位校准 */
#define ADC_CR2_SWSTART     (1 << 22)  /* 软件启动转换 */
#define ADC_SR_EOC          (1 << 1)   /* 转换结束标志 */

/**
  * @brief  初始化 ADC
  * @note   配置 ADC1, 使用软件触发, 单次转换模式
  * @retval None
  */
void ADC_Init(void)
{
    /* 使能时钟 */
    RCC->APB2ENR |= (1 << 9);   /* ADC1 时钟 */
    RCC->APB2ENR |= (1 << 2);   /* GPIOA 时钟 */
    
    /* 配置 ADC 时钟分频 PCLK2/6 = 72MHz/6 = 12MHz */
    RCC->CFGR &= ~(3 << 14);
    RCC->CFGR |= (2 << 14);
    
    /* 配置 GPIO 为模拟输入 
     * PA0-PA7 对应 ADC1_IN0 到 ADC1_IN7 */
    GPIO_Init(GPIOA, GPIO_PIN_0, GPIO_MODE_INPUT, 0);  /* 模拟输入 CNF=00 */
    GPIO_Init(GPIOA, GPIO_PIN_1, GPIO_MODE_INPUT, 0);
    GPIO_Init(GPIOA, GPIO_PIN_2, GPIO_MODE_INPUT, 0);
    GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_INPUT, 0);
    GPIO_Init(GPIOA, GPIO_PIN_4, GPIO_MODE_INPUT, 0);
    GPIO_Init(GPIOA, GPIO_PIN_5, GPIO_MODE_INPUT, 0);
    GPIO_Init(GPIOA, GPIO_PIN_6, GPIO_MODE_INPUT, 0);
    GPIO_Init(GPIOA, GPIO_PIN_7, GPIO_MODE_INPUT, 0);
    
    /* ADC 配置 */
    /* CR1: 独立模式 */
    ADC1->CR1 = 0;
    
    /* CR2: 数据右对齐, 单次转换, 软件触发 */
    ADC1->CR2 = 0;
    
    /* SQR1: 转换序列长度 = 1 */
    ADC1->SQR1 = 0;
    
    /* SMPR2: 采样时间 = 239.5 周期 (所有通道) */
    ADC1->SMPR2 = 0x3FFFFFFF;
    ADC1->SMPR1 = 0x00FFFFFF;
    
    /* 使能 ADC */
    ADC1->CR2 |= ADC_CR2_ADON;
    
    /* 延时等待稳定 */
    for(volatile uint32_t i = 0; i < 10000; i++);
    
    /* 校准 ADC */
    ADC1->CR2 |= ADC_CR2_RSTCAL;           /* 复位校准 */
    while(ADC1->CR2 & ADC_CR2_RSTCAL);     /* 等待复位完成 */
    
    ADC1->CR2 |= ADC_CR2_CAL;              /* 开始校准 */
    while(ADC1->CR2 & ADC_CR2_CAL);        /* 等待校准完成 */
}

/**
  * @brief  读取 ADC 值
  * @param  channel: ADC 通道 (0-9)
  * @retval ADC 转换值 (0-4095)
  */
uint16_t ADC_Read(uint8_t channel)
{
    /* 设置转换通道 */
    ADC1->SQR3 = channel;
    
    /* 启动转换 */
    ADC1->CR2 |= ADC_CR2_SWSTART;
    
    /* 等待转换完成 */
    while(!(ADC1->SR & ADC_SR_EOC));
    
    /* 读取转换结果 */
    return ADC1->DR;
}

/**
  * @brief  读取 ADC 电压值
  * @param  channel: ADC 通道
  * @retval 电压值 (V)
  * 
  * @note   STM32F103 ADC 为 12位, 参考电压 3.3V
  *         电压 = (ADC值 / 4095) * 3.3V
  */
float ADC_ReadVoltage(uint8_t channel)
{
    uint16_t adc_value = ADC_Read(channel);
    return (adc_value / 4095.0f) * 3.3f;
}

/**
  * @brief  读取 ADC 平均值 (减少噪声)
  * @param  channel: ADC 通道
  * @param  times: 采样次数
  * @retval ADC 平均值
  */
uint16_t ADC_ReadAverage(uint8_t channel, uint8_t times)
{
    uint32_t sum = 0;
    uint8_t i;
    
    for(i = 0; i < times; i++)
    {
        sum += ADC_Read(channel);
    }
    
    return (uint16_t)(sum / times);
}

