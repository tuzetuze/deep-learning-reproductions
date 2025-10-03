@echo off
REM ############################################################################
REM STM32 项目构建脚本 (Windows)
REM 用途：自动化编译、烧录、清理等操作
REM ############################################################################

setlocal enabledelayedexpansion

REM 设置项目名称
set PROJECT_NAME=stm32f103_project

REM 显示帮助信息
if "%1"=="help" goto :show_help
if "%1"=="--help" goto :show_help
if "%1"=="-h" goto :show_help
if "%1"=="/?" goto :show_help

REM 执行相应命令
if "%1"=="" goto :build
if "%1"=="build" goto :build
if "%1"=="clean" goto :clean
if "%1"=="flash" goto :flash
if "%1"=="rebuild" goto :rebuild
if "%1"=="size" goto :show_size

echo [错误] 未知命令: %1
goto :show_help

:show_help
echo STM32F103 项目构建脚本 (Windows)
echo.
echo 用法: build.bat [选项]
echo.
echo 选项:
echo     build       编译项目 (默认)
echo     clean       清理编译文件
echo     flash       烧录到开发板
echo     rebuild     清理后重新编译
echo     size        显示固件大小
echo     help        显示此帮助信息
echo.
echo 示例:
echo     build.bat build      # 编译项目
echo     build.bat flash      # 烧录固件
echo     build.bat rebuild    # 清理并重新编译
echo.
goto :eof

:check_toolchain
echo [信息] 检查工具链...
where arm-none-eabi-gcc >nul 2>&1
if %errorlevel% neq 0 (
    echo [错误] 未找到 arm-none-eabi-gcc！
    echo 请安装 ARM GCC 工具链并添加到 PATH。
    exit /b 1
)
for /f "tokens=*" %%i in ('arm-none-eabi-gcc --version ^| findstr /r "^arm"') do (
    echo [信息] 工具链版本: %%i
    goto :check_done
)
:check_done
goto :eof

:build
echo.
echo ========================================
echo   编译 STM32F103 项目
echo ========================================
echo.
call :check_toolchain
if %errorlevel% neq 0 exit /b 1

echo [信息] 开始编译...
make
if %errorlevel% neq 0 (
    echo [错误] 编译失败！
    exit /b 1
)
echo.
echo [成功] 编译完成！
call :show_size
goto :eof

:clean
echo [信息] 清理编译文件...
make clean
echo [成功] 清理完成！
goto :eof

:flash
echo.
echo ========================================
echo   烧录固件到 STM32F103
echo ========================================
echo.

if not exist "build\%PROJECT_NAME%.bin" (
    echo [错误] 固件文件不存在！
    echo 请先运行: build.bat build
    exit /b 1
)

where st-flash >nul 2>&1
if %errorlevel% neq 0 (
    echo [错误] 未找到 st-flash！
    echo 请安装 stlink 工具: https://github.com/stlink-org/stlink
    exit /b 1
)

echo [信息] 正在烧录固件...
st-flash write build\%PROJECT_NAME%.bin 0x8000000
if %errorlevel% neq 0 (
    echo [错误] 烧录失败！请检查 ST-Link 连接。
    exit /b 1
)

echo.
echo [成功] 烧录完成！
goto :eof

:rebuild
call :clean
call :build
goto :eof

:show_size
echo.
echo ========================================
echo   固件大小统计
echo ========================================
if exist "build\%PROJECT_NAME%.elf" (
    arm-none-eabi-size build\%PROJECT_NAME%.elf
    echo.
    echo Flash 容量: 64KB (65536 字节)
    echo RAM 容量:   20KB (20480 字节)
) else (
    echo [警告] ELF 文件不存在！
)
echo ========================================
goto :eof

:eof
endlocal

