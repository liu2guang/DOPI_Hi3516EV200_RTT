/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "rtthread.h"
#include "rtdevice.h"
#include "drv_clock.h" 

static int clk_atoi(const char *str)
{
    if(NULL == str)
        return 0;
    int ret = 0;
    while(0 != *str)
    {
        ret = ret * 10 + *str - '0';
        str++;
    }
    return ret;
}

static int clk_test(int argc, char *argv[])
{
    int i = 0; 

    if(argc != 2)
    {
        rt_kprintf("clk_test type: \n"); 
        rt_kprintf("0x0: pll_test_out_0\n"); 
        rt_kprintf("0x1: pll_test_out_1\n"); 
        rt_kprintf("0x4：clk_24m\n"); 
        rt_kprintf("0x5：audiocodec_mclk\n"); 
        rt_kprintf("0x6：usb2_phy_clk_0\n"); 
        rt_kprintf("0x7：rtc_clk_out\n"); 
        return (-RT_ERROR);
    }

    i = clk_atoi(argv[1]); 
    rt_kprintf("type = %d\n", i); 
    hi3516e_clock_test_output(i); 
    return 0; 
}
MSH_CMD_EXPORT(clk_test, test hi3516ev200 internal clock.); 
