/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   GPIO driver implementation
  ******************************************************************************
  */

#include "gpio.h"

/**
  * @brief  Initialize GPIO pin
  * @param  GPIOx: GPIO port (GPIOA, GPIOB, GPIOC)
  * @param  GPIO_Pin: GPIO pin number (GPIO_PIN_0 to GPIO_PIN_15)
  * @param  Mode: GPIO mode
  * @param  CNF: GPIO configuration
  * @retval None
  */
void GPIO_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t Mode, uint32_t CNF)
{
    uint32_t position = 0;
    uint32_t config = 0;
    
    /* Find pin position */
    while(GPIO_Pin >> position != 1)
    {
        position++;
    }
    
    /* Configure pin mode and CNF */
    config = (CNF << 2) | Mode;
    
    if(position < 8)
    {
        /* Configure CRL register (pins 0-7) */
        GPIOx->CRL &= ~(0xF << (position * 4));
        GPIOx->CRL |= (config << (position * 4));
    }
    else
    {
        /* Configure CRH register (pins 8-15) */
        position -= 8;
        GPIOx->CRH &= ~(0xF << (position * 4));
        GPIOx->CRH |= (config << (position * 4));
    }
}

/**
  * @brief  Write pin state
  * @param  GPIOx: GPIO port
  * @param  GPIO_Pin: GPIO pin
  * @param  PinState: GPIO_PIN_SET or GPIO_PIN_RESET
  * @retval None
  */
void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    if(PinState != GPIO_PIN_RESET)
    {
        GPIOx->BSRR = GPIO_Pin;
    }
    else
    {
        GPIOx->BRR = GPIO_Pin;
    }
}

/**
  * @brief  Toggle pin state
  * @param  GPIOx: GPIO port
  * @param  GPIO_Pin: GPIO pin
  * @retval None
  */
void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    uint32_t odr = GPIOx->ODR;
    GPIOx->BSRR = ((odr & GPIO_Pin) << 16) | (~odr & GPIO_Pin);
}

/**
  * @brief  Read pin state
  * @param  GPIOx: GPIO port
  * @param  GPIO_Pin: GPIO pin
  * @retval GPIO_PIN_SET or GPIO_PIN_RESET
  */
GPIO_PinState GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    if((GPIOx->IDR & GPIO_Pin) != (uint32_t)GPIO_PIN_RESET)
    {
        return GPIO_PIN_SET;
    }
    else
    {
        return GPIO_PIN_RESET;
    }
}

