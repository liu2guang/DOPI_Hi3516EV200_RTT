/*
* Copyright (c) 2006-2018, RT-Thread Development Team
*
* SPDX-License-Identifier: Apache-2.0
*/
#include "rtthread.h"
#include "rtdevice.h"
#include "board.h" 

static uint32_t parse_i2s_fs_reg(uint32_t rate)
{
    uint32_t fs = 0;
    switch(rate)
    {
    case 8000 : fs = 0x18; break; 
    case 12000: fs = 0x18; break; 
    case 11025: fs = 0x18; break; 
    case 16000: fs = 0x19; break; 
    case 22050: fs = 0x19; break; 
    case 24000: fs = 0x19; break; 
    case 32000: fs = 0x1a; break; 
    case 44100: fs = 0x1a; break; 
    case 48000: fs = 0x1a; break; 
    default: rt_kprintf("unsupport samplerate %d\n", rate); break; 
    }
    return fs; 
}

static uint32_t parse_mode_reg(uint32_t rate)
{
    uint32_t fs = 0;
    switch(rate)
    {
    case 8000 : fs = 0x1; break; 
    case 12000: fs = 0x0; break; 
    case 11025: fs = 0x0; break; 
    case 16000: fs = 0x1; break; 
    case 22050: fs = 0x0; break; 
    case 24000: fs = 0x0; break; 
    case 32000: fs = 0x1; break; 
    case 44100: fs = 0x0; break; 
    case 48000: fs = 0x0; break; 
    default: rt_kprintf("unsupport samplerate %d\n", rate); break; 
    }
    return fs; 
}

static void codec_soft_reset(void)
{
    ACODEC_DIGCTRL1_U acodec_digctrl1;
    ACODEC_DIGCTRL2_U acodec_digctrl2;
    ACODEC_DIGCTRL3_U acodec_digctrl3;
    ACODEC_DIGCTRL4_U acodec_digctrl4;
    ACODEC_DIGCTRL5_U acodec_digctrl5;

    ACODEC_ANAREG0_U acodec_anareg0;
    ACODEC_ANAREG1_U acodec_anareg1;
    ACODEC_ANAREG2_U acodec_anareg2;
    ACODEC_ANAREG3_U acodec_anareg3;
    ACODEC_ANAREG4_U acodec_anareg4;
    ACODEC_ANAREG5_U acodec_anareg5;

    acodec_anareg0.ul32 = 0x1C1C0000;
    writel(acodec_anareg0.ul32, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG0_REG); 

    acodec_anareg1.ul32 = 0xDF605E66;
    writel(acodec_anareg1.ul32, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG1_REG); 

    acodec_anareg2.ul32 = 0x00255544;
    writel(acodec_anareg2.ul32, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG2_REG); 

    acodec_anareg3.ul32 = 0x072D0451;
    writel(acodec_anareg3.ul32, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG3_REG); 

    acodec_anareg4.ul32 = 0x00000000;
    writel(acodec_anareg4.ul32, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG4_REG); 

    acodec_anareg5.ul32 = 0x00000000;
    writel(acodec_anareg5.ul32, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG5_REG); 

    acodec_digctrl1.ul32 = 0xff035a00;
    writel(acodec_digctrl1.ul32, AUDIO_CTRL_REG_BASE+AUDIO_DIGCTRL1_REG); 

    acodec_digctrl2.ul32 = 0x08000001;
    writel(acodec_digctrl2.ul32, AUDIO_CTRL_REG_BASE+AUDIO_DIGCTRL2_REG); 

    acodec_digctrl3.ul32 = 0x06062424;
    writel(acodec_digctrl3.ul32, AUDIO_CTRL_REG_BASE+AUDIO_DIGCTRL3_REG); 

    acodec_digctrl4.ul32 = 0x1e1ec001;
    writel(acodec_digctrl4.ul32, AUDIO_CTRL_REG_BASE+AUDIO_DIGCTRL4_REG); 

    acodec_digctrl5.ul32 = 0x24242424;
    writel(acodec_digctrl5.ul32, AUDIO_CTRL_REG_BASE+AUDIO_DIGCTRL5_REG); 
}

int hi3516ev200_codec_set_samplerate(uint32_t rate)
{
    ACODEC_DIGCTRL1_U unDigctrl1;
    ACODEC_ANAREG1_U  unAnaReg1;

    unDigctrl1.ul32 = readl(AUDIO_CTRL_REG_BASE+AUDIO_DIGCTRL1_REG); 
    unDigctrl1.bits.i2s1_fs_sel = parse_i2s_fs_reg(rate);
    writel(unDigctrl1.ul32, AUDIO_CTRL_REG_BASE+AUDIO_DIGCTRL1_REG); 

    unAnaReg1.ul32 = readl(AUDIO_CTRL_REG_BASE+AUDIO_ANAREG1_REG); 
    unAnaReg1.bits.mode_adcr = parse_mode_reg(rate);
    unAnaReg1.bits.mode_adcl = parse_mode_reg(rate);
    writel(unAnaReg1.ul32, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG1_REG); 

    unAnaReg1.ul32 = readl(AUDIO_CTRL_REG_BASE+AUDIO_ANAREG1_REG); 
    unAnaReg1.bits.en_rctune = 0;
    writel(unAnaReg1.ul32, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG1_REG); 

    rt_thread_mdelay(1);
    unAnaReg1.ul32 = readl(AUDIO_CTRL_REG_BASE+AUDIO_ANAREG1_REG); 
    unAnaReg1.bits.en_rctune = 1;
    writel(unAnaReg1.ul32, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG1_REG); 
    rt_thread_mdelay(100);
    
    return 0;
}

int hi3516ev200_codec_init(void)
{
    // 配置CRG
    write_reg_bit(1, 1, CRG_REG_BASE+PERI_CRG103); 
    write_reg_bit(1, 2, CRG_REG_BASE+PERI_CRG103); 

    // 配置AIAO时钟
    writel(0x00152EF0, AIAO_REG_BASE+I2S_CRG_CFG0_08);
    writel(0x00000115, AIAO_REG_BASE+I2S_CRG_CFG1_08);
    rt_thread_mdelay(50);

    // 配置AIAO Mux
    writel(0x0000000F, AUDIO_CTRL_REG_BASE+AUDIO_IIS_MUX_CTRL); 

    // 配置DIGCTRL
    writel(0XFF035A00, AUDIO_CTRL_REG_BASE+AUDIO_DIGCTRL1_REG); 
    writel(0x08000001, AUDIO_CTRL_REG_BASE+AUDIO_DIGCTRL2_REG);
    writel(0X7E7E2424, AUDIO_CTRL_REG_BASE+AUDIO_DIGCTRL3_REG);
    rt_thread_mdelay(1);

    // 配置ANAREG
    #define AUDIO_ANAREG0_DEFAULT 0x3434DFFF
    #define AUDIO_ANAREG1_DEFAULT 0xDF605E65
    #define AUDIO_ANAREG2_DEFAULT 0x00255548
    #define AUDIO_ANAREG3_DEFAULT 0x04293B50
    #define AUDIO_ANAREG4_DEFAULT 0x00000000
    #define AUDIO_ANAREG5_DEFAULT 0x00000000

    writel(AUDIO_ANAREG0_DEFAULT, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG0_REG); 
    writel(AUDIO_ANAREG1_DEFAULT, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG1_REG);
    writel(AUDIO_ANAREG2_DEFAULT, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG2_REG);
    writel(AUDIO_ANAREG3_DEFAULT, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG3_REG); 
    writel(AUDIO_ANAREG4_DEFAULT, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG4_REG);
    writel(AUDIO_ANAREG5_DEFAULT, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG5_REG);

    /* 配置内置codec */ 
    ACODEC_ANAREG0_U   acodec_anareg0;
    ACODEC_ANAREG1_U   acodec_anareg1;
    ACODEC_ANAREG2_U   acodec_anareg2;
    ACODEC_ANAREG3_U   acodec_anareg3;
    // ACODEC_ANAREG4_U   acodec_anareg4;
    // ACODEC_ANAREG5_U   acodec_anareg5;

    /* 2. */
    acodec_anareg3.ul32 = readl(AUDIO_CTRL_REG_BASE+AUDIO_ANAREG3_REG); 
    acodec_anareg3.bits.rstn = 0x1;
    acodec_anareg3.bits.pd_dac_clk = 0x0;
    writel(acodec_anareg3.ul32, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG3_REG); 
    // rt_kprintf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG3_REG, acodec_anareg3.ul32);

    /* 3. */
    acodec_anareg3.ul32 = readl(AUDIO_CTRL_REG_BASE+AUDIO_ANAREG3_REG); 
    acodec_anareg3.bits.pop_rst = 0x1;
    writel(acodec_anareg3.ul32, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG3_REG); 
    // rt_kprintf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG3_REG, acodec_anareg3.ul32);

    /* 5. */
    acodec_anareg0.ul32 = readl(AUDIO_CTRL_REG_BASE+AUDIO_ANAREG0_REG); 
    acodec_anareg0.bits.pd_ctcm_rx  = 0x0;
    acodec_anareg0.bits.pd_ctcm_tx  = 0x0;
    acodec_anareg0.bits.pd_dac_vref = 0x0;
    acodec_anareg0.bits.pd_ibias    = 0x0;
    acodec_anareg0.bits.pd_ldo      = 0x0;
    writel(acodec_anareg0.ul32, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG0_REG); 
    // rt_kprintf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG0_REG, acodec_anareg3.ul32);

    /* 6. */
    acodec_anareg3.ul32 = readl(AUDIO_CTRL_REG_BASE+AUDIO_ANAREG3_REG); 
    acodec_anareg3.bits.pd_dacl_dff = 0x0;
    acodec_anareg3.bits.pd_dacr_dff = 0x0;
    acodec_anareg3.bits.mute_dacl   = 0x0;
    acodec_anareg3.bits.mute_dacr   = 0x0;
    writel(acodec_anareg3.ul32, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG3_REG); 
    // rt_kprintf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG3_REG, acodec_anareg3.ul32);

    /* 7. */
    acodec_anareg3.ul32 = readl(AUDIO_CTRL_REG_BASE+AUDIO_ANAREG3_REG); 
    acodec_anareg3.bits.pu_pop_pullb_reg = 0x1;
    writel(acodec_anareg3.ul32, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG3_REG); 
    // rt_kprintf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG3_REG, acodec_anareg3.ul32);

    /* 8. */
    acodec_anareg0.ul32 = readl(AUDIO_CTRL_REG_BASE+AUDIO_ANAREG0_REG); 
    acodec_anareg0.bits.pd_vref = 0x0;
    writel(acodec_anareg0.ul32, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG0_REG); 
    // rt_kprintf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG0_REG, acodec_anareg3.ul32);

    acodec_anareg2.ul32 = readl(AUDIO_CTRL_REG_BASE+AUDIO_ANAREG2_REG); 
    acodec_anareg2.bits.vref_pu_pdb = 0x1;
    writel(acodec_anareg2.ul32, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG2_REG); 
    // rt_kprintf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG2_REG, acodec_anareg3.ul32);

    rt_thread_mdelay(20);

    /* 9. */
    acodec_anareg3.ul32 = readl(AUDIO_CTRL_REG_BASE+AUDIO_ANAREG3_REG); 
    acodec_anareg3.bits.pop_rst = 0x0;
    writel(acodec_anareg3.ul32, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG3_REG); 
    // rt_kprintf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG3_REG, acodec_anareg3.ul32);

    /* 10. */
    acodec_anareg0.ul32 = readl(AUDIO_CTRL_REG_BASE+AUDIO_ANAREG0_REG); 
    acodec_anareg0.bits.pd_lineoutl = 0x0;
    acodec_anareg0.bits.pd_lineoutr = 0x0;
    writel(acodec_anareg0.ul32, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG0_REG); 
    // rt_kprintf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG0_REG, acodec_anareg3.ul32);

    rt_thread_mdelay(10);

    /* 11. */
    acodec_anareg3.ul32 = readl(AUDIO_CTRL_REG_BASE+AUDIO_ANAREG3_REG); 
    acodec_anareg3.bits.pop_lineout_pull_en = 0x0;
    writel(acodec_anareg3.ul32, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG3_REG); 
    // rt_kprintf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG3_REG, acodec_anareg3.ul32);

    /* 12. */
    acodec_anareg0.ul32 = readl(AUDIO_CTRL_REG_BASE+AUDIO_ANAREG0_REG); 
    acodec_anareg0.bits.pd_micbias1 = 0x0;
    acodec_anareg0.bits.pd_micbias2 = 0x0;
    writel(acodec_anareg0.ul32, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG0_REG); 
    // rt_kprintf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG0_REG, acodec_anareg3.ul32);

    /* 13. */
    acodec_anareg1.ul32 = readl(AUDIO_CTRL_REG_BASE+AUDIO_ANAREG1_REG); 
    acodec_anareg1.bits.pd_rctune = 0x0;
    writel(acodec_anareg1.ul32, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG1_REG); 
    // rt_kprintf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG1_REG, acodec_anareg3.ul32);

    /* 14. */
    acodec_anareg1.ul32 = readl(AUDIO_CTRL_REG_BASE+AUDIO_ANAREG1_REG); 
    acodec_anareg1.bits.en_rctune = 0x1;
    writel(acodec_anareg1.ul32, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG1_REG);
    // rt_kprintf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG1_REG, acodec_anareg3.ul32);

    rt_thread_mdelay(1);

    /* 15. */
    acodec_anareg0.ul32 = readl(AUDIO_CTRL_REG_BASE+AUDIO_ANAREG0_REG); 
    acodec_anareg0.bits.pd_linein_l = 0x0;
    acodec_anareg0.bits.pd_linein_r = 0x0;
    acodec_anareg0.bits.pd_adcl = 0x0;
    acodec_anareg0.bits.pd_adcr = 0x0;
    writel(acodec_anareg0.ul32, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG0_REG); 
    // rt_kprintf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG0_REG, acodec_anareg3.ul32);

    /* 16. */
    acodec_anareg3.ul32 = readl(AUDIO_CTRL_REG_BASE+AUDIO_ANAREG3_REG); 
    acodec_anareg3.bits.rstb_dac = 0x1;
    writel(acodec_anareg3.ul32, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG3_REG);
    // rt_kprinstf("%s %d 0x%.8x = 0x%.8x \n", __func__, __LINE__, AUDIO_CTRL_REG_BASE+AUDIO_ANAREG3_REG, acodec_anareg3.ul32);

    rt_thread_mdelay(1);
    codec_soft_reset(); 

    return 0;  
}
