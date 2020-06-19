/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "rtthread.h" 
#include "board.h" 
#include "drv_clock.h" 

#define REG_SC_CTRL             0
#define REG_SC_SYSRES           0x4

#define TIMER_LOAD              0x0
#define TIMER_VALUE             0x4
#define TIMER_CONTROL           0x8
#define TIMER_INT_CLR           0xc
#define TIMER_RIS               0x10
#define TIMER_MIS               0x14
#define TIMER_BGLOAD            0x18

void hi3516e_clock_irq(int vector, void *param)
{
    (void)param;
    rt_interrupt_enter();
    WRITE_UINT32(1, TIMER_TICK_REG_BASE + TIMER_INT_CLR);
    rt_hw_interrupt_ack(vector);
    rt_tick_increase();
    rt_interrupt_leave();
}

void hi3516e_clock_reset(void)
{
    WRITE_UINT32(0xffffffff, (SYS_CTRL_REG_BASE + REG_SC_SYSRES));
}

rt_err_t hi3516e_clock_init(void)
{
    UINT32 temp;
    rt_uint32_t period = 0xC350; // 5*1000 ==> 1ms中断

    /*
     * enable time0, timer1 and timer2 here,
     * but only time0 is used for system clock.
     */
    READ_UINT32(temp, SYS_CTRL_REG_BASE + REG_SC_CTRL);
    // temp |= TIMER_TICK_ENABLE | TIMER_TIME_ENABLE;
    temp |= TIMER_TICK_ENABLE; // 总线时钟50MHz 
    WRITE_UINT32(temp, SYS_CTRL_REG_BASE + REG_SC_CTRL);

    /* disable first */
    WRITE_UINT32(0x0, TIMER_TICK_REG_BASE + TIMER_CONTROL);

    /* set init value as period */
    WRITE_UINT32(period, TIMER_TICK_REG_BASE + TIMER_LOAD);

    /*
     * Timing mode: 32bits [bit 1 set as 1]
     * ticking with 1/1 clock frequency [bit 3 set as 0, bit 2 set as 0]
     * timing circulary [bit 6 set as 1] 
     */
    temp = (1 << 6) | (1 << 5) | (1 << 1);
    WRITE_UINT32(temp, TIMER_TICK_REG_BASE + TIMER_CONTROL);

    /* enable time0 IRQ */
    rt_hw_interrupt_install(NUM_HAL_INTERRUPT_TIMER0, hi3516e_clock_irq, RT_NULL, "TIMER0");
    rt_hw_interrupt_umask(NUM_HAL_INTERRUPT_TIMER0);

    /* enable the time0 */
    temp = GET_UINT32(TIMER_TICK_REG_BASE + TIMER_CONTROL); 
    temp |= (1 << 7);
    WRITE_UINT32(temp, TIMER_TICK_REG_BASE + TIMER_CONTROL);

    return RT_EOK; 
}
