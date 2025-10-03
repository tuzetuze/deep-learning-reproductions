/**
  ******************************************************************************
  * @file    system_stm32f1xx.c
  * @author  MCD Application Team
  * @brief   CMSIS Cortex-M3 Device Peripheral Access Layer System Source File.
  ******************************************************************************
  */

#include "stm32f1xx.h"

/* System Clock Frequency */
uint32_t SystemCoreClock = 72000000; /* 72 MHz */

/**
  * @brief  Setup the microcontroller system
  *         Initialize the Embedded Flash Interface, the PLL and update the 
  *         SystemCoreClock variable.
  * @param  None
  * @retval None
  */
void SystemInit(void)
{
  /* Reset the RCC clock configuration to the default reset state */
  /* Set HSION bit */
  RCC->CR |= (uint32_t)0x00000001;

  /* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
  RCC->CFGR &= (uint32_t)0xF0FF0000;
  
  /* Reset HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xFEF6FFFF;

  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */
  RCC->CFGR &= (uint32_t)0xFF80FFFF;

  /* Configure the System clock source, PLL Multiplier and Divider factors, 
     AHB/APBx prescalers and Flash settings */
  SetSysClock();
}

/**
  * @brief  Configures the System clock frequency, AHB/APBx prescalers and Flash settings.
  * @note   This function should be called only once the RCC clock configuration  
  *         is reset to the default reset state (done in SystemInit() function).
  * @param  None
  * @retval None
  */
static void SetSysClock(void)
{
  volatile uint32_t StartUpCounter = 0, HSEStatus = 0;
  
  /* Enable HSE */    
  RCC->CR |= ((uint32_t)0x00010000);
 
  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & 0x00020000;
    StartUpCounter++;  
  } while((HSEStatus == 0) && (StartUpCounter != 0x0500));

  if ((RCC->CR & 0x00020000) != 0)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }  

  if (HSEStatus == (uint32_t)0x01)
  {
    /* Enable Prefetch Buffer */
    *(volatile uint32_t*)0x40022000 |= 0x00000010;

    /* Flash 2 wait state */
    *(volatile uint32_t*)0x40022000 &= (uint32_t)((uint32_t)~0x03);
    *(volatile uint32_t*)0x40022000 |= (uint32_t)0x02;    

    /* HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)0x00000000;
      
    /* PCLK2 = HCLK */
    RCC->CFGR |= (uint32_t)0x00000000;
    
    /* PCLK1 = HCLK/2 */
    RCC->CFGR |= (uint32_t)0x00000400;

    /*  PLL configuration: PLLCLK = HSE * 9 = 72 MHz */
    RCC->CFGR &= (uint32_t)0xFFC0FFFF;
    RCC->CFGR |= (uint32_t)(0x00010000 | 0x001C0000);

    /* Enable PLL */
    RCC->CR |= 0x01000000;

    /* Wait till PLL is ready */
    while((RCC->CR & 0x02000000) == 0)
    {
    }

    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~0x00000003);
    RCC->CFGR |= (uint32_t)0x00000002;    

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)0x0000000C) != (uint32_t)0x08)
    {
    }
  }
  else
  { 
    /* If HSE fails to start-up, the application will have wrong clock 
       configuration. User can add here some code to deal with this error */
  }
}

/**
  * @brief  Update SystemCoreClock variable according to Clock Register Values.
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate(void)
{
  /* This function updates the SystemCoreClock variable */
  SystemCoreClock = 72000000;
}

