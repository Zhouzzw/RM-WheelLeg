# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 项目概述

RM-WheelLeg — RoboMaster 比赛中**串联腿构型轮腿机器人**的底盘控制代码。

- **主控芯片**: STM32G474VETx (Arm Cortex-M4F)
- **实时系统**: FreeRTOS V10.3.1 (CMSIS RTOS v1 封装)
- **开发环境**: Keil MDK-ARM (uvprojx)
- **工程生成**: STM32CubeMX (.ioc 配置)
- **通信总线**: CAN (FDCAN1/2/3), USART (SBUS/RS485), SPI (IMU), I2C, USB CDC

## 项目架构

### 目录分层

```
Core/                    ← CubeMX 生成的代码（外设初始化、FreeRTOS 配置、中断）
  ├── Inc/               ← 外设头文件、FreeRTOSConfig.h、main.h
  └── Src/               ← 外设实现、app_freertos.c（任务创建入口）、main.c

Drivers/                 ← STM32G4xx HAL + CMSIS（官方库，建议不改动）

Middlewares/             ← 第三方中间件
  ├── FreeRTOS/          ← FreeRTOS 内核源码
  └── ST/                ← USB 设备库

Project/                 ← **应用层代码（主要工作区）**
  ├── Algorithm_Drivers/ ← 算法层：PID、卡尔曼滤波、四元数EKF、控制器、功率限制、VOFA调试
  ├── BSP/               ← 板级支持：BMI088驱动、DWT时钟、Buzzer、Key、LED、PWM、Flash
  ├── Commnuicate_Drivers/ ← CAN驱动、USART驱动、USB驱动
  ├── Hardware_Drivers/  ← 硬件驱动：电机（大疆/C620/Unitree）、裁判系统、遥控器、超电、激光
  ├── Robot_Application/ ← **机器人核心逻辑**：底盘控制、云台控制、发射、导航(INS)、自瞄
  ├── UI/                ← 裁判系统UI绘制
  ├── UI-backup/         ← UI旧版备份
  └── Vision_Old/        ← 旧版视觉代码（解算轨迹）

USB_Device/              ← USB CDC 设备配置

MDK-ARM/                 ← Keil 工程文件
  ├── Kawashiro_Frame_G474.uvprojx  ← 工程文件（核心）
  └── startup_stm32g474xx.s         ← 启动文件
```

### FreeRTOS 任务体系（在 app_freertos.c 中创建）

| 任务 | 优先级 | 栈大小 | 职责 |
|------|--------|--------|------|
| `Robo_Task` | osPriorityRealtime | 512B | 顶层机器人控制调度 |
| `Chassis_Task` | osPriorityHigh | 2048B | 底盘运动解算与控制 |
| `Communicate_Task` | osPriorityBelowNormal | 512B | 通信数据处理 |
| `INS_Task` | osPriorityLow | 1024B | 惯导解算 |
| `Gimbal_Task` | osPriorityIdle | 512B | 云台控制 |
| `Key_LED_Task` | osPriorityIdle | 256B | 按键与LED |
| `Buzzer_Task` | osPriorityIdle | 256B | 蜂鸣器 |
| `UI_Task` | osPriorityIdle | 512B | 裁判系统UI刷新 |

### 通信接口

- **底盘轮电机**: FDCAN2 (标准CAN, 250Hz力矩控制)
- **底盘关节电机**: FDCAN1 (串联腿关节控制)
- **云台**: FDCAN2 (Pitch) + FDCAN3 (Yaw)
- **发射**: FDCAN3 (摩擦轮 + 拨弹)
- **姿态**: BMI088 (SPI) + FDCAN3 (外部IMU)
- **遥控器**: USART1 (SBUS) + USART2/USART3 (RS485)
- **裁判系统**: UART4/UART5
- **超电**: FDCAN1 (CAN ID 0x030/0x031)
- **调试**: USB CDC (VOFA 浮点协议)

### 关键数据结构

`RoboControl_StructTypeDef` (在 RoboControl.h 中定义) 是机器人全局状态体，包含底盘目标速度、云台角度、关节参数、功率状态等，各任务间共享。

## 常用操作

### 构建

此项目使用 **Keil MDK-ARM** 编译，需要打开 `MDK-ARM/Kawashiro_Frame_G474.uvprojx` 点击 Build (F7)。

### 格式化

项目无统一格式化配置。修改代码时遵循现有风格：
- 缩进：4 空格
- 结构体 typedef 命名：`XXX_StructTypeDef`
- 枚举命名：`XXX_EnumTypedef`
- 宏定义：全大写加下划线

### Git

代码托管在 GitHub: `https://github.com/Zhouzzw/RM-WheelLeg`
- **分支**: main
- **提交规范**: chore/feat/fix 前缀 + 中文描述
- **忽略规则**: 见 `.gitignore`（排除 .o/.d/.hex/.map/.uvguix 等编译和个人文件）

### 硬件配置修改

修改外设引脚配置需通过 **STM32CubeMX** 打开 `Kawashiro_Frame_G474.ioc` 文件，重新生成代码后再编译。手动修改 CubeMX 标记 `/* USER CODE BEGIN */` 区域的代码会被保留。

### 调试

- 使用 USB CDC (虚拟串口) + VOFA 上位机进行浮点数据可视化调试
- 使用 J-Link 或 DAP-Link 通过 Keil 进行硬件调试
