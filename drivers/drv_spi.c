/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "rtthread.h"
#include "rtdevice.h"
#include "board.h" 
#include "drv_spi.h" 
#include "drv_pin.h" 

#define BSP_USING_SPI0

/* 开启SPI时钟 */
static void spi_clk_enable(rt_uint32_t spi_base)
{
    rt_uint32_t value; 
    rt_uint8_t bit; 
    if(spi_base == SPI0_REG_BASE) {bit = 12;} else {bit = 13;}
    value = readl(CRG_REG_BASE + PERI_CRG111); 
    value |= 1<<bit; 
    writel(value, CRG_REG_BASE + PERI_CRG111); 
}

/* 关闭SPI时钟 */
static void spi_clk_disable(rt_uint32_t spi_base)
{
    rt_uint32_t value; 
    rt_uint8_t bit; 
    if(spi_base == SPI0_REG_BASE) {bit = 12;} else {bit = 13;}
    value = readl(CRG_REG_BASE + PERI_CRG111); 
    value &= ~(1<<bit);
    writel(value, CRG_REG_BASE + PERI_CRG111);      
}

/* 使能SPI输出 */
static void spi_enable(rt_uint32_t spi_base)
{
    rt_uint16_t value;
    value = readw(spi_base + REG_SPI_CR1);
    value |= SPI_CR1_SSE;
    writew(value, spi_base + REG_SPI_CR1);
}

/* 关闭SPI输出 */
static void spi_disable(rt_uint32_t spi_base)
{
    rt_uint16_t value;
    value = readl(spi_base + REG_SPI_CR1);
    value &= ~SPI_CR1_SSE;
    writel(value, spi_base + REG_SPI_CR1);
}

/* SPI复位 */
static void spi_reset(rt_uint32_t spi_base)
{
    rt_uint32_t value; 
    rt_uint8_t bit; 
    if(spi_base == SPI0_REG_BASE) {bit = 15;} else {bit = 16;}
    value = readl(CRG_REG_BASE+PERI_CRG111); 
    value |= 1<<bit;
    writel(value, CRG_REG_BASE+PERI_CRG111); 
    rt_thread_mdelay(10);
    value &= ~(1<<bit);
    writel(value, CRG_REG_BASE+PERI_CRG111); 
}

/* 配置SPI外设, 时钟1-50MHz 步进单位1MHz */
static rt_err_t spi_config(rt_uint32_t spi_base, rt_uint8_t data_width, rt_uint32_t mode, rt_uint8_t MHz)
{
    rt_uint8_t scr = 0, cpsdvsr = 0;
    rt_uint16_t value; rt_uint16_t tmp;

    /* 关闭SPI外设 */ 
    spi_disable(spi_base); 

    /* 配置SPI外设CLK输出时钟 */ 
    /* Fsspclkout = Fsspclk/(CPSDVR*(1+SCR)), Fsspclk = 100MHz */
    if(MHz == 0) {MHz =  1;}
    if(MHz > 50) {MHz = 50;}

    // cpsdvsr = 2;    /* 2~254 */ 
    // scr = 50/(MHz-1);  /* 0~255 */ 

    cpsdvsr = 2; 
    scr = 0; 

    /* 配置SPICPSR寄存器, SPI时钟配置的一部分 */ 
    value = readw(spi_base + REG_SPI_CPSR);
    value &= ~SPI_CPSR_CPSDVSR;
    value |= cpsdvsr << SPI_CPSR_CPSDVSR_SHIFT;
    writew(value, spi_base + REG_SPI_CPSR);
    
    /* 配置SPICR0寄存器 */
    if(data_width != 8) /* 目前只支持8bit, 其他待定 */ 
    {
        data_width = 8;
    }

    /* 设置数据位宽, 手册634页 */
    value = readw(spi_base + REG_SPI_CR0);
    value &= ~SPI_CR0_DSS;
    value |= (data_width-1) << SPI_CR0_DSS_SHIFT;

    /* 帧格式: 00-MotorolaSPI帧格式, 01-TI同步串行帧格式, 10-NationalMicrowire帧格式 */
    value &= ~SPI_CR0_FRF;  

    /* SPI极性 */ 
    value &= ~SPI_CR0_SPO;
    tmp = (!!(mode & RT_SPI_CPOL)) << SPI_CR0_SPO_SHIFT;
    value |= tmp;

    /* SPI相位 */ 
    value &= ~SPI_CR0_SPH;
    tmp = (!!(mode & RT_SPI_CPHA)) << SPI_CR0_SPH_SHIFT;
    value |= tmp;

    /* 串行时钟率, SPI时钟配置的一部分 */ 
    value &= ~SPI_CR0_SCR;
    value |= (scr << SPI_CR0_SCR_SHIFT);

    writew(value, spi_base + REG_SPI_CR0);

    /* 配置SPICR1寄存器 */
    value = readw(spi_base + REG_SPI_CR1);

    /* SPI环回模式 */
#if 1 
    value &= ~SPI_CR1_LBN; /* 正常模式 */ 
#else
    value |=  SPI_CR1_LBN; /* 环回模式 */ 
#endif

    /* Master/Slave模式配置 */ 
    value &= ~SPI_CR1_MS;

    /* MSB/LSB配置 */ 
    value &= ~SPI_CR1_BIG_END;
    tmp = (!!(mode & RT_SPI_MSB)) << SPI_CR1_BIG_END_SHIFT;
    value |= tmp;

    /* 软件CS/硬件CS配置, TODO */ 
    value &= ~SPI_CR1_ALT;
    value |= 0x1 << SPI_CR1_ALT_SHIFT;

    writew(value, spi_base + REG_SPI_CR1);

    spi_enable(spi_base); 

    return RT_EOK; 
}

/* 等待发送fifo发送完毕, 并且等待spi总线不忙 */ 
static int spi_wait_tx_complete(rt_uint32_t spi_base)
{
    while(1) 
    {
        rt_uint16_t value;
        value = readw(spi_base + REG_SPI_SR);
        if ((value & SPI_SR_TFE) && (!(value & SPI_SR_BSY))) 
        {
            break;
        }
    }

    return 0; 
}

/* 清空FIFO数据 */ 
static int spi_flush_fifo(rt_uint32_t spi_base)
{
    rt_uint16_t value;

    spi_wait_tx_complete(spi_base); 

    while(1)
    {
        value = readw(spi_base + REG_SPI_SR);
        if (!(value & SPI_SR_RNE))
        {
            break;
        }
        readw(spi_base + REG_SPI_DR);
    }

    return 0; 
}

int spi_xfer_8bit(rt_uint32_t spi_base, struct rt_spi_message *message)
{
    rt_uint32_t len = message->length;
    rt_uint32_t tmp_len;
    rt_uint32_t count;
    const rt_uint8_t *tx = (const rt_uint8_t *)(message->send_buf);
    rt_uint8_t *rx = (rt_uint8_t *)(message->recv_buf);
    rt_uint8_t value;

    while(len)
    {
        if(len > 256) {tmp_len = 256;} else {tmp_len = len;}
        len -= tmp_len;

        /* 写数据到FIFO */ 
        count = tmp_len;
        value = 0;
        while(count)
        {
            if(tx)
            {
                value = *tx++;
            }
            writew(value, spi_base + REG_SPI_DR);
            count -= 1;
            readw(spi_base + REG_SPI_SR);
        }

        /* TODO: 超时机制 */ 
        spi_wait_tx_complete(spi_base); 

        /* 从FIFO读取数据 */ 
        count = tmp_len;
        while(count)
        {
            value = readw(spi_base + REG_SPI_DR);
            if(rx)
            {
                *rx++ = value;
            }
            count -= 1;
            readw(spi_base + REG_SPI_SR);
        }
    }

    return RT_EOK; 
}

struct hi_spi
{
    rt_uint32_t base;
    struct rt_spi_configuration *cfg;
};

#if defined(BSP_USING_SPI0)
static struct hi_spi spi0 = {0}; 
static struct rt_spi_bus spi0_bus = {0}; 
#endif

#if defined(BSP_USING_SPI1)
static struct hi_spi spi1 = {0}; 
static struct rt_spi_bus spi1_bus = {0}; 
#endif

rt_err_t hi_spi_bus_attach_device(const char *bus_name, const char *device_name, rt_uint32_t pin)
{
    rt_err_t ret = RT_EOK; 
    
    struct rt_spi_device *spi_device = (struct rt_spi_device *)rt_malloc(sizeof(struct rt_spi_device)); 
    RT_ASSERT(spi_device != RT_NULL); 
    
    struct hi_sw_spi_cs *cs_pin = (struct hi_sw_spi_cs *)rt_malloc(sizeof(struct hi_sw_spi_cs)); 
    RT_ASSERT(cs_pin != RT_NULL);
    
    cs_pin->pin = pin;
    rt_pin_mode(pin, PIN_MODE_OUTPUT); 
    rt_pin_write(pin, PIN_HIGH); 
    
    ret = rt_spi_bus_attach_device(spi_device, device_name, bus_name, (void *)cs_pin); 
    
    return ret; 
}

static rt_err_t configure(struct rt_spi_device *device, struct rt_spi_configuration *cfg)
{
    rt_err_t ret = RT_EOK; 
    struct hi_spi *spi = RT_NULL; 

    RT_ASSERT(cfg != RT_NULL);
    RT_ASSERT(device != RT_NULL);
    RT_ASSERT(device->bus->parent.user_data != RT_NULL);

    spi = (struct hi_spi *)(device->bus->parent.user_data); 
    spi->cfg = cfg; 
    rt_kprintf("w = %d, mode = %d, max_hz = %d\n", cfg->data_width, cfg->mode, cfg->max_hz/(1000*1000)); 
    spi_config(spi->base, cfg->data_width, cfg->mode, cfg->max_hz/(1000*1000)); 

    return ret;
}

static rt_uint32_t spixfer(struct rt_spi_device *device, struct rt_spi_message *message)
{
    RT_ASSERT(device != RT_NULL);
    RT_ASSERT(device->bus != RT_NULL);
    RT_ASSERT(device->bus->parent.user_data != RT_NULL);

    struct hi_spi *spi = (struct hi_spi *)(device->bus->parent.user_data); 
    struct hi_sw_spi_cs *cs = device->parent.user_data; 
    
    if(message->cs_take)
    {
        rt_pin_write(cs->pin, PIN_LOW);
    }
    
    spi_flush_fifo(spi->base); 
    spi_xfer_8bit(spi->base, message); 

    if(message->cs_release)
    {
        rt_pin_write(cs->pin, PIN_HIGH);
    }
    
    return message->length; 
}

static struct rt_spi_ops ops = 
{
    configure, 
    spixfer
}; 

int rt_hw_spi_init(void)
{
#if defined(BSP_USING_SPI0)
    spi_clk_disable(SPI0_REG_BASE); 
    spi_clk_enable(SPI0_REG_BASE); 
    spi_reset(SPI0_REG_BASE); 

    spi0.base = SPI0_REG_BASE; 
    spi0.cfg = RT_NULL; 
    spi0_bus.parent.user_data = &spi0; 

    rt_gpio_set_func( 9, 1, 0, 0x7, 0x7, 1); /* CLK */
    rt_gpio_set_func(10, 1, 0, 0x3, 0x7, 0); /* SDI */ 
    rt_gpio_set_func(11, 1, 0, 0x3, 0x7, 1); /* SDO */

    rt_spi_bus_register(&spi0_bus, "spi0", &ops); 
#endif

#if defined(BSP_USING_SPI1)
    spi_clk_disable(SPI1_REG_BASE); 
    spi_clk_enable(SPI1_REG_BASE); 
    spi_reset(SPI1_REG_BASE); 

    spi1.base = SPI1_REG_BASE; 
    spi1.cfg = RT_NULL; 
    spi1_bus.parent.user_data = &spi1; 

    rt_gpio_set_func(82, 1, 0, 0x7, 0x1, 1); /* CLK */
    rt_gpio_set_func(83, 1, 0, 0x3, 0x1, 0); /* SDI */ 
    rt_gpio_set_func(86, 1, 0, 0x3, 0x1, 1); /* SDO */

    rt_spi_bus_register(&spi1_bus, "spi1", &ops); 
#endif

    return RT_EOK; 
}
INIT_DEVICE_EXPORT(rt_hw_spi_init); 
