/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __HI_TIMER_H__
#define __HI_TIMER_H__

#define TIMER0_REG_BASE 0x12000000
#define TIMER1_REG_BASE 0x12000020
#define TIMER2_REG_BASE 0x12001000
#define TIMER3_REG_BASE 0x12001020

#define TIMER0_ENABLE (1<<16)
#define TIMER1_ENABLE (1<<17)
#define TIMER2_ENABLE (1<<18)
#define TIMER3_ENABLE (1<<19)

#define TIMER_LOAD    0x00
#define TIMER_VALUE   0x04
#define TIMER_CONTROL 0x08
#define TIMER_INT_CLR 0x0c
#define TIMER_RIS     0x10
#define TIMER_MIS     0x14
#define TIMER_BGLOAD  0x18

#endif /* __HI_TIMER_H__ */
