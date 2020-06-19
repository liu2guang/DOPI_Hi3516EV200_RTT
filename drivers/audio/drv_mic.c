/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "rtthread.h"
#include "rtdevice.h"
#include "board.h" 
#include "drv_mic.h" 

// MIC_OUT  <==> AC_IN

#define AIAO_REG_BASE              (0x100e0000)

void test(void)
{
    U_I2S_CRG_CFG0 value;
    value.aiao_mclk_div = ;
}

