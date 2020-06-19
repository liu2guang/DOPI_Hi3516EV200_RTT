/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __AIAO_H__
#define __AIAO_H__

#include "rtdef.h"

/* Peripheral Base Addr */ 
#define AIAO_REG_BASE 		0x100E0000 
 
/* Peripheral REG Offset */
#define AIAO_INT_ENA  		0x0000	/* AIAO 模块中断使能寄存器 */
#define AIAO_INT_STATUS 	0x0004 	/* AIAO 模块中断状态寄存器 */
#define AIAO_INT_RAW 		0x0008 	/* AIAO 模块原始中断寄存器 */ 
#define I2S_CRG_CFG0_00 	0x0100 	/* I2S00 CRG 配置 0 号寄存器 */ 
#define I2S_CRG_CFG1_00 	0x0104 	/* I2S00 CRG 配置 1 号寄存器 */ 
#define I2S_CRG_CFG0_08 	0x0140 	/* I2S08 CRG 配置 0 号寄存器 */  
#define I2S_CRG_CFG1_08 	0x0144 	/* I2S08 CRG 配置 1 号寄存器 */  
#define RX_IF_ATTRI 		0x1000 	/* 接收通道的接口属性设置寄存器 */  
#define RX_DSP_CTRL  		0x1004 	/* 接收处理通道的控制寄存器 */   
#define RX_BUFF_SADDR 		0x1080 	/* 接收通道的 DDR 缓存起始地址寄存器 */ 
#define RX_BUFF_SIZE  		0x1084 	/* 接收通道的 DDR 缓存大小寄存器 */   
#define RX_BUFF_WPTR 		0x1088 	/* 接收通道的 DDR 缓存写地址寄存器 */   
#define RX_BUFF_RPTR  		0x108C 	/* 接收通道的 DDR 缓存读地址寄存器 */ 
#define RX_BUFF_ALFULL_TH 	0x1090 	/* 接收通道的 DDR 缓存几乎满水线寄存器 */   
#define RX_TRANS_SIZE  		0x1094 	/* 接收通道的数据传输长度寄存器 */ 
#define RX_INT_ENA   		0x10A0 	/* 接收通道的中断使能寄存器 */  
#define RX_INT_RAW   		0x10A4 	/* 接收通道的原始中断寄存器 */ 
#define RX_INT_STATUS  		0x10A8 	/* 接收通道的中断状态寄存器 */  
#define RX_INT_CLR   		0x10AC 	/* 接收通道的中断清除寄存器 */ 
#define TX_IF_ATTRI  		0x2000 	/* 发送通道的接口属性设置寄存器 */ 
#define TX_DSP_CTRL  		0x2004 	/* 发送处理通道的控制寄存器 */  
#define TX_BUFF_SADDR 		0x2080 	/* 发送通道的 DDR 缓存起始地址寄存器 */ 
#define TX_BUFF_SIZE  		0x2084 	/* 发送通道的 DDR 缓存大小寄存器 */   
#define TX_BUFF_WPTR 		0x2088 	/* 发送通道的 DDR 缓存写地址寄存器 */   
#define TX_BUFF_RPTR		0x208C 	/* 发送通道的 DDR 缓存读地址寄存器 */ 
#define TX_BUFF_ALEMPTY_TH	0x2090 	/* 发送通道的 DDR 缓存几乎空水线寄存器 */ 
#define TX_TRANS_SIZE		0x2094 	/* 发送通道的数据传输长度寄存器 */ 
#define TX_INT_ENA			0x20A0 	/* 发送通道的中断使能寄存器 */ 
#define TX_INT_RAW			0x20A4 	/* 发送通道的原始中断寄存器 */ 
#define TX_INT_STATUS		0x20A8 	/* 发送通道的中断状态寄存器 */ 
#define TX_INT_CLR			0x20AC 	/* 发送通道的中断清除寄存器 */ 

typedef union 
{
	struct
	{
		rt_uint32_t rx_ch0_int_ena: 1;	/* 接收通道 0 的中断使能, 0 不使能, 1 使能 */
		rt_uint32_t reserved_1_15 : 15;
		rt_uint32_t tx_ch0_int_ena: 1;	/* 发送通道 0 的中断使能, 0 不使能, 1 使能 */
		rt_uint32_t reserved_17_31: 15;
	} bits;
	rt_uint32_t value;
} HI_AIAO_INT_ENA;

#endif  
