/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __HI_CRG_H__
#define __HI_CRG_H__

// 时钟管理模块对芯片时钟输入、时钟生成和控制进行统一的管理
// 1. 时钟输入的管理和控制
// 2. 时钟分频和控制
// 3. 生成各模块的工作时钟

// 芯片内部使用了 2 个 PLL，每个 PLL 使用两组配置寄存器: 分别为 APLL 和 VPLL

#define CRG_REG_BASE    0x12010000

#define PERI_CRG_PLL0   0x0000  /* APLL 配置寄存器 0 */ 
#define PERI_CRG_PLL1   0x0004  /* APLL 配置寄存器 1 */ 
#define PERI_CRG_PLL6   0x0018  /* VPLL 配置寄存器 0 */
#define PERI_CRG_PLL7   0x001C  /* VPLL 配置寄存器 1 */
#define PERI_CRG_PLL121 0x01E4  /* 复位状态寄存器 */
#define PERI_CRG_PLL122 0x01E8  /* PLL LOCK 状态寄存器 */
#define PERI_CRG_PLL124 0x01F0  /* CPU 高速 MEM 调速方式选择 */

#define PERI_CRG020     0x0050  /* APLL 展频配置寄存器 */
#define PERI_CRG023     0x005C  /* VPLL 展频配置寄存器 */
#define PERI_CRG030     0x0078  /* CPU 频率模式及复位配置寄存器 */
#define PERI_CRG031     0x007C  /* DDR 时钟及复位配置寄存器 */
#define PERI_CRG032     0x0080  /* SOC 时钟选择寄存器 */
#define PERI_CRG040     0x00A0  /* 媒体功能模块频率配置寄存器 */
#define PERI_CRG041     0x00A4  /* VEDU 时钟及软复位控制寄存器 */
#define PERI_CRG042     0x00A8  /* VPSS 时钟及软复位控制寄存器 */
#define PERI_CRG043     0x00AC  /* VGS 时钟及软复位控制寄存器 */
#define PERI_CRG044     0x00B0  /* JPGE 时钟及软复位控制寄存器 */
#define PERI_CRG047     0x00BC  /* IVE 时钟及软复位控制寄存器 */
#define PERI_CRG060     0x00F0  /* SENSOR 时钟及软复位控制寄存器 */
#define PERI_CRG061     0x00F4  /* VI 时钟及软复位控制寄存器 */
#define PERI_CRG062     0x00F8  /* MIPI RX 时钟及软复位控制寄存器 */
#define PERI_CRG063     0x00FC  /* VIPROC 时钟及软复位控制寄存器 */
#define PERI_CRG064     0x0100  /* ISP 时钟及软复位控制寄存器 */
#define PERI_CRG065     0x0104  /* LCD 时钟控制寄存器 */
#define PERI_CRG066     0x0108  /* VDP 时钟及软复位控制寄存器 */
#define PERI_CRG080     0x0140  /* USB2 相关的时钟及软复位控制寄存器 */
#define PERI_CRG081     0x0144  /* FMC 相关的时钟及软复位控制寄存器 */
#define PERI_CRG091     0x016C  /* ETH 接口相关的时钟及软复位控制寄存器 */
#define PERI_CRG100     0x0190  /* GZIP 时钟及软复位控制寄存器 */
#define PERI_CRG101     0x0194  /* DMAC 相关的时钟及软复位控制寄存器 */
#define PERI_CRG102     0x0198  /* DDRT 相关的时钟及软复位控制寄存器 */
#define PERI_CRG103     0x019C  /* AIAO/AUDIO CODEC 时钟复位控制寄存器 */
#define PERI_CRG104     0x01A0  /* SPACC/RSA/TRNG/KLAD 相关的时钟及软复位控制寄存器 */
#define PERI_CRG110     0x01B8  /* I2C/UART 时钟及复位控制寄存器 */
#define PERI_CRG111     0x01BC  /* SPI/LSADC/UART/OTP/PWM 相关的时钟及软复位控制寄存器 */
#define PERI_CRG112     0x01C0  /* SHPM 时钟及复位控制寄存器 */
#define PERI_CRG125     0x01F4  /* eMMC/SDIO0 时钟复位控制寄存器 */
#define PERI_CRG126     0x01F8  /* EMMC/SDIO0 SAMB 时钟相位配置寄存器 */
#define PERI_CRG127     0x01FC  /* eMMC/SDIO0 DRV 时钟相位配置寄存器 */
#define PERI_CRG129     0x0204  /* eMMC DS 180 时钟相位配置寄存器 */
#define PERI_CRG135     0x021C  /* SDIO1 SAMB 时钟相位配置寄存器 */
#define PERI_CRG136     0x0220  /* SDIO1 DRV 时钟相位配置寄存器 */
#define PERI_CRG139     0x022C  /* SDIO1 时钟复位控制寄存器 */

#endif /* __HI_CRG_H__ */ 
