/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-02-08     RT-Thread    the first version
 */
#include "rtthread.h"
#include "rtdevice.h"
#include "board.h" 
#include "drv_watchdog.h" 

#define WDT_BASE       0x12030000

#define WDT_LOAD       0x0000
#define WDT_VALUE      0x0004
#define WDT_CTRL       0x0008
#define WDT_INTCLR     0x000C
#define WDT_RIS        0x0010
#define WDT_MIS        0x0014
#define WDT_LOCK       0x0C00

#define WDT_UNLOCK_VAL 0x1ACCE551
#define WDT_LOCK_VAL   0x00000000

static struct rt_watchdog_device wdt;

// 看门狗时钟3M(3*1000*1000), 1ms有3000个计数
static rt_uint32_t rate1ms = 3*1000; 
static rt_uint32_t timeout = 0; 

static void hi3516ev200_wdt_set_timeout(rt_uint32_t ms)
{
    rt_uint32_t cnt = ms * rate1ms; 

    timeout = ms; 

    /* unlock watchdog registers */
    writel(WDT_UNLOCK_VAL, WDT_BASE + WDT_LOCK);

    writel(cnt, WDT_BASE + WDT_LOAD);
    writel(cnt, WDT_BASE + WDT_VALUE);
    
    /* lock watchdog registers */
    writel(WDT_LOCK_VAL, WDT_BASE + WDT_LOCK);
}

static void hi3516ev200_wdt_feed(void)
{
    /* unlock watchdog registers */
    writel(WDT_UNLOCK_VAL, WDT_BASE + WDT_LOCK);

    /* clear watchdog */
    writel(0x00, WDT_BASE + WDT_INTCLR);

    /* lock watchdog registers */
    writel(WDT_LOCK_VAL, WDT_BASE + WDT_LOCK);
}

static void hi3516ev200_wdt_startup(void)
{
    /* unlock watchdog registers */
    writel(WDT_UNLOCK_VAL, WDT_BASE + WDT_LOCK);

    writel(0x00, WDT_BASE + WDT_CTRL);
    writel(0x00, WDT_BASE + WDT_INTCLR);
    writel(0x03, WDT_BASE + WDT_CTRL);
    
    /* lock watchdog registers */
    writel(WDT_LOCK_VAL, WDT_BASE + WDT_LOCK);
}

static void hi3516ev200_wdt_stop(void)
{
    /* unlock watchdog registers */
    writel(WDT_UNLOCK_VAL, WDT_BASE + WDT_LOCK);

    writel(0x00, WDT_BASE + WDT_CTRL);
    writel(0x00, WDT_BASE + WDT_INTCLR);
    
    /* lock watchdog registers */
    writel(WDT_LOCK_VAL, WDT_BASE + WDT_LOCK);
}

static rt_err_t init(rt_watchdog_t *watchdog)
{
    return RT_EOK;
}

static rt_err_t control(rt_watchdog_t *watchdog, int cmd, void *arg)
{
    rt_uint32_t ms = 0;

    switch(cmd)
    {
    case RT_DEVICE_CTRL_WDT_START:
        hi3516ev200_wdt_startup(); 
        hi3516ev200_wdt_feed();
        break; 

    case RT_DEVICE_CTRL_WDT_GET_TIMEOUT:
        *(int *)arg = timeout;
        break; 

    case RT_DEVICE_CTRL_WDT_SET_TIMEOUT:
        ms = *(rt_uint32_t *)arg;
        hi3516ev200_wdt_set_timeout(ms); 
        break; 

    case RT_DEVICE_CTRL_WDT_GET_TIMELEFT:
        break; 

    case RT_DEVICE_CTRL_WDT_KEEPALIVE:
        hi3516ev200_wdt_feed(); 
        break; 

    case RT_DEVICE_CTRL_WDT_STOP:
        hi3516ev200_wdt_stop(); 
        hi3516ev200_wdt_set_timeout(0); 
        break; 

    default:
        break; 
    }

    return RT_EOK;
}

static struct rt_watchdog_ops ops =
{
    init,
    control
};

int rt_hw_wdt_init(void)
{
    wdt.ops = &ops;
    return rt_hw_watchdog_register(&wdt, "wdt", RT_DEVICE_FLAG_WRONLY, RT_NULL);
}
INIT_DEVICE_EXPORT(rt_hw_wdt_init);
