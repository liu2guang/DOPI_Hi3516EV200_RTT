/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __HI_IO_H__
#define __HI_IO_H__

#include "stdio.h"

#if 0
    #define isb() asm volatile ("isb":::"memory")
    #define dsb() asm volatile ("dsb":::"memory")
    #define dmb() asm volatile ("dmb":::"memory")
#else
    #define isb() 
    #define dsb() 
    #define dmb() 
#endif 

#define writeb(value, address) {dsb(); *((volatile uint8_t  *)((uint32_t)(address))) = (value);}
#define writew(value, address) {dsb(); *((volatile uint16_t *)((uint32_t)(address))) = (value);}
#define writel(value, address) {dsb(); *((volatile uint32_t *)((uint32_t)(address))) = (value);}

#define readb(address) ({uint8_t  r_ = *((volatile uint8_t  *)((uint32_t)(address))); dsb(); r_; })
#define readw(address) ({uint16_t r_ = *((volatile uint16_t *)((uint32_t)(address))); dsb(); r_; })
#define readl(address) ({uint32_t r_ = *((volatile uint32_t *)((uint32_t)(address))); dsb(); r_; })

#define write_reg_bit(value, offset, addr) \
({                                         \
    unsigned long t, mask;                 \
    mask = 1 << (offset);                  \
    t = readl(addr);                       \
    t &= ~mask;                            \
    t |= (value << (offset)) & mask;       \
    writel(t, addr);                       \
})

#endif /* __HI_IO_H__ */
