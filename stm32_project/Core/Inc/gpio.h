/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   GPIO driver header file
  ******************************************************************************
  */

#ifndef __GPIO_H
#define __GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx.h"

/* GPIO Mode definitions */
#define GPIO_MODE_INPUT         0x0
#define GPIO_MODE_OUTPUT_10MHZ  0x1
#define GPIO_MODE_OUTPUT_2MHZ   0x2
#define GPIO_MODE_OUTPUT_50MHZ  0x3

/* GPIO Configuration definitions */
#define GPIO_CNF_INPUT_ANALOG   0x0
#define GPIO_CNF_INPUT_FLOATING 0x1
#define GPIO_CNF_INPUT_PUPD     0x2

#define GPIO_CNF_OUTPUT_PP      0x0
#define GPIO_CNF_OUTPUT_OD      0x1
#define GPIO_CNF_AF_PP          0x2
#define GPIO_CNF_AF_OD          0x3

/* GPIO Pin definitions */
#define GPIO_PIN_0              ((uint16_t)0x0001)
#define GPIO_PIN_1              ((uint16_t)0x0002)
#define GPIO_PIN_2              ((uint16_t)0x0004)
#define GPIO_PIN_3              ((uint16_t)0x0008)
#define GPIO_PIN_4              ((uint16_t)0x0010)
#define GPIO_PIN_5              ((uint16_t)0x0020)
#define GPIO_PIN_6              ((uint16_t)0x0040)
#define GPIO_PIN_7              ((uint16_t)0x0080)
#define GPIO_PIN_8              ((uint16_t)0x0100)
#define GPIO_PIN_9              ((uint16_t)0x0200)
#define GPIO_PIN_10             ((uint16_t)0x0400)
#define GPIO_PIN_11             ((uint16_t)0x0800)
#define GPIO_PIN_12             ((uint16_t)0x1000)
#define GPIO_PIN_13             ((uint16_t)0x2000)
#define GPIO_PIN_14             ((uint16_t)0x4000)
#define GPIO_PIN_15             ((uint16_t)0x8000)

/* GPIO Pin State */
typedef enum
{
  GPIO_PIN_RESET = 0,
  GPIO_PIN_SET
} GPIO_PinState;

/* Function prototypes */
void GPIO_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t Mode, uint32_t CNF);
void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
GPIO_PinState GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

#ifdef __cplusplus
}
#endif

#endif /* __GPIO_H */

