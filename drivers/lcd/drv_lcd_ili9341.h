/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __DRV_LCD_ILI9341_H__
#define __DRV_LCD_ILI9341_H__

#include "rtthread.h"
#include "hi_chip.h"

// ILI9341 Commands
#define ILI9341_NOP            0x00
#define ILI9341_SWRESET        0x01
#define ILI9341_RDDID          0x04
#define ILI9341_RGB_BGR        0x08
#define ILI9341_RDDST          0x09
#define ILI9341_RDMODE         0x0A
#define ILI9341_RDMADCTL       0x0B
#define ILI9341_RDPIXFMT       0x0C
#define ILI9341_RDIMGFMT       0x0D
#define ILI9341_RDSELFDIAG     0x0F

#define ILI9341_SLPIN          0x10
#define ILI9341_SLPOUT         0x11
#define ILI9341_PTLON          0x12
#define ILI9341_NORON          0x13

#define ILI9341_INVOFF         0x20
#define ILI9341_INVON          0x21

#define ILI9341_GAMMASET       0x26
#define ILI9341_DISPOFF        0x28
#define ILI9341_DISPON         0x29
#define ILI9341_CASET          0x2A
#define ILI9341_PASET          0x2B
#define ILI9341_RAMWR          0x2C
#define ILI9341_RAMRD          0x2E

#define ILI9341_PTLAR          0x30
#define ILI9341_MADCTL         0x36
#define ILI9341_PIXFMT         0x3A

#define ILI9341_FRMCTR1        0xB1
#define ILI9341_FRMCTR2        0xB2
#define ILI9341_FRMCTR3        0xB3
#define ILI9341_INVCTR         0xB4
#define ILI9341_DFUNCTR        0xB6
#define ILI9341_ENTRYM         0xB7
#define ILI9341_VCOMS          0xBB

#define ILI9341_PWCTR1         0xC0
#define ILI9341_PWCTR2         0xC1
#define ILI9341_PWCTR3         0xC2
#define ILI9341_PWCTR4         0xC3
#define ILI9341_PWCTR5         0xC4
#define ILI9341_VMCTR1         0xC5
#define ILI9341_FRCTRL2        0xC6
#define ILI9341_VMCTR2         0xC7
#define ILI9341_POWERA         0xCB
#define ILI9341_POWERB         0xCF

//#define ILI9341_PWCTR1        0xD0
#define ILI9341_RDID1          0xDA
#define ILI9341_RDID2          0xDB
#define ILI9341_RDID3          0xDC
#define ILI9341_RDID4          0xDD

#define ILI9341_GMCTRP1        0xE0
#define ILI9341_GMCTRN1        0xE1
#define ILI9341_POWER_SEQ      0xED
#define ILI9341_DTCA           0xE8
#define ILI9341_DTCB           0xEA

#define ILI9341_PRC            0xF7
#define ILI9341_3GAMMA_EN      0xF2

#define ILI9341_INVALID_CMD    0xFF

#define MADCTL_MY              0x80
#define MADCTL_MX              0x40
#define MADCTL_MV              0x20
#define MADCTL_ML              0x10
#define MADCTL_MH              0x04

#define ILI9341_DELAY          0x80

// Color definitions for RGB in 565-format
#define ILI9341_BLACK          0x0000   /*   0,   0,   0 */
#define ILI9341_NAVY           0x000F   /*   0,   0, 128 */
#define ILI9341_DARKGREEN      0x03E0   /*   0, 128,   0 */
#define ILI9341_DARKCYAN       0x03EF   /*   0, 128, 128 */
#define ILI9341_MAROON         0x7800   /* 128,   0,   0 */
#define ILI9341_PURPLE         0x780F   /* 128,   0, 128 */
#define ILI9341_OLIVE          0x7BE0   /* 128, 128,   0 */
#define ILI9341_LIGHTGREY      0xC618   /* 192, 192, 192 */
#define ILI9341_DARKGREY       0x7BEF   /* 128, 128, 128 */
#define ILI9341_BLUE           0x001F   /*   0,   0, 255 */
#define ILI9341_GREEN          0x07E0   /*   0, 255,   0 */
#define ILI9341_CYAN           0x07FF   /*   0, 255, 255 */
#define ILI9341_RED            0xF800   /* 255,   0,   0 */
#define ILI9341_MAGENTA        0xF81F   /* 255,   0, 255 */
#define ILI9341_YELLOW         0xFFE0   /* 255, 255,   0 */
#define ILI9341_WHITE          0xFFFF   /* 255, 255, 255 */
#define ILI9341_ORANGE         0xFD20   /* 255, 165,   0 */
#define ILI9341_GREENYELLOW    0xAFE5   /* 173, 255,  47 */
#define ILI9341_PINK           0xF81F

// ILI9341 Display orientation bits, see README.md for details
#define ILI9341_MADCTL_MY      0x80   // LCD Row Order Bottom-Top
#define ILI9341_MADCTL_MX      0x40   // LCD Column Order Right-Left
#define ILI9341_MADCTL_MV      0x20   // LCD Row/Col Reverse
#define ILI9341_MADCTL_ML      0x10   // LCD Vertical Refresh Bottom-Top
#define ILI9341_MADCTL_BGR     0x08   // Blue-Green-Red pixel order
#define ILI9341_MADCTL_MH      0x04   // LCD Horizontal Refresh Right-Left

#define swap(a, b)    { rt_int16_t t = a; a = b; b = t; }

enum ili9341_rotation_t
{
    ILI9341_PORTRAIT = 0,
    ILI9341_LANDSCAPE = 1,
    ILI9341_PORTRAIT_FLIP = 2,
    ILI9341_LANDSCAPE_FLIP = 3,
};

void ili9341_init(void); 
uint16_t ili9341_color565(uint8_t r, uint8_t g, uint8_t b); 
uint16_t ili9341_get_screen_width(void); 
uint16_t ili9341_get_screen_height(void); 
void ili9341_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1); 
void ili9341_set_fgcolor(uint8_t r, uint8_t g, uint8_t b); 
void ili9341_set_bgcolor(uint8_t r, uint8_t g, uint8_t b); 
void ili9341_set_fgcolor565(uint16_t rgb); 
void ili9341_set_bgcolor565(uint16_t rgb); 
void ili9341_set_dimensions(uint16_t width, uint16_t height); 
void ili9341_set_display(uint8_t enable); 
void ili9341_set_orientation(uint8_t madctl, uint16_t rows, uint16_t cols); 
void ili9341_set_rotation(enum ili9341_rotation_t rotation); 
void ili9341_set_inverted(uint16_t inverted); 
void ili9341_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1); 
void ili9341_fill_rect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h); 
void ili9341_draw_pixel(uint16_t x0, uint16_t y0); 
void ili9341_fill_screen(void); 
void ili9341_send_pixels(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t *buf, uint32_t buflen); 

#endif /* __DRV_LCD_ILI9341_H__ */ 
