/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "rtthread.h"
#include "rtdevice.h"
#include "drv_spi.h"

#define SPI00_CS_PIN (12)

int spi_test(void)
{
    struct rt_spi_device *spi_lcd;
    rt_uint8_t id[5] = {0xFF, 0xAA, 0x55, 0x5A, 0x22};
     
__retry:
    spi_lcd = (struct rt_spi_device *)rt_device_find("spi00");
    if(spi_lcd == RT_NULL)
    {
        hi_spi_bus_attach_device("spi0", "spi00", SPI00_CS_PIN); 
        goto __retry; 
    }

    struct rt_spi_configuration cfg;
    cfg.data_width = 8;
    cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_0 | RT_SPI_MSB;
    cfg.max_hz = 20 * 1000 * 1000;  /* 20M */
    rt_spi_configure(spi_lcd, &cfg);

    rt_spi_send(spi_lcd, (const void *)id, 5);

    return 0; 
}
MSH_CMD_EXPORT(spi_test, spi_test); 

