/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "rtthread.h"
#include "rtdevice.h"
#include "drv_spi.h" 

int lcd_test(int argc, char **argv)
{
    return 0;
}
MSH_CMD_EXPORT(lcd_test, test hi3516ev200 ili9341 lcd.); 
