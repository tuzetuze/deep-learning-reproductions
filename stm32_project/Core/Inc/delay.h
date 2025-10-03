/**
  ******************************************************************************
  * @file    delay.h
  * @brief   Delay functions header file
  ******************************************************************************
  */

#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Function prototypes */
void Delay_Init(void);
void Delay_Ms(uint32_t ms);
void Delay_Us(uint32_t us);
uint32_t GetTick(void);

#ifdef __cplusplus
}
#endif

#endif /* __DELAY_H */

