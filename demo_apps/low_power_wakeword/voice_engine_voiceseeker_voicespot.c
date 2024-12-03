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

#include "public/rdsp_voicespot.h"
#include "HeyNXP_en-US_1.h"
#include "HeyNXP_en-US_1_params.h"
#include "app.h"
#include "app_srtm.h"
#include "libVoiceSeekerLight.h"


#define SAMPLES_PER_FRAME_IN  (32)        /* VoiceSeeker input. */
#define SAMPLES_PER_FRAME_OUT (200)       /* Voiceseeker output = VoiceSpot input. */
#define NUM_INPUT_MICS        (2)

#define SAMPLES_PER_PERIOD (SAMPLES_PER_FRAME_IN * NB_FRAMES_PER_PERIOD) /* For DMA */
#define EXTERN_BUFFER_SIZE (375 * 1024)                                  /* 1.5s * 4channels * 16KHz * 32bits*/

extern srtm_sai_adapter_t pdmAdapter;

/* Buffer for audio samples reception by DMA. */
int8_t g_rx_buffer[RX_BUFFER_LEN];
__attribute__((aligned(16)))

#if USE_SRTM_PDM_EDMA
static srtm_pdm_edma_local_buf_t g_local_rx_buf = {
#else
static srtm_pdm_sdma_local_buf_t g_local_rx_buf = {
#endif
    .buf              = (uint8_t *)&g_rx_buffer,
    .bufSize          = RX_BUFFER_LEN,
    .periods          = 0, // Ignored because samples per period below.
    .samplesPerPeriod = SAMPLES_PER_PERIOD,
    .threshold        = RX_BUFFER_THR,
};

int8_t g_external_audio_buffer[EXTERN_BUFFER_SIZE];
__attribute__((aligned(16)))

#if USE_SRTM_PDM_EDMA
static srtm_pdm_edma_ext_buf_t g_ext_buf = {
    .mem2memDmaBase     = APP_MEM2MEM_DMA,
#else
static srtm_pdm_sdma_ext_buf_t g_ext_buf = {
#endif
    .buf                = (uint8_t *)&g_external_audio_buffer,
    .bufSize            = EXTERN_BUFFER_SIZE,
    .bufWriteDmaChannel = APP_MEM2MEM_W_DMA_CHANNEL,
    .bufReadDmaChannel  = APP_MEM2MEM_R_DMA_CHANNEL,
    .channelPriority    = 1U,
};

extern uint32_t next_buf_size;
extern srtm_audio_format_type_t audio_format;

static rdsp_voicespot_control *voicespot_control; // Pointer to VoiceSpot control struct
static int32_t voicespot_handle;                  // VoiceSpot handle
static float input_buf[SAMPLES_PER_FRAME_IN * NUM_INPUT_MICS];
static int32_t *scores;


static rdsp_xyz_t mics_coords[2] = MIC_ARRAY_COORDS;

static RETUNE_VOICESEEKERLIGHT_plugin_t vsl = {0};
static rdsp_voiceseekerlight_config_t vsl_config;

static void voiceseeker_2mic_init(void)
{
    /* Voice Seeker init */
    RdspStatus voiceseeker_status;

    vsl_config.num_mics             = NUM_INPUT_MICS;
    vsl_config.num_spks             = 0;
    vsl_config.framesize_out        = SAMPLES_PER_FRAME_OUT;
    vsl_config.create_aec           = 0;
    vsl_config.buffer_length_sec    = 1.5f;
    vsl_config.aec_filter_length_ms = 0;
    vsl_config.mic_xyz_mm           = mics_coords;
    vsl_config.device_id            = DEVICE_ID;

    vsl.mem.FreePrivateDataSize = VoiceSeekerLight_GetRequiredHeapMemoryBytes(&vsl, &vsl_config);
    vsl.mem.pPrivateDataBase    = (void *)rdsp_malloc(vsl.mem.FreePrivateDataSize);
    vsl.mem.pPrivateDataNext    = (void *)vsl.mem.pPrivateDataBase;

    voiceseeker_status = VoiceSeekerLight_Create(&vsl, &vsl_config);
    PRINTF_VERBOSE("VoiceSeekerLight_Create status: 0x%x\r\n", voiceseeker_status);

    if (voiceseeker_status == OK) {
        VoiceSeekerLight_Init(&vsl);
    }

    // VoiceSeekerLight_GetConfig(&vsl, &vsl_config);
    // VoiceSeekerLight_PrintConfig(&vsl);
}

static void voicespot_init(void)
{
    int32_t voicespot_status;
    int32_t enable_highpass_filter = 1;
    int32_t generate_output        = 0;
    uint8_t *model_blob            = (uint8_t *)model_blob_header;
    uint32_t model_blob_size       = 0;
    int32_t adapt_threshold_mode   = 3;
    uint8_t *param_blob            = NULL;
    rdsp_voicespot_version voicespot_version;
    char *voicespot_model_string;
    char **voicespot_class_string;
    int32_t num_samples_per_frame;
    int32_t num_outputs;

    do
    {
        voicespot_status = rdspVoiceSpot_CreateControl(&voicespot_control, RDSP_DATA_TYPE__FLOAT32, DEVICE_ID);
        if (voicespot_status != RDSP_VOICESPOT_OK)
        {
            break;
        }

        voicespot_status = rdspVoiceSpot_CreateInstance(voicespot_control, &voicespot_handle, enable_highpass_filter,
                                                        generate_output); // Create VoiceSpot instance
        if (voicespot_status != RDSP_VOICESPOT_OK)
            break;

        model_blob_size = sizeof(model_blob_header);
        voicespot_status =
            rdspVoiceSpot_CheckModelIntegrity(model_blob_size, model_blob); // Check the integrity of the model
        if (voicespot_status != RDSP_VOICESPOT_OK)
            break;

        voicespot_status =
            rdspVoiceSpot_OpenInstance(voicespot_control, voicespot_handle, model_blob_size, model_blob, 0,
                                       0); // Open the VoiceSpot instance
        if (voicespot_status != RDSP_VOICESPOT_OK)
            break;

        voicespot_status = rdspVoiceSpot_EnableAdaptiveThreshold(
            voicespot_control, voicespot_handle,
            adapt_threshold_mode); // Enable use of the Adaptive Threshold mechanism
        if (voicespot_status != RDSP_VOICESPOT_OK)
            break;

        param_blob       = (uint8_t *)param_blob_header;
        voicespot_status = rdspVoiceSpot_SetParametersFromBlob(voicespot_control, voicespot_handle, param_blob);
        if (voicespot_status != RDSP_VOICESPOT_OK)
            break;

        rdspVoiceSpot_GetLibVersion(voicespot_control, &voicespot_version);
        PRINTF("VoiceSpot library version: %d.%d.%d.%u\r\n", (int)voicespot_version.major, (int)voicespot_version.minor,
               (int)voicespot_version.patch, (unsigned int)voicespot_version.build);
        rdspVoiceSpot_GetModelInfo(voicespot_control, voicespot_handle, &voicespot_version, &voicespot_model_string,
                                   &voicespot_class_string, &num_samples_per_frame, &num_outputs);
        PRINTF("VoiceSpot model version: %d.%d.%d\r\n", (int)voicespot_version.major, (int)voicespot_version.minor,
               (int)voicespot_version.patch);
        if (voicespot_model_string != NULL)
        {
            PRINTF("VoiceSpot model string: %s\r\n", voicespot_model_string);
        }
        scores = (int32_t *)rdsp_malloc(num_outputs * sizeof(int32_t));
    } while (0);

    if (voicespot_status != RDSP_VOICESPOT_OK)
    {
        PRINTF_VERBOSE("VoiceSpot init error 0x%x\r\n", voicespot_status);
    }
}

void voice_engine_algo_init(void)
{
    /* VoiceSeeker init */
    voiceseeker_2mic_init();
    /* VoiceSpot init */
    voicespot_init();

    /* Initialize buffers */

    /* wait for the pdmAdapter to be initialized by SRTM task. */
    while (pdmAdapter == NULL)
        ;

        /* Configure PDM adapter. */
#if USE_SRTM_PDM_EDMA
    SRTM_PdmEdmaAdapter_SetRxLocalBuf(pdmAdapter, &g_local_rx_buf);
    SRTM_PdmEdmaAdapter_SetRxExtBuf(pdmAdapter, &g_ext_buf);
#else
    SRTM_PdmSdmaAdapter_SetRxLocalBuf(pdmAdapter, &g_local_rx_buf);
    SRTM_PdmSdmaAdapter_SetRxExtBuf(pdmAdapter, &g_ext_buf);
#endif
}

uint8_t voice_engine_algo_process(void *data)
{
    int32_t voicespot_status;
    int32_t num_scores  = 0;
    int32_t score_index = 0;
    uint8_t ww_detected = 0;
    uint32_t input_nb_bytes_per_sample =
        next_buf_size / SAMPLES_PER_PERIOD; // delta in bytes between first byte of each sample
                                            // depends on the number of mics configured by the host and audio format
    uint32_t i, mic, frame_nb;
    float *voicespot_in_buf = NULL;

    for (frame_nb = 0; frame_nb < NB_FRAMES_PER_PERIOD; frame_nb++)
    {
        /* Extract one channel and convert audio samples to float. */
        switch (audio_format)
        {
            case SRTM_Audio_Stereo16Bits:
                for (i = 0; i < SAMPLES_PER_FRAME_IN; i++)
                {
                    for (mic = 0; mic < NUM_INPUT_MICS; mic++)
                    {
                        input_buf[i + mic * SAMPLES_PER_FRAME_IN] =
                            ((int16_t *)data)[frame_nb * SAMPLES_PER_FRAME_IN +
                                              i * input_nb_bytes_per_sample / sizeof(int16_t) + mic] *
                            (1.0f / 32768.0f);
                    }
                }
                break;
            case SRTM_Audio_Stereo32Bits:
                for (i = 0; i < SAMPLES_PER_FRAME_IN; i++)
                {
                    for (mic = 0; mic < NUM_INPUT_MICS; mic++)
                    {
                        input_buf[i + mic * SAMPLES_PER_FRAME_IN] =
                            ((int32_t *)data)[frame_nb * SAMPLES_PER_FRAME_IN +
                                              i * input_nb_bytes_per_sample / sizeof(int32_t) + mic] *
                            (1.0f / 2147483648.0f);
                    }
                }
                break;
            default:
                PRINTF_VERBOSE("ERROR: unsupported audio format: %d\r\n", audio_format);
                return 0;
                break;
        }

        RdspStatus voiceseeker_status;
        float *mic_in[NUM_INPUT_MICS];

        mic_in[0] = &input_buf[0];
        mic_in[1] = &input_buf[SAMPLES_PER_FRAME_IN];

        voicespot_in_buf   = NULL;
        voiceseeker_status = VoiceSeekerLight_Process(&vsl, (float **)mic_in, NULL, &voicespot_in_buf);
        if (voiceseeker_status != OK)
        {
            voicespot_in_buf = NULL;
            PRINTF_VERBOSE("VoiceSeekerLight_Process error: 0x%x\r\n", voiceseeker_status);
        }

        if (voicespot_in_buf != NULL)
        {
            voicespot_status = rdspVoiceSpot_Process(voicespot_control, voicespot_handle, RDSP_PROCESSING_LEVEL__FULL,
                                                     (uint8_t *)voicespot_in_buf, &num_scores, scores, NULL);

            if ((voicespot_status == RDSP_MODEL_OK) && (num_scores > 0))
            {
                /* VoiceSpot trigger check. */
                score_index = rdspVoiceSpot_CheckIfTriggered(voicespot_control, voicespot_handle, scores, 1, NULL, 4);
                if (score_index >= 0)
                {
                    PRINTF("VoiceSpot: wakeword detected\r\n");
                    ww_detected = 1;
                    break; /* No need to continue processing next frames. */
                }
            }
            else if (voicespot_status == RDSP_MODEL_LICENSE_EXPIRED)
            {
                PRINTF("VoiceSpot license has expired\r\n");
            }
            else if (voicespot_status != RDSP_MODEL_OK)
            {
                PRINTF_VERBOSE("VoiceSpot process error 0x%x\r\n", voicespot_status);
            }
        }
    }
    return ww_detected;
}

void voice_engine_algo_reset(void)
{
    rdspVoiceSpot_ResetProcessing(voicespot_control, voicespot_handle);
}
