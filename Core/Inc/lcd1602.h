/**
  ******************************************************************************
  * @file    lcd1602.h
  * @brief   LCD1602 字符显示屏驱动 (4位并口模式)
  ******************************************************************************
  */

#ifndef __LCD1602_H
#define __LCD1602_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx.h"

/* LCD1602 引脚配置 (可根据实际硬件修改) */
#define LCD_RS_PORT     GPIOB
#define LCD_RS_PIN      GPIO_PIN_12

#define LCD_RW_PORT     GPIOB
#define LCD_RW_PIN      GPIO_PIN_13

#define LCD_EN_PORT     GPIOB
#define LCD_EN_PIN      GPIO_PIN_14

#define LCD_D4_PORT     GPIOB
#define LCD_D4_PIN      GPIO_PIN_8

#define LCD_D5_PORT     GPIOB
#define LCD_D5_PIN      GPIO_PIN_9

#define LCD_D6_PORT     GPIOB
#define LCD_D6_PIN      GPIO_PIN_10

#define LCD_D7_PORT     GPIOB
#define LCD_D7_PIN      GPIO_PIN_11

/* LCD1602 命令 */
#define LCD_CMD_CLEAR           0x01  /* 清屏 */
#define LCD_CMD_HOME            0x02  /* 光标回home */
#define LCD_CMD_ENTRY_MODE      0x06  /* 光标右移 */
#define LCD_CMD_DISPLAY_ON      0x0C  /* 显示开，无光标，不闪烁 */
#define LCD_CMD_DISPLAY_OFF     0x08  /* 显示关 */
#define LCD_CMD_CURSOR_ON       0x0E  /* 显示开，有光标 */
#define LCD_CMD_CURSOR_BLINK    0x0F  /* 显示开，光标闪烁 */
#define LCD_CMD_FUNCTION_SET    0x28  /* 4位接口，2行，5x7点阵 */

/* 函数原型 */
void LCD1602_Init(void);
void LCD1602_Clear(void);
void LCD1602_SetCursor(uint8_t row, uint8_t col);
void LCD1602_Print(const char *str);
void LCD1602_PrintChar(char ch);
void LCD1602_Printf(uint8_t row, uint8_t col, const char *format, ...);
void LCD1602_CreateChar(uint8_t location, uint8_t charmap[8]);
void LCD1602_DisplayOn(void);
void LCD1602_DisplayOff(void);

#ifdef __cplusplus
}
#endif

#endif /* __LCD1602_H */

