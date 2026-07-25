# STM32G431 FreeRTOS Sensor Collection

基于 STM32G431RBT6 和 CT117E-M4 蓝桥开发板的 FreeRTOS 多传感器采集终端。项目使用 VS Code、STM32CubeMX、GCC、Makefile 和 OpenOCD 构建，脱离 Keil 完成嵌入式软件开发。

## 主要功能
- PA1 电位器 ADC 电压采集
- DHT11 温湿度采集
- 串口指令交互
- LCD 屏幕刷新
- LED 灯光控制
- FreeRTOS 多任务调度
- Makefile 命令行编译
- OpenOCD 下载和调试

## 硬件平台
| 项目 | 配置 |
| --- | --- |
| MCU | STM32G431RBT6 |
| 开发板 | CT117E-M4 |
| RTOS | FreeRTOS |
| 编译器 | arm-none-eabi-gcc |
| 构建工具 | GNU Make |
| 调试下载 | OpenOCD |
| 开发工具 | VS Code、STM32CubeMX |

## 工程结构
```text
.
├── Core/                         # 启动文件、系统文件和应用入口
├── Drivers/                      # HAL、BSP 和传感器驱动
├── Middlewares/Third_Party/
│   └── FreeRTOS/Source/          # FreeRTOS 内核源码
├── .vscode/                      # 本地开发配置，不纳入 Git
├── build/                        # 编译产物，不纳入 Git
├── Makefile                      # GCC 构建入口
├── RTOS-SenseG4.ioc              # STM32CubeMX 工程配置
├── STM32G431RBTx_FLASH.ld        # 链接脚本
└── startup_stm32g431xx.s         # 启动文件