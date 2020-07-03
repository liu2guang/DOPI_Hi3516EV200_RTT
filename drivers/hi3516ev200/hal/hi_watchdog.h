/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __HI_WATCHDOG_H__
#define __HI_WATCHDOG_H__

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

#endif /* __HI_WATCHDOG_H__ */ 
