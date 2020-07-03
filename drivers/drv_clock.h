/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __DRV_CLOCK_H__
#define __DRV_CLOCK_H__

#include "rtthread.h"
#include "hi_chip.h"

rt_err_t hi3516e_clock_init(void); 
void hi3516e_clock_reset(void); 
void hi3516e_clock_test_output(int type); 

#endif 