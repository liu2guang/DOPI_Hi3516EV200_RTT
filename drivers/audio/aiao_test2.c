/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "rtthread.h"
#include "rtdevice.h"
#include "board.h" 
#include "amp.h" 
#include "codec.h" 
#include "aiao.h" 

static uint32_t cnt  = 0;

extern const rt_uint8_t _romfs_root_flash_Ibegyou_wav[];

static volatile uint32_t addr = (uint32_t)_romfs_root_flash_Ibegyou_wav; 
static volatile uint32_t wptr = 0; 
static volatile uint32_t size = 1765002; 
static volatile uint32_t *ptr = RT_NULL; 
static volatile uint32_t cnt_int = 8000000;

void aiao_irq(int vector, void *param)
{
    uint32_t interrupt_status = 0; 
    (void)param;
    rt_interrupt_enter();

    interrupt_status = hi3516ev200_ao_get_interrupt_status(); 
    if(interrupt_status & tx_trans_int)
    {
        // rt_kprintf("tx_trans_int\n");
        hi3516ev200_ao_clear_interrupt_status(tx_trans_int); 
    }

    if(interrupt_status & tx_empty_int)
    {
        // rt_kprintf("wptr = %d\n", wptr);
        // rt_kprintf("TX_BUFF_WPTR: 0x%.8x\n", readl(AIAO_REG_BASE+TX_BUFF_WPTR)); 
        // rt_kprintf("TX_BUFF_RPTR: 0x%.8x\n", readl(AIAO_REG_BASE+TX_BUFF_RPTR));  

        rt_kprintf("tx_empty_int\n");

        wptr+=4096;
        if(wptr > size)
        {
            wptr = 0; 
        }

        // hi3516ev200_ao_set_buff_addr((int)ptr+wptr);
        cnt_int = 2; 
        while(cnt_int--); 
        rt_kprintf("tx_empty_int 0\n");
        
        rt_memcpy(ptr, (const void *)(addr+wptr), 4096); 
        rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, ptr, 4096);

        hi3516ev200_ao_set_buff_size(4096+32);
        hi3516ev200_ao_set_buff_wptr(4096);
        hi3516ev200_ao_set_buff_rptr(0);
        hi3516ev200_ao_set_tran_size(2048);
        hi3516ev200_ao_clear_interrupt_status(tx_empty_int); 
    }

    if(interrupt_status & tx_alempty_int)
    {
        // rt_kprintf("tx_alempty_int\n");
        hi3516ev200_ao_clear_interrupt_status(tx_alempty_int); 
    }

    rt_hw_interrupt_ack(vector);
    rt_interrupt_leave();
}

int sound(int argc, char *argv[])
{
    struct hi_ao_attr attr; 

    hi3516ev200_amp_init(); 
    hi3516ev200_codec_init(); 
    hi3516ev200_codec_set_samplerate(44100); 
    hi3516ev200_amp_mute(0); 

    attr.samplerate  = 44100; 
    attr.bitwidth    = 16; 
    attr.workmod     = 0; 
    attr.channels    = 2; 
    attr.exflag      = 0; 
    attr.frmnum      = 30; 
    attr.ptnumperfrm = 4096; 
    attr.chncnt      = 2; 
    attr.clksel      = 0; 

    // ptr = rt_malloc_align(size, 1024); 
    // rt_memcpy(ptr, (const void *)addr, size); 

    wptr = 0; 
    ptr = rt_malloc_align(4096+32, 1024); 
    rt_memcpy(ptr, (const void *)(addr+wptr), 4096); 

    hi3516ev200_aiao_init(); 
    hi3516ev200_ao_set_attr(&attr); 
    // hi3516ev200_ao_set_buff_addr((int)ptr+wptr);
    hi3516ev200_ao_set_buff_addr((int)ptr);
    hi3516ev200_ao_set_buff_size(4096+32);
    hi3516ev200_ao_set_buff_wptr(4096);
    hi3516ev200_ao_set_buff_rptr(0);
    hi3516ev200_ao_set_tran_size(2048);
    hi3516ev200_ao_set_volume(0);
    hi3516ev200_ao_install_interrupt(aiao_irq, RT_NULL); 
    hi3516ev200_ao_set_alempty_threshold(2048); 
    hi3516ev200_ao_set_enable(tx_trans_int|tx_empty_int);

    rt_kprintf("wait play finsh!\n"); 

    return 0;    
}
MSH_CMD_EXPORT(sound, sound); 

int sstop(int argc, char *argv[])
{
    rt_kprintf("exit play!\n"); 
    hi3516ev200_ao_set_disable(tx_trans_int|tx_empty_int); 
    hi3516ev200_aiao_deinit(); 
    return 0; 
}
MSH_CMD_EXPORT(sstop, sstop); 