/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "rtthread.h"
#include "rtdevice.h"

static rt_device_t wdt;
static rt_uint32_t timeout = 5000;

int wdt_test(int argc, char **argv)
{
    wdt = rt_device_find("wdt");
    rt_device_init(wdt);
    rt_device_control(wdt, RT_DEVICE_CTRL_WDT_SET_TIMEOUT, &timeout);
    rt_device_control(wdt, RT_DEVICE_CTRL_WDT_START, RT_NULL); 

    return 0;
}
MSH_CMD_EXPORT(wdt_test, test hi3516ev200 wdt.); 
