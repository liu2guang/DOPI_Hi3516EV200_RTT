/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "rtthread.h"
#include "rtdevice.h"
#include "board.h" 
#include "drv_pin.h" 

#define PIN_NUM(n) (sizeof(n) / sizeof(n[0]))

#define GPIO_DATA(base, off) (((base) + 0x000) + (1 << ((off) + 2)))
#define GPIO_DIR(base)       ((base) + 0x400)
#define GPIO_IS(base)        ((base) + 0x404)
#define GPIO_IBE(base)       ((base) + 0x408)
#define GPIO_IEV(base)       ((base) + 0x40C)
#define GPIO_IE(base)        ((base) + 0x410)
#define GPIO_RIS(base)       ((base) + 0x414)
#define GPIO_MIS(base)       ((base) + 0x418)
#define GPIO_IC(base)        ((base) + 0x41C)

/* Hi3516EV200_PINOUT_CN.xlsx */ 
#define iocfg_reg00          0x100C0000
#define iocfg_reg01          0x100C0004
#define iocfg_reg02          0x100C0008
#define iocfg_reg03          0x100C000C
#define iocfg_reg04          0x100C0010
#define iocfg_reg05          0x100C0014
#define iocfg_reg06          0x100C0018
#define iocfg_reg07          0x100C001C
#define iocfg_reg08          0x100C0020
#define iocfg_reg09          0x100C0024
#define iocfg_reg10          0x100C0028
#define iocfg_reg11          0x100C002C

#define iocfg_reg12          0x100C0040
#define iocfg_reg13          0x100C0044
#define iocfg_reg14          0x100C0048
#define iocfg_reg15          0x100C004C
#define iocfg_reg16          0x100C0050
#define iocfg_reg17          0x100C0054
#define iocfg_reg18          0x100C005C

#define iocfg_reg19          0x112C0000
#define iocfg_reg20          0x112C0004
#define iocfg_reg21          0x112C0008
#define iocfg_reg22          0x112C000C
#define iocfg_reg23          0x112C0010
#define iocfg_reg24          0x112C0014

#define iocfg_reg25          0x112C0028
#define iocfg_reg26          0x112C002C
#define iocfg_reg27          0x112C0030
#define iocfg_reg28          0x112C0034
#define iocfg_reg29          0x112C0038
#define iocfg_reg30          0x112C003C
#define iocfg_reg31          0x112C0040
#define iocfg_reg32          0x112C0044
#define iocfg_reg33          0x112C0048
#define iocfg_reg34          0x112C004C
#define iocfg_reg35          0x112C0050
#define iocfg_reg36          0x112C0054
#define iocfg_reg37          0x112C0058
#define iocfg_reg38          0x112C005C
#define iocfg_reg39          0x112C0060
#define iocfg_reg40          0x112C0064
#define iocfg_reg41          0x112C0068
#define iocfg_reg42          0x112C006C
#define iocfg_reg43          0x112C0070
#define iocfg_reg44          0x112C0074

#define iocfg_reg45          0x120C0000
#define iocfg_reg46          0x120C0004
#define iocfg_reg47          0x120C0010
#define iocfg_reg48          0x120C0014
#define iocfg_reg49          0x120C0018
#define iocfg_reg50          0x120C001C
#define iocfg_reg51          0x120C0020

struct _pin_index
{
    rt_uint8_t  id;
    rt_base_t   gpio_base;  /* gpio group base reg */ 
    rt_base_t   pin;        /* gpio pin offset */ 
    rt_base_t   mux_base;   /* gpio mux */ 
    rt_uint8_t  mux_func;   /* gpio mux func */ 
};

/* convert each pin number to its port and internal pin */
static struct _pin_index pin_index[] = 
{
//  id      gpio_base      pin mux_base     mux_func 
    {0,     0,              0, 0          , 0x00}, /* Unused */
    {1,     0,              0, 0          , 0x00}, /* AVDD_BAT */
    {2,     0,              0, 0          , 0x00}, /* RTC_XIN */
    {3,     0,              0, 0          , 0x00}, /* RTC_XOUT */
    {4,     0,              0, 0          , 0x00}, /* AVDD33_PLL */
    {5,     0,              0, 0          , 0x00}, /* AVDD_PLL */
    {6,     0,              0, 0          , 0x00}, /* XIN */
    {7,     0,              0, 0          , 0x00}, /* XOUT */
    {8,     0,              0, 0          , 0x00}, /* DVDD33 */
    {9,     GPIO6_REG_BASE, 4, iocfg_reg44, 0x00}, /* GPIO6_4 */
    {10,    GPIO6_REG_BASE, 5, iocfg_reg43, 0x00}, /* GPIO6_5 */
    {11,    GPIO6_REG_BASE, 6, iocfg_reg42, 0x00}, /* GPIO6_6 */
    {12,    GPIO6_REG_BASE, 7, iocfg_reg41, 0x00}, /* GPIO6_7 */
    {13,    0,              0, 0          , 0x00}, /* VDD */
    {14,    GPIO7_REG_BASE, 0, iocfg_reg37, 0x00}, /* GPIO7_0 */
    {15,    GPIO7_REG_BASE, 1, iocfg_reg38, 0x00}, /* GPIO7_1 */
    {16,    GPIO7_REG_BASE, 2, iocfg_reg39, 0x00}, /* GPIO7_2 */
    {17,    0,              0, 0          , 0x00}, /* DVDD3318_VIVO */
    {18,    GPIO7_REG_BASE, 3, iocfg_reg40, 0x00}, /* GPIO7_3 */
    {19,    GPIO8_REG_BASE, 7, iocfg_reg33, 0x00}, /* GPIO8_7 */
    {20,    GPIO8_REG_BASE, 5, iocfg_reg34, 0x00}, /* GPIO8_5 */
    {21,    GPIO8_REG_BASE, 6, iocfg_reg35, 0x00}, /* GPIO8_6 */
    {22,    GPIO8_REG_BASE, 4, iocfg_reg36, 0x00}, /* GPIO8_4 */
    {23,    0,              0, 0          , 0x00}, /* ETH_MDI_BN */
    {24,    0,              0, 0          , 0x00}, /* ETH_MDI_BP */
    {25,    0,              0, 0          , 0x00}, /* AVDD33_FE */
    {26,    0,              0, 0          , 0x00}, /* ETH_MDI_AN */
    {27,    0,              0, 0          , 0x00}, /* ETH_MDI_AP */
    {28,    GPIO5_REG_BASE, 1, iocfg_reg32, 0x00}, /* GPIO85_1 */
    {29,    GPIO5_REG_BASE, 0, iocfg_reg31, 0x00}, /* GPIO5_0 */
    {30,    GPIO6_REG_BASE, 3, iocfg_reg30, 0x00}, /* GPIO6_3 */
    {31,    GPIO6_REG_BASE, 2, iocfg_reg29, 0x00}, /* GPIO6_2 */
    {32,    0,              0, 0          , 0x00}, /* DVDD3318_SENSOR */
    {33,    GPIO5_REG_BASE, 6, iocfg_reg27, 0x00}, /* GPIO5_6 */
    {34,    GPIO5_REG_BASE, 7, iocfg_reg28, 0x00}, /* GPIO5_7 */
    {35,    GPIO5_REG_BASE, 4, iocfg_reg25, 0x04}, /* GPIO5_4 */
    {36,    GPIO5_REG_BASE, 5, iocfg_reg26, 0x00}, /* GPIO5_5 */
    {37,    0,              0, 0          , 0x00}, /* VDD */
    {38,    0,              0, 0          , 0x00}, /* AVDD3318_MIPIRX */
    {39,    0,              0, iocfg_reg20, 0x00}, /* MIPI_RX_CK0P */
    {40,    0,              0, iocfg_reg19, 0x00}, /* MIPI_RX_CK0N */
    {41,    0,              0, iocfg_reg22, 0x00}, /* MIPI_RX_D0P */
    {42,    0,              0, iocfg_reg21, 0x00}, /* MIPI_RX_D0N */
    {43,    0,              0, iocfg_reg24, 0x00}, /* MIPI_RX_D2P */
    {44,    0,              0, iocfg_reg23, 0x00}, /* MIPI_RX_D2N */
    {45,    0,              0, 0          , 0x00}, /* AVSS_AC */
    {46,    0,              0, 0          , 0x00}, /* AC_VREF */
    {47,    0,              0, 0          , 0x00}, /* AC_INL */
    {48,    0,              0, 0          , 0x00}, /* AC_MICBIAS */
    {49,    0,              0, 0          , 0x00}, /* AC_OUTL */
    {50,    0,              0, 0          , 0x00}, /* AVDD33_AC_U2 */
    {51,    0,              0, 0          , 0x00}, /* USB_DP */
    {52,    0,              0, 0          , 0x00}, /* USB_DM */
    {53,    0,              0, 0          , 0x00}, /* VDD */
    {54,    GPIO4_REG_BASE, 4, iocfg_reg16, 0x00}, /* GPIO4_4 */
    {55,    GPIO4_REG_BASE, 5, iocfg_reg17, 0x00}, /* GPIO4_5 */
    {56,    GPIO4_REG_BASE, 1, iocfg_reg13, 0x00}, /* GPIO4_1 */
    {57,    GPIO4_REG_BASE, 0, iocfg_reg12, 0x00}, /* GPIO4_0 */
    {58,    GPIO4_REG_BASE, 2, iocfg_reg14, 0x00}, /* GPIO4_2 */  
    {59,    0,              0, 0          , 0x00}, /* DVDD3318_FLASH */ 
    {60,    GPIO4_REG_BASE, 3, iocfg_reg15, 0x00}, /* GPIO4_3 */ 
    {61,    GPIO4_REG_BASE, 7, iocfg_reg18, 0x00}, /* GPIO4_7 */ 
    {62,    0,              0, 0          , 0x00}, /* VDD */ 
    {63,    0,              0, iocfg_reg08, 0x00}, /* SFC_WP_IO2 */ 
    {64,    0,              0, iocfg_reg07, 0x00}, /* SFC_MISO_IO1 */ 
    {65,    0,              0, iocfg_reg10, 0x00}, /* SFC_CSN */ 
    {66,    0,              0, iocfg_reg09, 0x00}, /* SFC_HOLD_IO3 */ 
    {67,    0,              0, iocfg_reg05, 0x00}, /* SFC_CLK */ 
    {68,    0,              0, iocfg_reg06, 0x00}, /* SFC_MOSI_IO0 */ 
    {69,    0,              0, iocfg_reg11, 0x00}, /* SYS_RSTN_OUT */ 
    {70,    GPIO0_REG_BASE, 3, iocfg_reg03, 0x00}, /* GPIO0_3 */ 
    {71,    GPIO0_REG_BASE, 4, iocfg_reg04, 0x00}, /* GPIO0_4 */ 
    {72,    GPIO0_REG_BASE, 0, iocfg_reg02, 0x00}, /* GPIO0_0 */ 
    {73,    GPIO0_REG_BASE, 2, iocfg_reg01, 0x00}, /* GPIO0_2 */ 
    {74,    GPIO0_REG_BASE, 1, iocfg_reg00, 0x00}, /* GPIO0_1 */ 
    {75,    0,              0, 0          , 0x00}, /* VDDIO_DDR */ 
    {76,    0,              0, 0          , 0x00}, /* AVDD33_DDR_PLL */ 
    {77,    0,              0, 0          , 0x00}, /* VDD */ 
    {78,    0,              0, 0          , 0x00}, /* VDDIO_DDR */ 
    {79,    0,              0, 0          , 0x00}, /* VSS */ 
    {80,    0,              0, 0          , 0x00}, /* VDD */ 
    {81,    GPIO1_REG_BASE, 4, iocfg_reg47, 0x02}, /* GPIO1_4 */ 
    {82,    GPIO1_REG_BASE, 5, iocfg_reg48, 0x02}, /* GPIO1_5 */ 
    {83,    GPIO1_REG_BASE, 6, iocfg_reg49, 0x02}, /* GPIO1_6 */ 
    {84,    0,              0, 0          , 0x00}, /* DVDD33 */ 
    {85,    GPIO2_REG_BASE, 0, iocfg_reg51, 0x02}, /* GPIO2_0 */ 
    {86,    GPIO1_REG_BASE, 7, iocfg_reg50, 0x02}, /* GPIO1_7 */ 
    {87,    GPIO1_REG_BASE, 1, iocfg_reg46, 0x00}, /* GPIO1_1 */ 
    {88,    GPIO1_REG_BASE, 0, iocfg_reg45, 0x00}, /* GPIO1_0 */ 
};

/* 设置管脚复用 */ 
int rt_gpio_set_func(rt_base_t pin, rt_uint8_t speed, rt_uint8_t pull, rt_uint8_t drive_capability, rt_uint32_t func, rt_uint8_t dir)
{
    rt_uint32_t val = 0;
    rt_uint32_t addr = 0;  

    /* SPEED PULL FUNC */ 
    if(speed) {val &= ~(1 << 10);} else {val |= 1 << 10;}   // 0low, 1high
    if(pull == 1) {val |= 0x1 << 8;}                        // 0none, 1down, 2up
    if(pull == 2) {val |= 0x2 << 8;}
    val |= (drive_capability<<4 | func);                   // gpio func
    writel(val, pin_index[pin].mux_base); 

    /* DIR */
    addr = GPIO_DIR(pin_index[pin].gpio_base);
    val = readl(addr);
    if(dir) {val |= 1 << pin_index[pin].pin;} else {val &= ~(1 << pin_index[pin].pin);}
    writel(val, addr);

    // rt_kprintf("pin = %d, dirx(0x%.8x) = 0x%.8x\n", pin, addr, val); 

    return RT_EOK;   
}

int hi3516ev200_gpio_init(rt_base_t pin, rt_uint8_t speed, rt_uint8_t pull, rt_uint8_t drive_capability, rt_uint8_t dir)
{
    rt_uint32_t val = 0;
    rt_gpio_set_func(pin, speed, pull, drive_capability, pin_index[pin].mux_func, dir); 
    // rt_kprintf("pin = %d, mode(0x%.8x) = 0x%.8x\n", pin, pin_index[pin].mux_base, val); 

    return RT_EOK;
}

static void hi3516ev200_gpio_write(rt_base_t pin, rt_uint8_t level)
{
    rt_uint32_t val = 0;
    rt_uint32_t addr = 0; 

    addr = GPIO_DATA(pin_index[pin].gpio_base, pin_index[pin].pin);
    val = readl(addr);
    if (level) {val |= (1 << pin_index[pin].pin);} else {val &= ~(1 << pin_index[pin].pin);}
    writel(val, addr);
}

static rt_uint32_t hi3516ev200_gpio_read(rt_base_t pin)
{
    rt_uint32_t val = 0;
    rt_uint32_t addr = 0; 

    addr = GPIO_DATA(pin_index[pin].gpio_base, pin_index[pin].pin);
    val = readl(addr);
    if (val & (1 << pin_index[pin].pin)) {val = 1;} else {val = 0;}
    return val;
}

static void pin_mode(struct rt_device *dev, rt_base_t pin, rt_base_t mode)
{
    rt_uint8_t speed = 1; 
    rt_uint8_t pull = 0;
    rt_uint8_t drive_capability = 0;
    rt_uint8_t dir = 0;

    switch (mode)
    {
    case PIN_MODE_OUTPUT:
        dir = 1; pull = 0; 
        break;
    case PIN_MODE_INPUT:
        dir = 0; pull = 0; 
        break;
    case PIN_MODE_INPUT_PULLUP:
        dir = 0; pull = 2; 
        break;
    case PIN_MODE_INPUT_PULLDOWN:
        dir = 0; pull = 1; 
        break;
    }
    
    hi3516ev200_gpio_init(pin, speed, pull, drive_capability, dir); 
}

static void pin_write(struct rt_device *dev, rt_base_t pin, rt_base_t value)
{
    hi3516ev200_gpio_write(pin, value);
}

static int pin_read(struct rt_device *device, rt_base_t pin)
{
    return hi3516ev200_gpio_read(pin);
}

static const struct rt_pin_ops ops =
{
    pin_mode,
    pin_write,
    pin_read,
};

int rt_hw_pin_init(void)
{
    rt_device_pin_register("pin", &ops, RT_NULL);
    return 0;
}
INIT_DEVICE_EXPORT(rt_hw_pin_init);
