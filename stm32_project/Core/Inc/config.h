/**
  ******************************************************************************
  * @file    config.h
  * @brief   项目配置文件 - 集中管理所有配置参数
  ******************************************************************************
  */

#ifndef __CONFIG_H
#define __CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*/
/* 系统配置                                                                    */
/*============================================================================*/

/* 系统时钟频率 (Hz) */
#define SYSTEM_CLOCK_FREQ       72000000UL

/* 外部高速时钟 (Hz) */
#define HSE_FREQ                8000000UL

/* PLL 倍频系数 */
#define PLL_MUL                 9

/*============================================================================*/
/* 串口配置                                                                    */
/*============================================================================*/

/* USART1 配置 */
#define USART1_BAUDRATE         115200
#define USART1_TX_PIN           GPIO_PIN_9
#define USART1_RX_PIN           GPIO_PIN_10
#define USART1_PORT             GPIOA

/* USART2 配置 (如果使用) */
#define USART2_BAUDRATE         9600
#define USART2_TX_PIN           GPIO_PIN_2
#define USART2_RX_PIN           GPIO_PIN_3
#define USART2_PORT             GPIOA

/*============================================================================*/
/* GPIO 配置                                                                   */
/*============================================================================*/

/* LED 配置 */
#define LED_PIN                 GPIO_PIN_13
#define LED_PORT                GPIOC
#define LED_ON()                GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET)
#define LED_OFF()               GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET)
#define LED_TOGGLE()            GPIO_TogglePin(LED_PORT, LED_PIN)

/* 按键配置 (示例 - 如果使用) */
#define BUTTON_PIN              GPIO_PIN_0
#define BUTTON_PORT             GPIOA
#define BUTTON_READ()           GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN)

/*============================================================================*/
/* 定时器配置                                                                  */
/*============================================================================*/

/* SysTick 配置 */
#define SYSTICK_FREQ            1000    /* 1000Hz = 1ms tick */

/*============================================================================*/
/* 调试配置                                                                    */
/*============================================================================*/

/* 启用调试输出 */
#define DEBUG_ENABLE            1

/* 调试串口 */
#define DEBUG_UART              USART1

/* 调试宏 */
#if DEBUG_ENABLE
    #define DEBUG_PRINT(fmt, ...) UART_Printf(DEBUG_UART, fmt, ##__VA_ARGS__)
#else
    #define DEBUG_PRINT(fmt, ...)
#endif

/*============================================================================*/
/* 应用配置                                                                    */
/*============================================================================*/

/* LED 闪烁周期 (ms) */
#define LED_BLINK_PERIOD        500

/* 心跳消息间隔 (ms) */
#define HEARTBEAT_INTERVAL      1000

/* 版本信息 */
#define FIRMWARE_VERSION        "1.0.0"
#define FIRMWARE_DATE           __DATE__
#define FIRMWARE_TIME           __TIME__

#ifdef __cplusplus
}
#endif

#endif /* __CONFIG_H */

