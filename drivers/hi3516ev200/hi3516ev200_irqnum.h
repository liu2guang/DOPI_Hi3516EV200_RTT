/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef HI3516EV200_IRQNUM_H_
#define HI3516EV200_IRQNUM_H_

#define REG_A7_PERI_GIC_DIST            0x1000
#define REG_A7_PERI_GIC_CPU             0x2000
#define ARM_REG_BASE                    0x10300000
#define REG_BASE_A7_PERI                ARM_REG_BASE

#define IO_ADDRESS(x) (x)

#define CFG_ARM_GIC_CPU_BASE            (IO_ADDRESS(REG_BASE_A7_PERI) + REG_A7_PERI_GIC_CPU)
#define CFG_ARM_GIC_DIST_BASE           (IO_ADDRESS(REG_BASE_A7_PERI) + REG_A7_PERI_GIC_DIST)

#define OS_HWI_MAX_NUM                  96
#define OS_HWI_MAX                      ((OS_HWI_MAX_NUM) - 1)
#define OS_HWI_MIN                      32
#define OS_USER_HWI_MAX                 OS_HWI_MAX
#define OS_USER_HWI_MIN                 OS_HWI_MIN

#define NUM_HAL_INTERRUPT_TIMER0        37
#define NUM_HAL_INTERRUPT_TIMER1        37
#define NUM_HAL_INTERRUPT_TIMER2        38
#define NUM_HAL_INTERRUPT_TIMER3        38

#define NUM_HAL_INTERRUPT_UART0         39
#define NUM_HAL_INTERRUPT_UART1         40
#define NUM_HAL_INTERRUPT_UART2         41

#define NUM_HAL_INTERRUPT_GPIO0         48
#define NUM_HAL_INTERRUPT_GPIO1         49
#define NUM_HAL_INTERRUPT_GPIO2         50
#define NUM_HAL_INTERRUPT_GPIO3         51
#define NUM_HAL_INTERRUPT_GPIO4         52
#define NUM_HAL_INTERRUPT_GPIO5         53
#define NUM_HAL_INTERRUPT_GPIO6         54
#define NUM_HAL_INTERRUPT_GPIO7         55
#define NUM_HAL_INTERRUPT_GPIO8         56
#define NUM_HAL_INTERRUPT_GPIO9         57

#define NUM_HAL_INTERRUPT_SDIO0         62
#define NUM_HAL_INTERRUPT_SDIO1         63

#define NUM_HAL_INTERRUPT_FMC           64
#define NUM_HAL_INTERRUPT_ETH           65

#define NUM_HAL_INTERRUPT_DMAC          70
#define NUM_HAL_INTERRUPT_USB_2         71
#define NUM_HAL_INTERRUPT_USB_EHCI      71
#define NUM_HAL_INTERRUPT_USB_DEV       71

#define NUM_HAL_INTERRUPT_TIMER         NUM_HAL_INTERRUPT_TIMER0
#define NUM_HAL_INTERRUPT_HRTIMER       NUM_HAL_INTERRUPT_TIMER3

#define NUM_HAL_INTERRUPT_NONE          -1

#define NUM_HAL_ISR_MIN                 OS_HWI_MIN
#define NUM_HAL_ISR_MAX                 1020
#define NUM_HAL_ISR_COUNT               (NUM_HAL_ISR_MAX - NUM_HAL_ISR_MIN + 1)

#endif  /* HI3516EV200_IRQNUM_H_ */
