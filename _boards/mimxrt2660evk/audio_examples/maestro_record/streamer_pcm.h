/*
 * Copyright 2021-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Board-local variant of the shared examples/audio_examples/maestro_record/
 * RT/streamer_pcm.h for MIMXRT2660-EVK.
 *
 * RT2660 uses AUDIO__EDMA3 (edma4 driver) for both SAI0 TX and MICFIL RX, with
 * peripheral request routing via EDMA_SetChannelMux, NOT via DMAMUX. The
 * fsl_dmamux.h header does not exist on RT2660, so this local copy drops the
 * DMAMUX include and simply pulls in fsl_sai_edma.h (which comes from the
 * edma4 driver on RT2660 and defines the sai_edma_handle_t / edma_handle_t
 * types used by pcm_rtos_t).
 */

#ifndef _FSL_STREAMER_PCM_H_
#define _FSL_STREAMER_PCM_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include "fsl_sai_edma.h"
#include "fsl_pdm_edma.h"
#include "fsl_os_abstraction.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief PCM interface structure */
typedef struct _pcm_rtos_t
{
    sai_transfer_t saiTx;
    sai_edma_handle_t saiTxHandle;
    edma_handle_t dmaTxHandle;

    sai_transfer_t saiRx;
    sai_edma_handle_t saiRxHandle;
    pdm_edma_handle_t pdmRxHandle;
    edma_handle_t dmaRxHandle;

    uint32_t sample_rate;
    uint32_t bit_width;
    uint8_t num_channels;

    OSA_SEMAPHORE_HANDLE_DEFINE(semaphoreRX);
    OSA_SEMAPHORE_HANDLE_DEFINE(semaphoreTX);

    uint8_t isFirstRx;
    uint8_t isFirstTx;

    bool dummy_tx_enable;
} pcm_rtos_t;

/*******************************************************************************
 * API
 ******************************************************************************/
void streamer_pcm_init(void);
int streamer_pcm_tx_open(uint32_t num_buffers);
int streamer_pcm_rx_open(uint32_t num_buffers);
void streamer_pcm_tx_close(void);
void streamer_pcm_rx_close(void);
int streamer_pcm_write(uint8_t *data, uint32_t size);
int streamer_pcm_read(uint8_t *data, uint32_t size);
int streamer_pcm_setparams(
    uint32_t sample_rate, uint32_t bit_width, uint8_t num_channels, bool transfer, bool dummy_tx, int volume);
void streamer_pcm_getparams(uint32_t *sample_rate, uint32_t *bit_width, uint8_t *num_channels);
int streamer_pcm_mute(bool mute);
int streamer_pcm_set_volume(int volume);

__attribute__((weak)) void BOARD_MASTER_CLOCK_CONFIG(void)
{
}

int streamer_set_master_clock(int sample_rate);

#if defined(__cplusplus)
}
#endif

#endif
