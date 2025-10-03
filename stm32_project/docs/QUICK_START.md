# STM32F103 快速入门指南

## 🚀 5分钟快速开始

### 第一步：安装工具链（仅需一次）

#### Windows 系统
```powershell
# 1. 下载并安装 ARM GCC 工具链
# 访问：https://developer.arm.com/downloads/-/gnu-rm
# 下载 gcc-arm-none-eabi-XX-win32.exe 并安装
# 确保安装时勾选"添加到 PATH"

# 2. 下载并安装 ST-Link 驱动和工具
# 访问：https://www.st.com/en/development-tools/stsw-link009.html

# 3. 安装 Make 工具（选择一个）
# 方案A：安装 MinGW (推荐)
# 访问：http://www.mingw.org/
# 方案B：安装 MSYS2
# 访问：https://www.msys2.org/

# 4. 验证安装
arm-none-eabi-gcc --version
make --version
st-flash --version
```

#### Linux 系统 (Ubuntu/Debian)
```bash
# 一键安装所有工具
sudo apt-get update
sudo apt-get install -y \
    gcc-arm-none-eabi \
    gdb-arm-none-eabi \
    stlink-tools \
    build-essential \
    git

# 验证安装
arm-none-eabi-gcc --version
st-flash --version
```

#### macOS 系统
```bash
# 使用 Homebrew 安装
brew tap osx-cross/arm
brew install arm-gcc-bin
brew install stlink

# 验证安装
arm-none-eabi-gcc --version
st-flash --version
```

### 第二步：获取项目

```bash
# 如果你已经有项目文件夹
cd stm32_project

# 查看项目结构
ls -la
```

### 第三步：编译项目

#### Windows
```cmd
# 进入项目目录
cd stm32_project

# 使用批处理脚本编译（推荐）
build.bat build

# 或使用 make
make
```

#### Linux/macOS
```bash
# 进入项目目录
cd stm32_project

# 赋予脚本执行权限
chmod +x build.sh

# 使用脚本编译（推荐）
./build.sh build

# 或使用 make
make
```

**编译成功标志：**
```
[INFO] 编译成功！
   text    data     bss     dec     hex filename
   4256     108     536    4900    1324 build/stm32f103_project.elf
```

### 第四步：硬件连接

#### ST-Link 连接
```
ST-Link V2 → STM32F103C8T6
────────────────────────────
SWDIO      → PA13 (SWDIO)
SWCLK      → PA14 (SWCLK)
GND        → GND
3.3V       → 3.3V (可选)
```

#### UART 连接（用于查看输出）
```
USB-TTL    → STM32F103C8T6
────────────────────────────
TX         → PA10 (RX1)  ← 交叉
RX         → PA9  (TX1)  ← 交叉
GND        → GND
```

### 第五步：烧录程序

#### Windows
```cmd
# 确保开发板已通过 ST-Link 连接
build.bat flash

# 或手动烧录
st-flash write build\stm32f103_project.bin 0x8000000
```

#### Linux/macOS
```bash
# 确保开发板已通过 ST-Link 连接
./build.sh flash

# 或手动烧录
st-flash write build/stm32f103_project.bin 0x8000000
```

**烧录成功标志：**
```
st-flash 1.7.0
2024-10-03T10:00:00 INFO common.c: F1xx Medium-density: 64 KiB SRAM, 128 KiB flash
2024-10-03T10:00:00 INFO common.c: Attempting to write 4364 (0x110c) bytes to stm32 address: 134217728 (0x8000000)
Flash page at addr: 0x08001000 erased
2024-10-03T10:00:00 INFO common.c: Finished erasing 5 pages of 1024 (0x400) bytes
2024-10-03T10:00:00 INFO common.c: Starting Flash write for F2/F4/L4
2024-10-03T10:00:00 INFO flash_loader.c: Successfully loaded flash loader in sram
2024-10-03T10:00:00 INFO common.c: wrote 4364 bytes to 0x8000000
```

### 第六步：查看运行结果

#### 观察 LED
- PC13 板载 LED 应该每 500ms 闪烁一次
- 如果 LED 闪烁，说明程序运行成功！

#### 查看串口输出

**Windows - 使用 PuTTY：**
1. 打开 PuTTY
2. 连接类型选择 "Serial"
3. 串口号：COM3（根据实际情况调整）
4. 波特率：115200
5. 点击 "Open"

**Windows - 使用串口助手：**
- 推荐：SSCOM、Serial Port Utility、Termite

**Linux：**
```bash
# 使用 minicom
sudo minicom -D /dev/ttyUSB0 -b 115200

# 或使用 screen
sudo screen /dev/ttyUSB0 115200

# 或使用 picocom
sudo picocom /dev/ttyUSB0 -b 115200
```

**macOS：**
```bash
# 查找串口设备
ls /dev/tty.*

# 使用 screen
screen /dev/tty.usbserial-XXX 115200
```

**预期输出：**
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
[3] LED Toggle - System Running OK
...
```

## 🎯 完成！恭喜你成功运行第一个 STM32 程序！

---

## 🔧 常见问题快速解决

### 问题1：编译失败 - 找不到 arm-none-eabi-gcc

**解决方案：**
```bash
# 检查是否安装
arm-none-eabi-gcc --version

# 如果未安装，参考"第一步"重新安装

# 如果已安装但找不到，添加到 PATH
# Windows: 控制面板 → 系统 → 高级系统设置 → 环境变量 → 添加路径
# Linux/macOS: 编辑 ~/.bashrc 或 ~/.zshrc
export PATH=$PATH:/path/to/gcc-arm-none-eabi/bin
```

### 问题2：烧录失败 - 找不到设备

**解决方案：**
```bash
# 1. 检查 ST-Link 连接
st-info --probe

# 2. 如果 Linux 显示权限错误，添加 udev 规则
sudo nano /etc/udev/rules.d/49-stlinkv2.rules
# 添加内容：
SUBSYSTEMS=="usb", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="3748", MODE:="0666"
# 重新加载：
sudo udevadm control --reload-rules
sudo udevadm trigger

# 3. Windows：确保安装了 ST-Link 驱动
```

### 问题3：LED 不闪烁

**检查清单：**
- [ ] BOOT0 是否接地（正常运行模式）
- [ ] 开发板是否供电
- [ ] 程序是否成功烧录（查看烧录日志）
- [ ] 按下复位键，重新启动程序
- [ ] PC13 LED 是否损坏（用万用表测试）

### 问题4：串口无输出

**检查清单：**
- [ ] TX/RX 是否接反（TX→RX, RX→TX）
- [ ] 波特率是否为 115200
- [ ] USB-TTL 驱动是否安装
- [ ] 串口号是否正确
- [ ] GND 是否连接

### 问题5：Windows 上 make 命令不存在

**解决方案：**
```cmd
# 方案1：使用提供的批处理脚本
build.bat build

# 方案2：安装 MinGW
# 下载：http://www.mingw.org/
# 安装后添加到 PATH：C:\MinGW\bin

# 方案3：使用 MSYS2
# 下载：https://www.msys2.org/
# 在 MSYS2 终端中运行 make
```

---

## 📚 下一步学习

### 1️⃣ 尝试示例程序
```bash
# LED 闪烁示例
# 查看文件：examples/blink_led.c

# UART 回显示例
# 查看文件：examples/uart_echo.c
```

### 2️⃣ 修改代码
```c
// 修改 Core/Src/main.c
// 尝试改变 LED 闪烁频率：
Delay_Ms(500);  // 改成 Delay_Ms(100);

// 重新编译和烧录
make clean
make
make flash
```

### 3️⃣ 添加新功能
- 添加按键检测
- 添加 PWM 输出
- 添加 ADC 采集
- 连接传感器

### 4️⃣ 学习调试
```bash
# 使用 OpenOCD + GDB 进行调试
# 详见：docs/DEBUG_GUIDE.md（如果需要，可以创建）
```

---

## 💡 实用技巧

### 快速清理和重新编译
```bash
# Linux/macOS
./build.sh rebuild

# Windows
build.bat rebuild
```

### 查看固件大小
```bash
# Linux/macOS
./build.sh size

# Windows
build.bat size
```

### 自动烧录和打开串口（高级）
```bash
# Linux/macOS 组合命令
./build.sh build && ./build.sh flash && minicom -D /dev/ttyUSB0 -b 115200
```

---

## 🎓 推荐学习资源

1. **官方文档**
   - STM32F103 参考手册
   - Cortex-M3 编程手册

2. **在线教程**
   - STM32 中文社区
   - Bilibili STM32 视频教程

3. **开源项目**
   - GitHub STM32 项目
   - STM32duino

4. **工具软件**
   - STM32CubeMX (图形化配置)
   - Keil MDK (商业 IDE)
   - VS Code + Cortex-Debug (免费方案)

---

**祝你在 STM32 开发之旅中学习愉快！** 🎉

有问题？查看完整文档：`README.md`  
硬件连接？查看：`docs/HARDWARE_SETUP.md`

