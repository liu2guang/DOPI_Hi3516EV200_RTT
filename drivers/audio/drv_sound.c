/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "rtthread.h"
#include "rtdevice.h"
#include "board.h" 
#include "amp.h" 
#include "codec.h" 
#include "aiao.h" 

#define DBG_TAG "drv_sound"
#define DBG_LVL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>

#define TX_DMA_FIFO_SIZE (2048)

// 

struct hi_audio
{
    struct rt_audio_configure replay_config;
    int replay_volume;
    rt_uint8_t *tx_fifo;
};

static struct hi_audio _g_hi_audio = {0}; 
static struct rt_audio_device _g_audio_device = {0};

static void aiao_irq(int vector, void *param)
{
    (void)param;
    rt_interrupt_enter();

    rt_audio_tx_complete(&_g_audio_device);
    hi3516ev200_ao_set_buff_wptr(TX_DMA_FIFO_SIZE);
    hi3516ev200_ao_set_buff_rptr(0);
    hi3516ev200_ao_clear_interrupt_status(tx_empty_int); 

    rt_hw_interrupt_ack(vector);
    rt_interrupt_leave();
}

static rt_err_t audio_getcaps(struct rt_audio_device *audio, struct rt_audio_caps *caps)
{
    return RT_EOK;
}

static rt_err_t audio_configure(struct rt_audio_device *audio, struct rt_audio_caps *caps)
{
    rt_err_t result = RT_EOK;
    struct hi_audio *_hi_audio = (struct hi_audio *)audio->parent.user_data;
    int volume = 0; 

    switch (caps->main_type)
    {
    case AUDIO_TYPE_MIXER:
        switch (caps->sub_type)
        {
        case AUDIO_MIXER_VOLUME:
            volume = caps->udata.value;
            rt_kprintf("configure volume %d\n", volume);
            _hi_audio->replay_volume = volume;
            hi3516ev200_ao_set_volume(volume);
            break;
        default:
            result = -RT_ERROR;
            break;
        }
        break;
    }

    return RT_EOK;
}

static rt_err_t audio_init(struct rt_audio_device *audio)
{
    rt_kprintf("audio init\n");
    hi3516ev200_amp_init(); 
    hi3516ev200_codec_init(); 
    hi3516ev200_codec_set_samplerate(44100); 
    hi3516ev200_amp_mute(1); 
    return RT_EOK;
}

static rt_err_t audio_start(struct rt_audio_device *audio, int stream)
{
    struct hi_ao_attr attr; 

    if (stream == AUDIO_STREAM_REPLAY)
    {
        rt_kprintf("start\n"); 
        rt_memset(_g_hi_audio.tx_fifo, 0, TX_DMA_FIFO_SIZE);
        hi3516ev200_amp_mute(0); 
        
        attr.samplerate  = 44100; 
        attr.bitwidth    = 16; 
        attr.workmod     = 0; 
        attr.channels    = 2; 
        attr.exflag      = 0; 
        attr.frmnum      = 30; 
        attr.ptnumperfrm = TX_DMA_FIFO_SIZE; 
        attr.chncnt      = 2; 
        attr.clksel      = 0; 

        hi3516ev200_aiao_init(); 
        hi3516ev200_ao_set_attr(&attr); 
        hi3516ev200_ao_set_buff_addr((int)_g_hi_audio.tx_fifo);
        hi3516ev200_ao_set_buff_size(TX_DMA_FIFO_SIZE+32);
        hi3516ev200_ao_set_buff_wptr(TX_DMA_FIFO_SIZE);
        hi3516ev200_ao_set_buff_rptr(0);
        hi3516ev200_ao_set_tran_size(TX_DMA_FIFO_SIZE);
        hi3516ev200_ao_set_volume(_g_hi_audio.replay_volume);
        hi3516ev200_ao_install_interrupt(aiao_irq, RT_NULL); 
        hi3516ev200_ao_set_enable(tx_empty_int);
    }

    return RT_EOK;
}

static rt_err_t audio_stop(struct rt_audio_device *audio, int stream)
{
    if (stream == AUDIO_STREAM_REPLAY)
    {
        rt_kprintf("stop\n");
        hi3516ev200_amp_mute(1); 
        hi3516ev200_ao_set_disable(tx_empty_int); 
        hi3516ev200_aiao_deinit(); 
        rt_memset(_g_hi_audio.tx_fifo, 0, TX_DMA_FIFO_SIZE);
    }

    return RT_EOK;
}

static void audio_buffer_info(struct rt_audio_device *audio, struct rt_audio_buf_info *info)
{
    /**
     *               TX_FIFO
     * +----------------+----------------+
     * |     block1     |     block2     |
     * +----------------+----------------+
     *  \  block_size  /
     */
    info->buffer      = _g_hi_audio.tx_fifo;
    info->total_size  = TX_DMA_FIFO_SIZE;
    info->block_size  = TX_DMA_FIFO_SIZE;
    info->block_count = 1;
}

static rt_size_t audio_transmit(struct rt_audio_device *audio, const void *writeBuf, void *readBuf, rt_size_t size)
{
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, writeBuf, TX_DMA_FIFO_SIZE);
    return size;
}

static struct rt_audio_ops _g_audio_ops =
{
    .getcaps     = audio_getcaps,
    .configure   = audio_configure,
    .init        = audio_init,
    .start       = audio_start,
    .stop        = audio_stop,
    .transmit    = audio_transmit,
    .buffer_info = audio_buffer_info,
};

int rt_hw_audio_init(void)
{
    rt_uint8_t *tx_fifo = RT_NULL;

    tx_fifo = rt_malloc(TX_DMA_FIFO_SIZE+32);
    if (tx_fifo == RT_NULL)
        return -RT_ENOMEM;
    rt_memset(tx_fifo, 0, TX_DMA_FIFO_SIZE+32);
    _g_hi_audio.tx_fifo = tx_fifo;

    /* register sound device */
    _g_audio_device.ops = &_g_audio_ops;
    rt_audio_register(&_g_audio_device, "sound0", RT_DEVICE_FLAG_WRONLY, &_g_hi_audio);

    return RT_EOK;
}
INIT_DEVICE_EXPORT(rt_hw_audio_init);
