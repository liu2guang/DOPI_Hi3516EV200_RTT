/* Generated by mkromfs. Edit with caution. */
#include <rtthread.h>
#include <dfs_romfs.h>



static const struct romfs_dirent _romfs_root_flash[] = {
    {ROMFS_DIRENT_FILE, ".gitkeep", RT_NULL, 0}
};



static const struct romfs_dirent _romfs_root_sd[] = {
    {ROMFS_DIRENT_FILE, ".gitkeep", RT_NULL, 0}
};



static const struct romfs_dirent _romfs_root_tmp[] = {
    {ROMFS_DIRENT_FILE, ".gitkeep", RT_NULL, 0}
};



static const struct romfs_dirent _romfs_root_udisk[] = {
    {ROMFS_DIRENT_FILE, ".gitkeep", RT_NULL, 0}
};

static const struct romfs_dirent _romfs_root[] = {
    {ROMFS_DIRENT_DIR, "flash", (rt_uint8_t *)_romfs_root_flash, sizeof(_romfs_root_flash)/sizeof(_romfs_root_flash[0])},
    {ROMFS_DIRENT_DIR, "sd", (rt_uint8_t *)_romfs_root_sd, sizeof(_romfs_root_sd)/sizeof(_romfs_root_sd[0])},
    {ROMFS_DIRENT_DIR, "tmp", (rt_uint8_t *)_romfs_root_tmp, sizeof(_romfs_root_tmp)/sizeof(_romfs_root_tmp[0])},
    {ROMFS_DIRENT_DIR, "udisk", (rt_uint8_t *)_romfs_root_udisk, sizeof(_romfs_root_udisk)/sizeof(_romfs_root_udisk[0])}
};

const struct romfs_dirent romfs_root = {
    ROMFS_DIRENT_DIR, "/", (rt_uint8_t *)_romfs_root, sizeof(_romfs_root)/sizeof(_romfs_root[0])
};
