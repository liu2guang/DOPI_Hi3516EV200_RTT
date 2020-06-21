/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __DRV_LCD_H__
#define __DRV_LCD_H__

#include "rtthread.h"
#include "platform.h"

#define LCD_RST 69  /* SYS_RST_OUT */ 
#define LCD_NCS 12  /* SPI0_CSN */ 
#define LCD_DC  21  /* PIN 21 */

int rt_hw_lcd_init(void); 

#endif 


