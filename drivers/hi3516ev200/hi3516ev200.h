/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __HI3516EV200_H__
#define __HI3516EV200_H__

#include "rtthread.h"
#include "rtdef.h"
#include "interrupt.h"
#include "platform.h"
#include "hi3516ev200_irqnum.h"

/* RT-Thread GIC need */ 
#define __REG32(x)  (*((volatile unsigned int *)(x)))
#define __REG16(x)  (*((volatile unsigned short *)(x)))

typedef char INT8;
typedef short INT16;
typedef int INT32;
typedef long long INT64;
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;

#define READ_UINT8(ucValue, Addr)   (ucValue) = *((volatile UINT8 *)((UINT32)(Addr)))
#define READ_UINT16(usValue, Addr)  (usValue) = *((volatile UINT16 *)((UINT32)(Addr)))
#define READ_UINT32(uwValue, Addr)  (uwValue) = *((volatile UINT32 *)((UINT32)(Addr)))
#define READ_UINT64(ullValue, Addr) (ullValue) = *((volatile UINT64 *)((UINT32)(Addr)))

#define WRITE_UINT8(ucValue, Addr)   *((volatile UINT8 *)((UINT32)(Addr))) = (ucValue)
#define WRITE_UINT16(usValue, Addr)  *((volatile UINT16 *)((UINT32)(Addr))) = (usValue)
#define WRITE_UINT32(uwValue, Addr)  *((volatile UINT32 *)((UINT32)(Addr))) = (uwValue)
#define WRITE_UINT64(ullValue, Addr) *((volatile UINT64 *)((UINT32)(Addr))) = (ullValue)

#define GET_UINT8(Addr)  ({UINT8  r_ = *((volatile UINT8 *)((UINT32)(Addr))); r_;})
#define GET_UINT16(Addr) ({UINT16 r_ = *((volatile UINT16 *)((UINT32)(Addr))); r_;})
#define GET_UINT32(Addr) ({UINT32 r_ = *((volatile UINT32 *)((UINT32)(Addr))); r_;})
#define GET_UINT64(Addr) ({UINT64 r_ = *((volatile UINT64 *)((UINT32)(Addr))); r_;})

#define writeb(value, address)  WRITE_UINT8(value, address)
#define writew(value, address)  WRITE_UINT16(value, address)
#define writel(value, address)  WRITE_UINT32(value, address)

#define readb(address)          GET_UINT8(address)
#define readw(address)          GET_UINT16(address)
#define readl(address)          GET_UINT32(address)

#define SET_UINT32(bit, Addr)   ((Addr) |= (1<<(bit)))

#define CLR_UINT32(bit, Addr)   ((Addr) &= ~(1<<(bit)))

#define MAX_HANDLERS        (OS_HWI_MAX_NUM+1) 
#define ARM_GIC_NR_IRQS     (OS_HWI_MAX_NUM+1) 
#define ARM_GIC_MAX_NR      1     

/* the basic constants and interfaces needed by gic */
rt_inline rt_uint32_t platform_get_gic_dist_base(void)
{
    return CFG_ARM_GIC_DIST_BASE; /* 0x10301000 */ 
}

rt_inline rt_uint32_t platform_get_gic_cpu_base(void)
{
    return CFG_ARM_GIC_CPU_BASE;  /* 0x10302000 */ 
}

#define GIC_IRQ_START       0  
#define GIC_ACK_INTID_MASK  0x000003ff 

#endif  /* __HI3516EV200_H__ */
