/**
  ******************************************************************************
  * @file    stm32f1xx.h
  * @author  MCD Application Team
  * @brief   CMSIS STM32F1xx Device Peripheral Access Layer Header File.
  ******************************************************************************
  */

#ifndef __STM32F1XX_H
#define __STM32F1XX_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/** @addtogroup Library_configuration_section
  * @{
  */

#if !defined (STM32F103xB)
  #define STM32F103xB
#endif /* STM32F103xB */

/* Uncomment the line below according to the target STM32 device used in your application */
#if defined(STM32F103xB)
  #include "stm32f103xb.h"
#else
 #error "Please select first the target STM32F1xx device used in your application"
#endif

typedef enum 
{
  RESET = 0, 
  SET = !RESET
} FlagStatus, ITStatus;

typedef enum 
{
  DISABLE = 0, 
  ENABLE = !DISABLE
} FunctionalState;

typedef enum
{
  ERROR = 0,
  SUCCESS = !ERROR
} ErrorStatus;

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __STM32F1XX_H */

