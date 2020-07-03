/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "rtthread.h" 
#include "board.h" 
#include "drv_clock.h" 
#include "drv_pin.h" 

void hi3516e_clock_irq(int vector, void *param)
{
    (void)param;
    rt_interrupt_enter();
    writel(1, TIMER0_REG_BASE + TIMER_INT_CLR);
    rt_hw_interrupt_ack(vector);
    rt_tick_increase();
    rt_interrupt_leave();
}

void hi3516e_clock_reset(void)
{
    writel(0xFFFFFFFF, (SYS_CTRL_REG_BASE + REG_SC_SYSRES));
}

// 0x0：pll_test_out_0
// 0x1：pll_test_out_1
// 0x4：clk_24m
// 0x5：audiocodec_mclk
// 0x6：usb2_phy_clk_0
// 0x7：rtc_clk_out
void hi3516e_clock_test_output(int type)
{
    RT_ASSERT(type >= 0x00 && type <= 0x07); 
    uint32_t value = readl(SYS_CTRL_REG_BASE+REG_MISC_CTRL0); 
    value &= ~(0xF<<16);
    value |= (type<<16); 
    writel(value, SYS_CTRL_REG_BASE+REG_MISC_CTRL0); 
    rt_gpio_set_func(35, 1, 0, 0, 0x00, 1); 
    rt_kprintf("REG_MISC_CTRL0 = 0x%.8x\n", value); 
}

rt_err_t hi3516e_clock_init(void)
{
    uint32_t temp;
    rt_uint32_t period = 0xC350; // 5*1000 ==> 1ms中断

    /*
     * enable time0, timer1 and timer2 here,
     * but only time0 is used for system clock.
     */
    temp = readl(SYS_CTRL_REG_BASE + REG_SC_CTRL); 
    // temp |= TIMER0_ENABLE | TIMER_TIME_ENABLE;
    temp |= TIMER0_ENABLE; // 总线时钟50MHz 
    writel(temp, SYS_CTRL_REG_BASE + REG_SC_CTRL);

    /* disable first */
    writel(0x0, TIMER0_REG_BASE + TIMER_CONTROL);

    /* set init value as period */
    writel(period, TIMER0_REG_BASE + TIMER_LOAD);

    /*
     * Timing mode: 32bits [bit 1 set as 1]
     * ticking with 1/1 clock frequency [bit 3 set as 0, bit 2 set as 0]
     * timing circulary [bit 6 set as 1] 
     */
    temp = (1 << 6) | (1 << 5) | (1 << 1);
    writel(temp, TIMER0_REG_BASE + TIMER_CONTROL);

    /* enable time0 IRQ */
    rt_hw_interrupt_install(HI_INTERRUPT_TIMER0, hi3516e_clock_irq, RT_NULL, "TIMER0");
    rt_hw_interrupt_umask(HI_INTERRUPT_TIMER0);

    /* enable the time0 */
    temp = readl(TIMER0_REG_BASE + TIMER_CONTROL); 
    temp |= (1 << 7);
    writel(temp, TIMER0_REG_BASE + TIMER_CONTROL);

    return RT_EOK; 
}
