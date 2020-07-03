/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __HI_GIC_H__
#define __HI_GIC_H__

#define REG_A7_PERI_GIC_DIST            0x1000
#define REG_A7_PERI_GIC_CPU             0x2000
#define ARM_REG_BASE                    0x10300000
#define REG_BASE_A7_PERI                ARM_REG_BASE

#define CFG_ARM_GIC_CPU_BASE            (REG_BASE_A7_PERI + REG_A7_PERI_GIC_CPU)
#define CFG_ARM_GIC_DIST_BASE           (REG_BASE_A7_PERI + REG_A7_PERI_GIC_DIST)

#endif /* __HI_GIC_H__ */
