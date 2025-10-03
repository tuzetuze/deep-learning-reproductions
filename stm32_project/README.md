# STM32F103C8T6 完整项目

这是一个**生产级别的 STM32F103C8T6 嵌入式项目**，包含完整的硬件抽象层、驱动程序和示例应用。

## 📋 项目特性

### 核心功能
- ✅ **完整的启动代码** - 包含中断向量表和系统初始化
- ✅ **时钟配置** - 72MHz 系统时钟（HSE + PLL）
- ✅ **GPIO 驱动** - 灵活的引脚配置和控制
- ✅ **UART 通信** - 支持 printf 风格的串口输出
- ✅ **精确延时** - 基于 SysTick 的毫秒/微秒级延时
- ✅ **示例程序** - LED 闪烁 + UART 调试输出

### 技术规格
| 项目 | 规格 |
|------|------|
| MCU | STM32F103C8T6 (Cortex-M3) |
| Flash | 64KB |
| RAM | 20KB |
| 系统时钟 | 72MHz |
| 工具链 | ARM GCC |
| 调试器 | ST-Link V2 |

## 🗂️ 项目结构

```
stm32_project/
├── Core/
│   ├── Inc/                    # 头文件
│   │   ├── stm32f1xx.h        # 设备头文件
│   │   ├── stm32f103xb.h      # MCU 具体定义
│   │   ├── system_stm32f1xx.h # 系统配置
│   │   ├── gpio.h             # GPIO 驱动
│   │   ├── uart.h             # UART 驱动
│   │   └── delay.h            # 延时函数
│   └── Src/                    # 源文件
│       ├── main.c             # 主程序
│       ├── system_stm32f1xx.c # 系统初始化
│       ├── gpio.c             # GPIO 实现
│       ├── uart.c             # UART 实现
│       └── delay.c            # 延时实现
├── Drivers/
│   └── CMSIS/                 # CMSIS 标准接口
│       └── Include/
│           └── core_cm3.h     # Cortex-M3 核心定义
├── Startup/
│   └── startup_stm32f103xb.s  # 启动文件（汇编）
├── Linker/
│   └── STM32F103C8Tx_FLASH.ld # 链接脚本
├── Makefile                    # 构建系统
└── README.md                   # 本文档
```

## 🔧 环境配置

### 必需工具

1. **ARM GCC 工具链**
   ```bash
   # Ubuntu/Debian
   sudo apt-get install gcc-arm-none-eabi

   # macOS
   brew install arm-none-eabi-gcc

   # Windows
   # 下载并安装: https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm
   ```

2. **ST-Link 工具**（用于烧录）
   ```bash
   # Ubuntu/Debian
   sudo apt-get install stlink-tools

   # macOS
   brew install stlink

   # Windows
   # 下载: https://github.com/stlink-org/stlink/releases
   ```

3. **Make 工具**
   ```bash
   # Ubuntu/Debian
   sudo apt-get install build-essential

   # Windows
   # 使用 MinGW 或 MSYS2
   ```

### 可选工具

- **OpenOCD** - 用于调试
- **PuTTY/minicom** - 串口终端
- **STM32CubeMX** - 图形化配置工具（如需修改配置）

## 🚀 快速开始

### 1. 编译项目

```bash
cd stm32_project
make
```

编译成功后会在 `build/` 目录生成：
- `stm32f103_project.elf` - ELF 可执行文件
- `stm32f103_project.hex` - HEX 固件文件
- `stm32f103_project.bin` - BIN 二进制文件
- `stm32f103_project.map` - 内存映射文件

### 2. 烧录到硬件

**使用 ST-Link：**
```bash
make flash
```

**或手动烧录：**
```bash
st-flash write build/stm32f103_project.bin 0x8000000
```

### 3. 查看串口输出

连接 UART1（PA9=TX, PA10=RX），波特率 115200：

```bash
# Linux/macOS
minicom -D /dev/ttyUSB0 -b 115200

# Windows
# 使用 PuTTY 或 串口助手
```

预期输出：
```
========================================
  STM32F103C8T6 Complete Project
  System Clock: 72MHz
  UART Baud Rate: 115200
========================================
System initialized successfully!

[0] LED Toggle - System Running OK
[1] LED Toggle - System Running OK
[2] LED Toggle - System Running OK
...
```

### 4. 清理编译文件

```bash
make clean
```

## 📌 硬件连接

### 最小系统
```
STM32F103C8T6
├── VDD    → 3.3V
├── GND    → GND
├── BOOT0  → GND (正常运行模式)
├── NRST   → 10kΩ 上拉 + 复位按钮
├── PC13   → LED（板载）
├── PA9    → UART TX
├── PA10   → UART RX
└── OSC_IN/OUT → 8MHz 晶振
```

### ST-Link 连接
```
ST-Link V2  →  STM32
  SWDIO    →  SWDIO
  SWCLK    →  SWCLK
  GND      →  GND
  3.3V     →  3.3V (可选供电)
```

## 🎯 功能说明

### 主程序功能

当前示例程序实现以下功能：

1. **系统初始化**
   - 配置 HSE + PLL 达到 72MHz
   - 初始化系统时钟
   - 启用外设时钟

2. **LED 闪烁**
   - PC13 LED 每 500ms 翻转一次
   - 视觉指示系统正常运行

3. **UART 调试输出**
   - 115200 波特率
   - 每次 LED 翻转时输出状态信息
   - 支持 printf 风格的格式化输出

### 时钟配置

```
HSE (8MHz) → PLL (x9) → SYSCLK (72MHz)
                       ↓
                    AHB (72MHz)
                    ├── APB1 (36MHz) - USART2, TIM2-4
                    └── APB2 (72MHz) - USART1, GPIO
```

## 📝 自定义开发

### 添加新的外设

1. **创建驱动文件**
   ```bash
   # 在 Core/Inc/ 创建头文件
   # 在 Core/Src/ 创建源文件
   ```

2. **修改 Makefile**
   ```makefile
   C_SOURCES += Core/Src/your_driver.c
   ```

3. **在 main.c 中使用**
   ```c
   #include "your_driver.h"
   ```

### 修改时钟配置

编辑 `Core/Src/system_stm32f1xx.c` 中的 `SetSysClock()` 函数。

### 添加中断处理

在 `main.c` 或单独文件中定义中断处理函数：

```c
void TIM2_IRQHandler(void)
{
    // 你的中断处理代码
}
```

## 🐛 常见问题

### 编译错误

**问题：** `arm-none-eabi-gcc: command not found`

**解决：** 确保 ARM GCC 工具链已安装并在 PATH 中

---

**问题：** 链接错误（内存不足）

**解决：** 检查 `STM32F103C8Tx_FLASH.ld` 中的内存配置

### 烧录问题

**问题：** `Error: libusb not found`

**解决：** 
```bash
sudo apt-get install libusb-1.0-0-dev
```

---

**问题：** ST-Link 无法识别

**解决：** 
- 检查 USB 连接
- 检查 ST-Link 驱动程序
- 尝试：`st-info --probe`

### 运行问题

**问题：** LED 不闪烁

**解决：** 
- 检查 BOOT0 是否接地
- 检查电源供电
- 用调试器检查程序是否运行

---

**问题：** 串口无输出

**解决：** 
- 确认波特率为 115200
- 检查 TX/RX 引脚连接
- 确认 USB-TTL 模块工作正常

## 📚 代码示例

### GPIO 控制示例

```c
// 初始化 LED 引脚
GPIO_Init(GPIOC, GPIO_PIN_13, GPIO_MODE_OUTPUT_50MHZ, GPIO_CNF_OUTPUT_PP);

// 点亮 LED
GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

// 熄灭 LED
GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

// 翻转 LED
GPIO_TogglePin(GPIOC, GPIO_PIN_13);
```

### UART 输出示例

```c
// 初始化 UART
UART_Init(USART1, 115200);

// 发送字符串
UART_SendString(USART1, "Hello STM32!\r\n");

// 格式化输出
int value = 42;
UART_Printf(USART1, "Value = %d\r\n", value);
```

### 延时示例

```c
// 初始化延时功能
Delay_Init();

// 延时 1 秒
Delay_Ms(1000);

// 延时 100 微秒
Delay_Us(100);

// 获取系统运行时间
uint32_t tick = GetTick();
```

## 🔬 高级功能扩展建议

### 1. 添加 SPI 驱动
- 用于连接 SD 卡、LCD、传感器等

### 2. 添加 I2C 驱动
- 用于 EEPROM、RTC、温湿度传感器等

### 3. 添加 PWM 功能
- 用于电机控制、LED 调光等

### 4. 添加 ADC 采集
- 用于模拟信号采集

### 5. 实现 FreeRTOS
- 多任务实时操作系统

### 6. 添加 DMA 支持
- 高效的数据传输

## 📖 参考资料

- [STM32F103 数据手册](https://www.st.com/resource/en/datasheet/stm32f103c8.pdf)
- [STM32F103 参考手册](https://www.st.com/resource/en/reference_manual/cd00171190.pdf)
- [Cortex-M3 编程手册](https://developer.arm.com/documentation/ddi0337/e/)
- [ARM GCC 工具链文档](https://gcc.gnu.org/onlinedocs/)

## 📄 许可证

本项目采用 MIT 许可证。您可以自由使用、修改和分发。

## 🤝 贡献

欢迎提交 Issue 和 Pull Request！

## 📧 联系方式

如有问题，欢迎提交 GitHub Issue。

---

**项目状态：** ✅ 生产就绪

**最后更新：** 2025-10-03

**版本：** 1.0.0

