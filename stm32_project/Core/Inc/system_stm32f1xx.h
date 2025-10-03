/**
  ******************************************************************************
  * @file    system_stm32f1xx.h
  * @author  MCD Application Team
  * @brief   CMSIS Cortex-M3 Device System Source File for STM32F1xx devices.
  ******************************************************************************
  */

#ifndef __SYSTEM_STM32F1XX_H
#define __SYSTEM_STM32F1XX_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include <stdint.h>

extern uint32_t SystemCoreClock;          /*!< System Clock Frequency (Core Clock) */

extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);
static void SetSysClock(void);

#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_STM32F1XX_H */

