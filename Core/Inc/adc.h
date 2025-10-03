/**
  ******************************************************************************
  * @file    adc.h
  * @brief   ADC 模数转换驱动头文件
  ******************************************************************************
  */

#ifndef __ADC_H
#define __ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx.h"

/* ADC 通道定义 */
#define ADC_CHANNEL_0       0
#define ADC_CHANNEL_1       1
#define ADC_CHANNEL_2       2
#define ADC_CHANNEL_3       3
#define ADC_CHANNEL_4       4
#define ADC_CHANNEL_5       5
#define ADC_CHANNEL_6       6
#define ADC_CHANNEL_7       7
#define ADC_CHANNEL_8       8
#define ADC_CHANNEL_9       9

/* 函数原型 */
void ADC_Init(void);
uint16_t ADC_Read(uint8_t channel);
float ADC_ReadVoltage(uint8_t channel);
uint16_t ADC_ReadAverage(uint8_t channel, uint8_t times);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H */

