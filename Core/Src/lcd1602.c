/**
  ******************************************************************************
  * @file    lcd1602.c
  * @brief   LCD1602 字符显示屏驱动实现
  ******************************************************************************
  */

#include "lcd1602.h"
#include "gpio.h"
#include "delay.h"
#include <stdarg.h>
#include <stdio.h>

/* 私有函数声明 */
static void LCD_WriteNibble(uint8_t nibble);
static void LCD_WriteByte(uint8_t data, uint8_t rs);
static void LCD_WriteCommand(uint8_t cmd);
static void LCD_WriteData(uint8_t data);
static void LCD_Enable(void);

/**
  * @brief  初始化 LCD1602
  * @retval None
  */
void LCD1602_Init(void)
{
    /* 使能 GPIOB 时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    
    /* 配置 GPIO 为输出 */
    GPIO_Init(LCD_RS_PORT, LCD_RS_PIN, GPIO_MODE_OUTPUT_50MHZ, GPIO_CNF_OUTPUT_PP);
    GPIO_Init(LCD_RW_PORT, LCD_RW_PIN, GPIO_MODE_OUTPUT_50MHZ, GPIO_CNF_OUTPUT_PP);
    GPIO_Init(LCD_EN_PORT, LCD_EN_PIN, GPIO_MODE_OUTPUT_50MHZ, GPIO_CNF_OUTPUT_PP);
    GPIO_Init(LCD_D4_PORT, LCD_D4_PIN, GPIO_MODE_OUTPUT_50MHZ, GPIO_CNF_OUTPUT_PP);
    GPIO_Init(LCD_D5_PORT, LCD_D5_PIN, GPIO_MODE_OUTPUT_50MHZ, GPIO_CNF_OUTPUT_PP);
    GPIO_Init(LCD_D6_PORT, LCD_D6_PIN, GPIO_MODE_OUTPUT_50MHZ, GPIO_CNF_OUTPUT_PP);
    GPIO_Init(LCD_D7_PORT, LCD_D7_PIN, GPIO_MODE_OUTPUT_50MHZ, GPIO_CNF_OUTPUT_PP);
    
    /* 初始化引脚状态 */
    GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_RESET);
    GPIO_WritePin(LCD_RW_PORT, LCD_RW_PIN, GPIO_PIN_RESET);
    GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_RESET);
    
    /* 延时等待 LCD 上电稳定 */
    Delay_Ms(50);
    
    /* 初始化序列 (4位模式) */
    LCD_WriteNibble(0x03);
    Delay_Ms(5);
    LCD_WriteNibble(0x03);
    Delay_Us(150);
    LCD_WriteNibble(0x03);
    Delay_Us(150);
    LCD_WriteNibble(0x02);  /* 设置为4位模式 */
    Delay_Us(150);
    
    /* 功能设置: 4位接口, 2行, 5x7点阵 */
    LCD_WriteCommand(LCD_CMD_FUNCTION_SET);
    
    /* 显示设置: 显示开, 无光标 */
    LCD_WriteCommand(LCD_CMD_DISPLAY_ON);
    
    /* 清屏 */
    LCD1602_Clear();
    
    /* 输入模式: 光标右移 */
    LCD_WriteCommand(LCD_CMD_ENTRY_MODE);
    
    Delay_Ms(10);
}

/**
  * @brief  清屏
  * @retval None
  */
void LCD1602_Clear(void)
{
    LCD_WriteCommand(LCD_CMD_CLEAR);
    Delay_Ms(2);
}

/**
  * @brief  设置光标位置
  * @param  row: 行号 (0-1)
  * @param  col: 列号 (0-15)
  * @retval None
  */
void LCD1602_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t address;
    
    if(row == 0)
        address = 0x00 + col;  /* 第一行地址: 0x00-0x0F */
    else
        address = 0x40 + col;  /* 第二行地址: 0x40-0x4F */
    
    LCD_WriteCommand(0x80 | address);  /* 设置 DDRAM 地址 */
}

/**
  * @brief  打印字符串
  * @param  str: 字符串
  * @retval None
  */
void LCD1602_Print(const char *str)
{
    while(*str)
    {
        LCD_WriteData(*str++);
    }
}

/**
  * @brief  打印单个字符
  * @param  ch: 字符
  * @retval None
  */
void LCD1602_PrintChar(char ch)
{
    LCD_WriteData(ch);
}

/**
  * @brief  格式化打印 (类似 printf)
  * @param  row: 行号
  * @param  col: 列号
  * @param  format: 格式化字符串
  * @retval None
  */
void LCD1602_Printf(uint8_t row, uint8_t col, const char *format, ...)
{
    char buffer[17];  /* LCD1602 每行最多16个字符 */
    va_list args;
    
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    LCD1602_SetCursor(row, col);
    LCD1602_Print(buffer);
}

/**
  * @brief  创建自定义字符
  * @param  location: 字符位置 (0-7)
  * @param  charmap: 字符点阵数据 (8字节)
  * @retval None
  */
void LCD1602_CreateChar(uint8_t location, uint8_t charmap[8])
{
    uint8_t i;
    
    location &= 0x07;  /* 限制在 0-7 */
    LCD_WriteCommand(0x40 | (location << 3));
    
    for(i = 0; i < 8; i++)
    {
        LCD_WriteData(charmap[i]);
    }
    
    LCD_WriteCommand(LCD_CMD_HOME);
}

/**
  * @brief  打开显示
  * @retval None
  */
void LCD1602_DisplayOn(void)
{
    LCD_WriteCommand(LCD_CMD_DISPLAY_ON);
}

/**
  * @brief  关闭显示
  * @retval None
  */
void LCD1602_DisplayOff(void)
{
    LCD_WriteCommand(LCD_CMD_DISPLAY_OFF);
}

/**
  * @brief  写4位数据
  * @param  nibble: 4位数据
  * @retval None
  */
static void LCD_WriteNibble(uint8_t nibble)
{
    GPIO_WritePin(LCD_D4_PORT, LCD_D4_PIN, (nibble & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    GPIO_WritePin(LCD_D5_PORT, LCD_D5_PIN, (nibble & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    GPIO_WritePin(LCD_D6_PORT, LCD_D6_PIN, (nibble & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    GPIO_WritePin(LCD_D7_PORT, LCD_D7_PIN, (nibble & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    
    LCD_Enable();
}

/**
  * @brief  写8位数据
  * @param  data: 8位数据
  * @param  rs: RS引脚电平 (0=命令, 1=数据)
  * @retval None
  */
static void LCD_WriteByte(uint8_t data, uint8_t rs)
{
    /* 设置 RS 引脚 */
    GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, rs ? GPIO_PIN_SET : GPIO_PIN_RESET);
    
    /* RW = 0 (写模式) */
    GPIO_WritePin(LCD_RW_PORT, LCD_RW_PIN, GPIO_PIN_RESET);
    
    /* 写高4位 */
    LCD_WriteNibble(data >> 4);
    
    /* 写低4位 */
    LCD_WriteNibble(data & 0x0F);
    
    Delay_Us(50);
}

/**
  * @brief  写命令
  * @param  cmd: 命令字节
  * @retval None
  */
static void LCD_WriteCommand(uint8_t cmd)
{
    LCD_WriteByte(cmd, 0);
}

/**
  * @brief  写数据
  * @param  data: 数据字节
  * @retval None
  */
static void LCD_WriteData(uint8_t data)
{
    LCD_WriteByte(data, 1);
}

/**
  * @brief  使能脉冲
  * @retval None
  */
static void LCD_Enable(void)
{
    GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_SET);
    Delay_Us(1);
    GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_RESET);
    Delay_Us(1);
}

