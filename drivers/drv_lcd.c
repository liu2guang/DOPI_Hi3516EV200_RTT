/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "rtthread.h"
#include "rtdevice.h"
#include "board.h"
#include "drv_lcd.h"
#include "drv_spi.h"

static struct rt_spi_device *spi = RT_NULL;
volatile rt_uint16_t height = 240;
volatile rt_uint16_t width  = 320;

static void ili9341_spi_send(rt_uint8_t data)
{
    rt_spi_send(spi, (const void *)&data, 1);
}

static void ili9341_write_cmd(rt_uint8_t cmd)
{
    rt_pin_write(LCD_DC, PIN_LOW); 	
    ili9341_spi_send(cmd); 
}

static void ili9341_write_data(rt_uint8_t data)
{
    rt_pin_write(LCD_DC, PIN_HIGH); 	
    ili9341_spi_send(data); 
}

static void ili9341_set_address(rt_uint16_t x1, rt_uint16_t y1, rt_uint16_t x2, rt_uint16_t y2)
{
    ili9341_write_cmd(0x2A);
    ili9341_write_data(x1>>8);
    ili9341_write_data(x1);
    ili9341_write_data(x2>>8);
    ili9341_write_data(x2);

    ili9341_write_cmd(0x2B);
    ili9341_write_data(y1>>8);
    ili9341_write_data(y1);
    ili9341_write_data(y2>>8);
    ili9341_write_data(y2);

    ili9341_write_cmd(0x2C); 
}

static void ili9341_reset(void)
{
    rt_pin_write(LCD_RST, PIN_LOW); 
    rt_thread_mdelay(200);
    rt_pin_write(LCD_NCS, PIN_LOW); 
    rt_thread_mdelay(200);
    rt_pin_write(LCD_RST, PIN_HIGH); 
}

static void ili9341_set_rotation(rt_uint8_t rotation) 
{
    ili9341_write_cmd(0x36);
    rt_thread_mdelay(1);

    switch(rotation) 
    {
    case 0:
        ili9341_write_data(0x40|0x08);
        width = 240;
        height = 320;
        break;
    case 1:
        ili9341_write_data(0x20|0x08);
        width  = 320;
        height = 240;
        break;
    case 2:
        ili9341_write_data(0x80|0x08);
        width  = 240;
        height = 320;
        break;
    case 3:
        ili9341_write_data(0x40|0x80|0x20|0x08);
        width  = 320;
        height = 240;
        break;
    default:
        break;
    }
}

static void ili9341_enable(void)
{
    rt_pin_write(LCD_RST, PIN_HIGH); 
}

static void ili9341_init(void)
{
    ili9341_enable();
    ili9341_reset();

    ili9341_write_cmd(0x01);
    rt_thread_mdelay(100);

    // POWER CONTROL A
    ili9341_write_cmd(0xCB);
    ili9341_write_data(0x39);
    ili9341_write_data(0x2C);
    ili9341_write_data(0x00);
    ili9341_write_data(0x34);
    ili9341_write_data(0x02);

    // POWER CONTROL B
    ili9341_write_cmd(0xCF);
    ili9341_write_data(0x00);
    ili9341_write_data(0xC1);
    ili9341_write_data(0x30);

    // DRIVER TIMING CONTROL A
    ili9341_write_cmd(0xE8);
    ili9341_write_data(0x85);
    ili9341_write_data(0x00);
    ili9341_write_data(0x78);

    //DRIVER TIMING CONTROL B
    ili9341_write_cmd(0xEA);
    ili9341_write_data(0x00);
    ili9341_write_data(0x00);

    // POWER ON SEQUENCE CONTROL
    ili9341_write_cmd(0xED);
    ili9341_write_data(0x64);
    ili9341_write_data(0x03);
    ili9341_write_data(0x12);
    ili9341_write_data(0x81);

    // PUMP RATIO CONTROL
    ili9341_write_cmd(0xF7);
    ili9341_write_data(0x20);

    // POWER CONTROL,VRH[5:0]
    ili9341_write_cmd(0xC0);
    ili9341_write_data(0x23);

    // POWER CONTROL,SAP[2:0];BT[3:0]
    ili9341_write_cmd(0xC1);
    ili9341_write_data(0x10);

    // VCM CONTROL
    ili9341_write_cmd(0xC5);
    ili9341_write_data(0x3E);
    ili9341_write_data(0x28);

    // VCM CONTROL 2
    ili9341_write_cmd(0xC7);
    ili9341_write_data(0x86);

    // MEMORY ACCESS CONTROL
    ili9341_write_cmd(0x36);
    ili9341_write_data(0x48);

    // PIXEL FORMAT
    ili9341_write_cmd(0x3A);
    ili9341_write_data(0x55);

    // FRAME RATIO CONTROL, STANDARD RGB COLOR
    ili9341_write_cmd(0xB1);
    ili9341_write_data(0x00);
    ili9341_write_data(0x18);

    // DISPLAY FUNCTION CONTROL
    ili9341_write_cmd(0xB6);
    ili9341_write_data(0x08);
    ili9341_write_data(0x82);
    ili9341_write_data(0x27);

    // 3GAMMA FUNCTION DISABLE
    ili9341_write_cmd(0xF2);
    ili9341_write_data(0x00);

    // GAMMA CURVE SELECTED
    ili9341_write_cmd(0x26);
    ili9341_write_data(0x01);

    // POSITIVE GAMMA CORRECTION
    ili9341_write_cmd(0xE0);
    ili9341_write_data(0x0F);
    ili9341_write_data(0x31);
    ili9341_write_data(0x2B);
    ili9341_write_data(0x0C);
    ili9341_write_data(0x0E);
    ili9341_write_data(0x08);
    ili9341_write_data(0x4E);
    ili9341_write_data(0xF1);
    ili9341_write_data(0x37);
    ili9341_write_data(0x07);
    ili9341_write_data(0x10);
    ili9341_write_data(0x03);
    ili9341_write_data(0x0E);
    ili9341_write_data(0x09);
    ili9341_write_data(0x00);

    // NEGATIVE GAMMA CORRECTION
    ili9341_write_cmd(0xE1);
    ili9341_write_data(0x00);
    ili9341_write_data(0x0E);
    ili9341_write_data(0x14);
    ili9341_write_data(0x03);
    ili9341_write_data(0x11);
    ili9341_write_data(0x07);
    ili9341_write_data(0x31);
    ili9341_write_data(0xC1);
    ili9341_write_data(0x48);
    ili9341_write_data(0x08);
    ili9341_write_data(0x0F);
    ili9341_write_data(0x0C);
    ili9341_write_data(0x31);
    ili9341_write_data(0x36);
    ili9341_write_data(0x0F);

    // EXIT SLEEP
    ili9341_write_cmd(0x11);
    rt_thread_mdelay(120);

    // TURN ON DISPLAY
    ili9341_write_cmd(0x29);

    // STARTING ROTATION
    ili9341_set_rotation(0);
}

void ili9341_draw_colour(rt_uint16_t colour)
{
    // SENDS COLOUR
    unsigned char buffer[2] = {colour>>8, colour};	
    rt_pin_write(LCD_DC, PIN_HIGH); 
    rt_spi_send(spi, (const void *)buffer, 2);
}

#define BURST_MAX_SIZE 250
static void ili9341_draw_colour_burst(rt_uint16_t Colour, rt_uint32_t Size)
{
    rt_uint32_t bfsz = 0;

    if ((Size * 2) < BURST_MAX_SIZE)
    {
        bfsz = Size;
    }
    else
    {
        bfsz = BURST_MAX_SIZE;
    }

    rt_pin_write(LCD_DC, PIN_HIGH); 

    unsigned char chifted = Colour >> 8;
    unsigned char burst_buffer[bfsz];
    for (uint32_t j = 0; j < bfsz; j += 2)
    {
        burst_buffer[j] = chifted;
        burst_buffer[j + 1] = Colour;
    }

    uint32_t sending_size = Size * 2;
    uint32_t sending_in_block = sending_size / bfsz;
    uint32_t remainder_from_block = sending_size % bfsz;

    if (sending_in_block != 0)
    {
        for (uint32_t j = 0; j < (sending_in_block); j++)
        {
            rt_spi_send(spi, (const void *)burst_buffer, bfsz);
        }
    }

    rt_spi_send(spi, (const void *)burst_buffer, remainder_from_block);
}

static void ili9341_fill_screen(rt_uint16_t colour)
{
    ili9341_set_address(0, 0, width, height);	
    ili9341_draw_colour_burst(colour, width * height);	
}

static void ili9341_draw_pixel(rt_uint16_t x, rt_uint16_t y, rt_uint16_t colour)
{
    if ((x >= width) || (y >= height))
    {
        return;
    }
         
    // ADDRESS
    rt_pin_write(LCD_DC, PIN_LOW); 
    ili9341_spi_send(0x2A);
    rt_pin_write(LCD_DC, PIN_HIGH); 

    // XDATA
    unsigned char buffer1[4] = {x >> 8, x, (x + 1) >> 8, (x + 1)};
    rt_spi_send(spi, (const void *)buffer1, 4);

    // ADDRESS
    rt_pin_write(LCD_DC, PIN_LOW); 
    ili9341_spi_send(0x2B);
    rt_pin_write(LCD_DC, PIN_HIGH); 

    // YDATA
    rt_pin_write(LCD_DC, PIN_LOW); 
    unsigned char buffer2[4] = {y >> 8, y, (y + 1) >> 8, (y + 1)};
    rt_spi_send(spi, (const void *)buffer2, 4);
    rt_pin_write(LCD_DC, PIN_HIGH); 

    // ADDRESS
    rt_pin_write(LCD_DC, PIN_LOW); 
    ili9341_spi_send(0x2C);
    rt_pin_write(LCD_DC, PIN_HIGH); 

    // COLOUR
    rt_pin_write(LCD_DC, PIN_LOW); 
    unsigned char buffer3[2] = {colour >> 8, colour};
    rt_spi_send(spi, (const void *)buffer3, 2);
    rt_pin_write(LCD_DC, PIN_HIGH); 
}

void ili9341_draw_rectangle(rt_uint16_t x, rt_uint16_t y, rt_uint16_t w, rt_uint16_t h, rt_uint16_t colour)
{
    if ((x >= width) || (y >= height))
    {
        return;
    }
        
    if ((x + w - 1) >= width)
    {
        w = width - x;
    }
    if ((y + h - 1) >= height)
    {
        h = height - y;
    }

    ili9341_set_address(x, y, x + w - 1, y + h - 1);
    ili9341_draw_colour_burst(colour, h * w);
}

void ili9341_draw_horizontal_line(rt_uint16_t x, rt_uint16_t y, rt_uint16_t w, rt_uint16_t colour)
{
    if ((x >= width) || (y >= height))
    {
        return;
    }
        
    if ((x + w - 1) >= width)
    {
        w = width - x; 
    }
    ili9341_set_address(x, y, x + w - 1, y);
    ili9341_draw_colour_burst(colour, w);
}

void ili9341_draw_vertical_line(rt_uint16_t x, rt_uint16_t y, rt_uint16_t h, rt_uint16_t colour)
{
    if ((x >= width) || (y >= height))
        return;
    if ((y + h - 1) >= height)
    {
        h = height - y;
    }
    ili9341_set_address(x, y, x, y + h - 1);
    ili9341_draw_colour_burst(colour, h);
}

static struct rt_device lcd; 
static struct rt_device_graphic_info info;

static void set_pixel(const char *pixel, int x, int y)
{
    ili9341_draw_pixel(x, y, *(rt_uint16_t *)pixel); 
}

static void get_pixel(char *pixel, int x, int y)
{
    *(rt_uint16_t *)pixel = (rt_uint16_t)0xFFFF; 
}

static void draw_hline(const char *pixel, int x1, int x2, int y)
{
    ili9341_draw_horizontal_line(x1, y, x2-x1+1, *(rt_uint16_t *)pixel); 
}

static void draw_vline(const char *pixel, int x, int y1, int y2)
{
    ili9341_draw_vertical_line(x, y1, y2-y1+1, *(rt_uint16_t *)pixel); 
}

static void blit_line(const char *pixel, int x, int y, rt_size_t size)
{

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
    hi_spi_bus_attach_device("spi0", "spi00", LCD_NCS); 
    spi = (struct rt_spi_device *)rt_device_find("spi00"); 

    struct rt_spi_configuration cfg;
    cfg.data_width = 8;
    cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_0;
    cfg.max_hz = 50 * 1000 * 1000;  
    rt_spi_configure(spi, &cfg);

    rt_pin_mode(LCD_RST, PIN_MODE_OUTPUT); 
    rt_pin_mode(LCD_DC, PIN_MODE_OUTPUT); 

    ili9341_init(); 
    ili9341_fill_screen(0xF81F); 

    return RT_EOK;
}

static rt_err_t control(rt_device_t dev, int cmd, void *args)
{
    return RT_EOK;
}

int rt_hw_lcd_init(void)
{
    /* lcd info */
    info.bits_per_pixel = 16;
    info.pixel_format   = RTGRAPHIC_PIXEL_FORMAT_RGB565;
    info.framebuffer    = RT_NULL;
    info.width          = 320;
    info.height         = 240;

    /* init device structure */
    lcd.type      = RT_Device_Class_Unknown;
    lcd.init      = init;
    lcd.open      = RT_NULL;
    lcd.close     = RT_NULL;
    lcd.control   = control;
    lcd.user_data = (void *)&ops;
    rt_device_register(&lcd, "lcd", RT_DEVICE_FLAG_RDWR);

    return RT_EOK;
}
INIT_ENV_EXPORT(rt_hw_lcd_init); 
