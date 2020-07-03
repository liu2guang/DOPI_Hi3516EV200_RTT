/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __BOARD_H__
#define __BOARD_H__

#include "rtthread.h"
#include "interrupt.h"
#include "hi_chip.h"
#include "drv_clock.h"
#include "drv_uart.h"

/* GIC配置 */ 
#define ARM_GIC_MAX_NR      1     
#define GIC_IRQ_START       0  
#define GIC_ACK_INTID_MASK  0x000003FF
#define MAX_HANDLERS        (HI_INTERRUPT_MAX+1)
#define ARM_GIC_NR_IRQS     (HI_INTERRUPT_MAX+1)

/* RT-Thread GIC need */ 
#define __REG32(x)  (*((volatile unsigned int *)(x)))
#define __REG16(x)  (*((volatile unsigned short *)(x)))

/* the basic constants and interfaces needed by gic */
rt_inline rt_uint32_t platform_get_gic_dist_base(void)
{
    return CFG_ARM_GIC_DIST_BASE; /* 0x10301000 */ 
}

rt_inline rt_uint32_t platform_get_gic_cpu_base(void)
{
    return CFG_ARM_GIC_CPU_BASE;  /* 0x10302000 */ 
}

/* 内存配置 */ 
extern unsigned char __bss_start;
extern unsigned char __bss_end;
#define RT_HW_HEAP_BEGIN (void*)&__bss_end
#define RT_HW_HEAP_END   (void*)(0x40000000 + 64 * 1024 * 1024)

void rt_hw_board_init(void); 

#endif /* __BOARD_H__ */ 
