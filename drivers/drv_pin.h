/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __DRV_PIN_H__
#define __DRV_PIN_H__

#include "rtthread.h"
#include "platform.h"

int rt_hw_pin_init(void); 
int rt_gpio_set_func(rt_base_t pin, rt_uint8_t speed, rt_uint8_t pull, 
    rt_uint8_t drive_capability, rt_uint32_t func, rt_uint8_t dir); 

#endif 
