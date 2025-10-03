/**
  ******************************************************************************
  * @file    pwm.c
  * @brief   PWM 驱动实现 - 支持电机、舵机控制
  ******************************************************************************
  */

#include "pwm.h"
#include "gpio.h"
#include "system_stm32f1xx.h"

/**
  * @brief  初始化 PWM
  * @param  TIMx: 定时器 (TIM2, TIM3, TIM4)
  * @param  frequency: PWM 频率 (Hz)
  * @retval None
  * 
  * 示例: PWM_Init(TIM3, 1000); // 1kHz PWM
  */
void PWM_Init(TIM_TypeDef *TIMx, uint32_t frequency)
{
    uint32_t timer_clock;
    uint16_t prescaler;
    uint16_t period;
    
    /* 获取定时器时钟 */
    if(TIMx == TIM2 || TIMx == TIM3 || TIMx == TIM4)
    {
        timer_clock = SystemCoreClock / 2;  /* APB1 = 36MHz */
    }
    else
    {
        timer_clock = SystemCoreClock;      /* APB2 = 72MHz */
    }
    
    /* 计算预分频器和周期值 */
    prescaler = 72 - 1;  /* 72MHz / 72 = 1MHz */
    period = (timer_clock / (prescaler + 1) / frequency) - 1;
    
    /* 配置定时器 */
    TIMx->PSC = prescaler;
    TIMx->ARR = period;
    
    /* PWM 模式 1 配置 */
    /* 通道 1 */
    TIMx->CCMR1 &= ~(0xFF << 0);
    TIMx->CCMR1 |= (0x6 << 4);   /* PWM 模式 1 */
    TIMx->CCMR1 |= (0x1 << 3);   /* 预装载使能 */
    
    /* 通道 2 */
    TIMx->CCMR1 &= ~(0xFF << 8);
    TIMx->CCMR1 |= (0x6 << 12);  /* PWM 模式 1 */
    TIMx->CCMR1 |= (0x1 << 11);  /* 预装载使能 */
    
    /* 通道 3 */
    TIMx->CCMR2 &= ~(0xFF << 0);
    TIMx->CCMR2 |= (0x6 << 4);   /* PWM 模式 1 */
    TIMx->CCMR2 |= (0x1 << 3);   /* 预装载使能 */
    
    /* 通道 4 */
    TIMx->CCMR2 &= ~(0xFF << 8);
    TIMx->CCMR2 |= (0x6 << 12);  /* PWM 模式 1 */
    TIMx->CCMR2 |= (0x1 << 11);  /* 预装载使能 */
    
    /* 自动重装载预装载使能 */
    TIMx->CR1 |= (0x1 << 7);
    
    /* 启用定时器 */
    TIMx->CR1 |= (0x1 << 0);
}

/**
  * @brief  设置 PWM 占空比
  * @param  TIMx: 定时器
  * @param  channel: PWM 通道
  * @param  duty_cycle: 占空比 (0.0 - 100.0)
  * @retval None
  */
void PWM_SetDutyCycle(TIM_TypeDef *TIMx, PWM_Channel_t channel, float duty_cycle)
{
    uint16_t pulse;
    
    /* 限制占空比范围 */
    if(duty_cycle > 100.0f) duty_cycle = 100.0f;
    if(duty_cycle < 0.0f) duty_cycle = 0.0f;
    
    /* 计算脉冲宽度 */
    pulse = (uint16_t)((duty_cycle / 100.0f) * (TIMx->ARR + 1));
    
    /* 设置比较值 */
    switch(channel)
    {
        case PWM_CHANNEL_1:
            TIMx->CCR1 = pulse;
            break;
        case PWM_CHANNEL_2:
            TIMx->CCR2 = pulse;
            break;
        case PWM_CHANNEL_3:
            TIMx->CCR3 = pulse;
            break;
        case PWM_CHANNEL_4:
            TIMx->CCR4 = pulse;
            break;
    }
}

/**
  * @brief  启动 PWM 输出
  * @param  TIMx: 定时器
  * @param  channel: PWM 通道
  * @retval None
  */
void PWM_Start(TIM_TypeDef *TIMx, PWM_Channel_t channel)
{
    switch(channel)
    {
        case PWM_CHANNEL_1:
            TIMx->CCER |= (0x1 << 0);  /* CC1E */
            break;
        case PWM_CHANNEL_2:
            TIMx->CCER |= (0x1 << 4);  /* CC2E */
            break;
        case PWM_CHANNEL_3:
            TIMx->CCER |= (0x1 << 8);  /* CC3E */
            break;
        case PWM_CHANNEL_4:
            TIMx->CCER |= (0x1 << 12); /* CC4E */
            break;
    }
}

/**
  * @brief  停止 PWM 输出
  * @param  TIMx: 定时器
  * @param  channel: PWM 通道
  * @retval None
  */
void PWM_Stop(TIM_TypeDef *TIMx, PWM_Channel_t channel)
{
    switch(channel)
    {
        case PWM_CHANNEL_1:
            TIMx->CCER &= ~(0x1 << 0);
            break;
        case PWM_CHANNEL_2:
            TIMx->CCER &= ~(0x1 << 4);
            break;
        case PWM_CHANNEL_3:
            TIMx->CCER &= ~(0x1 << 8);
            break;
        case PWM_CHANNEL_4:
            TIMx->CCER &= ~(0x1 << 12);
            break;
    }
}

/**
  * @brief  初始化直流电机控制
  * @note   使用 TIM3 CH1/CH2 控制电机1，CH3/CH4 控制电机2
  * @note   引脚: PA6(TIM3_CH1), PA7(TIM3_CH2), PB0(TIM3_CH3), PB1(TIM3_CH4)
  * @retval None
  */
void Motor_Init(void)
{
    /* 使能时钟 */
    RCC->APB1ENR |= (0x1 << 1);  /* TIM3 */
    RCC->APB2ENR |= (0x1 << 2);  /* GPIOA */
    RCC->APB2ENR |= (0x1 << 3);  /* GPIOB */
    
    /* 配置 GPIO - PA6, PA7 (TIM3_CH1, TIM3_CH2) */
    GPIO_Init(GPIOA, GPIO_PIN_6, GPIO_MODE_OUTPUT_50MHZ, GPIO_CNF_AF_PP);
    GPIO_Init(GPIOA, GPIO_PIN_7, GPIO_MODE_OUTPUT_50MHZ, GPIO_CNF_AF_PP);
    
    /* 配置 GPIO - PB0, PB1 (TIM3_CH3, TIM3_CH4) */
    GPIO_Init(GPIOB, GPIO_PIN_0, GPIO_MODE_OUTPUT_50MHZ, GPIO_CNF_AF_PP);
    GPIO_Init(GPIOB, GPIO_PIN_1, GPIO_MODE_OUTPUT_50MHZ, GPIO_CNF_AF_PP);
    
    /* 初始化 PWM - 1kHz */
    PWM_Init(TIM3, 1000);
    
    /* 启动所有通道 */
    PWM_Start(TIM3, PWM_CHANNEL_1);
    PWM_Start(TIM3, PWM_CHANNEL_2);
    PWM_Start(TIM3, PWM_CHANNEL_3);
    PWM_Start(TIM3, PWM_CHANNEL_4);
    
    /* 初始状态：停止 */
    Motor_Stop(1);
    Motor_Stop(2);
}

/**
  * @brief  设置电机速度
  * @param  motor_id: 电机编号 (1 或 2)
  * @param  speed: 速度 (-100 到 +100)
  *         正值: 正转, 负值: 反转, 0: 停止
  * @retval None
  */
void Motor_SetSpeed(uint8_t motor_id, int16_t speed)
{
    float duty;
    
    /* 限制速度范围 */
    if(speed > 100) speed = 100;
    if(speed < -100) speed = -100;
    
    duty = (speed >= 0) ? speed : -speed;
    
    if(motor_id == 1)
    {
        if(speed >= 0)
        {
            /* 正转 */
            PWM_SetDutyCycle(TIM3, PWM_CHANNEL_1, duty);
            PWM_SetDutyCycle(TIM3, PWM_CHANNEL_2, 0);
        }
        else
        {
            /* 反转 */
            PWM_SetDutyCycle(TIM3, PWM_CHANNEL_1, 0);
            PWM_SetDutyCycle(TIM3, PWM_CHANNEL_2, duty);
        }
    }
    else if(motor_id == 2)
    {
        if(speed >= 0)
        {
            /* 正转 */
            PWM_SetDutyCycle(TIM3, PWM_CHANNEL_3, duty);
            PWM_SetDutyCycle(TIM3, PWM_CHANNEL_4, 0);
        }
        else
        {
            /* 反转 */
            PWM_SetDutyCycle(TIM3, PWM_CHANNEL_3, 0);
            PWM_SetDutyCycle(TIM3, PWM_CHANNEL_4, duty);
        }
    }
}

/**
  * @brief  停止电机
  * @param  motor_id: 电机编号 (1 或 2)
  * @retval None
  */
void Motor_Stop(uint8_t motor_id)
{
    if(motor_id == 1)
    {
        PWM_SetDutyCycle(TIM3, PWM_CHANNEL_1, 0);
        PWM_SetDutyCycle(TIM3, PWM_CHANNEL_2, 0);
    }
    else if(motor_id == 2)
    {
        PWM_SetDutyCycle(TIM3, PWM_CHANNEL_3, 0);
        PWM_SetDutyCycle(TIM3, PWM_CHANNEL_4, 0);
    }
}

/**
  * @brief  初始化舵机控制
  * @note   使用 TIM2 CH1/CH2 控制舵机
  * @note   引脚: PA0(TIM2_CH1), PA1(TIM2_CH2)
  * @retval None
  */
void Servo_Init(void)
{
    /* 使能时钟 */
    RCC->APB1ENR |= (0x1 << 0);  /* TIM2 */
    RCC->APB2ENR |= (0x1 << 2);  /* GPIOA */
    
    /* 配置 GPIO */
    GPIO_Init(GPIOA, GPIO_PIN_0, GPIO_MODE_OUTPUT_50MHZ, GPIO_CNF_AF_PP);
    GPIO_Init(GPIOA, GPIO_PIN_1, GPIO_MODE_OUTPUT_50MHZ, GPIO_CNF_AF_PP);
    
    /* 初始化 PWM - 50Hz (舵机标准频率) */
    PWM_Init(TIM2, 50);
    
    /* 启动通道 */
    PWM_Start(TIM2, PWM_CHANNEL_1);
    PWM_Start(TIM2, PWM_CHANNEL_2);
    
    /* 设置初始角度 90度 */
    Servo_SetAngle(1, 90);
    Servo_SetAngle(2, 90);
}

/**
  * @brief  设置舵机角度
  * @param  servo_id: 舵机编号 (1 或 2)
  * @param  angle: 角度 (0-180度)
  * @retval None
  * 
  * @note   舵机脉冲宽度: 0.5ms(0度) - 2.5ms(180度)
  *         占空比计算: duty = (0.5ms + angle/180 * 2ms) / 20ms * 100%
  */
void Servo_SetAngle(uint8_t servo_id, uint16_t angle)
{
    float duty_cycle;
    
    /* 限制角度范围 */
    if(angle > 180) angle = 180;
    
    /* 计算占空比 
     * 0度   -> 0.5ms -> 2.5% 
     * 90度  -> 1.5ms -> 7.5%
     * 180度 -> 2.5ms -> 12.5%
     */
    duty_cycle = 2.5f + (angle / 180.0f) * 10.0f;
    
    if(servo_id == 1)
    {
        PWM_SetDutyCycle(TIM2, PWM_CHANNEL_1, duty_cycle);
    }
    else if(servo_id == 2)
    {
        PWM_SetDutyCycle(TIM2, PWM_CHANNEL_2, duty_cycle);
    }
}

