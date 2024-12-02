/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <xtensa/config/core.h>
#include <xtensa/xos.h>

#include "app.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"

#ifdef USE_PDM_EDMA
#include "fsl_pdm_edma.h"
#else
#include "fsl_dma.h"
#include "fsl_i2s_dma.h"
#include "fsl_dmic.h"
#include "fsl_dmic_dma.h"
#endif

#include "pin_mux.h"

#ifdef VOICE_SPOT
#include "public/rdsp_voicespot.h"
#include "public/rdsp_voicespot_utils.h"
#include "RdspMemoryUtilsPublic.h"
#include "HeyNXP_en-US_1_params.h"
#include "HeyNXP_en-US_1.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define VOICESPOT_NUM_SAMPLES_PER_FRAME 200U
#define VOICESPOT_SAMPLE_RATE           16000
#ifdef VOICE_SPOT_DEMO_REDUCED_MEMORY_FOOTPRINT
#define IN_BUFFER_NUM_FRAMES (4U)
#else
#define IN_BUFFER_NUM_FRAMES (20U)
#endif
#define BUFFER_SIZE (VOICESPOT_NUM_SAMPLES_PER_FRAME * IN_BUFFER_NUM_FRAMES)

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint32_t volatile s_writeIndex = 0U;
static uint32_t volatile s_readIndex  = 0U;

#ifdef USE_PDM_EDMA
static uint32_t volatile s_dmaIndex = 0U;
static pdm_edma_transfer_t pdmXfer;
static edma_config_t dmaConfig      = {0};
static const pdm_config_t pdmConfig = {
    .enableDoze        = false,
    .fifoWatermark     = DEMO_PDM_FIFO_WATERMARK,
    .qualityMode       = DEMO_PDM_QUALITY_MODE,
    .cicOverSampleRate = DEMO_PDM_CIC_OVERSAMPLE_RATE,
};
static const pdm_channel_config_t channelConfig = {
#if (defined(FSL_FEATURE_PDM_HAS_DC_OUT_CTRL) && (FSL_FEATURE_PDM_HAS_DC_OUT_CTRL))
    .outputCutOffFreq = kPDM_DcRemoverCutOff40Hz,
#else
    .cutOffFreq = kPDM_DcRemoverCutOff152Hz,
#endif
#if defined(DEMO_PDM_DFOUTPUT_GAIN)
    .gain = DEMO_PDM_DFOUTPUT_GAIN,
#else
    .gain       = kPDM_DfOutputGain7,
#endif
};

#else
static dma_handle_t s_i2sTxDmaHandle;
static i2s_dma_handle_t s_i2sTxHandle;
static dmic_dma_handle_t s_dmicDmaHandle;
static dma_handle_t s_dmicRxDmaHandle;
static dmic_transfer_t s_receiveXfer[IN_BUFFER_NUM_FRAMES];
static i2s_transfer_t i2sTxTransfer;
#endif

#if (XCHAL_DCACHE_SIZE > 0)
#if BYTE_DEPTH_DMIC == 2U
AT_NONCACHEABLE_SECTION_ALIGN(static uint16_t s_buffer[BUFFER_SIZE], 4);
#else
AT_NONCACHEABLE_SECTION_ALIGN(static uint32_t s_buffer[BUFFER_SIZE], 16);
#endif
#ifdef USE_PDM_EDMA
AT_NONCACHEABLE_SECTION_ALIGN(pdm_edma_handle_t s_pdmRxHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN(edma_handle_t s_pdmDmaHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN(edma_tcd_t s_edmaTcd[IN_BUFFER_NUM_FRAMES], 32);
uint32_t *frame_buffer;
#else
AT_NONCACHEABLE_SECTION_ALIGN(static uint16_t i2s_buffer[BUFFER_SIZE], 4);
AT_NONCACHEABLE_SECTION_ALIGN(dma_descriptor_t s_dmaDescriptorPingpong[IN_BUFFER_NUM_FRAMES], 16);
/* Frame buffer into VOICE_SPOT */
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t frame_buffer[VOICESPOT_NUM_SAMPLES_PER_FRAME], 16);
#endif
#else
#if BYTE_DEPTH_DMIC == 2U
SDK_ALIGN(static uint16_t s_buffer[BUFFER_SIZE], 4);
#else
SDK_ALIGN(static uint32_t s_buffer[BUFFER_SIZE], 16);
#endif
#ifdef USE_PDM_EDMA
SDK_ALIGN(pdm_edma_handle_t s_pdmRxHandle, 4);
SDK_ALIGN(edma_handle_t s_pdmDmaHandle, 4);
SDK_ALIGN(edma_tcd_t s_edmaTcd[IN_BUFFER_NUM_FRAMES], 32);
uint32_t *frame_buffer;
#else
SDK_ALIGN(static uint16_t i2s_buffer[BUFFER_SIZE], 4);
SDK_ALIGN(dma_descriptor_t s_dmaDescriptorPingpong[IN_BUFFER_NUM_FRAMES], 16);
/* Frame buffer into VOICE_SPOT */
SDK_ALIGN(uint32_t frame_buffer[VOICESPOT_NUM_SAMPLES_PER_FRAME], 16);
#endif
#endif

#ifdef VOICE_SPOT

static rdsp_voicespot_control *voicespot_control;
static int32_t voicespot_handle;
static char **voicespot_class_string;
static int32_t num_samples_per_frame;
static int32_t num_outputs;

/* Frame counter */
static int64_t frame_count = 0;
/* Frame count where last trigger occurred */
int64_t frame_count_trigger = -1;
const char *model_file_name = "HeyNXP_en-US_1.bin";
const char *param_file_name = "HeyNXP_1_params.bin";
static int32_t *max_scores;
static int64_t *max_frame_count;
static int32_t *scores;
static int32_t *scores_trigger;
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

#ifdef USE_PDM_EDMA
static void pdmCallback(PDM_Type *base, pdm_edma_handle_t *handle, status_t status, void *userData)
{
    s_writeIndex += VOICESPOT_NUM_SAMPLES_PER_FRAME;
    if (s_writeIndex >= BUFFER_SIZE)
    {
        s_writeIndex -= BUFFER_SIZE;
    }

    pdmXfer.data         = (uint8_t *)&s_buffer[s_dmaIndex];
    pdmXfer.dataSize     = VOICESPOT_NUM_SAMPLES_PER_FRAME * BYTE_DEPTH_DMIC;
    pdmXfer.linkTransfer = NULL;
    if (kStatus_Success != PDM_TransferReceiveEDMA(DEMO_PDM, &s_pdmRxHandle, &pdmXfer))
    {
        PRINTF("ERROR starting next transfer \r\n");
    }
    s_dmaIndex += VOICESPOT_NUM_SAMPLES_PER_FRAME;
    if (s_dmaIndex >= BUFFER_SIZE)
        s_dmaIndex -= BUFFER_SIZE;
}
#else
void dmic_Callback(DMIC_Type *base, dmic_dma_handle_t *handle, status_t status, void *userData)
{
    /* Convert from 32 bit fixed-point to 16 bits */
    for (int32_t i = 0; i < VOICESPOT_NUM_SAMPLES_PER_FRAME; i++)
    {
        i2s_buffer[s_writeIndex + i] = (uint16_t)s_buffer[s_writeIndex + i];
    }

    i2sTxTransfer.data     = (uint8_t *)&i2s_buffer[s_writeIndex];
    i2sTxTransfer.dataSize = VOICESPOT_NUM_SAMPLES_PER_FRAME * sizeof(uint16_t);
    I2S_TxTransferSendDMA(DEMO_I2S_TX, &s_i2sTxHandle, i2sTxTransfer);

    // Update write index and check for wrap-around
    s_writeIndex += VOICESPOT_NUM_SAMPLES_PER_FRAME;
    if (s_writeIndex >= BUFFER_SIZE)
        s_writeIndex -= BUFFER_SIZE;
}
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
#ifdef VOICE_SPOT

static int32_t VoiceSpotInit(void)
{
    int32_t data_type        = RDSP_DATA_TYPE__INT32;
    int32_t voicespot_status = -1;

    /* Create VOICE_SPOT control structure */
    voicespot_status = rdspVoiceSpot_CreateControl(&voicespot_control, data_type, DEVICE_ID);
    PRINTF("rdspVoiceSpot_CreateControl: voicespot_status = %d\r\n", (int)voicespot_status);

    int32_t enable_highpass_filter = 1;
    int32_t generate_output        = 0;
    /* Create VOICE_SPOT instance */
    voicespot_status =
        rdspVoiceSpot_CreateInstance(voicespot_control, &voicespot_handle, enable_highpass_filter, generate_output);
    PRINTF("rdspVoiceSpot_CreateInstance: voicespot_status = %d\r\n", (int)voicespot_status);

    uint8_t *model_blob      = NULL;
    uint32_t model_blob_size = sizeof(model_blob_header);

#ifdef VOICE_SPOT_DEMO_REDUCED_MEMORY_FOOTPRINT
    model_blob = (uint8_t *)model_blob_header;
#else
    /* Allocate memory for an aligned copy of the model contained in header file. This allows dynamic modification for
     * e.g. weight permutation depending on target platform*/
    model_blob = rdsp_malloc_align(model_blob_size, 16);
    /* Copy model_blob from header file */
    rdsp_memcpy(model_blob, (uint8_t *)model_blob_header, model_blob_size);
#endif

    if (rdspVoiceSpot_CheckModelIntegrity(model_blob_size, model_blob) != RDSP_VOICESPOT_OK)
    {
        /* Check the integrity of the model */
        PRINTF("rdspVoiceSpot_CheckModelIntegrity: Model integrity check failed\r\n");
        return -1;
    }
    /* Open the VOICE_SPOT instance */
    voicespot_status =
        rdspVoiceSpot_OpenInstance(voicespot_control, voicespot_handle, model_blob_size, model_blob, 0, 0);
    PRINTF("rdspVoiceSpot_OpenInstance: voicespot_status = %d\r\n", (int)voicespot_status);

    /* Adaptive threshold modes
    0: fixed threshold (uses the array event_thresholds as thresholds)
    1: adaptive threshold
    2: adaptive sensitivity
    3: adaptive threshold + adaptive sensitivity */
    int32_t adapt_threshold_mode = 3;
    /* Enable use of the Adaptive Threshold mechanism */
    voicespot_status = rdspVoiceSpot_EnableAdaptiveThreshold(voicespot_control, voicespot_handle, adapt_threshold_mode);
    PRINTF("rdspVoiceSpot_EnableAdaptiveThreshold: voicespot_status = %d\r\n", (int)voicespot_status);

    uint8_t *param_blob = (uint8_t *)param_blob_header;

    voicespot_status = rdspVoiceSpot_SetParametersFromBlob(voicespot_control, voicespot_handle, param_blob);
    printf("rdspVoiceSpot_SetParametersFromBlob: voicespot_status = %d\r\n", (int)voicespot_status);

    rdsp_voicespot_version voicespot_version;
    char *voicespot_model_string;

    rdspVoiceSpot_GetLibVersion(voicespot_control, &voicespot_version);
    PRINTF("VoiceSpot library version: %d.%d.%d.%u\r\n", (int)voicespot_version.major, (int)voicespot_version.minor,
           (int)voicespot_version.patch, (unsigned int)voicespot_version.build);
    rdspVoiceSpot_GetModelInfo(voicespot_control, voicespot_handle, &voicespot_version, &voicespot_model_string,
                               &voicespot_class_string, &num_samples_per_frame, &num_outputs);
    PRINTF("VoiceSpot model version: %d.%d.%d\r\n", (int)voicespot_version.major, (int)voicespot_version.minor,
           (int)voicespot_version.patch);
    if (voicespot_model_string != NULL)
        PRINTF("VoiceSpot model string: %s\r\n", voicespot_model_string);
    if (VOICESPOT_NUM_SAMPLES_PER_FRAME != num_samples_per_frame)
    {
        PRINTF("Error: Mismatch in frame size\r\n");
        return -1;
    }

    return 0;
}

static void VoiceSpotProcess(void)
{
    int32_t voicespot_status      = -1;
    int32_t **sfb_output          = NULL;
    int32_t num_frames_per_second = VOICESPOT_SAMPLE_RATE / num_samples_per_frame;

    /* event_thresholds is the array of manually set minimum thresholds for trigger event per class. NULL means
    automatic, i.e. no manually set minimum thresholds. */
    int32_t *event_thresholds     = NULL;
    int32_t event_duration_frames = 2 * num_frames_per_second;
    static int64_t last_event     = 0;
    int32_t processing_period     = 4;
    /* Used for saving score_index of latest trigger */
    int32_t score_index_trigger;
    /* Number of triggers events found in audio */
    static int32_t num_trigger_events = 0;
    /* Number of frames to delay estimation from trigger occurred. If an accurate stop estimate is needed, a
    delay may be needed as trigger may occur before keyword has been completely spoken. */
    int32_t estimation_delay_frames = 0;

    /* Process frame */
    int32_t num_scores = 0;

    /* Set processing level - Will be ignored if adaptive power state is enabled */
    int32_t processing_level;
    if ((frame_count % processing_period) < 4)
    {
        processing_level = RDSP_PROCESSING_LEVEL__FULL;
    }
    else
    {
        processing_level = RDSP_PROCESSING_LEVEL__SKIP_OUTPUT;
    }
    voicespot_status = rdspVoiceSpot_Process(voicespot_control, voicespot_handle, processing_level,
                                             (uint8_t *)frame_buffer, &num_scores, scores, (uint8_t **)sfb_output);

    if (voicespot_status != RDSP_VOICESPOT_OK)
    {
        PRINTF("[%d] rdspVoiceSpot_Process: voicespot_status = %d\r\n", (int)frame_count, (int)voicespot_status);
    }

    if (num_scores > 0)
    {
        /* Store maximum over the scores */
        for (int32_t score_index = 0; score_index < num_outputs; score_index++)
        {
            if (scores[score_index] > max_scores[score_index])
            {
                max_scores[score_index]      = scores[score_index];
                max_frame_count[score_index] = frame_count;
            }
        }
        int32_t condition   = (frame_count - last_event >= event_duration_frames);
        int32_t score_index = rdspVoiceSpot_CheckIfTriggered(voicespot_control, voicespot_handle, scores, condition,
                                                             event_thresholds, processing_period);
        if (score_index >= 0 && frame_count_trigger == -1)
        {
            /* Store trigger frame count for later estimation */
            frame_count_trigger = frame_count;
            /* Store trigger scores for later estimation */
            rdsp_memcpy(scores_trigger, scores, num_outputs * sizeof(int32_t));
            /* Store trigger score_index for later estimation */
            score_index_trigger = score_index;
            last_event          = frame_count;
        }
        if (frame_count_trigger >= 0 && (frame_count - frame_count_trigger >= estimation_delay_frames))
        {
            /* We found a trigger, so estimate the starting point now */
            int32_t start_offset_samples = 0;
            int32_t stop_offset_samples  = 0;
            /* Accuracy of the timing estimate, in frames */
            int32_t timing_accuracy = 4;

#ifndef VOICE_SPOT_DEMO_REDUCED_MEMORY_FOOTPRINT
            /* Comment out this line if timing estimation is not needed */
            voicespot_status =
                rdspVoiceSpot_EstimateStartAndStop(voicespot_control, voicespot_handle, score_index_trigger, -1,
                                                   timing_accuracy, &start_offset_samples, &stop_offset_samples);
            if (voicespot_status != RDSP_VOICESPOT_OK)
                PRINTF("[%d] rdspVoiceSpot_EstimateStartAndStop: voicespot_status = %d\r\n", (int)frame_count,
                       (int)voicespot_status);
#endif

            int32_t trigger_sample    = (frame_count_trigger + 1) * num_samples_per_frame;
            int32_t estimation_sample = (frame_count + 1) * num_samples_per_frame;
            /* Find absolute start sample index by going back from current location */
            int32_t start_sample = estimation_sample - start_offset_samples;
            /* Find absolute stop sample index by going back from current location */
            int32_t stop_sample = estimation_sample - stop_offset_samples;

            /* Increment the number of found trigger events */
            PRINTF(
                "Trigger event found: Event = %d, Frame = %d, class_string = %s, Score = %d, "
                "trigger_sample = %d, estimation_sample = %d, start_offset_samples = %d, start_sample = "
                "%d, stop_offset_samples = %d, stop_sample = %d\r\n",
                (int)num_trigger_events, (int)frame_count_trigger, voicespot_class_string[score_index_trigger],
                (int)scores_trigger[score_index_trigger], (int)trigger_sample, (int)estimation_sample,
                (int)start_offset_samples, (int)start_sample, (int)stop_offset_samples, (int)stop_sample);
            num_trigger_events++;
        }
        /* Indicate that current trigger has been handled */
        frame_count_trigger = -1;
    }
    /* Increment the frame counter */
    frame_count++;
}

#endif

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    xos_start_main("main", 7, 0);

    PRINTF("VoiceSpot example started on core '%s'\r\n", XCHAL_CORE_ID);

#ifdef VOICE_SPOT

#ifdef VOICE_SPOT_DEMO_REDUCED_MEMORY_FOOTPRINT
    const uint32_t plugin_heap_memory_size = 27000 & 0xFFFFFFF0; // Size is multiple of 16
#else
    const uint32_t plugin_heap_memory_size = 100000 & 0xFFFFFFF0; // Size is multiple of 16
#endif
    uint8_t __attribute__((aligned(16))) plugin_heap_memory[plugin_heap_memory_size];
    rdsp_plugin_malloc_init(plugin_heap_memory, plugin_heap_memory, plugin_heap_memory_size);

    if (VoiceSpotInit() != 0)
    {
        assert(false);
    }
    PRINTF("Voice spot init done.\r\n");

    max_scores      = (int32_t *)rdsp_malloc(num_outputs * sizeof(int32_t));
    max_frame_count = (int64_t *)rdsp_malloc(num_outputs * sizeof(int64_t));
    scores          = (int32_t *)rdsp_malloc(num_outputs * sizeof(int32_t));
    /* Used for saving scores of latest trigger */
    scores_trigger = (int32_t *)rdsp_malloc(num_outputs * sizeof(int32_t));

    for (int i = 0; i < num_outputs; i++)
    {
        max_scores[i]      = 0;
        max_frame_count[i] = -1;
    }

#endif

#ifdef USE_PDM_EDMA
    // target device uses EDMA for PDM samples
    /* Create EDMA handle */
    /*
     * dmaConfig.enableRoundRobinArbitration = false;
     * dmaConfig.enableHaltOnError = true;
     * dmaConfig.enableContinuousLinkMode = false;
     * dmaConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(DEMO_DMA, &dmaConfig);
    EDMA_CreateHandle(&s_pdmDmaHandle, DEMO_DMA, DEMO_PDM_EDMA_CHANNEL);
#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
    EDMA_SetChannelMux(DEMO_DMA, DEMO_PDM_EDMA_CHANNEL, DEMO_PDM_EDMA_SOURCE);
#endif

    /* Setup pdm */
    PDM_Init(DEMO_PDM, &pdmConfig);
    PDM_TransferCreateHandleEDMA(DEMO_PDM, &s_pdmRxHandle, pdmCallback, NULL, &s_pdmDmaHandle);
    PDM_TransferInstallEDMATCDMemory(&s_pdmRxHandle, s_edmaTcd, 4);
    PDM_TransferSetChannelConfigEDMA(DEMO_PDM, &s_pdmRxHandle, DEMO_PDM_ENABLE_CHANNEL_LEFT, &channelConfig);
    if (PDM_SetSampleRateConfig(DEMO_PDM, DEMO_PDM_CLK_FREQ, DEMO_AUDIO_SAMPLE_RATE) != kStatus_Success)
    {
        PRINTF("PDM configure sample rate failed DEMO_PDM:%p DEMO_PDM_CLK_FREQ:%d DEMO_AUDIO_SAMPLE_RATE:%d.\r\n",
               DEMO_PDM, DEMO_PDM_CLK_FREQ, DEMO_AUDIO_SAMPLE_RATE);
    }
    PDM_Reset(DEMO_PDM);

    /* Initiate the first 2 DMA transfers. */
    for (int i = 0; i < 2; i++)
    {
        pdmXfer.data         = (uint8_t *)&s_buffer[s_dmaIndex];
        pdmXfer.dataSize     = VOICESPOT_NUM_SAMPLES_PER_FRAME * BYTE_DEPTH_DMIC;
        pdmXfer.linkTransfer = NULL;
        if (kStatus_Success != PDM_TransferReceiveEDMA(DEMO_PDM, &s_pdmRxHandle, &pdmXfer))
        {
            PRINTF("ERROR starting transfer\r\n");
        }
        s_dmaIndex += VOICESPOT_NUM_SAMPLES_PER_FRAME;
    }

#else
    for (int ibuf = 0; ibuf < IN_BUFFER_NUM_FRAMES; ibuf++)
    {
        s_receiveXfer[ibuf].data                   = &s_buffer[ibuf * VOICESPOT_NUM_SAMPLES_PER_FRAME];
        s_receiveXfer[ibuf].dataWidth              = BYTE_DEPTH_DMIC;
        s_receiveXfer[ibuf].dataSize               = VOICESPOT_NUM_SAMPLES_PER_FRAME * BYTE_DEPTH_DMIC;
        s_receiveXfer[ibuf].dataAddrInterleaveSize = kDMA_AddressInterleave1xWidth;
        int ibuf_next                              = ibuf + 1;
        if (ibuf_next == IN_BUFFER_NUM_FRAMES)
            ibuf_next = 0;
        s_receiveXfer[ibuf].linkTransfer = &s_receiveXfer[ibuf_next];
    }

    DMA_CreateHandle(&s_i2sTxDmaHandle, DEMO_DMA, DEMO_I2S_TX_CHANNEL);
    DMA_CreateHandle(&s_dmicRxDmaHandle, DEMO_DMA, DEMO_DMIC_RX_CHANNEL);

    I2S_TxTransferCreateHandleDMA(DEMO_I2S_TX, &s_i2sTxHandle, &s_i2sTxDmaHandle, NULL, NULL);
    DMIC_TransferCreateHandleDMA(DMIC0, &s_dmicDmaHandle, dmic_Callback, NULL, &s_dmicRxDmaHandle);
    DMIC_InstallDMADescriptorMemory(&s_dmicDmaHandle, s_dmaDescriptorPingpong, IN_BUFFER_NUM_FRAMES);
    DMIC_TransferReceiveDMA(DMIC0, &s_dmicDmaHandle, s_receiveXfer, DEMO_DMIC_CHANNEL);

#endif

    while (1)
    {
#ifdef VOICE_SPOT
        int32_t n_avail_Mic_buffer = s_writeIndex - s_readIndex;
        if (n_avail_Mic_buffer < 0)
            n_avail_Mic_buffer += BUFFER_SIZE;

        if (n_avail_Mic_buffer >= num_samples_per_frame)
        {
#ifdef USE_PDM_EDMA
            /* From EDMA the data are already in appropriate int32 format.
             * Process directly from the input buffer.
             */
            frame_buffer = &s_buffer[s_readIndex];
#else
            /* Convert from 16 bit fixed-point to 32 bits */
            for (int32_t i = 0; i < num_samples_per_frame; i++)
            {
#if BYTE_DEPTH_DMIC == 2U

                /* Hifi implementation input is 32 bit fixed-point */
                frame_buffer[i] = s_buffer[s_readIndex + i] << 16;
#else
                /* On board DMIC provides data in 24-bit format */
                frame_buffer[i] = s_buffer[s_readIndex + i] << 8;
#endif
            }
#endif
            /* Update Mic read index and check for wrap-around */
            s_readIndex += num_samples_per_frame;
            if (s_readIndex >= BUFFER_SIZE)
                s_readIndex -= BUFFER_SIZE;

            VoiceSpotProcess();
        }
#endif
    }
}
