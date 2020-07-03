/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __HI_SYS_H__
#define __HI_SYS_H__

#define SYS_CTRL_REG_BASE 0x12020000

#define REG_SC_CTRL     0x0000  /* 系统控制寄存器 */ 
#define REG_SC_SYSRES   0x0004  /* 系统软复位寄存器 */ 
#define REG_SC_VERID    0x0018  /* 版本 ID 寄存器 */ 
#define REG_SOFTINT     0x001C  /* 软中断寄存器 */ 
#define REG_SOFTTYPE    0x0020  /* 软中断向量寄存器 */ 
#define REG_SC_LOCKEN   0x0044  /* 关键系统控制寄存器的锁定寄存器 */ 
#define REG_OTP_PO_BIT2 0x0088  /* OTP 上电回读寄存器 */ 
#define REG_SYSSTAT     0x008C  /* 系统状态寄存器 */ 
#define REG_DIE_ID0     0x0400  /* DIE ID0 寄存器 */ 
#define REG_DIE_ID1     0x0404  /* DIE ID1 寄存器 */ 
#define REG_DIE_ID2     0x0408  /* DIE ID2 寄存器 */  
#define REG_DIE_ID3     0x040C  /* DIE ID3 寄存器 */  
#define REG_DIE_ID4     0x0410  /* DIE ID4 寄存器 */ 
#define REG_DIE_ID5     0x0414  /* DIE ID5 寄存器 */ 
#define REG_CHIP_ID     0x0EE0  /* 芯片 ID 寄存器 */  

#define REG_VENDOR_ID   0x0EEC  /* 厂商 ID 寄存器 */ 
#define REG_MISC_CTRL0  0x8000  /* MSIC 功能选择寄存器 0  */ 
#define REG_MISC_CTRL1  0x8004  /* USB PHY TRIM 参数寄存器 */  
#define REG_MISC_CTRL3  0x800C  /* CPU 控制寄存器 1 */ 
#define REG_MISC_CTRL4  0x8010  /* MSIC 功能选择寄存器 4 */  
#define REG_MISC_CTRL6  0x8018  /* VICAP 模式控制寄存器 1 */ 
#define REG_MISC_CTRL7  0x801C  /* USB2.0 控制寄存器 0 */ 
#define REG_MISC_CTRL8  0x8020  /* 内置 FEPHY 控制寄存器 0 */  
#define REG_MISC_CTRL9  0x8024  /* 内置 FEPHY 控制寄存器 1 */  
#define REG_MISC_CTRL28 0x8070  /* 总线优先级控制寄存器 15 */  
#define REG_MISC_CTRL32 0x8080  /* MDDRC W QOS 寄存器 0 */ 
#define REG_MISC_CTRL33 0x8084  /* MDDRC W QOS 寄存器 1 */  
#define REG_MISC_CTRL34 0x8088  /* MDDRC W QOS 寄存器 2 */ 
#define REG_MISC_CTRL36 0x8090  /* MDDRC R QOS 寄存器 0 */  
#define REG_MISC_CTRL37 0x8094  /* MDDRC R QOS 寄存器 1 */ 
#define REG_MISC_CTRL38 0x8098  /* MDDRC R QOS 寄存器 2 */  
#define REG_MISC_CTRL45 0x80B4  /* Tsensor 控制寄存器 */  
#define REG_MISC_CTRL46 0x80B8  /* Tsensor 状态寄存器 */  
#define REG_MISC_CTRL47 0x80BC  /* Tsensor 温度记录值寄存器 0 */  
#define REG_MISC_CTRL48 0x80C0  /* Tsensor 温度记录值寄存器 1 */  
#define REG_MISC_CTRL49 0x80C4  /* Tsensor 温度记录值寄存器 2 */  
#define REG_MISC_CTRL50 0x80C8  /* Tsensor 温度记录值寄存器 3 */  
#define REG_MISC_CTRL62 0x80F8  /* Stopwatch 控制寄存器 */  
#define REG_MISC_CTRL63 0x80FC  /* Stopwatch 计时值寄存器 */  

#endif /* __HI_SYS_H__ */ 
