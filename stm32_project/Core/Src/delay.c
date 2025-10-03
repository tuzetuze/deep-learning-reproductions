/**
  ******************************************************************************
  * @file    delay.c
  * @brief   Delay functions implementation using SysTick
  ******************************************************************************
  */

#include "delay.h"
#include "stm32f1xx.h"

static volatile uint32_t g_SysTick_Counter = 0;

/**
  * @brief  Initialize SysTick for delay functions
  * @param  None
  * @retval None
  */
void Delay_Init(void)
{
    /* Configure SysTick to generate interrupt every 1ms */
    /* 72MHz / 1000 = 72000 ticks for 1ms */
    SysTick_Config(72000);
}

/**
  * @brief  SysTick interrupt handler
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    g_SysTick_Counter++;
}

/**
  * @brief  Delay in milliseconds
  * @param  ms: Number of milliseconds to delay
  * @retval None
  */
void Delay_Ms(uint32_t ms)
{
    uint32_t start_tick = g_SysTick_Counter;
    
    while((g_SysTick_Counter - start_tick) < ms)
    {
        /* Wait */
    }
}

/**
  * @brief  Delay in microseconds (approximate)
  * @param  us: Number of microseconds to delay
  * @retval None
  */
void Delay_Us(uint32_t us)
{
    uint32_t cycles = us * 72; /* 72 cycles per microsecond at 72MHz */
    
    while(cycles--)
    {
        __asm("NOP");
    }
}

/**
  * @brief  Get current tick count
  * @param  None
  * @retval Current tick count in milliseconds
  */
uint32_t GetTick(void)
{
    return g_SysTick_Counter;
}

