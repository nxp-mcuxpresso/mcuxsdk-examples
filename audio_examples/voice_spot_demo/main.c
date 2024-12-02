/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_sai_edma.h"
#include "fsl_codec_common.h"

#ifdef USE_PDM_EDMA
#include "fsl_pdm.h"
#include "fsl_pdm_edma.h"
#endif

#ifdef VOICE_SPOT
#include "arm_math.h"
#include "public/rdsp_voicespot.h"
#include "public/rdsp_voicespot_utils.h"
#include "HeyNXP_en-US_1_params.h"
#include "HeyNXP_en-US_1.h"
#endif
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef DEMO_CODEC_VOLUME
#define DEMO_CODEC_VOLUME 100U
#endif
#define VOICESPOT_NUM_SAMPLES_PER_FRAME 200
#define VOICESPOT_SAMPLE_RATE           16000
#define BYTE_WIDTH                      4U
/* One DMA transfer has to be divisible by bytes_per_frame * count (= 64), 64 * 25 = BUFFER_SIZE) */
#define DMA_TRANSFER_MULT 2U
#define BUFFER_SIZE       (VOICESPOT_NUM_SAMPLES_PER_FRAME * BYTE_WIDTH * DMA_TRANSFER_MULT)
#define BUFFER_NUMBER     2U


#if defined(MIMX8UD7_cm33_SERIES)
#define DEVICE_ID Device_IMX8ULP_CM33
#else
#error "Incompatible device"
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t Buffer[BUFFER_NUMBER * BUFFER_SIZE], BYTE_WIDTH);
AT_QUICKACCESS_SECTION_DATA(sai_edma_handle_t txHandle);
AT_QUICKACCESS_SECTION_DATA(sai_edma_handle_t rxHandle);
static uint32_t rx_index = 0U;
#ifndef USE_PDM_EDMA
static uint32_t tx_index = 0U;
#endif
volatile uint32_t bufferRead    = 0;
volatile uint32_t bufferWritten = 0;
uint32_t firstRead              = 1;
uint32_t firstWrite             = 1;
edma_handle_t dmaTxHandle = {0}, dmaRxHandle = {0};
extern codec_config_t boardCodecConfig;
codec_handle_t codecHandle;

#ifdef VOICE_SPOT
static rdsp_voicespot_control *voicespot_control;
static int32_t voicespot_handle;
static char **voicespot_class_string;
static int32_t num_samples_per_frame;
static int32_t num_outputs;
static int buf_num = 0;
#endif

/* Frame counter */
static int64_t frame_count = 0;
/* Frame count where last trigger occurred */
int64_t frame_count_trigger = -1;
const char *model_file_name = "HeyNXP_en-US_1.bin";
const char *param_file_name = "HeyNXP_1_params.bin";
static int32_t *max_scores;
static int64_t *max_frame_count;
static int32_t *frame_buffer;
static int32_t *scores;
static int32_t *scores_trigger;
static int numrx_errors = 0;
#ifndef USE_PDM_EDMA
static int numtx_errors = 0;
#endif

#ifdef USE_PDM_EDMA
AT_NONCACHEABLE_SECTION_ALIGN(pdm_edma_handle_t s_pdmRxHandle, 4);
AT_NONCACHEABLE_SECTION_ALIGN(edma_handle_t s_pdmDmaHandle, 4);
AT_QUICKACCESS_SECTION_DATA_ALIGN(edma_tcd_t s_edmaTcd[4], 32U);
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
    .gain = DEMO_PDM_CHANNEL_GAIN,
};
#endif

void voice_spot_demo_idle_callback(void);


/*******************************************************************************
 * Code
 ******************************************************************************/
#ifdef USE_PDM_EDMA
static void pdmCallback(PDM_Type *base, pdm_edma_handle_t *handle, status_t status, void *userData)
#else
static void rx_callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
#endif
{
    if (kStatus_SAI_RxError == status)
    {
        numrx_errors++;
    }
    else
    {
        bufferRead++;
    }
}

#ifndef USE_PDM_EDMA
static void tx_callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_SAI_TxError == status)
    {
        numtx_errors++;
    }
    else
    {
        bufferWritten++;
    }
}
#endif

#ifdef VOICE_SPOT

static int32_t VoiceSpotInit(void)
{
    int32_t data_type        = RDSP_DATA_TYPE__FLOAT32;
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
    /* Allocate memory for an aligned copy of the model contained in header file. This allows dynamic modification for
     * e.g. weight permutation depending on target platform*/
    model_blob = rdsp_malloc_align(model_blob_size, 16);

    /* Copy model_blob from header file */
    rdsp_memcpy(model_blob, (uint8_t *)model_blob_header, model_blob_size);

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
    PRINTF("rdspVoiceSpot_SetParametersFromBlob: voicespot_status = %d\r\n", (int)voicespot_status);

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
    int32_t score_index_trigger = 0;
    /* Number of triggers events found in audio */
    static int32_t num_trigger_events = 0;
    /* Number of frames to delay estimation from trigger occurred. If an accurate stop estimate is needed, a
    delay may be needed as trigger may occur before keyword has been completely spoken. */
    int32_t estimation_delay_frames = 0;

    int32_t offset = rx_index * BUFFER_SIZE + buf_num * VOICESPOT_NUM_SAMPLES_PER_FRAME * sizeof(int32_t);
    int8_t *src    = (int8_t *)(Buffer + offset);

    arm_q31_to_float((int32_t *)src, (float32_t *)frame_buffer, VOICESPOT_NUM_SAMPLES_PER_FRAME);

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

            /* Comment out this line if timing estimation is not needed */
            voicespot_status =
                rdspVoiceSpot_EstimateStartAndStop(voicespot_control, voicespot_handle, score_index_trigger, -1,
                                                   timing_accuracy, &start_offset_samples, &stop_offset_samples);
            if (voicespot_status != RDSP_VOICESPOT_OK)
                PRINTF("[%d] rdspVoiceSpot_EstimateStartAndStop: voicespot_status = %d\r\n", (int)frame_count,
                       (int)voicespot_status);
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
#ifdef USE_PDM_EDMA
    pdm_edma_transfer_t pdmXfer;
    edma_config_t dmaConfig = {0};
#else
    sai_transfer_t xfer;
    sai_transceiver_t saiConfig;
#endif

    BOARD_InitHardware();

    PRINTF("VoiceSpot example started!\r\n");

#ifdef USE_PDM_EDMA

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
        PRINTF("PDM configure sample rate failed.\r\n");
        return -1;
    }
    PDM_Reset(DEMO_PDM);

#else
    /* Init DMA and create handle for DMA */
    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(DEMO_DMA, &dmaConfig);
    EDMA_CreateHandle(&dmaTxHandle, DEMO_DMA, DEMO_TX_EDMA_CHANNEL);
    EDMA_CreateHandle(&dmaRxHandle, DEMO_DMA, DEMO_RX_EDMA_CHANNEL);
#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
    EDMA_SetChannelMux(DEMO_DMA, DEMO_TX_EDMA_CHANNEL, DEMO_SAI_TX_EDMA_CHANNEL);
    EDMA_SetChannelMux(DEMO_DMA, DEMO_RX_EDMA_CHANNEL, DEMO_SAI_RX_EDMA_CHANNEL);
#endif

    /* SAI init */
    SAI_Init(DEMO_SAI);

    SAI_TransferTxCreateHandleEDMA(DEMO_SAI, &txHandle, tx_callback, NULL, &dmaTxHandle);
    SAI_TransferRxCreateHandleEDMA(DEMO_SAI, &rxHandle, rx_callback, NULL, &dmaRxHandle);

    /* I2S mode configurations */
    SAI_GetClassicI2SConfig(&saiConfig, DEMO_AUDIO_BIT_WIDTH, kSAI_MonoRight, 1U << DEMO_SAI_CHANNEL);
    saiConfig.syncMode              = DEMO_SAI_TX_SYNC_MODE;
    saiConfig.bitClock.bclkPolarity = DEMO_SAI_TX_BIT_CLOCK_POLARITY;
    saiConfig.masterSlave           = DEMO_SAI_MASTER_SLAVE;
    SAI_TransferTxSetConfigEDMA(DEMO_SAI, &txHandle, &saiConfig);
    saiConfig.syncMode = DEMO_SAI_RX_SYNC_MODE;
    SAI_TransferRxSetConfigEDMA(DEMO_SAI, &rxHandle, &saiConfig);

    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);
    SAI_RxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);

    /* master clock configurations */
    BOARD_MASTER_CLOCK_CONFIG();

    /* Use default setting to init codec */
    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        assert(false);
    }
    if (CODEC_SetVolume(&codecHandle, kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight,
                        DEMO_CODEC_VOLUME) != kStatus_Success)
    {
        assert(false);
    }
#endif

#ifdef VOICE_SPOT

    if (VoiceSpotInit() != 0)
    {
        assert(false);
    }

    max_scores      = (int32_t *)rdsp_malloc(num_outputs * sizeof(int32_t));
    max_frame_count = (int64_t *)rdsp_malloc(num_outputs * sizeof(int64_t));
    /* Frame buffer into VOICE_SPOT */
    frame_buffer = (int32_t *)rdsp_malloc(num_samples_per_frame * sizeof(int32_t));
    scores       = (int32_t *)rdsp_malloc(num_outputs * sizeof(int32_t));
    /* Used for saving scores of latest trigger */
    scores_trigger = (int32_t *)rdsp_malloc(num_outputs * sizeof(int32_t));

    for (int i = 0; i < num_outputs; i++)
    {
        max_scores[i]      = 0;
        max_frame_count[i] = -1;
    }

#endif
    while (1)
    {
#ifndef USE_PDM_EDMA
        xfer.data     = Buffer + rx_index * BUFFER_SIZE;
        xfer.dataSize = BUFFER_SIZE;
        xfer.data     = Buffer + tx_index * BUFFER_SIZE;
        xfer.dataSize = BUFFER_SIZE;
#endif
        if (firstRead)
        {
#ifdef USE_PDM_EDMA
            /* In case of EDMA, start 2 transfers so there is always one ready to start when
             * previous one completes.
             */
            pdmXfer.data         = Buffer + rx_index * BUFFER_SIZE;
            pdmXfer.dataSize     = BUFFER_SIZE;
            pdmXfer.linkTransfer = NULL;
            if (kStatus_Success == PDM_TransferReceiveEDMA(DEMO_PDM, &s_pdmRxHandle, &pdmXfer))
            {
                rx_index++;
            }
            pdmXfer.data         = Buffer + rx_index * BUFFER_SIZE;
            pdmXfer.dataSize     = BUFFER_SIZE;
            pdmXfer.linkTransfer = NULL;
            if (kStatus_Success == PDM_TransferReceiveEDMA(DEMO_PDM, &s_pdmRxHandle, &pdmXfer))
#else
            if (kStatus_Success == SAI_TransferReceiveEDMA(DEMO_SAI, &rxHandle, &xfer))
#endif
            {
                rx_index++;
            }
            if (rx_index == BUFFER_NUMBER)
            {
                rx_index = 0U;
            }
            firstRead = 0;
        }

        while (!bufferRead)
        {
#ifdef ENABLE_IDLE_CALLBACK
            /* Let chip-specific implementation enter idle to save power and wake-up when data are available. */
            voice_spot_demo_idle_callback();
#endif
        }

#ifdef USE_PDM_EDMA
        // start following EDMA transfer as soon as possible (before voicespot processing) to not miss some data
        pdmXfer.data         = Buffer + rx_index * BUFFER_SIZE;
        pdmXfer.dataSize     = BUFFER_SIZE;
        pdmXfer.linkTransfer = NULL;
        if (kStatus_Success == PDM_TransferReceiveEDMA(DEMO_PDM, &s_pdmRxHandle, &pdmXfer))
        {
            rx_index++;
            bufferRead = 0;
        }
        if (rx_index == BUFFER_NUMBER)
        {
            rx_index = 0U;
        }
#endif

#ifdef VOICE_SPOT
        for (buf_num = 0; buf_num < DMA_TRANSFER_MULT; buf_num++)
        {
            VoiceSpotProcess();
        }
#endif

#ifndef USE_PDM_EDMA // SAI Tx support not implemented when using PDM as Rx
        if (kStatus_Success == SAI_TransferReceiveEDMA(DEMO_SAI, &rxHandle, &xfer))
        {
            rx_index++;
            bufferRead = 0;
        }
        if (rx_index == BUFFER_NUMBER)
        {
            rx_index = 0U;
        }

        if (firstWrite)
        {
            if (kStatus_Success == SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer))
            {
                tx_index++;
            }
            if (tx_index == BUFFER_NUMBER)
            {
                tx_index = 0U;
            }
            firstWrite = 0;
        }
        while (!bufferWritten)
        {
        }

        if (kStatus_Success == SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer))
        {
            tx_index++;
            bufferWritten = 0;
        }
        if (tx_index == BUFFER_NUMBER)
        {
            tx_index = 0U;
        }
#endif
    }
}
