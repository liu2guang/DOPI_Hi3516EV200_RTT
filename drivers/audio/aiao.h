/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __XX_AIAO_H__
#define __XX_AIAO_H__

#include "rtthread.h"

enum hi_ao_interrupt
{
    tx_trans_int        = 1<<0,
    tx_empty_int        = 1<<1,
    tx_alempty_int      = 1<<2,
    tx_bfifo_empty_int  = 1<<3,
    tx_ififo_empty_int  = 1<<4,
    tx_stop_int         = 1<<5,
    tx_mfade_int        = 1<<6,
    tx_dat_break_int    = 1<<7
}; 
typedef int hi_ao_interrupt_t; 

struct hi_ao_attr
{
    uint32_t samplerate;    /* 8K 12K 11.025K 16K 22.05K 24K 32K 44.1K 48K */ 
    uint32_t bitwidth;      /* 16bit */ 
    uint32_t workmod;       /* 0: master */ 
    uint32_t channels;      /* 0: momo 1: steror */ 
    uint32_t exflag; 
    uint32_t frmnum;        /* (2~50) */ 
    uint32_t ptnumperfrm;   /* (80~4096) */ 
    uint32_t chncnt;        /* 1 2 */ 
    uint32_t clksel;        /* 0 1 */ 
}; 
typedef struct hi_ao_attr* hi_ao_attr_t; 

void hi3516ev200_aiao_init(void); 
void hi3516ev200_aiao_deinit(void); 
void hi3516ev200_ao_set_attr(hi_ao_attr_t attr);
void hi3516ev200_ao_set_enable(hi_ao_interrupt_t interrupt); 
void hi3516ev200_ao_set_disable(hi_ao_interrupt_t interrupt);
void hi3516ev200_ao_set_volume(int32_t volume);
void hi3516ev200_ao_set_buff_addr(uint32_t addr);
void hi3516ev200_ao_set_buff_size(uint32_t size);
void hi3516ev200_ao_set_tran_size(uint32_t size);
void hi3516ev200_ao_set_buff_wptr(uint32_t offset);
void hi3516ev200_ao_set_buff_rptr(uint32_t offset); 
void hi3516ev200_ao_install_interrupt(rt_isr_handler_t handler, void *param); 
void hi3516ev200_ao_clear_interrupt_status(hi_ao_interrupt_t interrupt); 
uint32_t hi3516ev200_ao_get_interrupt_status(void); 
void hi3516ev200_ao_set_alempty_threshold(uint32_t byte); 

#endif /* __HI_AO_H__ */ 