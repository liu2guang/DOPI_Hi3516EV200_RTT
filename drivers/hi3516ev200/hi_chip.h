/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __HI_CHIP_H__
#define __HI_CHIP_H__

#include "rtthread.h"
#include "rtdef.h"
#include "hi_io.h"
#include "hi_interrupt.h"

/* HI3516EV200 HAL */ 
#include "hal/hi_crg.h"         /* 时钟控制器 */ 
#include "hal/hi_sys.h"         /* 系统控制器 */ 
#include "hal/hi_gic.h"         /* 中断控制器 */
#include "hal/hi_timer.h"       /* 定时器 */
#include "hal/hi_gpio.h"        /* 通用输入输出 */
#include "hal/hi_uart.h"        /* 串口 */
#include "hal/hi_i2c.h"         /* I2C接口 */ 
#include "hal/hi_spi.h"         /* SPI接口 */ 
#include "hal/hi_eth.h"         /* 以太网接口 */ 
#include "hal/hi_sdio.h"        /* SDIO接口 */ 
#include "hal/hi_watchdog.h"    /* 看门狗 */ 
#include "hal/hi_aiao.h"        /* AIAO */ 
#include "hal/hi_codec.h"       /* 内置codec */ 

#endif  /* __HI_CHIP_H__ */ 
