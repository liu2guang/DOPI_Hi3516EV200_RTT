# DOPI HI3516EV200 开发板 BSP 说明

## 简介

本 BSP 是流光为 DOPI HI3516EV200 开发板提供的 BSP (板级支持包) 说明，感谢大法师提供的移植技术支持。该 BSP 仅供娱乐，欢迎各位大佬尝试移植各种驱动和应用，本人为鸽子王随时可鸽。

## 开发板介绍

DOPI HI3516EV200 是 Tony 大佬推出的一款基于 ARM Cortex-A7 内核的开发板，最高主频为 900Mhz，该开发板具有丰富的板载资源，可以充分发挥 HI3516EV200 的芯片性能。

开发板外观如下图所示：

![board](figures/board.png)

该开发板常用 **板载资源** 如下：

- MCU：HI3516EV200，主频 900MHz，64MByte DDR2
- 外部 Nadn FLASH：型号W25N01GVZEIG，128MByte
- 常用外设
  - UART: 1个
  - SDIO: 2个, SD卡+WIFI
  - ETH: 1个
  - SENSOR: 1个
  - LED：1个
  - 按键：5个，S1兼具USB升级功能
  - LCD：SPI接口屏幕
  - 音频：片内CODEC+SGM8903+NS4150

开发板更多详细信息请参考 DOPI 的 [DOPI HI3516EV200 开发板介绍](https://whycan.cn/t_4466.html)。

## 外设支持

本 BSP 目前对外设的支持情况如下：

| **板载外设** | **支持情况** | **备注** |
| :-----: | :-----: | :-----: |
| SDRAM  | 支持 | 64MB |
| LCD | 暂不支持 | SPI LCD |
| SDCARD | 暂不支持 | - |
| GPIO  | 支持 | 中断模式未做 |
| USART | 支持 | UART0 |
| SPI | 暂不支持 | - |
| I2C | 暂不支持 | - |
| FLASH | 暂不支持 | - |
| SDIO | 暂不支持 | - |
| USB Device | 暂不支持 | - |
| WDT | 支持 | - |
