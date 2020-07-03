/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "rtthread.h"
#include "rtdevice.h"
#include "board.h"
#include "drv_spi.h"
#include "drv_lcd_ili9341.h"

// 参考: https://github.com/mongoose-os-libs/ili9341-spi
struct ili9341_window
{
    uint16_t x0;
    uint16_t x1;
    uint16_t y0;
    uint16_t y1;
    uint16_t fg_color; 
    uint16_t bg_color; 
};

static struct rt_spi_device *spi = RT_NULL;
static uint32_t rst = 0;
static uint32_t dc = 0;
static uint16_t screen_width;
static uint16_t screen_height;
static struct ili9341_window s_window;

static const uint8_t initcmds[] = 
{
    ILI9341_SWRESET,   ILI9341_DELAY, 5,                                  //  1: Software reset, no args, w/ 5 ms delay afterwards
    ILI9341_POWERA,    5,             0x39, 0x2c, 0x00, 0x34, 0x02,
    ILI9341_POWERB,    3,             0x00, 0xc1, 0x30,
    0xef,              3,             0x03, 0x80, 0x02,
    ILI9341_DTCA,      3,             0x85, 0x00, 0x78,
    ILI9341_DTCB,      2,             0x00, 0x00,
    ILI9341_POWER_SEQ, 4,             0x64, 0x03, 0x12, 0x81,
    ILI9341_PRC,       1,             0x20,
    ILI9341_PWCTR1,    1,             0x23,                               // Power control VRH[5:0]
    ILI9341_PWCTR2,    1,             0x10,                               // Power control SAP[2:0];BT[3:0]
    ILI9341_VMCTR1,    2,             0x3e, 0x28,                         // VCM control
    ILI9341_VMCTR2,    1,             0x86,                               // VCM control2
    ILI9341_MADCTL,    1,             (MADCTL_MX | ILI9341_RGB_BGR),      // Memory Access Control (orientation)
    // *** INTERFACE PIXEL FORMAT: 0x66 -> 18 bit; 0x55 -> 16 bit
    ILI9341_PIXFMT,    1,             0x55,
    ILI9341_INVOFF,    0,
    ILI9341_FRMCTR1,   2,             0x00, 0x13,                         // 0x18 79Hz, 0x1B default 70Hz, 0x13 100Hz
    ILI9341_DFUNCTR,   4,             0x08, 0x82, 0x27, 0x00,             // Display Function Control
    ILI9341_PTLAR,     4,             0x00, 0x00, 0x01, 0x3F,
    ILI9341_3GAMMA_EN, 1,             0x00,                               // 3Gamma Function: Disable (0x02), Enable (0x03)
    ILI9341_GAMMASET,  1,             0x01,                               // Gamma curve selected (0x01, 0x02, 0x04, 0x08)
    ILI9341_GMCTRP1,   15,                                                // Positive Gamma Correction
    0x0F,              0x31,          0x2B, 0x0C, 0x0E, 0x08, 0x4E,0xF1,  0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
    ILI9341_GMCTRN1,   15 | ILI9341_DELAY,                                // Negative Gamma Correction
    0x00,              0x0E,          0x14, 0x03, 0x11, 0x07, 0x31,0xC1,  0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
    120,                                                                  // 120 ms delay before sleepout
    ILI9341_SLPOUT,    0,                                                 // Sleep out
    ILI9341_DISPON,    0,
    ILI9341_INVALID_CMD,                                                  // End of sequence.
};

static uint16_t lcd_htons(uint16_t n)
{
  return ((n & 0xff) << 8) | ((n & 0xff00) >> 8);
}

/* 通过spi发送一个字节数据, 忽略接收数据 */ 
static void lcd_spi_write(const uint8_t *data, uint32_t size)
{
    if(size > 0)
    {
        rt_spi_send(spi, (const void *)data, (rt_size_t)size);
    }   
}

/* 向LCD发送一个字节命令 */ 
static void lcd_send_cmd(uint8_t byte)
{
    rt_pin_write(dc, PIN_LOW); 
    lcd_spi_write((const uint8_t *)&byte, 1);
}

/* 向LCD发送一个字节数据 */ 
static void lcd_send_dat(uint8_t byte) 
{
    rt_pin_write(dc, PIN_HIGH); 
    lcd_spi_write((const uint8_t *)&byte, 1);
}

/* 向LCD发送一系列命令(初始化用) */ 
static void lcd_send_cmd_list(const uint8_t *addr)
{
    uint8_t cmd, num_args, delay; 

    while(1)
    {
        cmd = *addr++;
        if(cmd == ILI9341_INVALID_CMD)
        {
            break; 
        }

        num_args = *addr++;
        delay = num_args & ILI9341_DELAY;
        num_args &= ~ILI9341_DELAY;

        rt_pin_write(dc, PIN_LOW); 
        lcd_spi_write((const uint8_t *)&cmd, 1); 

        rt_pin_write(dc, PIN_HIGH); 
        lcd_spi_write((const uint8_t *)addr, num_args);
        addr += num_args;

        if(delay)
        {
            delay = *addr++;
            rt_thread_mdelay(delay); 
        }
    }
}

/* LCD开窗设置 */ 
static void lcd_set_clip(rt_uint16_t x0, rt_uint16_t y0, rt_uint16_t x1, rt_uint16_t y1)
{
    lcd_send_cmd(ILI9341_CASET);  // Column addr set
    lcd_send_dat(x0 >> 8);
    lcd_send_dat(x0 & 0xFF);      // XSTART
    lcd_send_dat(x1 >> 8);
    lcd_send_dat(x1 & 0xFF);      // XEND
    lcd_send_cmd(ILI9341_PASET);  // Row addr set
    lcd_send_dat(y0 >> 8);
    lcd_send_dat(y0);             // YSTART
    lcd_send_dat(y1 >> 8);
    lcd_send_dat(y1);             // YEND
    lcd_send_cmd(ILI9341_RAMWR);  // write to RAM
}

/* 开窗写颜色数据 */ 
static void lcd_send_pixels(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t *buf, uint32_t buflen)
{
    uint16_t winsize = (x1 - x0 + 1) * (y1 - y0 + 1);

    if (y0 + s_window.y0 > s_window.y1 || x0 + s_window.x0 > s_window.x1)
    {
        return;
    }
    if (y1 + s_window.y0 > s_window.y1)
    {
        y1 = s_window.y1 - s_window.y0;
    }
    if (x1 + s_window.x0 > s_window.x1)
    {
        x1 = s_window.x1 - s_window.x0;
    }

    if (buflen != winsize * 2)
    {
        rt_kprintf("Want buflen(%d), to be twice the window size(%d)\n", buflen, winsize);
        return;
    }

    winsize = (x1 - x0 + 1) * (y1 - y0 + 1);

    lcd_set_clip(x0 + s_window.x0, y0 + s_window.y0, x1 + s_window.x0, y1 + s_window.y0);
    lcd_send_cmd(ILI9341_RAMWR);
    rt_pin_write(dc, PIN_HIGH); 
    lcd_spi_write(buf, winsize * 2);
}

/* 按块填充背景 */ 
#define ILI9341_FILLRECT_CHUNK 256
static void lcd_fill_rect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h)
{
    static uint16_t buf[ILI9341_FILLRECT_CHUNK/2]; 
    uint32_t i;
    uint32_t todo_len;
    uint32_t buflen;

    todo_len = w * h;
    if (todo_len == 0)
    {
        return;
    }

    buflen = (todo_len < ILI9341_FILLRECT_CHUNK ? todo_len : ILI9341_FILLRECT_CHUNK);
    for (i = 0; i < buflen; i++)
    {
        buf[i] = s_window.fg_color;
    }

    lcd_set_clip(x0, y0, x0 + w - 1, y0 + h - 1);
    lcd_send_cmd(ILI9341_RAMWR);
    rt_pin_write(dc, PIN_HIGH); 
    while (todo_len)
    {
        if (todo_len >= buflen)
        {
            lcd_spi_write((const uint8_t *)buf, buflen*2);
            todo_len -= buflen;
        }
        else
        {
            lcd_spi_write((const uint8_t *)buf, todo_len*2);
            todo_len = 0;
        }
    }
}

/* 绘制一个像素点 */ 
static void lcd_draw_pixel(uint16_t x0, uint16_t y0)
{
    if (x0 + s_window.x0 > s_window.x1)
    {
        return;
    }
    if (y0 + s_window.y0 > s_window.y1)
    {
        return;
    }

    lcd_set_clip(x0 + s_window.x0, y0 + s_window.y0, x0 + s_window.x0 + 1, y0 + s_window.y0 + 1);
    rt_pin_write(dc, PIN_HIGH); 
    lcd_spi_write((uint8_t *)&s_window.fg_color, 2);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

uint16_t ili9341_color565(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}

uint16_t ili9341_get_screen_width(void)
{
    return screen_width;
}

uint16_t ili9341_get_screen_height(void)
{
    return screen_height;
}

void ili9341_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    if(x0 > x1)
    {
        swap(x0, x1);
    }
    if(y0 > y1)
    {
        swap(y0, y1);
    }
    s_window.x0 = x0;
    s_window.x1 = x1;
    s_window.y0 = y0;
    s_window.y1 = y1;
}

void ili9341_set_fgcolor(uint8_t r, uint8_t g, uint8_t b)
{
    s_window.fg_color = lcd_htons(ili9341_color565(r, g, b));
}

void ili9341_set_bgcolor(uint8_t r, uint8_t g, uint8_t b)
{
    s_window.bg_color = lcd_htons(ili9341_color565(r, g, b));
}

void ili9341_set_fgcolor565(uint16_t rgb)
{
    s_window.fg_color = lcd_htons(rgb);
}

void ili9341_set_bgcolor565(uint16_t rgb)
{
    s_window.bg_color = lcd_htons(rgb);
}

void ili9341_set_dimensions(uint16_t width, uint16_t height) 
{
    screen_width  = width;
    screen_height = height;
}

void ili9341_set_display(uint8_t enable)
{
    if(enable)
    {
        lcd_send_cmd(ILI9341_DISPON);
    }
    else
    {
        lcd_send_cmd(ILI9341_DISPOFF);
    }
}

/* Many screen implementations differ in orientation. Here's some application hints:
 * #define ADAFRUIT_LCD_PORTRAIT        (ILI9341_MADCTL_MX)
 * #define ADAFRUIT_LCD_LANDSCAPE       (ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV)
 * #define ADAFRUIT_LCD_PORTRAIT_FLIP   (ILI9341_MADCTL_MY)
 * #define ADAFRUIT_LCD_LANDSCAPE_FLIP  (ILI9341_MADCTL_MV)
 *
 * #define M5STACK_LCD_PORTRAIT         (ILI9341_MADCTL_MV | ILI9341_MADCTL_MY)
 * #define M5STACK_LCD_LANDSCAPE        (ILI9341_MADCTL_NONE)
 * #define M5STACK_LCD_PORTRAIT_FLIP    (ILI9341_MADCTL_MV | ILI9341_MADCTL_MX)
 * #define M5STACK_LCD_LANDSCAPE_FLIP   (ILI9341_MADCTL_MY | ILI9341_MADCTL_ML | ILI9341_MADCTL_MX)
 */
void ili9341_set_orientation(uint8_t madctl, uint16_t rows, uint16_t cols)
{
    madctl |= ILI9341_MADCTL_BGR;
    lcd_send_cmd(ILI9341_MADCTL);
    lcd_send_dat(madctl);
    ili9341_set_dimensions(rows, cols);
    ili9341_set_window(0, 0, ili9341_get_screen_width() - 1, ili9341_get_screen_height() - 1);
}

void ili9341_set_rotation(enum ili9341_rotation_t rotation)
{
    uint8_t madctl;
    uint16_t w = ili9341_get_screen_width();
    uint16_t h = ili9341_get_screen_height();

    switch (rotation)
    {
    case ILI9341_LANDSCAPE:
        madctl = (ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR);
        if (w > h)
            ili9341_set_dimensions(w, h);
        else
            ili9341_set_dimensions(h, w);
        break;

    case ILI9341_LANDSCAPE_FLIP:
        madctl = (ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR);
        if (w > h)
            ili9341_set_dimensions(w, h);
        else
            ili9341_set_dimensions(h, w);
        break;

    case ILI9341_PORTRAIT_FLIP:
        madctl = (ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
        if (h > w)
            ili9341_set_dimensions(w, h);
        else
            ili9341_set_dimensions(h, w);
        break;

    default: // ILI9331_PORTRAIT
        madctl = (ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR);
        if (h > w)
            ili9341_set_dimensions(w, h);
        else
            ili9341_set_dimensions(h, w);
    }

    lcd_send_cmd(ILI9341_MADCTL);
    lcd_send_dat(madctl);
    ili9341_set_window(0, 0, ili9341_get_screen_width()-1, ili9341_get_screen_height()-1);
    return;
}

void ili9341_set_inverted(uint16_t inverted)
{
    if (inverted)
    {
        lcd_send_cmd(ILI9341_INVON);
    }
    else
    {
        lcd_send_cmd(ILI9341_INVOFF);
    }
}

void ili9341_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    // Vertical line
    if (x0 == x1)
    {
        if (y1 < y0)
        {
            swap(y0, y1);
        }
        if (y0 + s_window.y0 > s_window.y1 || x0 + s_window.x0 > s_window.x1)
        {
            // LOG(LL_DEBUG, ("VLINE [%d,%d] length %d starts outside of window", x0, y0, y1));
            return;
        }
        // LOG(LL_DEBUG, ("VLINE [%d,%d]-[%d,%d] window [%d,%d]-[%d,%d]", x0, y0, x1, y1, s_window.x0, s_window.y0, s_window.x1, s_window.y1));
        if (y1 + s_window.y0 > s_window.y1)
        {
            // LOG(LL_DEBUG, ("VLINE [%d,%d] length %d ends outside of window, clipping it", x0, y0, y1-y0));
            y1 = s_window.y1 - s_window.y0;
        }
        return lcd_fill_rect(s_window.x0 + x0, s_window.y0 + y0, 1, y1 - y0);
    }

    // Horizontal line
    if (y0 == y1)
    {
        if (x1 < x0)
        {
            swap(x0, x1);
        }
        if (x0 + s_window.x0 > s_window.x1 || y0 + s_window.y0 > s_window.y1)
        {
            // LOG(LL_DEBUG, ("HLINE [%d,%d] length %d starts outside of window", x0, y0, y1));
            return;
        }
        // LOG(LL_DEBUG, ("HLINE [%d,%d]-[%d,%d] window [%d,%d]-[%d,%d]", x0, y0, x1, y1, s_window.x0, s_window.y0, s_window.x1, s_window.y1));
        if (x1 + s_window.x0 > s_window.x1)
        {
            // LOG(LL_DEBUG, ("HLINE [%d,%d] length %d ends outside of window, clipping it", x0, y0, x1-x0));
            x1 = s_window.x1 - s_window.x0;
        }
        return lcd_fill_rect(s_window.x0 + x0, s_window.y0 + y0, x1 - x0, 1);
    }

    int steep = 0;
    if (abs(y1 - y0) > abs(x1 - x0))
    {
        steep = 1;
    }
    if (steep)
    {
        swap(x0, y0);
        swap(x1, y1);
    }
    if (x0 > x1)
    {
        swap(x0, x1);
        swap(y0, y1);
    }

    int16_t dx = x1 - x0, dy = abs(y1 - y0);
    int16_t err = dx >> 1, ystep = -1, xs = x0, dlen = 0;

    if (y0 < y1)
    {
        ystep = 1;
    }
    // Split into steep and not steep for FastH/V separation
    if (steep)
    {
        for (; x0 <= x1; x0++)
        {
            dlen++;
            err -= dy;
            if (err < 0)
            {
                err += dx;
                if (dlen == 1)
                {
                    lcd_draw_pixel(y0, xs);
                }
                else
                {
                    ili9341_draw_line(y0, xs, y0, xs + dlen);
                }
                dlen = 0;
                y0 += ystep;
                xs = x0 + 1;
            }
        }
        if (dlen)
        {
            ili9341_draw_line(y0, xs, y0, xs + dlen);
        }
    }
    else
    {
        for (; x0 <= x1; x0++)
        {
            dlen++;
            err -= dy;
            if (err < 0)
            {
                err += dx;
                if (dlen == 1)
                {
                    lcd_draw_pixel(xs, y0);
                }
                else
                {
                    ili9341_draw_line(xs, y0, xs + dlen, y0);
                }
                dlen = 0;
                y0 += ystep;
                xs = x0 + 1;
            }
        }
        if (dlen)
        {
            ili9341_draw_line(xs, y0, xs + dlen, y0);
        }
    }
}

void ili9341_fill_rect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h)
{
    if (x0 + s_window.x0 > s_window.x1)
    {
        return;
    }
    if (y0 + s_window.y0 > s_window.y1)
    {
        return;
    }
    if (x0 + s_window.x0 + w > s_window.x1)
    {
        w = s_window.x1 - s_window.x0 - x0;
    }
    if (y0 + s_window.y0 + h > s_window.y1)
    {
        h = s_window.y1 - s_window.y0 - y0;
    }
    return lcd_fill_rect(s_window.x0 + x0, s_window.y0 + y0, w, h);
}

void ili9341_draw_pixel(uint16_t x0, uint16_t y0)
{
    return lcd_draw_pixel(x0, y0);
}

void ili9341_fill_screen(void)
{
    return lcd_fill_rect(0, 0, ili9341_get_screen_width(), ili9341_get_screen_height());
}

void ili9341_send_pixels(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t *buf, uint32_t buflen)
{
    lcd_send_pixels(x0, y0, x1, y1, buf, buflen); 
}

void ili9341_init(void)
{
    struct rt_spi_configuration cfg;

    hi_spi_bus_attach_device("spi0", "spi00", 12); 
    spi = (struct rt_spi_device *)rt_device_find("spi00"); 
    rt_device_open(&(spi->bus->parent), RT_DEVICE_FLAG_RDWR); 
    rt_device_open(&(spi->parent), RT_DEVICE_FLAG_RDWR); 

    cfg.data_width = 8;
    cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_0;
    cfg.max_hz = 50 * 1000 * 1000;  
    rt_spi_configure(spi, &cfg);

    rst = 69; 
    dc = 21; 

    rt_pin_mode(rst, PIN_MODE_OUTPUT); 
    rt_pin_mode(dc, PIN_MODE_OUTPUT); 
    rt_pin_write(dc, PIN_LOW); 

    /* Issue a 20uS negative pulse on the reset pin and wait 5 mS so interface gets ready */ 
    rt_pin_write(rst, PIN_HIGH); 
    rt_thread_mdelay(10);
    rt_pin_write(rst, PIN_LOW); 
    rt_thread_mdelay(100);
    rt_pin_write(rst, PIN_HIGH);

    /* Init */
    lcd_send_cmd_list(initcmds); 

    ili9341_set_dimensions(240, 320); 
    ili9341_set_rotation(ILI9341_PORTRAIT);
}

int ili9341_test(int argc, char **argv)
{
    ili9341_init(); 

    ili9341_set_fgcolor565(ILI9341_RED); 
    ili9341_fill_rect(0, 0, 240, 320); 
    rt_kprintf("ili9341_send_pixels end\n"); 

    return 0;
}
MSH_CMD_EXPORT(ili9341_test, test hi3516ev200 ili9341 lcd.); 
