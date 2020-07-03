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
static uint32_t addr = 0x400536d0; 
static uint32_t size = 0x002a0000; 

static uint32_t *ptr = RT_NULL; 

void aiao_irq(int vector, void *param)
{
    (void)param;
    rt_interrupt_enter();

    cnt++; 
    rt_kprintf("aiao_irq %d\n", cnt); 
    hi3516ev200_ao_set_buff_wptr(size-32);
    hi3516ev200_ao_set_buff_rptr(0);
    hi3516ev200_ao_clear_interrupt_status(); 

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

    // startao 0x43000000 0x200000 44100 2 0
    // setenv bootcmd "nand read 0x40000000 0x100000 0x100000; nand read 0x43000000 0x200000 0x2a0000; go 0x40000000"
    // setenv bootcmd "nand read 0x40000000 0x100000 0x4a0000; nand read 0x43000000 0x200000 0x2a0000; go 0x40000000"
    attr.samplerate  = 44100; 
    attr.bitwidth    = 16; 
    attr.workmod     = 0; 
    attr.channels    = 2; 
    attr.exflag      = 0; 
    attr.frmnum      = 30; 
    attr.ptnumperfrm = 2048; 
    attr.chncnt      = 2; 
    attr.clksel      = 0; 

    ptr = rt_malloc_align(size, 1024); 
    rt_memcpy(ptr, (const void *)addr, size); 

    hi3516ev200_aiao_init(); 
    hi3516ev200_ao_set_attr(&attr); 
    hi3516ev200_ao_set_buff_addr((int)ptr);
    hi3516ev200_ao_set_buff_size(size);
    hi3516ev200_ao_set_buff_wptr(size-32);
    hi3516ev200_ao_set_buff_rptr(0);
    hi3516ev200_ao_set_tran_size(2048);
    hi3516ev200_ao_set_volume(-30);
    hi3516ev200_ao_install_interrupt(aiao_irq, RT_NULL); 
    hi3516ev200_ao_set_enable();

    rt_kprintf("wait play finsh!\n"); 

    return 0;    
}
MSH_CMD_EXPORT(sound, sound); 

int sstop(int argc, char *argv[])
{
    rt_kprintf("exit play!\n"); 
    hi3516ev200_ao_set_disable(); 
    hi3516ev200_aiao_deinit(); 
    cnt = 0;
    return 0; 
}
MSH_CMD_EXPORT(sstop, sstop); 


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

static uint32_t addr = (uint32_t)_romfs_root_flash_Ibegyou_wav; 
static uint32_t wptr = 0; 
static uint32_t size = 0x002a0000; 
static uint32_t *ptr = RT_NULL; 

void aiao_irq(int vector, void *param)
{
    (void)param;
    rt_interrupt_enter();

    rt_kprintf("wptr = %d\n", wptr); 

    wptr+=2048;
    hi3516ev200_ao_set_buff_addr((int)ptr+wptr);
     
    hi3516ev200_ao_set_buff_size(2048);
    hi3516ev200_ao_set_buff_wptr(2048-32);
    hi3516ev200_ao_set_buff_rptr(0);
    hi3516ev200_ao_set_tran_size(2048);
    hi3516ev200_ao_clear_interrupt_status(); 

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
    attr.ptnumperfrm = 2048; 
    attr.chncnt      = 2; 
    attr.clksel      = 0; 

    ptr = rt_malloc_align(size, 1024); 
    rt_memcpy(ptr, (const void *)addr, size); 

    hi3516ev200_aiao_init(); 
    hi3516ev200_ao_set_attr(&attr); 
    hi3516ev200_ao_set_buff_addr((int)ptr+wptr);
    hi3516ev200_ao_set_buff_size(2048);
    hi3516ev200_ao_set_buff_wptr(2048-32);
    hi3516ev200_ao_set_buff_rptr(0);
    hi3516ev200_ao_set_tran_size(2048);
    hi3516ev200_ao_set_volume(0);
    hi3516ev200_ao_install_interrupt(aiao_irq, RT_NULL); 
    hi3516ev200_ao_set_enable();

    rt_kprintf("wait play finsh!\n"); 

    return 0;    
}
MSH_CMD_EXPORT(sound, sound); 

int sstop(int argc, char *argv[])
{
    rt_kprintf("exit play!\n"); 
    hi3516ev200_ao_set_disable(); 
    hi3516ev200_aiao_deinit(); 
    return 0; 
}
MSH_CMD_EXPORT(sstop, sstop); 


// 杂音
// /*
//  * Copyright (c) 2006-2018, RT-Thread Development Team
//  *
//  * SPDX-License-Identifier: Apache-2.0
//  */
// #include "rtthread.h"
// #include "rtdevice.h"
// #include "board.h" 
// #include "amp.h" 
// #include "codec.h" 
// #include "aiao.h" 

// static uint32_t cnt  = 0;

// extern const rt_uint8_t _romfs_root_flash_Ibegyou_wav[];

// static uint32_t addr = (uint32_t)_romfs_root_flash_Ibegyou_wav; 
// static uint32_t wptr = 0; 
// static uint32_t size = 0x002a0000; 
// static uint32_t *ptr = RT_NULL; 

// extern void hexdump(const rt_uint8_t *p, rt_size_t len); 

// void aiao_irq(int vector, void *param)
// {
//     (void)param;
//     rt_interrupt_enter();

//     rt_kprintf("wptr = %d\n", wptr); 

//     // 数据没有拷贝进去
//     rt_memcpy(ptr, (const void *)(addr+wptr), 2048); 
//     hexdump(ptr, 32); 

//     // 地址设置失败
//     hi3516ev200_ao_set_buff_addr((int)ptr);
//     wptr+=2048;
     
//     hi3516ev200_ao_set_buff_size(2048);
//     hi3516ev200_ao_set_buff_wptr(2048-32);
//     hi3516ev200_ao_set_buff_rptr(0);
//     hi3516ev200_ao_set_tran_size(2048);
//     hi3516ev200_ao_clear_interrupt_status(); 

//     rt_hw_interrupt_ack(vector);
//     rt_interrupt_leave();
// }

// int sound(int argc, char *argv[])
// {
//     struct hi_ao_attr attr; 

//     hi3516ev200_amp_init(); 
//     hi3516ev200_codec_init(); 
//     hi3516ev200_codec_set_samplerate(44100); 
//     hi3516ev200_amp_mute(0); 

//     // startao 0x43000000 0x200000 44100 2 0
//     // setenv bootcmd "nand read 0x40000000 0x100000 0x100000; nand read 0x43000000 0x200000 0x2a0000; go 0x40000000"
//     // setenv bootcmd "nand read 0x40000000 0x100000 0x4a0000; nand read 0x43000000 0x200000 0x2a0000; go 0x40000000"
//     attr.samplerate  = 44100; 
//     attr.bitwidth    = 16; 
//     attr.workmod     = 0; 
//     attr.channels    = 2; 
//     attr.exflag      = 0; 
//     attr.frmnum      = 30; 
//     attr.ptnumperfrm = 2048; 
//     attr.chncnt      = 2; 
//     attr.clksel      = 0; 

//     wptr = 0; 
//     ptr = rt_malloc_align(2048, 1024); 
//     rt_memcpy(ptr, (const void *)(addr+wptr), 2048); 
//     wptr+=2048;

//     hi3516ev200_aiao_init(); 
//     hi3516ev200_ao_set_attr(&attr); 
//     hi3516ev200_ao_set_buff_addr((int)ptr);
//     hi3516ev200_ao_set_buff_size(size);
//     hi3516ev200_ao_set_buff_wptr(2048-32);
//     hi3516ev200_ao_set_buff_rptr(0);
//     hi3516ev200_ao_set_tran_size(2048);
//     hi3516ev200_ao_set_volume(0);
//     hi3516ev200_ao_install_interrupt(aiao_irq, RT_NULL); 
//     hi3516ev200_ao_set_enable();

//     rt_kprintf("wait play finsh!\n"); 

//     return 0;    
// }
// MSH_CMD_EXPORT(sound, sound); 

// int sstop(int argc, char *argv[])
// {
//     rt_kprintf("exit play!\n"); 
//     hi3516ev200_ao_set_disable(); 
//     hi3516ev200_aiao_deinit(); 
//     return 0; 
// }
// MSH_CMD_EXPORT(sstop, sstop); 

