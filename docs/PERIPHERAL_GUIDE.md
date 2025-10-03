# STM32F103 外设使用指南

本文档详细介绍项目中所有外设驱动的使用方法。

---

## 📡 目录

1. [PWM 驱动 - 电机和舵机控制](#pwm-驱动)
2. [LCD1602 驱动 - 字符显示屏](#lcd1602-驱动)
3. [ADC 驱动 - 模拟信号采集](#adc-驱动)
4. [GPIO 驱动 - 通用输入输出](#gpio-驱动)
5. [UART 驱动 - 串口通信](#uart-驱动)
6. [延时函数 - 精确定时](#延时函数)

---

## 🔄 PWM 驱动

### 功能简介
PWM (脉冲宽度调制) 用于：
- **直流电机速度控制**
- **舵机角度控制**
- **LED 调光**
- **蜂鸣器音调控制**

### 硬件连接

#### 电机控制 (使用 L298N 或 TB6612)
```
STM32         →  电机驱动板
──────────────────────────────
PA6 (TIM3_CH1) →  IN1 (电机1+)
PA7 (TIM3_CH2) →  IN2 (电机1-)
PB0 (TIM3_CH3) →  IN3 (电机2+)
PB1 (TIM3_CH4) →  IN4 (电机2-)
GND            →  GND

电机驱动板     →  电机
──────────────────────────────
OUT1/OUT2      →  电机1
OUT3/OUT4      →  电机2
VCC            →  电机电源 (6-12V)
```

#### 舵机控制
```
STM32         →  舵机
──────────────────────────────
PA0 (TIM2_CH1) →  信号线 (橙/黄)
3.3V/5V        →  VCC (红)
GND            →  GND (棕/黑)
```

### 代码示例

#### 1. 初始化电机
```c
#include "pwm.h"

int main(void)
{
    /* 初始化电机控制 */
    Motor_Init();
    
    while(1)
    {
        /* 电机1 正转，速度 80% */
        Motor_SetSpeed(1, 80);
        Delay_Ms(2000);
        
        /* 电机1 反转，速度 60% */
        Motor_SetSpeed(1, -60);
        Delay_Ms(2000);
        
        /* 停止电机 */
        Motor_Stop(1);
        Delay_Ms(1000);
    }
}
```

#### 2. 控制舵机
```c
#include "pwm.h"

int main(void)
{
    /* 初始化舵机 */
    Servo_Init();
    
    while(1)
    {
        /* 舵机转到 0 度 */
        Servo_SetAngle(1, 0);
        Delay_Ms(1000);
        
        /* 舵机转到 90 度 */
        Servo_SetAngle(1, 90);
        Delay_Ms(1000);
        
        /* 舵机转到 180 度 */
        Servo_SetAngle(1, 180);
        Delay_Ms(1000);
    }
}
```

#### 3. 自定义 PWM
```c
#include "pwm.h"

int main(void)
{
    /* 初始化 TIM3，频率 1kHz */
    PWM_Init(TIM3, 1000);
    
    /* 启动通道 1 */
    PWM_Start(TIM3, PWM_CHANNEL_1);
    
    while(1)
    {
        /* 设置占空比 50% */
        PWM_SetDutyCycle(TIM3, PWM_CHANNEL_1, 50.0f);
        Delay_Ms(1000);
        
        /* 设置占空比 75% */
        PWM_SetDutyCycle(TIM3, PWM_CHANNEL_1, 75.0f);
        Delay_Ms(1000);
    }
}
```

### API 参考

| 函数 | 参数 | 功能 |
|------|------|------|
| `Motor_Init()` | 无 | 初始化电机控制 |
| `Motor_SetSpeed(id, speed)` | id: 1或2<br>speed: -100~+100 | 设置电机速度 |
| `Motor_Stop(id)` | id: 1或2 | 停止电机 |
| `Servo_Init()` | 无 | 初始化舵机 |
| `Servo_SetAngle(id, angle)` | id: 1或2<br>angle: 0~180 | 设置舵机角度 |
| `PWM_Init(TIMx, freq)` | TIMx: 定时器<br>freq: 频率(Hz) | 初始化 PWM |
| `PWM_SetDutyCycle(TIMx, ch, duty)` | ch: 通道<br>duty: 0~100 | 设置占空比 |

---

## 🖥️ LCD1602 驱动

### 功能简介
LCD1602 是一款字符型液晶显示屏：
- **2行 x 16列** 字符显示
- **支持自定义字符**
- **4位并口模式**（节省IO口）

### 硬件连接
```
LCD1602引脚    →  STM32
────────────────────────────
VSS (GND)      →  GND
VDD (电源)     →  5V
V0  (对比度)   →  可调电阻中间端
RS  (寄存器)   →  PB12
RW  (读写)     →  PB13
EN  (使能)     →  PB14
D0-D3          →  (不连接，4位模式)
D4             →  PB8
D5             →  PB9
D6             →  PB10
D7             →  PB11
A   (背光+)    →  5V (或串220Ω电阻)
K   (背光-)    →  GND
```

**对比度调节电路：**
```
5V ─┬─ 10kΩ 可调电阻 ─┬─ GND
    │                  │
    └──────> V0 <──────┘
```

### 代码示例

#### 1. 基本显示
```c
#include "lcd1602.h"

int main(void)
{
    LCD1602_Init();
    
    /* 清屏 */
    LCD1602_Clear();
    
    /* 第1行第0列开始显示 */
    LCD1602_Printf(0, 0, "Hello STM32!");
    
    /* 第2行第0列开始显示 */
    LCD1602_Printf(1, 0, "LCD1602 Test");
    
    while(1);
}
```

#### 2. 动态显示
```c
#include "lcd1602.h"
#include "delay.h"

int main(void)
{
    uint32_t counter = 0;
    float temperature = 25.6;
    
    LCD1602_Init();
    LCD1602_Clear();
    LCD1602_Printf(0, 0, "Count:");
    LCD1602_Printf(1, 0, "Temp:");
    
    while(1)
    {
        /* 更新计数 */
        LCD1602_Printf(0, 7, "%lu   ", counter++);
        
        /* 更新温度 */
        LCD1602_Printf(1, 6, "%.1fC ", temperature);
        
        Delay_Ms(1000);
    }
}
```

#### 3. 自定义字符
```c
/* 心形图案 */
uint8_t heart[] = {
    0x00, 0x0A, 0x1F, 0x1F,
    0x0E, 0x04, 0x00, 0x00
};

int main(void)
{
    LCD1602_Init();
    
    /* 创建自定义字符（位置0） */
    LCD1602_CreateChar(0, heart);
    
    LCD1602_Clear();
    LCD1602_Print("I ");
    LCD1602_PrintChar(0);  /* 显示心形 */
    LCD1602_Print(" STM32!");
    
    while(1);
}
```

### API 参考

| 函数 | 功能 |
|------|------|
| `LCD1602_Init()` | 初始化 LCD |
| `LCD1602_Clear()` | 清屏 |
| `LCD1602_SetCursor(row, col)` | 设置光标位置 |
| `LCD1602_Print(str)` | 打印字符串 |
| `LCD1602_PrintChar(ch)` | 打印单个字符 |
| `LCD1602_Printf(row, col, fmt, ...)` | 格式化打印 |
| `LCD1602_CreateChar(loc, map[8])` | 创建自定义字符 |

---

## 📊 ADC 驱动

### 功能简介
ADC (模数转换器) 用于采集模拟信号：
- **12位分辨率** (0-4095)
- **参考电压 3.3V**
- **10个通道** (PA0-PA7, PB0-PB1)

### 硬件连接

#### 电位器
```
3.3V ─── 电位器 ─── PA0
             │
            GND
```

#### 光敏电阻
```
3.3V ─── 光敏电阻 ─── PA1 ─── 10kΩ ─── GND
```

#### LM35 温度传感器
```
LM35
  VCC  →  5V (或3.3V)
  OUT  →  PA2
  GND  →  GND
```

### 代码示例

#### 1. 读取电压
```c
#include "adc.h"

int main(void)
{
    float voltage;
    
    ADC_Init();
    
    while(1)
    {
        /* 读取 PA0 电压 */
        voltage = ADC_ReadVoltage(ADC_CHANNEL_0);
        
        printf("电压: %.2fV\r\n", voltage);
        Delay_Ms(1000);
    }
}
```

#### 2. 读取ADC原始值
```c
uint16_t adc_value;

ADC_Init();

while(1)
{
    /* 读取 ADC 原始值 (0-4095) */
    adc_value = ADC_Read(ADC_CHANNEL_0);
    
    printf("ADC值: %u\r\n", adc_value);
    Delay_Ms(100);
}
```

#### 3. 平均采样（降噪）
```c
uint16_t adc_avg;

ADC_Init();

while(1)
{
    /* 采样10次取平均 */
    adc_avg = ADC_ReadAverage(ADC_CHANNEL_0, 10);
    
    printf("平均ADC值: %u\r\n", adc_avg);
    Delay_Ms(500);
}
```

#### 4. 温度测量 (LM35)
```c
float temperature;

ADC_Init();

while(1)
{
    /* LM35: 10mV/°C */
    float voltage = ADC_ReadVoltage(ADC_CHANNEL_2);
    temperature = voltage * 100.0f;
    
    printf("温度: %.1f°C\r\n", temperature);
    Delay_Ms(1000);
}
```

### API 参考

| 函数 | 返回值 | 功能 |
|------|--------|------|
| `ADC_Init()` | 无 | 初始化 ADC |
| `ADC_Read(channel)` | 0-4095 | 读取 ADC 值 |
| `ADC_ReadVoltage(channel)` | 0-3.3V | 读取电压值 |
| `ADC_ReadAverage(channel, times)` | 0-4095 | 读取平均值 |

---

## 🔌 GPIO 驱动

### 功能简介
GPIO (通用输入输出) 是最基本的外设：
- **数字输入/输出**
- **8种配置模式**
- **支持中断**（EXTI）

### 代码示例

#### 1. LED 控制
```c
#include "gpio.h"

/* 初始化 LED (PC13) */
RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
GPIO_Init(GPIOC, GPIO_PIN_13, GPIO_MODE_OUTPUT_50MHZ, GPIO_CNF_OUTPUT_PP);

/* 点亮 LED */
GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

/* 熄灭 LED */
GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

/* 翻转 LED */
GPIO_TogglePin(GPIOC, GPIO_PIN_13);
```

#### 2. 按键检测
```c
/* 初始化按键 (PA0，带下拉电阻) */
RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
GPIO_Init(GPIOA, GPIO_PIN_0, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PUPD);

/* 读取按键状态 */
if(GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)
{
    /* 按键按下 */
}
```

---

## 📡 UART 驱动

### 功能简介
UART (串口通信) 用于：
- **调试输出**
- **数据传输**
- **模块通信** (蓝牙、WiFi、GPS等)

### 代码示例

#### 1. 基本输出
```c
#include "uart.h"

UART_Init(USART1, 115200);

UART_SendString(USART1, "Hello UART!\r\n");
```

#### 2. Printf 风格
```c
int value = 42;
float temp = 25.6;

UART_Printf(USART1, "Value: %d, Temp: %.1f°C\r\n", value, temp);
```

#### 3. 接收数据
```c
char received;

while(1)
{
    received = UART_ReceiveChar(USART1);
    UART_SendChar(USART1, received);  /* 回显 */
}
```

---

## ⏱️ 延时函数

### 代码示例

```c
#include "delay.h"

/* 初始化（仅需一次） */
Delay_Init();

/* 毫秒延时 */
Delay_Ms(1000);  /* 延时 1 秒 */

/* 微秒延时 */
Delay_Us(100);   /* 延时 100 微秒 */

/* 获取系统时钟 */
uint32_t tick = GetTick();  /* 获取系统运行时间(ms) */
```

---

## 🎯 综合示例

### 多功能控制系统
结合所有外设的完整示例：

```c
#include "stm32f1xx.h"
#include "gpio.h"
#include "uart.h"
#include "adc.h"
#include "pwm.h"
#include "lcd1602.h"
#include "delay.h"

int main(void)
{
    uint16_t adc_value;
    int16_t motor_speed;
    
    /* 初始化所有外设 */
    SystemInit();
    Delay_Init();
    UART_Init(USART1, 115200);
    ADC_Init();
    Motor_Init();
    LCD1602_Init();
    
    /* LCD 显示标题 */
    LCD1602_Printf(0, 0, "Speed Control");
    
    while(1)
    {
        /* 读取电位器值 */
        adc_value = ADC_Read(ADC_CHANNEL_0);
        
        /* 计算电机速度 (0-100%) */
        motor_speed = (adc_value * 100) / 4095;
        
        /* 控制电机 */
        Motor_SetSpeed(1, motor_speed);
        
        /* LCD 显示速度 */
        LCD1602_Printf(1, 0, "Speed: %d%%   ", motor_speed);
        
        /* UART 输出 */
        UART_Printf(USART1, "ADC:%u Speed:%d%%\r\n", 
                    adc_value, motor_speed);
        
        Delay_Ms(100);
    }
}
```

---

## 📚 更多示例

完整示例程序位于 `examples/` 目录：
- `motor_control.c` - 电机控制示例
- `lcd_display.c` - LCD 显示示例
- `adc_sensor.c` - ADC 采集示例
- `comprehensive_demo.c` - 综合示例

---

**提示：** 所有示例均已测试可用，可直接复制到 `main.c` 运行！

