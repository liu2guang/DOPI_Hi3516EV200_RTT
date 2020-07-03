/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "rtthread.h"
#include "rtdevice.h"
#include "board.h" 
#include "aiao.h" 

int aiao_regdump(void)
{
    uint32_t addr = 0; 

    uint32_t cfg_regs[] = {
        PERI_CRG103
    }; 

    uint32_t aiao_regs[] = {
        AIAO_INT_ENA, AIAO_INT_STATUS, AIAO_INT_RAW,
        AIP_SWITCH_RX_BCLK, AOP_SWITCH_TX_BCLK,
        I2S_CRG_CFG0_00, I2S_CRG_CFG1_00, I2S_CRG_CFG0_08, I2S_CRG_CFG1_08, 
        RX_IF_ATTRI, RX_DSP_CTRL, RX_BUFF_SADDR, RX_BUFF_SIZE, RX_BUFF_WPTR, RX_BUFF_RPTR, RX_BUFF_ALFULL_TH, RX_TRANS_SIZE, RX_BUF_WPTR_TMP, RX_INT_ENA, RX_INT_RAW, RX_INT_STATUS, RX_INT_CLR, 
        TX_IF_ATTRI, TX_DSP_CTRL, TX_BUFF_SADDR, TX_BUFF_SIZE, TX_BUFF_WPTR, TX_BUFF_RPTR, TX_BUFF_ALEMPTY_TH, TX_TRANS_SIZE, TX_BUF_WPTR_TMP, TX_INT_ENA, TX_INT_RAW, TX_INT_STATUS, TX_INT_CLR 
    }; 

    uint32_t codec_regs[] = {
        AUDIO_CTRL_REG, AUDIO_DAC_REG_0, AUDIO_DAC_REG_1, AUDIO_ADC_REG_0, AUDIO_ADC_REG_1, AUDIO_IIS_MUX_CTRL, 
        AUDIO_ANAREG0_REG, AUDIO_ANAREG1_REG, AUDIO_ANAREG2_REG, AUDIO_ANAREG3_REG, AUDIO_ANAREG4_REG, AUDIO_ANAREG5_REG
    }; 

    rt_kprintf("cfg:\n"); 
    for(int index = 0; index < sizeof(cfg_regs)/sizeof(cfg_regs[0]); index++)
    {
        addr = CRG_REG_BASE+cfg_regs[index]; 
        rt_kprintf("0x%.8x: 0x%.8x\n", addr, readl(addr)); 
    }

    rt_kprintf("aiao:\n"); 
    for(int index = 0; index < sizeof(aiao_regs)/sizeof(aiao_regs[0]); index++)
    {
        addr = AIAO_REG_BASE+aiao_regs[index]; 
        rt_kprintf("0x%.8x: 0x%.8x\n", addr, readl(addr)); 
    }

    rt_kprintf("codec:\n"); 
    for(int index = 0; index < sizeof(codec_regs)/sizeof(codec_regs[0]); index++)
    {
        addr = AUDIO_CTRL_REG_BASE+codec_regs[index]; 
        rt_kprintf("0x%.8x: 0x%.8x\n", addr, readl(addr)); 
    }

    return 0; 
}
MSH_CMD_EXPORT_ALIAS(aiao_regdump, aiao_regdump, dump hi3516ev200 crg/aiao/codec reg.); 

/* 使能/失能AIAO时钟 */ 
static void aiao_enable_clk(uint8_t enable)
{
    uint32_t tmp = (enable == 0) ? 0 : 1;
    write_reg_bit(tmp, 1, CRG_REG_BASE+PERI_CRG103); 
    write_reg_bit(tmp, 2, CRG_REG_BASE+PERI_CRG103); 
}

/* 软复位/取消复位AIAO外设 */ 
static void aiao_reset(uint8_t reset)
{
    uint32_t tmp = (reset == 0) ? 0 : 1;
    write_reg_bit(tmp, 0, CRG_REG_BASE+PERI_CRG103);  
}

void hi3516ev200_aiao_init(void)
{
    aiao_enable_clk(1);
    aiao_reset(0); 
}

void hi3516ev200_aiao_deinit(void)
{
    aiao_reset(1); 
    aiao_enable_clk(0);
}

static void aiao_get_mclk_cfg(hi_ao_attr_t attr, uint32_t *mclk_set)
{
    switch(attr->samplerate)
    {
    case  8000: case 16000: case 32000: *mclk_set = 0x00095218; break;
    case 12000: case 24000: case 48000: *mclk_set = 0x000DFB24; break;
    case 11025: case 22050: case 44100: *mclk_set = 0x000CD856; break;
    }
}

static void aiao_get_bclk_fs_cfg(hi_ao_attr_t attr, uint32_t mclk_set, uint32_t *bclk_set, uint32_t *fs_set)
{
    uint32_t u32MclkRateNum = 0;
    uint32_t u32FsBit;

    u32FsBit = 2*attr->bitwidth;
    switch(mclk_set)
    {
    case 0x000DFB24: u32MclkRateNum = 48000*256; break;
    case 0x00095218: u32MclkRateNum = 32000*256; break;
    case 0x000CD856: u32MclkRateNum = 44100*256; break;
    }

    switch(u32MclkRateNum / (u32FsBit*(attr->samplerate)))
    {
    case  1: *bclk_set = 0x00; break; 
    case  2: *bclk_set = 0x02; break; 
    case  3: *bclk_set = 0x01; break; 
    case  4: *bclk_set = 0x03; break; 
    case  6: *bclk_set = 0x04; break; 
    case  8: *bclk_set = 0x05; break; 
    case 12: *bclk_set = 0x06; break; 
    case 16: *bclk_set = 0x07; break; 
    case 24: *bclk_set = 0x08; break; 
    case 32: *bclk_set = 0x09; break; 
    case 48: *bclk_set = 0x0a; break; 
    case 64: *bclk_set = 0x0b; break; 
    }

    switch(u32FsBit)
    {
    case  16: *fs_set = 0x00; break; 
    case  32: *fs_set = 0x01; break; 
    case  64: *fs_set = 0x03; break; 
    case 128: *fs_set = 0x04; break; 
    case 256: *fs_set = 0x05; break; 
    }
}

static void aiao_set_sysctl(hi_ao_attr_t attr)
{
    uint32_t mclk_set = 0, bclk_set = 0, fs_set = 0;

    U_I2S_CRG_CFG0 I2SCrg_cfg0;
    U_I2S_CRG_CFG1 I2SCrg_cfg1;
    U_AIAO_SWITCH_TX_BCLK switch_tx_bclk;

    /* 计算MCLK BCLK FS分频配置 */ 
    aiao_get_mclk_cfg(attr, &mclk_set); 
    aiao_get_bclk_fs_cfg(attr, mclk_set, &bclk_set, &fs_set); 

    // rt_kprintf("mclk_set = %d\n", mclk_set); 
    // rt_kprintf("bclk_set = %d\n", bclk_set); 
    // rt_kprintf("fs_set   = %d\n", fs_set); 

    /* 配置MCLK BCLK FS分频参数 */ 
    I2SCrg_cfg0.bits.aiao_mclk_div = mclk_set; 
    I2SCrg_cfg0.bits.Reserved_0    = 0;
    writel(I2SCrg_cfg0.u32, AIAO_REG_BASE+I2S_CRG_CFG0_08); 

    I2SCrg_cfg1.u32 = readl(AIAO_REG_BASE+I2S_CRG_CFG1_08); 
    I2SCrg_cfg1.bits.aiao_bclk_div      = bclk_set;
	I2SCrg_cfg1.bits.aiao_fsclk_div     = fs_set;
    I2SCrg_cfg1.bits.aiao_bclk_oen      = 0; /* master */ 
    I2SCrg_cfg1.bits.aiao_bclk_sel      = 0; /* master */  
    I2SCrg_cfg1.bits.aiao_cken          = 1;
    I2SCrg_cfg1.bits.aiao_srst_req      = 0;
    I2SCrg_cfg1.bits.aiao_bclkin_pctrl  = 0;
    I2SCrg_cfg1.bits.aiao_bclkout_pctrl = 0;
    writel(I2SCrg_cfg1.u32, AIAO_REG_BASE+I2S_CRG_CFG1_08); 

    /* 配置手册上未知寄存器 */ 
    switch_tx_bclk.u32 = readl(AIAO_REG_BASE+AOP_SWITCH_TX_BCLK);
    switch_tx_bclk.bits.inner_bclk_ws_sel_tx_00 = 0x8;
    writel(switch_tx_bclk.u32, AIAO_REG_BASE+AOP_SWITCH_TX_BCLK); 
}

void hi3516ev200_ao_set_attr(hi_ao_attr_t attr)
{
    U_TX_IF_ATTRI stAopAttrReg;
    U_TX_DSP_CTRL stAopCtrlReg;

    aiao_set_sysctl(attr); 

    /* 配置U_TX_IF_ATTRI */ 
    stAopAttrReg.u32 = readl(AIAO_REG_BASE+TX_IF_ATTRI);
    stAopAttrReg.bits.tx_underflow_ctrl = 1;
    stAopAttrReg.bits.Reserved_1        = 0;
    stAopAttrReg.bits.Reserved_0        = 0;
    stAopAttrReg.bits.tx_mode           = 0; /* I2S mode */ 
    stAopAttrReg.bits.tx_sd_offset      = 0; /* I2S mode */
    // stAopAttrReg.bits.tx_ch_num      = 0; /* 1 chncnt */ 
    stAopAttrReg.bits.tx_ch_num         = 1; /* 2 chncnt */ 
    stAopAttrReg.bits.tx_sd_source_sel  = 0;
    stAopAttrReg.bits.tx_sd0_sel        = 0;
    stAopAttrReg.bits.tx_sd1_sel        = 1;
    stAopAttrReg.bits.tx_sd2_sel        = 2;
    stAopAttrReg.bits.tx_sd3_sel        = 3;
    stAopAttrReg.bits.tx_trackmode      = 0;

    switch(attr->bitwidth)
    {
    case 8:
        stAopAttrReg.bits.tx_i2s_precision = 0;
        break; 
    case 16:
        stAopAttrReg.bits.tx_i2s_precision = 1;
        break; 
    case 32:
        stAopAttrReg.bits.tx_i2s_precision = 2;
        break;   
    }
    writel(stAopAttrReg.u32, AIAO_REG_BASE+TX_IF_ATTRI); 

    /* 配置U_TX_DSP_CTRL */ 
    stAopCtrlReg.u32 = readl(AIAO_REG_BASE+TX_DSP_CTRL);
    stAopCtrlReg.bits.mute_en      = 0;
    stAopCtrlReg.bits.mute_fade_en = 0;
    stAopCtrlReg.bits.Reserved_3   = 0;
    stAopCtrlReg.bits.volume       = 0x79;  /*0db*/
    stAopCtrlReg.bits.Reserved_2   = 0;
    stAopCtrlReg.bits.fade_in_rate = 0;
    stAopCtrlReg.bits.fade_out_rate= 0;
    stAopCtrlReg.bits.Reserved_1   = 0;
    stAopCtrlReg.bits.bypass_en    = 0;
    stAopCtrlReg.bits.tx_enable    = 0;
    stAopCtrlReg.bits.Reserved_0   = 0;
    writel(stAopCtrlReg.u32, AIAO_REG_BASE+TX_DSP_CTRL);
}

/* 配置AO中断 */ 
void hi3516ev200_ao_set_alempty_threshold(uint32_t byte)
{
    U_TX_BUFF_ALEMPTY_TH unTmp; 
    unTmp.bits.tx_buff_alempty_th = byte; 
    writel(unTmp.u32, AIAO_REG_BASE+TX_BUFF_ALEMPTY_TH);

#if defined(DEBUG_AO)
    rt_kprintf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AIAO_REG_BASE+TX_BUFF_ALEMPTY_TH, unTmp.u32);
#endif 
}

/* 配置AO中断 */ 
void hi3516ev200_ao_install_interrupt(rt_isr_handler_t handler, void *param)
{
    /* 安装中断 */
    rt_hw_interrupt_install(HI_INTERRUPT_AIAO, handler, param, "aiao");
    rt_hw_interrupt_umask(HI_INTERRUPT_AIAO);  
}

/* 配置AO中断 */ 
static void ao_set_interrupt(hi_ao_interrupt_t interrupt)
{
    U_TX_INT_ENA unTmp;
    unTmp.u32 = readl(AIAO_REG_BASE+TX_INT_ENA);
    // unTmp.bits.tx_trans_int_ena       = 0;  /* 传输是指块传输完成 */ 
    // unTmp.bits.tx_empty_int_ena       = 1;
    // unTmp.bits.tx_alempty_int_ena     = 0;
    // unTmp.bits.tx_bfifo_empty_int_ena = 0;
    // unTmp.bits.tx_ififo_empty_int_ena = 0;
    // unTmp.bits.tx_stop_int_ena        = 0;
    // unTmp.bits.tx_mfade_int_ena       = 0;
    // unTmp.bits.tx_dat_break_int_ena   = 0;
    unTmp.u32 |= interrupt; 
    writel(unTmp.u32, AIAO_REG_BASE+TX_INT_ENA);

#if defined(DEBUG_AO)
    rt_kprintf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AIAO_REG_BASE+TX_INT_ENA, unTmp.u32);
#endif 
}

/* 使能/失能AO中断 */ 
static void ao_enable_interrupt(uint8_t enable)
{
    U_AIAO_INT_ENA unTmp;
    unTmp.u32 = readl(AIAO_REG_BASE+AIAO_INT_ENA); 
    unTmp.bits.tx_ch0_int_ena = enable; 
    writel(unTmp.u32, AIAO_REG_BASE+AIAO_INT_ENA); 

#if defined(DEBUG_AO)
    rt_kprintf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AIAO_REG_BASE+AIAO_INT_ENA, unTmp.u32);
#endif 
}

/* 获取中断类型 */ 
uint32_t hi3516ev200_ao_get_interrupt_status(void)
{
    U_TX_INT_STATUS u32IntMask; 
    u32IntMask.u32 = readl(AIAO_REG_BASE+TX_INT_STATUS); 
    return u32IntMask.u32; 
}

/* 清空AO中断所有状态 */ 
void hi3516ev200_ao_clear_interrupt_status(hi_ao_interrupt_t interrupt)
{
    U_TX_INT_CLR u32IntMask;
    u32IntMask.u32 = readl(AIAO_REG_BASE+TX_INT_CLR); 
    // u32IntMask.bits.tx_trans_int_clear       = 0;
    // u32IntMask.bits.tx_empty_int_clear       = 1;
    // u32IntMask.bits.tx_alempty_int_clear     = 0;
    // u32IntMask.bits.tx_bfifo_empty_int_clear = 0;
    // u32IntMask.bits.tx_ififo_empty_int_clear = 0;
    // u32IntMask.bits.tx_stop_int_clear        = 0;
    // u32IntMask.bits.tx_mfade_int_clear       = 0;
    // u32IntMask.bits.tx_dat_break_int_clear   = 0;

    u32IntMask.u32 |= interrupt; 
    writel(u32IntMask.u32, AIAO_REG_BASE+TX_INT_CLR); 

#if defined(DEBUG_AO)
    rt_kprintf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AIAO_REG_BASE+TX_INT_CLR, u32IntMask.u32);
#endif 
}

/* 启动AO传输 */ 
static void ao_startup(uint8_t enable)
{
    U_TX_DSP_CTRL unTmp;
    unTmp.u32 = readl(AIAO_REG_BASE+TX_DSP_CTRL);
    unTmp.bits.tx_enable = enable; 
    writel(unTmp.u32, AIAO_REG_BASE+TX_DSP_CTRL);  

#if defined(DEBUG_AO)
    rt_kprintf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AIAO_REG_BASE+TX_DSP_CTRL, unTmp.u32);
#endif 
}

void hi3516ev200_ao_set_enable(hi_ao_interrupt_t interrupt)
{
    U_I2S_CRG_CFG1 I2SCrg_cfg1;

    /* 配置中断 */ 
    ao_set_interrupt(interrupt); 
    ao_enable_interrupt(1); 
    
    /* AIAO通道配置, 使用内部Codec */ 
    I2SCrg_cfg1.u32 = readl(AIAO_REG_BASE+I2S_CRG_CFG1_08);
    I2SCrg_cfg1.bits.aiao_bclk_en  = 1;
    I2SCrg_cfg1.bits.aiao_ws_en    = 1;
    I2SCrg_cfg1.bits.aiao_srst_req = 0;
    I2SCrg_cfg1.bits.aiao_cken     = 1;
    writel(I2SCrg_cfg1.u32, AIAO_REG_BASE+I2S_CRG_CFG1_08); 

#if defined(DEBUG_AO)
    rt_kprintf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AIAO_REG_BASE+I2S_CRG_CFG1_08, I2SCrg_cfg1.u32);
#endif 

    /* 启动AO传输 */ 
    ao_startup(1); 
}

/* AO停止完成标识 */ 
static uint32_t ao_get_dis_done(void)
{
    uint32_t u32Status = 0;
    U_TX_DSP_CTRL unTmp;
    unTmp.u32 = readl(AIAO_REG_BASE+TX_DSP_CTRL);
    u32Status = unTmp.bits.tx_disable_done;
    return u32Status; 
}

void hi3516ev200_ao_set_disable(hi_ao_interrupt_t interrupt)
{
    uint32_t cnt = 0;
    U_I2S_CRG_CFG1 I2SCrg_cfg1;

    /* 关闭AO传输 */ 
    ao_startup(0); 
    ao_enable_interrupt(0); 
    hi3516ev200_ao_clear_interrupt_status(interrupt); 

    /* 等待AO完全关闭 */ 
    while(!ao_get_dis_done())
    {
        if(cnt++ > 100)
        {
            I2SCrg_cfg1.u32 = readl(AIAO_REG_BASE+I2S_CRG_CFG1_08);
            I2SCrg_cfg1.bits.aiao_srst_req = 1;
            I2SCrg_cfg1.bits.aiao_cken     = 0;
            writel(I2SCrg_cfg1.u32, AIAO_REG_BASE+I2S_CRG_CFG1_08); 
            rt_thread_mdelay(10); 

            I2SCrg_cfg1.u32 = readl(AIAO_REG_BASE+I2S_CRG_CFG1_08);
            I2SCrg_cfg1.bits.aiao_srst_req = 0;
            I2SCrg_cfg1.bits.aiao_cken     = 1;
            writel(I2SCrg_cfg1.u32, AIAO_REG_BASE+I2S_CRG_CFG1_08); 
            rt_kprintf("Can't get ao stop flag. Ao clock is not config, please check your clock!\n");
            break; 
        }
        rt_thread_mdelay(10); 
    }

    /* AIAO通道配置 */ 
    I2SCrg_cfg1.u32 = readl(AIAO_REG_BASE+I2S_CRG_CFG1_08);
    I2SCrg_cfg1.bits.aiao_bclk_en  = 0;
    I2SCrg_cfg1.bits.aiao_ws_en    = 0;
    I2SCrg_cfg1.bits.aiao_srst_req = 0;
    I2SCrg_cfg1.bits.aiao_cken     = 1;
    writel(I2SCrg_cfg1.u32, AIAO_REG_BASE+I2S_CRG_CFG1_08); 
}

// 0x00~0x28 静音, 0x29(-80db) ~ 0x7F(+6db)
void hi3516ev200_ao_set_volume(int32_t volume) 
{
    U_TX_DSP_CTRL unTmp;

    unTmp.u32 = readl(AIAO_REG_BASE+TX_DSP_CTRL); 
    if(volume == 0)
    {
        unTmp.bits.volume = 0; 
    }
    else 
    {
        unTmp.bits.volume = volume*86/100+0x29;
    }
    writel(unTmp.u32, AIAO_REG_BASE+TX_DSP_CTRL); 

#if defined(DEBUG_AO)
    rt_kprintf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AIAO_REG_BASE+TX_DSP_CTRL, unTmp.u32);
#endif 
}

void hi3516ev200_ao_set_buff_addr(uint32_t addr)
{
    writel(addr, AIAO_REG_BASE+TX_BUFF_SADDR); 

#if defined(DEBUG_AO)
    rt_kprintf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AIAO_REG_BASE+TX_BUFF_SADDR, addr);
#endif 
}

void hi3516ev200_ao_set_buff_size(uint32_t size)
{
    U_TX_BUFF_SIZE unTmp;
    unTmp.u32 = readl(AIAO_REG_BASE+TX_BUFF_SIZE);
    unTmp.bits.tx_buff_size = size; 
    writel(unTmp.u32, AIAO_REG_BASE+TX_BUFF_SIZE); 

#if defined(DEBUG_AO)
    rt_kprintf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AIAO_REG_BASE+TX_BUFF_SIZE, size);
#endif 
}

void hi3516ev200_ao_set_tran_size(uint32_t size)
{
    U_TX_TRANS_SIZE unTmp;
    unTmp.u32 = readl(AIAO_REG_BASE+TX_TRANS_SIZE);
    unTmp.bits.tx_trans_size = size; 
    writel(unTmp.u32, AIAO_REG_BASE+TX_TRANS_SIZE);  

#if defined(DEBUG_AO)
    rt_kprintf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AIAO_REG_BASE+TX_TRANS_SIZE, size);
#endif 
}

void hi3516ev200_ao_set_buff_wptr(uint32_t offset)
{
    U_TX_BUFF_WPTR unTmp;
    unTmp.u32 = readl(AIAO_REG_BASE+TX_BUFF_WPTR);
    unTmp.bits.tx_buff_wptr = offset; 
    writel(unTmp.u32, AIAO_REG_BASE+TX_BUFF_WPTR); 

#if defined(DEBUG_AO)
    rt_kprintf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AIAO_REG_BASE+TX_BUFF_WPTR, offset);
#endif  
}

void hi3516ev200_ao_set_buff_rptr(uint32_t offset)
{
    U_TX_BUFF_RPTR unTmp;
    unTmp.u32 = readl(AIAO_REG_BASE+TX_BUFF_RPTR);
    unTmp.bits.tx_buff_rptr = offset; 
    writel(unTmp.u32, AIAO_REG_BASE+TX_BUFF_RPTR); 

#if defined(DEBUG_AO)
    rt_kprintf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AIAO_REG_BASE+TX_BUFF_RPTR, offset);
#endif  
}
