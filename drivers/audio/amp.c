/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "rtthread.h"
#include "rtdevice.h"
#include "board.h" 

#define HI3516EV200_AMP_PIN 88

int hi3516ev200_amp_init(void)
{
    rt_pin_mode(HI3516EV200_AMP_PIN, PIN_MODE_OUTPUT); 
    return 0; 
}

int hi3516ev200_amp_mute(int enable)
{
    rt_pin_write(HI3516EV200_AMP_PIN, (enable==0)?PIN_HIGH:PIN_LOW); 
    return 0; 
}
