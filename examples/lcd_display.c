/**
  ******************************************************************************
  * @file    lcd_display.c
  * @brief   LCD1602 显示示例程序
  ******************************************************************************
  */

/*
使用方法：
将此文件内容复制到 Core/Src/main.c 即可运行此示例

功能：
- LCD1602 字符显示
- 显示静态文本、动态数字、自定义字符
- 演示各种显示效果

硬件连接 (4位并口模式):
- PB12: LCD RS
- PB13: LCD RW
- PB14: LCD EN
- PB8:  LCD D4
- PB9:  LCD D5
- PB10: LCD D6
- PB11: LCD D7
- VSS → GND
- VDD → 5V
- V0  → 可调电阻 (对比度调节)
- A   → 5V (背光+)
- K   → GND (背光-)
*/

#include "stm32f1xx.h"
#include "system_stm32f1xx.h"
#include "lcd1602.h"
#include "delay.h"

/* 自定义字符: 心形 ♥ */
uint8_t heart[] = {
    0x00, 0x0A, 0x1F, 0x1F,
    0x0E, 0x04, 0x00, 0x00
};

/* 自定义字符: 笑脸 ☺ */
uint8_t smile[] = {
    0x00, 0x0A, 0x0A, 0x00,
    0x11, 0x0E, 0x00, 0x00
};

/* 自定义字符: 温度 */
uint8_t temp[] = {
    0x04, 0x0A, 0x0A, 0x0A,
    0x0A, 0x11, 0x11, 0x0E
};

int main(void)
{
    uint32_t counter = 0;
    
    /* 系统初始化 */
    SystemInit();
    
    /* 初始化外设 */
    Delay_Init();
    LCD1602_Init();
    
    /* 创建自定义字符 */
    LCD1602_CreateChar(0, heart);
    LCD1602_CreateChar(1, smile);
    LCD1602_CreateChar(2, temp);
    
    /* 显示欢迎信息 */
    LCD1602_Clear();
    LCD1602_Printf(0, 0, "  STM32F103  ");
    LCD1602_Printf(1, 0, "   LCD1602   ");
    Delay_Ms(2000);
    
    /* 显示自定义字符 */
    LCD1602_Clear();
    LCD1602_Printf(0, 0, "Custom Char:");
    LCD1602_SetCursor(1, 0);
    LCD1602_PrintChar(0);  /* 心形 */
    LCD1602_PrintChar(' ');
    LCD1602_PrintChar(1);  /* 笑脸 */
    LCD1602_PrintChar(' ');
    LCD1602_PrintChar(2);  /* 温度 */
    Delay_Ms(2000);
    
    /* 主循环: 显示计数器 */
    LCD1602_Clear();
    LCD1602_Printf(0, 0, "Counter Demo:");
    
    while(1)
    {
        /* 更新计数器 */
        LCD1602_Printf(1, 0, "Value: %lu   ", counter);
        counter++;
        
        /* 每10次显示一个心形 */
        if(counter % 10 == 0)
        {
            LCD1602_SetCursor(1, 15);
            LCD1602_PrintChar(0);  /* 心形 */
        }
        
        Delay_Ms(500);
        
        /* 每100次清除心形 */
        if(counter % 100 == 0)
        {
            LCD1602_SetCursor(1, 15);
            LCD1602_PrintChar(' ');
        }
        
        /* 演示不同的显示效果 */
        if(counter == 50)
        {
            LCD1602_Clear();
            LCD1602_Printf(0, 0, "Temperature:");
            LCD1602_SetCursor(0, 13);
            LCD1602_PrintChar(2);  /* 温度图标 */
            LCD1602_Printf(1, 0, "25.6");
            LCD1602_PrintChar(0xDF);  /* 度数符号 */
            LCD1602_Print("C");
        }
        else if(counter == 100)
        {
            LCD1602_Clear();
            LCD1602_Printf(0, 0, "Voltage:");
            LCD1602_Printf(1, 0, "3.30 V");
        }
        else if(counter == 150)
        {
            LCD1602_Clear();
            LCD1602_Printf(0, 0, "STM32 Project");
            LCD1602_SetCursor(1, 0);
            LCD1602_PrintChar(1);  /* 笑脸 */
            LCD1602_Print(" Ready! ");
            LCD1602_PrintChar(1);
        }
        else if(counter >= 200)
        {
            /* 重新开始 */
            counter = 0;
            LCD1602_Clear();
            LCD1602_Printf(0, 0, "Counter Demo:");
        }
    }
}

