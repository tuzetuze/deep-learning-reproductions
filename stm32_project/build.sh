#!/bin/bash
###############################################################################
# STM32 项目构建脚本
# 用途：自动化编译、烧录、清理等操作
###############################################################################

set -e  # 遇到错误立即退出

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# 打印带颜色的信息
print_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

# 显示帮助信息
show_help() {
    cat << EOF
STM32F103 项目构建脚本

用法: ./build.sh [选项]

选项:
    build       编译项目
    clean       清理编译文件
    flash       烧录到开发板
    rebuild     清理后重新编译
    size        显示固件大小
    help        显示此帮助信息

示例:
    ./build.sh build      # 编译项目
    ./build.sh flash      # 烧录固件
    ./build.sh rebuild    # 清理并重新编译

EOF
}

# 检查工具链
check_toolchain() {
    print_info "检查工具链..."
    
    if ! command -v arm-none-eabi-gcc &> /dev/null; then
        print_error "未找到 arm-none-eabi-gcc！请安装 ARM GCC 工具链。"
        exit 1
    fi
    
    print_info "工具链版本: $(arm-none-eabi-gcc --version | head -n1)"
}

# 编译项目
build_project() {
    print_info "开始编译项目..."
    check_toolchain
    
    make -j$(nproc) || {
        print_error "编译失败！"
        exit 1
    }
    
    print_info "编译成功！"
    show_size
}

# 清理编译文件
clean_project() {
    print_info "清理编译文件..."
    make clean
    print_info "清理完成！"
}

# 烧录固件
flash_firmware() {
    print_info "准备烧录固件..."
    
    if [ ! -f "build/stm32f103_project.bin" ]; then
        print_error "固件文件不存在！请先编译项目。"
        exit 1
    fi
    
    if ! command -v st-flash &> /dev/null; then
        print_error "未找到 st-flash！请安装 stlink 工具。"
        exit 1
    fi
    
    print_info "正在烧录..."
    st-flash write build/stm32f103_project.bin 0x8000000 || {
        print_error "烧录失败！请检查 ST-Link 连接。"
        exit 1
    }
    
    print_info "烧录成功！"
}

# 显示固件大小
show_size() {
    print_info "固件大小统计："
    if [ -f "build/stm32f103_project.elf" ]; then
        arm-none-eabi-size build/stm32f103_project.elf
        
        # 计算使用百分比
        TEXT_SIZE=$(arm-none-eabi-size build/stm32f103_project.elf | awk 'NR==2 {print $1}')
        DATA_SIZE=$(arm-none-eabi-size build/stm32f103_project.elf | awk 'NR==2 {print $2}')
        BSS_SIZE=$(arm-none-eabi-size build/stm32f103_project.elf | awk 'NR==2 {print $3}')
        
        FLASH_USED=$((TEXT_SIZE + DATA_SIZE))
        RAM_USED=$((DATA_SIZE + BSS_SIZE))
        
        FLASH_PERCENT=$((FLASH_USED * 100 / 65536))
        RAM_PERCENT=$((RAM_USED * 100 / 20480))
        
        echo ""
        print_info "Flash 使用: ${FLASH_USED} / 65536 字节 (${FLASH_PERCENT}%)"
        print_info "RAM 使用: ${RAM_USED} / 20480 字节 (${RAM_PERCENT}%)"
    else
        print_warning "ELF 文件不存在！"
    fi
}

# 重新编译
rebuild_project() {
    clean_project
    build_project
}

# 主函数
main() {
    case "${1:-build}" in
        build)
            build_project
            ;;
        clean)
            clean_project
            ;;
        flash)
            flash_firmware
            ;;
        rebuild)
            rebuild_project
            ;;
        size)
            show_size
            ;;
        help|--help|-h)
            show_help
            ;;
        *)
            print_error "未知选项: $1"
            show_help
            exit 1
            ;;
    esac
}

main "$@"

