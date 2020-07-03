/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "rtthread.h"
#include "rtdevice.h"
#include "rthw.h"
#include "mmu.h"
#include "board.h"

/* Hi3516EV200 19页 地址空间引射表 */ 
struct mem_desc platform_mem_desc[] = {
    {0x00000000, 0x14FFFFFF, 0x00000000, DEVICE_MEM},  /* SYS */ 
    {0x40000000, 0x43FFFFFF, 0x40000000, NORMAL_MEM}   /* DDR2 512Mb(64MB)  */ 
};
const rt_uint32_t platform_mem_desc_size = sizeof(platform_mem_desc)/sizeof(platform_mem_desc[0]);

static void reboot(void)
{
    hi3516e_clock_reset();
}
MSH_CMD_EXPORT_ALIAS(reboot, reboot, reboot hi3516ev200 system.);

void rt_hw_board_init(void)
{
    /* initialize interrupts */
    rt_hw_interrupt_init();

    /* initialize PLLs and the clock tree */
    hi3516e_clock_init(); 

    /* init hardware interrupt */
    rt_hw_uart_init(); 
    
#ifdef RT_USING_CONSOLE
    /* set console device */
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif /* RT_USING_CONSOLE */

#ifdef RT_USING_HEAP
    /* init memory system */
    rt_kprintf("head start: 0x%08x, head end: 0x%08x.\n", (rt_uint32_t) RT_HW_HEAP_BEGIN, (rt_uint32_t) RT_HW_HEAP_END);
    rt_system_heap_init(RT_HW_HEAP_BEGIN, RT_HW_HEAP_END);
#endif


#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif
}
