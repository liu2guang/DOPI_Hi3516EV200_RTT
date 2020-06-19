/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "rtthread.h"
#include "dfs.h"
#include "dfs_fs.h"

int main(int argc, char **argv)
{
    /* mount ROMFS as root directory */
    extern const struct romfs_dirent romfs_root;
    if (dfs_mount(RT_NULL, "/", "rom", 0, (const void *)(&romfs_root)) == 0)
    {
        rt_kprintf("ROMFS File System initialized!\n");
    }
    else
    {
        rt_kprintf("ROMFS File System initialized Failed!\n");
    }

    return 0;
}
