/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "rtthread.h"
#include "rtdevice.h"

#define LED_PIN    (83)
#define UPDATE_PIN (72)

int pin_test(int argc, char **argv)
{
    int cnt; 

    rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT); 
    rt_pin_write(LED_PIN, PIN_LOW); 
    rt_pin_mode(UPDATE_PIN, PIN_MODE_INPUT_PULLUP); 

    cnt = 10; 
    while(cnt--)
    {
        rt_pin_write(LED_PIN, PIN_LOW); 
        rt_thread_mdelay(500); 
        rt_pin_write(LED_PIN, PIN_HIGH); 
        rt_thread_mdelay(500); 
    }

    cnt = 20; 
    while(cnt--)
    {
        rt_kprintf("pin = %d\n", rt_pin_read(72)); 
        rt_thread_mdelay(500);
    }

    rt_kprintf("test pin finish!\n"); 

    return 0;
}
MSH_CMD_EXPORT(pin_test, test hi3516ev200 pin.); 
