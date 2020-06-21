/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "rtthread.h"
#include "rtdevice.h"
#include "board.h" 
#include "drv_pin.h" 
#include "drv_uart.h" 

#define CONFIG_UART_CLK_INPUT   (24000000) // 24M or 6M
#define UART_REG_BASE           UART0_REG_BASE
#define CONSOLE_UART_BAUDRATE   115200

#define UART_DR                 0x0   // data register
#define UART_RSR                0x04
#define UART_FR                 0x18  // flag register
#define UART_IBRD               0x24  // interge baudrate register
#define UART_FBRD               0x28  // decimal baudrate register
#define UART_LCR_H              0x2C
#define UART_CR                 0x30  // control register
#define UART_IFLS               0x34  // fifo register
#define UART_IMSC               0x38  // interrupt mask register
#define UART_RIS                0x3C  // base interrupt state register
#define UART_MIS                0x40  // mask interrupt state register
#define UART_CLR                0x44  // interrupt clear register
#define UART_DMACR              0x48  // DMA control register

struct hw_uart_device
{
    rt_ubase_t hw_base;
    rt_uint32_t irqno;
};

static rt_err_t hi3516e_uart_init(rt_base_t uart_base, rt_uint32_t baudrate)
{
    UINT32 uwBaudRate;
    UINT32 divider;
    UINT32 remainder;
    UINT32 fraction;

    /* First, disable everything */
    WRITE_UINT32(0x0, uart_base + UART_CR);

    /* set baud rate */
    uwBaudRate = 16 * baudrate;
    divider = CONFIG_UART_CLK_INPUT / uwBaudRate;
    remainder = CONFIG_UART_CLK_INPUT % uwBaudRate;
    uwBaudRate = (8 * remainder) / baudrate;
    fraction = (uwBaudRate >> 1) + (uwBaudRate & 1);

    WRITE_UINT32(divider, uart_base + UART_IBRD);
    WRITE_UINT32(fraction, uart_base + UART_FBRD);

    /* Set the UART to be 8 bits, 1 stop bit, no parity, fifo enabled. */
    WRITE_UINT32((3 << 5) | (1 << 4), uart_base + UART_LCR_H);

    /*set the fifo threshold of recv interrupt >= 1/8 full */
    WRITE_UINT32((2 << 3) | (4 << 0), uart_base + UART_IFLS);

    /* set nonblock of recv interrupt and recv timeout interrupt */
    WRITE_UINT32((1 << 4) | (1 << 6), uart_base + UART_IMSC);
    
    /* enable the UART */
    WRITE_UINT32((1 << 0) | (1 << 8) | (1 << 9), uart_base + UART_CR);

    return RT_EOK; 
}

#ifdef BSP_USING_UART0
static struct rt_serial_device _serial0;
#endif

static void rt_hw_uart_isr(int irqno, void *param)
{ 
    (void)param;
    rt_interrupt_enter();
    WRITE_UINT32((1 << 4) | (1 << 6), UART_REG_BASE+UART_CLR);
    rt_hw_serial_isr(&_serial0, RT_SERIAL_EVENT_RX_IND);
    rt_interrupt_leave();
} 

static rt_err_t uart_configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    struct hw_uart_device *uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (struct hw_uart_device *)serial->parent.user_data;
    hi3516e_uart_init(uart->hw_base, cfg->baud_rate); 
    return RT_EOK; 
}

static rt_err_t uart_control(struct rt_serial_device *serial, int cmd, void *arg)
{
    struct hw_uart_device *uart; 

    RT_ASSERT(serial != RT_NULL);
    uart = (struct hw_uart_device *)serial->parent.user_data;

    switch(cmd)
    {
    case RT_DEVICE_CTRL_CLR_INT:
        /* Disable the UART Interrupt */
        WRITE_UINT32(0, UART_REG_BASE+UART_IMSC);
        rt_hw_interrupt_mask(uart->irqno);
        break;

    case RT_DEVICE_CTRL_SET_INT:
        /* Enable the UART Interrupt */
        WRITE_UINT32((1 << 4) | (1 << 6), UART_REG_BASE+UART_IMSC);
        rt_hw_interrupt_umask(uart->irqno);
        break;
    }
    return RT_EOK;
}

static int uart_putc(struct rt_serial_device *serial, char c)
{
    struct hw_uart_device *uart;
    
    RT_ASSERT(serial != RT_NULL);
    uart = (struct hw_uart_device *)serial->parent.user_data;
    UINT32 base = uart->hw_base;

    /* Wait for Tx FIFO not full */
    UINT32 status = 0;
    do {
        READ_UINT32(status, base + UART_FR);
    } while (status & 0x20);

    WRITE_UINT8(c, base + UART_DR);

    return 1;
}

static int uart_getc(struct rt_serial_device *serial)
{
    UINT8 ch = -1;
    struct hw_uart_device *uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (struct hw_uart_device *)serial->parent.user_data;
    UINT32 base = uart->hw_base;

    UINT32 status;
    READ_UINT32(status, base + UART_FR);
    if ((status & (1<<4))) 
    {
        return -1; 
    }
    READ_UINT8(ch, base + UART_DR);

    return ch; 
}

static const struct rt_uart_ops _uart_ops =
{
    uart_configure,
    uart_control,
    uart_putc,
    uart_getc,
};

#ifdef BSP_USING_UART0
/* UART device driver structure */
static struct hw_uart_device _uart0_device =
{
    UART0_REG_BASE,
    NUM_HAL_INTERRUPT_UART0,
};
#endif

int rt_hw_uart_init(void)
{
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;

#ifdef BSP_USING_UART0
    struct hw_uart_device *uart0;
    uart0 = &_uart0_device;

    _serial0.ops    = &_uart_ops;
    _serial0.config = config;

    #define PIN_74_UART0_RX_FUNC 0x01
    #define PIN_73_UART0_TX_FUNC 0x01

    rt_gpio_set_func(73, 1, 0, 0, PIN_73_UART0_TX_FUNC, 1); 
    rt_gpio_set_func(74, 1, 0, 0, PIN_74_UART0_RX_FUNC, 0); 

    /* register UART0 device */
    rt_hw_serial_register(&_serial0, "uart0",
                          RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX,
                          uart0);
    rt_hw_interrupt_install(NUM_HAL_INTERRUPT_UART0, rt_hw_uart_isr, &_serial0, "uart0");
    rt_hw_interrupt_umask(NUM_HAL_INTERRUPT_UART0);
#endif 

    return 0; 
}
