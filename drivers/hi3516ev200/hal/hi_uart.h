/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __HI_UART_H__
#define __HI_UART_H__

#define UART0_REG_BASE 0x12040000
#define UART1_REG_BASE 0x12041000
#define UART2_REG_BASE 0x12042000

#define UART_DR        0x0   // data register
#define UART_RSR       0x04
#define UART_FR        0x18  // flag register
#define UART_IBRD      0x24  // interge baudrate register
#define UART_FBRD      0x28  // decimal baudrate register
#define UART_LCR_H     0x2C
#define UART_CR        0x30  // control register
#define UART_IFLS      0x34  // fifo register
#define UART_IMSC      0x38  // interrupt mask register
#define UART_RIS       0x3C  // base interrupt state register
#define UART_MIS       0x40  // mask interrupt state register
#define UART_CLR       0x44  // interrupt clear register
#define UART_DMACR     0x48  // DMA control register

#endif /* __HI_UART_H__ */ 
