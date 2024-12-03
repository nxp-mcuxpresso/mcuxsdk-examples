/*
 * Copyright 2021-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <FreeRTOS.h>
#include "board.h"
#include "fsl_debug_console.h"
#if USE_SRTM_PDM_EDMA
#include "srtm_pdm_edma_adapter.h"
#else
#include "srtm_pdm_sdma_adapter.h"
#endif
#include "task.h"
#include "voice_engine_common.h"

#define VOICE_ENGINE_STACK_SIZE (512)

extern srtm_sai_adapter_t pdmAdapter;

static TaskHandle_t engine_task_handle = NULL;

uint32_t next_buf_size                = 0;
srtm_audio_format_type_t audio_format = 0xFF;

static void dataCallback(srtm_sai_adapter_t adapter, void *data, uint32_t bytes, void *params)
{
    if (data != NULL)
    {
        /* pass following info to processing task through global vars.*/
        next_buf_size = bytes;
#if USE_SRTM_PDM_EDMA
        audio_format = SRTM_PdmEdmaAdapter_GetDataFormat(adapter);
#else
        audio_format = SRTM_PdmSdmaAdapter_GetDataFormat(adapter);
#endif
    }

    /* Notify processing task of a new data buffer. */
    if (xTaskNotify(engine_task_handle, (uint32_t)data, eSetValueWithoutOverwrite) != pdPASS)
    {
        PRINTF_VERBOSE("voice_engine_process previous buffer not processed\r\n");
    }
}

static void voice_engine_task(void *params)
{
    uint32_t data_addr;

    /* Initialize voice engine. */
    voice_engine_algo_init();

#if USE_SRTM_PDM_EDMA
    SRTM_PdmEdmaAdapter_SetDataHandlerOnHostSuspend(pdmAdapter, dataCallback, NULL);
#else
    SRTM_PdmSdmaAdapter_SetDataHandlerOnHostSuspend(pdmAdapter, dataCallback, NULL);
#endif

    while (1)
    {
        if (xTaskNotifyWait(0, 0xFFFFFFFF, &data_addr, portMAX_DELAY) == pdTRUE)
        {
            if (data_addr != 0)
            {
                if (voice_engine_algo_process((void *)data_addr))
                {
#if USE_SRTM_PDM_EDMA
                    SRTM_PdmEdmaAdapter_ResumeHost(pdmAdapter);
#else
                    SRTM_PdmSdmaAdapter_ResumeHost(pdmAdapter);
#endif
                }
            }
            else
            {
                /* data_addr NULL indicates that host has resumed from suspend
                 * voice engine won't be executed anymore until next suspend
                 * so reset it to prepare for future executions. */
                voice_engine_algo_reset();
            }
        }
        else
        {
            PRINTF_VERBOSE("xTaskNotifyWait timeout\r\n");
        }
    }
}

void voice_engine_create(void)
{
    if (xTaskCreate(voice_engine_task, "Voice engine", VOICE_ENGINE_STACK_SIZE, NULL, 2, &engine_task_handle) != pdPASS)
    {
        PRINTF_VERBOSE("Voice Task creation failed!.\r\n");
        while (1)
            ;
    }
}
