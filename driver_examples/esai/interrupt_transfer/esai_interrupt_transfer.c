/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_esai.h"
#include "fsl_lpi2c.h"
#include "fsl_codec_common.h"
#ifdef ESAI_SYNC_PATCH
#include "fsl_common.h"
#endif
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define OVER_SAMPLE_RATE (256U)
#define SAMPLE_RATE      (kESAI_SampleRate48KHz)
#define BUFFER_SIZE      (1024)
#define BUFFER_NUM       (4)
#define PLAY_COUNT       (5000)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void txCallback(ESAI_Type *base, esai_handle_t *handle, status_t status, void *userData);
static void rxCallback(ESAI_Type *base, esai_handle_t *handle, status_t status, void *userData);
/*******************************************************************************
 * Variables
 ******************************************************************************/
esai_handle_t txHandle = {0};
esai_handle_t rxHandle = {0};
static uint8_t audioBuff[BUFFER_SIZE * BUFFER_NUM];
volatile bool istxFinished     = false;
volatile bool isrxFinished     = false;
volatile uint32_t beginCount   = 0;
volatile uint32_t sendCount    = 0;
volatile uint32_t receiveCount = 0;
codec_handle_t codecHandle;
extern codec_config_t boardCodecConfig;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void txCallback(ESAI_Type *base, esai_handle_t *handle, status_t status, void *userData)
{
    esai_transfer_t xfer = {0};

    sendCount++;

    if (sendCount == beginCount)
    {
        istxFinished = true;
        ESAI_TransferAbortSend(base, handle);
        sendCount = 0;
    }
    else
    {
        xfer.data     = audioBuff + ((sendCount - 1U) % BUFFER_NUM) * BUFFER_SIZE;
        xfer.dataSize = BUFFER_SIZE;
        ESAI_TransferSendNonBlocking(base, handle, &xfer);
    }
}

static void rxCallback(ESAI_Type *base, esai_handle_t *handle, status_t status, void *userData)
{
    esai_transfer_t xfer = {0};

    receiveCount++;

    if (receiveCount == beginCount)
    {
        isrxFinished = true;
        ESAI_TransferAbortReceive(base, handle);
        receiveCount = 0;
    }
    else
    {
        xfer.data     = audioBuff + ((receiveCount - 1U) % BUFFER_NUM) * BUFFER_SIZE;
        xfer.dataSize = BUFFER_SIZE;
        ESAI_TransferReceiveNonBlocking(base, handle, &xfer);
    }
}

int main(void)
{
    esai_config_t config;
    uint32_t hclkSourceClockHz = 0U;
    esai_transfer_t txfer;
    esai_transfer_t rxfer;
    esai_format_t format;

    BOARD_InitHardware();

    PRINTF("\r\nESAI Interrupt example started! \n\r");

    /* Init codec */
    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        PRINTF("CODEC_Init failed!\r\n");
        return -1;
    }

    /* Initialize ESAI module */
    ESAI_GetDefaultConfig(&config);
#if defined CODEC_CS42888
    /* CS42888 have seperate clock source for Tx and Rx, shall not use sync mode */
    config.syncMode = kESAI_ModeAsync;
#endif
    ESAI_Init(DEMO_ESAI, &config);

    /* Configure the audio format */
    format.slotType      = kESAI_SlotLen32WordLen16;
    format.sampleRate_Hz = SAMPLE_RATE;
    format.sectionMap    = 0x1;

    ESAI_TransferTxCreateHandle(DEMO_ESAI, &txHandle, txCallback, NULL);
    ESAI_TransferRxCreateHandle(DEMO_ESAI, &rxHandle, rxCallback, NULL);

    hclkSourceClockHz = ESAI_SOURCE_CLOCK_FREQ;
#if defined ESAI_TX_CHANNEL
    format.sectionMap = (1U << ESAI_TX_CHANNEL);
#endif
    ESAI_TransferTxSetFormat(DEMO_ESAI, &txHandle, &format, format.sampleRate_Hz * 256U, hclkSourceClockHz);
#if defined ESAI_RX_CHANNEL
    format.sectionMap = (1U << ESAI_RX_CHANNEL);
#else
    format.sectionMap = 1U;
#endif
    ESAI_TransferRxSetFormat(DEMO_ESAI, &rxHandle, &format, format.sampleRate_Hz * 256U, hclkSourceClockHz);

    /*  xfer structure */
    rxfer.data     = audioBuff;
    rxfer.dataSize = BUFFER_SIZE;
    txfer.data     = audioBuff;
    txfer.dataSize = BUFFER_SIZE;

    /* Set the time to record and playback */
    beginCount = PLAY_COUNT;

    ESAI_TransferReceiveNonBlocking(DEMO_ESAI, &rxHandle, &rxfer);
#ifdef ESAI_SYNC_PATCH
    /*
     * Delay half of the FIFO Length to make sure RX handler happens prior to TX handler
     */
    SDK_DelayAtLeastUs(1000000 * 32 / SAMPLE_RATE, SystemCoreClock);
#endif
    ESAI_TransferSendNonBlocking(DEMO_ESAI, &txHandle, &txfer);

    /* Waiting for transfer finished */
    while ((isrxFinished == false) || (istxFinished == false))
    {
    }

    ESAI_TransferAbortReceive(DEMO_ESAI, &rxHandle);
    ESAI_TransferAbortSend(DEMO_ESAI, &txHandle);
    PRINTF("\r\nESAI Interrupt example Succeed! \n\r");

    while (1)
    {
    }
}

void AUDIO_IRQHandler(void)
{
    /* Handle Rx operation */
    if ((ESAI_GetStatusFlag(DEMO_ESAI) & kESAI_ReceiveFIFOFullFlag) && (DEMO_ESAI->RCR & kESAI_TransmitInterruptEnable))
    {
        ESAI_TransferRxHandleIRQ(DEMO_ESAI, &rxHandle);
    }

    /* Handle Tx operation */
    if ((ESAI_GetStatusFlag(DEMO_ESAI) & kESAI_TransmitFIFOEmptyFlag) &&
        (DEMO_ESAI->TCR & kESAI_TransmitInterruptEnable))
    {
        ESAI_TransferTxHandleIRQ(DEMO_ESAI, &txHandle);
    }
    SDK_ISR_EXIT_BARRIER;
}
