/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "rtthread.h"
#include "rtdevice.h"
#include "board.h"
#include "drv_lcd.h"
#include "drv_lcd_ili9341.h"

static struct rt_device lcd; 
static struct rt_device_graphic_info info;

static void set_pixel(const char *pixel, int x, int y)
{
    ili9341_set_fgcolor565(*(rt_uint16_t *)pixel); 
    ili9341_draw_pixel(x, y); 
}

static void get_pixel(char *pixel, int x, int y)
{
    *(rt_uint16_t *)pixel = (rt_uint16_t)0xFFFF; 
}

static void draw_hline(const char *pixel, int x1, int x2, int y)
{
    ili9341_set_fgcolor565(*(rt_uint16_t *)pixel); 
    ili9341_draw_line(x1, y, x2, y); 
}

static void draw_vline(const char *pixel, int x, int y1, int y2)
{
    ili9341_set_fgcolor565(*(rt_uint16_t *)pixel); 
    ili9341_draw_line(x, y1, x, y2); 
}

static void blit_line(const char *pixel, int x, int y, rt_size_t size)
{
    /* TODO */
    ili9341_send_pixels(x, y, x+size-1, y, (uint8_t *)pixel, size); 
}

const struct rt_device_graphic_ops ops =
{
    set_pixel,
    get_pixel,
    draw_hline,
    draw_vline,
    blit_line
};

static rt_err_t init(rt_device_t dev)
{
    return RT_EOK;
}

static rt_err_t control(rt_device_t dev, int cmd, void *args)
{
    switch (cmd)
    {
    case RTGRAPHIC_CTRL_RECT_UPDATE:
        rt_kprintf("unable to support rect update\n"); 
        break;
    case RTGRAPHIC_CTRL_POWERON:
        ili9341_set_display(1); 
        break;
    case RTGRAPHIC_CTRL_POWEROFF:
        ili9341_set_display(0); 
        break;
    case RTGRAPHIC_CTRL_GET_INFO:
        rt_memcpy(args, &info, sizeof(struct rt_device_graphic_info));
        break;
    case RTGRAPHIC_CTRL_SET_MODE: 
        rt_kprintf("unable to switch lcd show formats\n"); 
        break;
    }
    return RT_EOK;
}

int rt_hw_lcd_init(void)
{
    ili9341_init(); 

    /* lcd info */
    info.bits_per_pixel = 16;
    info.pixel_format   = RTGRAPHIC_PIXEL_FORMAT_RGB565;
    info.framebuffer    = RT_NULL;
    info.width          = 320;
    info.height         = 240;

    /* init device structure */
    lcd.type      = RT_Device_Class_Graphic;
    lcd.init      = init;
    lcd.open      = RT_NULL;
    lcd.close     = RT_NULL;
    lcd.control   = control;
    lcd.user_data = (void *)&ops;
    rt_device_register(&lcd, "lcd", RT_DEVICE_FLAG_RDWR);

    return RT_EOK;
}
INIT_ENV_EXPORT(rt_hw_lcd_init); 
