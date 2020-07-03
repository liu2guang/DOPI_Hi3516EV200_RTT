/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __HI_GPIO_H__
#define __HI_GPIO_H__

#define GPIO0_REG_BASE       0x120b0000
#define GPIO1_REG_BASE       0x120b1000
#define GPIO2_REG_BASE       0x120b2000
#define GPIO3_REG_BASE       0x120b3000
#define GPIO4_REG_BASE       0x120b4000
#define GPIO5_REG_BASE       0x120b5000
#define GPIO6_REG_BASE       0x120b6000
#define GPIO7_REG_BASE       0x120b7000
#define GPIO8_REG_BASE       0x120b8000
#define GPIO9_REG_BASE       0x120b9000

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

#endif /* __HI_GPIO_H__ */ 
