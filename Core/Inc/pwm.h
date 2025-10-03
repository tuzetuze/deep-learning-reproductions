/**
  ******************************************************************************
  * @file    pwm.h
  * @brief   PWM 驱动头文件 - 用于电机控制、舵机、LED调光等
  ******************************************************************************
  */

#ifndef __PWM_H
#define __PWM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx.h"

/* PWM 通道定义 */
typedef enum
{
    PWM_CHANNEL_1 = 0,
    PWM_CHANNEL_2 = 1,
    PWM_CHANNEL_3 = 2,
    PWM_CHANNEL_4 = 3
} PWM_Channel_t;

/* PWM 配置结构体 */
typedef struct
{
    TIM_TypeDef *TIMx;          /* 定时器 */
    uint32_t Frequency;         /* PWM 频率 (Hz) */
    uint16_t Period;            /* 周期值 (ARR) */
    uint16_t Prescaler;         /* 预分频器 (PSC) */
} PWM_Config_t;

/* 函数原型 */
void PWM_Init(TIM_TypeDef *TIMx, uint32_t frequency);
void PWM_SetDutyCycle(TIM_TypeDef *TIMx, PWM_Channel_t channel, float duty_cycle);
void PWM_Start(TIM_TypeDef *TIMx, PWM_Channel_t channel);
void PWM_Stop(TIM_TypeDef *TIMx, PWM_Channel_t channel);

/* 电机控制函数 */
void Motor_Init(void);
void Motor_SetSpeed(uint8_t motor_id, int16_t speed);  /* -100 到 +100 */
void Motor_Stop(uint8_t motor_id);

/* 舵机控制函数 */
void Servo_Init(void);
void Servo_SetAngle(uint8_t servo_id, uint16_t angle);  /* 0-180度 */

#ifdef __cplusplus
}
#endif

#endif /* __PWM_H */

