#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#define DDR_MEM_BASE              0x40000000
#define FMC_MEM_BASE              0x14000000

#define GPIO9_REG_BASE            0x120b9000
#define GPIO8_REG_BASE            0x120b8000
#define GPIO7_REG_BASE            0x120b7000
#define GPIO6_REG_BASE            0x120b6000
#define GPIO5_REG_BASE            0x120b5000
#define GPIO4_REG_BASE            0x120b4000
#define GPIO3_REG_BASE            0x120b3000
#define GPIO2_REG_BASE            0x120b2000
#define GPIO1_REG_BASE            0x120b1000
#define GPIO0_REG_BASE            0x120b0000

#define SPI1_REG_BASE             0x12071000
#define SPI0_REG_BASE             0x12070000

#define I2C2_REG_BASE             0x12062000
#define I2C1_REG_BASE             0x12061000
#define I2C0_REG_BASE             0x12060000

#define UART2_REG_BASE            0x12042000
#define UART1_REG_BASE            0x12041000
#define UART0_REG_BASE            0x12040000

#define MISC_REG_BASE             0x12028000
#define SYS_CTRL_REG_BASE         0x12020000
#define CRG_REG_BASE              0x12010000

#define SDIO1_REG_BASE            0x10020000
#define SDIO0_REG_BASE            0x10010000

#define ETH_REG_BASE              0x10040000

#define DMAC_REG_BASE             0x100b0000
#define IO_MUX_REG_BASE           0x100c0000
#define FMC_REG_BASE              0x10000000

#define BIT(n)                    (1U << (n))
#define TIMER0_ENABLE             BIT(16)
#define TIMER1_ENABLE             BIT(17)
#define TIMER2_ENABLE             BIT(18)
#define TIMER3_ENABLE             BIT(19)

#define TIMER3_REG_BASE           0x12001020
#define TIMER2_REG_BASE           0x12001000
#define TIMER1_REG_BASE           0x12000020
#define TIMER0_REG_BASE           0x12000000
#define TIMER_TICK_REG_BASE       TIMER0_REG_BASE   /* timer for tick */
#define TIMER_TICK_ENABLE         TIMER0_ENABLE
#define TIMER_TIME_REG_BASE       TIMER1_REG_BASE   /* timer for time */
#define TIMER_TIME_ENABLE         TIMER1_ENABLE
#define HRTIMER_TIMER_REG_BASE    TIMER3_REG_BASE   /* timer for hrtimer */
#define HRTIMER_TIMER_ENABLE      TIMER3_ENABLE

#define CACHE_ALIGNED_SIZE        64
#define ARM_REG_BASE              0x10300000

#define write_reg_bit(value, offset, addr) ({ \
        unsigned long t, mask; \
        mask = 1 << (offset); \
        t = GET_UINT32(addr); \
        t &= ~mask;            \
        t |= (value << (offset)) & mask; \
        WRITE_UINT32(t, addr);   \
    })

#endif
