/*
 * Copyright 2025-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_audmix.h"
#include "fsl_sai.h"
#include "fsl_sai_edma.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_codec_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define AUDMIX_SYNC_RETRY_COUNT (1000U)
#define AUDMIX_SYNC_RETRY_DELAY_US (1000U)
#define DEMO_PLAYBACK_DURATION_MS (10000U)
#define DEMO_TDM2_ONLY_DURATION_MS (3000U)
#define DEMO_TDM1_PLAYBACK_DURATION_MS (4000U)
#define DEMO_PLAYBACK_POLL_INTERVAL_US (1000U)
#define DEMO_TDM2_TONE_FREQUENCY_HZ (400U)
#define DEMO_TDM1_TONE_FREQUENCY_HZ (1000U)
#define DEMO_TDM2_ATTENUATION_FACTOR_10_PERCENT_Q18 (0x06666U)
#define DEMO_TDM2_ATTENUATION_STEP_TARGET (1U)
#define DEMO_FRAME_SYNC_LEN     kSAI_FrameSyncLenOneBitClk
#define DEMO_AUDIO_DATA_CHANNEL (8U)
#define DEMO_ACTIVE_AUDIO_CHANNELS (2U)
#define DEMO_MASKED_TDM_WORDS ((1UL << DEMO_AUDIO_DATA_CHANNEL) - (1UL << DEMO_ACTIVE_AUDIO_CHANNELS))
#define DEMO_AUDIO_FRAME_SIZE (DEMO_SAI_CHANNEL_NUM * (DEMO_SAI_BITWIDTH / 8U))
/* 3840 bytes = 480 stereo frames = 4 periods at 400 Hz and also 256-byte aligned for EDMA. */
#define DEMO_TDM2_LOOP_BUFFER_SIZE (3840U)
/* 768 bytes = 96 stereo frames = 2 periods at 1000 Hz and also 256-byte aligned for EDMA. */
#define DEMO_TDM1_LOOP_BUFFER_SIZE (768U)
#define DEMO_TDM2_LOOP_TRANSFER_COUNT (1U)
#define DEMO_TDM1_LOOP_TRANSFER_COUNT (1U)
#define DEMO_TDM2_TONE_FRAME_COUNT (DEMO_TDM2_LOOP_BUFFER_SIZE / DEMO_AUDIO_FRAME_SIZE)
#define DEMO_TDM1_TONE_FRAME_COUNT (DEMO_TDM1_LOOP_BUFFER_SIZE / DEMO_AUDIO_FRAME_SIZE)
#define DEMO_Q30_SHIFT (30U)
#define DEMO_TONE_AMPLITUDE (0x10000000L)
#define DEMO_SINE_COEFF_400HZ_Q30 ((int32_t)2144540596UL)
#define DEMO_SINE_COEFF_1000HZ_Q30 ((int32_t)2129111628UL)
#define DEMO_SINE_INITIAL_400HZ_Q30 (56195305L)
#define DEMO_SINE_INITIAL_1000HZ_Q30 (140151432L)
#define DEMO_TDM_CHANNEL_1 (0U)
#define DEMO_TDM_CHANNEL_2 (1U)

#ifndef DEMO_DMAMUX
#define DEMO_DMAMUX DMAMUX0
#endif
#ifndef DEMO_CODEC_INIT_DELAY_MS
#define DEMO_CODEC_INIT_DELAY_MS (1000U)
#endif
#ifndef DEMO_CODEC_VOLUME
#define DEMO_CODEC_VOLUME 75U
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DEMO_InitSAI(void);
static void DEMO_InitAUDMIX(void);
static void DEMO_TxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData);
static void DEMO_InitEDMA(void);
static void DEMO_StartSAITransfer(void);
static void DEMO_GenerateToneBuffer(int32_t *destBuf, uint32_t frameCount, int32_t coeffQ30, int32_t initialQ30);

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION(sai_edma_handle_t txHandle1);
AT_NONCACHEABLE_SECTION(sai_edma_handle_t txHandle2);
AT_NONCACHEABLE_SECTION(edma_handle_t dmaTxHandle1);
AT_NONCACHEABLE_SECTION(edma_handle_t dmaTxHandle2);

extern codec_config_t boardCodecConfig;
codec_handle_t codecHandle;

AT_NONCACHEABLE_SECTION_ALIGN(static int32_t s_tdm2ToneBuffer[DEMO_TDM2_LOOP_BUFFER_SIZE / sizeof(int32_t)], 4);
AT_NONCACHEABLE_SECTION_ALIGN(static int32_t s_tdm1ToneBuffer[DEMO_TDM1_LOOP_BUFFER_SIZE / sizeof(int32_t)], 4);

/* SAI transfer configurations */
sai_transfer_t tdm2TxTransfer;
sai_transfer_t tdm1TxTransfer;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    /* Initialize board hardware */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);
    PRINTF("AUDMIX Example started!\r\n");

    /* Initialize EDMA */
    DEMO_InitEDMA();

    /* Initialize SAI peripherals */
    DEMO_InitSAI();


    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        assert(false);
    }
    if (CODEC_SetVolume(&codecHandle, kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight,
                        DEMO_CODEC_VOLUME) != kStatus_Success)
    {
        assert(false);
    }
    /* delay for codec output stable */
    SDK_DelayAtLeastUs(DEMO_CODEC_INIT_DELAY_MS * 1000U, SystemCoreClock);

    /* Set AUDMIX to mixed mode */
    BOARD_SetAudmixMode(0x1U);

    /* Initialize AUDMIX peripheral */
    DEMO_InitAUDMIX();

    /* Start SAI transfers */
    DEMO_StartSAITransfer();

    bool attenuationStarted = false;
    bool attenuationEnabled = false;
    bool tdm1PlaybackEnabled = false;
    bool tdm1PlaybackStopped = false;
    bool mixedOutputEnabled = false;

    /* Keep playback active for the requested duration, adding TDM1 and TDM2 attenuation after 3 seconds. */
    uint32_t playbackElapsedMs = 0U;
    while (playbackElapsedMs < DEMO_PLAYBACK_DURATION_MS)
    {
        if (!attenuationStarted && (playbackElapsedMs >= DEMO_TDM2_ONLY_DURATION_MS))
        {
            audmix_attenuation_config_t attConfig;
            uint32_t syncRetryCount = 0U;

            AUDMIX_GetDefaultAttenuationConfig(&attConfig);
            attConfig.attenuationEnable = true;
            attConfig.attenuationDirection = kAUDMIX_AttenuationDirectionDown;
            attConfig.stepDownFactor = DEMO_TDM2_ATTENUATION_FACTOR_10_PERCENT_Q18;
            attConfig.stepTarget = DEMO_TDM2_ATTENUATION_STEP_TARGET;
            attenuationStarted = true;

            if (AUDMIX_SetAttenuationConfig(DEMO_AUDMIX, DEMO_TDM_CHANNEL_2, &attConfig) == kStatus_Success)
            {
                attenuationEnabled = true;
                PRINTF("TDM2 attenuation enabled to approximately 10%% after %u ms.\r\n", DEMO_TDM2_ONLY_DURATION_MS);
            }
            else
            {
                PRINTF("Failed to enable TDM2 attenuation.\r\n");
            }

            if (SAI_TransferSendLoopEDMA(DEMO_TDM1, &txHandle2, &tdm1TxTransfer, DEMO_TDM1_LOOP_TRANSFER_COUNT) ==
                kStatus_Success)
            {
                SAI_TxEnable(DEMO_TDM1, true);
                tdm1PlaybackEnabled = true;
                PRINTF("TDM1 %u Hz playback started.\r\n", DEMO_TDM1_TONE_FREQUENCY_HZ);

                while ((syncRetryCount < AUDMIX_SYNC_RETRY_COUNT) &&
                       !(AUDMIX_IsFrameRateMatched(DEMO_AUDMIX) && AUDMIX_IsClockFrequencyMatched(DEMO_AUDMIX)))
                {
                    SDK_DelayAtLeastUs(AUDMIX_SYNC_RETRY_DELAY_US, SystemCoreClock);
                    syncRetryCount++;
                }

                if (syncRetryCount < AUDMIX_SYNC_RETRY_COUNT)
                {
                    if (AUDMIX_SetOutputSource(DEMO_AUDMIX, kAUDMIX_OutputMixed) == kStatus_Success)
                    {
                        PRINTF("TDM streams are synchronized, enabling mixed output.\r\n");
                        mixedOutputEnabled = true;
                    }
                }
                else
                {
                    PRINTF("Mixer synchronization timed out.\r\n");
                }
            }
            else
            {
                PRINTF("Failed to start TDM1 playback.\r\n");
            }
        }

        if (attenuationEnabled && (playbackElapsedMs >= (DEMO_TDM2_ONLY_DURATION_MS + DEMO_TDM1_PLAYBACK_DURATION_MS)))
        {
            AUDMIX_EnableAttenuation(DEMO_AUDMIX, DEMO_TDM_CHANNEL_2, false);
            attenuationEnabled = false;
            PRINTF("TDM2 attenuation disabled after %u ms.\r\n",
                   DEMO_TDM2_ONLY_DURATION_MS + DEMO_TDM1_PLAYBACK_DURATION_MS);
        }

        if (tdm1PlaybackEnabled && !tdm1PlaybackStopped &&
            (playbackElapsedMs >= (DEMO_TDM2_ONLY_DURATION_MS + DEMO_TDM1_PLAYBACK_DURATION_MS)))
        {
            if (mixedOutputEnabled)
            {
                if (AUDMIX_SetOutputSource(DEMO_AUDMIX, kAUDMIX_OutputFromTDM2) == kStatus_Success)
                {
                    mixedOutputEnabled = false;
                }
            }

                     SAI_TransferAbortSendEDMA(DEMO_TDM1, &txHandle2);
                     SAI_TxEnable(DEMO_TDM1, false);
         tdm1PlaybackStopped = true;
         tdm1PlaybackEnabled = false;
         PRINTF("TDM1 playback stopped after %u ms.\r\n",
             DEMO_TDM2_ONLY_DURATION_MS + DEMO_TDM1_PLAYBACK_DURATION_MS);
        }

        SDK_DelayAtLeastUs(DEMO_PLAYBACK_POLL_INTERVAL_US, SystemCoreClock);
        playbackElapsedMs += (DEMO_PLAYBACK_POLL_INTERVAL_US / 1000U);
    }

    PRINTF("AUDMIX Example completed!\r\n");

    /* Cleanup */
    SAI_TransferAbortSendEDMA(DEMO_TDM2, &txHandle1);
    SAI_TransferAbortSendEDMA(DEMO_TDM1, &txHandle2);
    SAI_Deinit(DEMO_TDM2);
    SAI_Deinit(DEMO_TDM1);
    AUDMIX_Deinit(DEMO_AUDMIX);

    while (1)
    {
    }
}

/*!
 * @brief Initialize the EDMA
 */
static void DEMO_InitEDMA(void)
{
    edma_config_t dmaConfig = {0};

    /* Create EDMA handle */
    EDMA_GetDefaultConfig(&dmaConfig);
#if defined(BOARD_GetEDMAConfig)
    BOARD_GetEDMAConfig(dmaConfig);
#endif
    EDMA_Init(DEMO_DMA, &dmaConfig);

    /* Create EDMA handles for TDM2 and TDM1. */
    EDMA_CreateHandle(&dmaTxHandle1, DEMO_DMA, DEMO_EDMA_CHANNEL);
    EDMA_CreateHandle(&dmaTxHandle2, DEMO_DMA, DEMO_EDMA_CHANNEL + 1);

#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
#if defined(DEMO_TDM2_TX_SOURCE)
    EDMA_SetChannelMux(DEMO_DMA, DEMO_EDMA_CHANNEL, DEMO_TDM2_TX_SOURCE);
#endif
#if defined(DEMO_TDM1_TX_SOURCE)
    EDMA_SetChannelMux(DEMO_DMA, DEMO_EDMA_CHANNEL + 1, DEMO_TDM1_TX_SOURCE);
#endif
#endif

#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
    DMAMUX_Init(DEMO_DMAMUX);
    DMAMUX_SetSource(DEMO_DMAMUX, DEMO_EDMA_CHANNEL, DEMO_TDM2_TX_SOURCE);
    DMAMUX_EnableChannel(DEMO_DMAMUX, DEMO_EDMA_CHANNEL);
    DMAMUX_SetSource(DEMO_DMAMUX, DEMO_EDMA_CHANNEL + 1, DEMO_TDM1_TX_SOURCE);
    DMAMUX_EnableChannel(DEMO_DMAMUX, DEMO_EDMA_CHANNEL + 1);
#endif
}

/*!
 * @brief Initialize the SAI peripherals
 */
static void DEMO_InitSAI(void)
{
    sai_transceiver_t tdm2TxConfig = {0};
    sai_transceiver_t tdm1TxConfig = {0};
    uint32_t sourceClock = DEMO_SAI_CLK_FREQ;

    /* Initialize TDM2 */
    SAI_Init(DEMO_TDM2);

    /* Initialize TDM1 */
    SAI_Init(DEMO_TDM1);

    /* Create EDMA SAI handle */
    SAI_TransferTxCreateHandleEDMA(DEMO_TDM2, &txHandle1, DEMO_TxCallback, NULL, &dmaTxHandle1);
    SAI_TransferTxCreateHandleEDMA(DEMO_TDM1, &txHandle2, DEMO_TxCallback, NULL, &dmaTxHandle2);

    /* TDM mode configurations */
    SAI_GetTDMConfig(&tdm2TxConfig, DEMO_FRAME_SYNC_LEN, DEMO_SAI_BITWIDTH, DEMO_AUDIO_DATA_CHANNEL, DEMO_SAI_CHANNEL);
    SAI_GetTDMConfig(&tdm1TxConfig, DEMO_FRAME_SYNC_LEN, DEMO_SAI_BITWIDTH, DEMO_AUDIO_DATA_CHANNEL, DEMO_SAI_CHANNEL);
    /* Keep the 8-word TDM frame for codec timing, but only drive the first two slots with audio data. */
    tdm2TxConfig.serialData.dataMaskedWord = DEMO_MASKED_TDM_WORDS;
    tdm1TxConfig.serialData.dataMaskedWord = DEMO_MASKED_TDM_WORDS;
    tdm2TxConfig.frameSync.frameSyncEarly = true;
    tdm1TxConfig.frameSync.frameSyncEarly = true;

    /* Apply configuration to both TDM transmitters. */
    SAI_TransferTxSetConfigEDMA(DEMO_TDM2, &txHandle1, &tdm2TxConfig);
    SAI_TransferTxSetConfigEDMA(DEMO_TDM1, &txHandle2, &tdm1TxConfig);

    /* In TDM mode the bit clock must cover all 8 data words per frame, not just stereo samples. */
    SAI_TxSetBitClockRate(DEMO_TDM2, sourceClock, DEMO_SAI_SAMPLE_RATE, DEMO_SAI_BITWIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);
    SAI_TxSetBitClockRate(DEMO_TDM1, sourceClock, DEMO_SAI_SAMPLE_RATE, DEMO_SAI_BITWIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);

    /* If need to handle audio error, enable sai interrupt */
#if defined(DEMO_SAI_IRQ)
    EnableIRQ(DEMO_SAI_IRQ);
    SAI_TxEnableInterrupts(DEMO_TDM2, kSAI_FIFOErrorInterruptEnable);
    SAI_TxEnableInterrupts(DEMO_TDM1, kSAI_FIFOErrorInterruptEnable);
#endif
}

/*!
 * @brief Initialize the AUDMIX peripheral
 */
static void DEMO_InitAUDMIX(void)
{
    audmix_config_t audmixConfig;
    audmix_attenuation_config_t attenuationConfig;

    /* Initialize AUDMIX */
    AUDMIX_Init(DEMO_AUDMIX);

    /* Configure AUDMIX */
    AUDMIX_GetDefaultConfig(&audmixConfig);
    audmixConfig.outputWidth = kAUDMIX_OutputWidth32Bit;
    audmixConfig.outputClockPolarity = kAUDMIX_OutputClockPolarityNegativeEdge;
    audmixConfig.mixClockSource = kAUDMIX_MixClockFromTDM2;
    audmixConfig.outputSource = kAUDMIX_OutputFromTDM2; /* Start with output disabled */
    AUDMIX_SetConfig(DEMO_AUDMIX, &audmixConfig);

    AUDMIX_GetDefaultAttenuationConfig(&attenuationConfig);
    AUDMIX_SetAttenuationConfig(DEMO_AUDMIX, DEMO_TDM_CHANNEL_1, &attenuationConfig);
}

/*!
 * @brief SAI EDMA callback
 */
static void DEMO_TxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    (void)base;
    (void)handle;
    (void)status;
    (void)userData;
}

/*!
 * @brief Start SAI transfers
 */
static void DEMO_StartSAITransfer(void)
{
    DEMO_GenerateToneBuffer(s_tdm2ToneBuffer, DEMO_TDM2_TONE_FRAME_COUNT, DEMO_SINE_COEFF_400HZ_Q30,
                            DEMO_SINE_INITIAL_400HZ_Q30);
    DEMO_GenerateToneBuffer(s_tdm1ToneBuffer, DEMO_TDM1_TONE_FRAME_COUNT, DEMO_SINE_COEFF_1000HZ_Q30,
                            DEMO_SINE_INITIAL_1000HZ_Q30);

    tdm2TxTransfer.data = (uint8_t *)s_tdm2ToneBuffer;
    tdm2TxTransfer.dataSize = DEMO_TDM2_LOOP_BUFFER_SIZE;

    tdm1TxTransfer.data = (uint8_t *)s_tdm1ToneBuffer;
    tdm1TxTransfer.dataSize = DEMO_TDM1_LOOP_BUFFER_SIZE;

    /* Start TDM2 immediately. TDM1 is started later from the playback timeline. */
    SAI_TransferSendLoopEDMA(DEMO_TDM2, &txHandle1, &tdm2TxTransfer, DEMO_TDM2_LOOP_TRANSFER_COUNT);

    /* Enable TDM2 first. TDM1 is enabled when its playback window begins. */
    SAI_TxEnable(DEMO_TDM2, true);
    PRINTF("TDM2 %u Hz playback started.\r\n", DEMO_TDM2_TONE_FREQUENCY_HZ);
}

/*!
 * @brief Process Audio data
 *
 * This function fills an interleaved stereo buffer with a generated sine tone.
 * - destBuf: Pointer to destination stereo PCM buffer.
 * - frameCount: Number of stereo frames to generate.
 * - coeffQ30: Q30 oscillator coefficient equal to 2*cos(omega).
 * - initialQ30: Q30 oscillator seed equal to sin(omega).
 */
static void DEMO_GenerateToneBuffer(int32_t *destBuf, uint32_t frameCount, int32_t coeffQ30, int32_t initialQ30)
{
    int32_t previousSampleQ30 = 0;
    int32_t currentSampleQ30 = initialQ30;

    for (uint32_t frameIndex = 0; frameIndex < frameCount; frameIndex++)
    {
        int32_t sampleQ30;
        int32_t pcmSample;

        if (frameIndex == 0U)
        {
            sampleQ30 = previousSampleQ30;
        }
        else if (frameIndex == 1U)
        {
            sampleQ30 = currentSampleQ30;
        }
        else
        {
            int64_t nextSampleQ30 = (((int64_t)coeffQ30 * currentSampleQ30) >> DEMO_Q30_SHIFT) - previousSampleQ30;

            previousSampleQ30 = currentSampleQ30;
            currentSampleQ30 = (int32_t)nextSampleQ30;
            sampleQ30 = currentSampleQ30;
        }

        pcmSample = (int32_t)(((int64_t)DEMO_TONE_AMPLITUDE * sampleQ30) >> DEMO_Q30_SHIFT);
        destBuf[(frameIndex * DEMO_SAI_CHANNEL_NUM)] = pcmSample;
        destBuf[(frameIndex * DEMO_SAI_CHANNEL_NUM) + 1U] = pcmSample;
    }
}

#if defined(DEMO_SAITxIRQHandler)
void DEMO_SAITxIRQHandler(void)
{
    /* Clear the FIFO error flag */
    if (SAI_TxGetStatusFlag(DEMO_TDM2) & kSAI_FIFOErrorFlag)
    {
        SAI_TxClearStatusFlags(DEMO_TDM2, kSAI_FIFOErrorFlag);
    }
    if (SAI_TxGetStatusFlag(DEMO_TDM1) & kSAI_FIFOErrorFlag)
    {
        SAI_TxClearStatusFlags(DEMO_TDM1, kSAI_FIFOErrorFlag);
    }

    /* Reset FIFO */
    SAI_TxSoftwareReset(DEMO_TDM2, kSAI_ResetTypeFIFO);
    SAI_TxSoftwareReset(DEMO_TDM1, kSAI_ResetTypeFIFO);

    SDK_ISR_EXIT_BARRIER;
}
#endif
