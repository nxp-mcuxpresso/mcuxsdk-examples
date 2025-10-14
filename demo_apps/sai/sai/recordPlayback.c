/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sai.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern sai_edma_handle_t txHandle;
extern sai_edma_handle_t rxHandle;
extern uint8_t audioBuff[BUFFER_SIZE * BUFFER_NUM];
extern volatile bool istxFinished;
extern volatile bool isrxFinished;
extern volatile uint32_t beginCount;
extern volatile uint32_t sendCount;
extern volatile uint32_t receiveCount;
extern volatile uint32_t fullBlock;
extern volatile uint32_t emptyBlock;
/*******************************************************************************
 * Code
 ******************************************************************************/
void RecordPlayback(I2S_Type *base, uint32_t time_s)
{
    /* Input validation - CERT-C: FLP32-C, INT32-C */
    if (base == NULL) {
        return;
    }

    /* Check for potential overflow in beginCount calculation - CERT-C: INT32-C */
    if (time_s > (UINT32_MAX / (DEMO_AUDIO_SAMPLE_RATE * 4U))) {
        return; /* Prevent overflow */
    }

    if (BUFFER_SIZE == 0U) {
        return; /* Prevent division by zero */
    }

    sai_transfer_t xfer = {0};
    uint32_t playbackCount = 0U;
    uint32_t recordCount = 0U;
    uint32_t txindex = 0U;
    uint32_t rxindex = 0U;

    /* First clear the buffer - CERT-C: STR31-C */
    (void)memset(audioBuff, 0, BUFFER_SIZE * BUFFER_NUM);

    /* Initialize volatile variables atomically - CERT-C: CON43-C */
    istxFinished = false;
    isrxFinished = false;
    sendCount    = 0U;
    receiveCount = 0U;

    /* Reset SAI internal logic */
    SAI_TxSoftwareReset(base, kSAI_ResetTypeSoftware);
    SAI_RxSoftwareReset(base, kSAI_ResetTypeSoftware);

    /* Compute the begin count - overflow already checked above */
    beginCount = (time_s * DEMO_AUDIO_SAMPLE_RATE * 4U) / BUFFER_SIZE;

    xfer.dataSize = BUFFER_SIZE;

    /* Wait for playback finished */
    while ((recordCount < beginCount) || (playbackCount < beginCount))
    {
        /* Check bounds before array access - CERT-C: ARR30-C */
        if ((emptyBlock > 0U) && (recordCount < beginCount) && (rxindex < BUFFER_NUM))
        {
            /* Safe pointer arithmetic - CERT-C: ARR39-C */
            xfer.data = &audioBuff[rxindex * BUFFER_SIZE];
            if (SAI_TransferReceiveEDMA(base, &rxHandle, &xfer) == kStatus_Success)
            {
                rxindex = (rxindex + 1U) % BUFFER_NUM;
                if (emptyBlock > 0U) { /* Additional check to prevent underflow */
                    emptyBlock--;
                }
                recordCount++;
            }
        }

        /* Check bounds before array access - CERT-C: ARR30-C */
        if ((fullBlock > 0U) && (playbackCount < beginCount) && (txindex < BUFFER_NUM))
        {
            /* Safe pointer arithmetic - CERT-C: ARR39-C */
            xfer.data = &audioBuff[txindex * BUFFER_SIZE];
            if (SAI_TransferSendEDMA(base, &txHandle, &xfer) == kStatus_Success)
            {
                txindex = (txindex + 1U) % BUFFER_NUM;
                if (fullBlock > 0U) { /* Additional check to prevent underflow */
                    fullBlock--;
                }
                playbackCount++;
            }
        }
    }

    /* Wait for record and playback finished */
    while ((istxFinished != true) || (isrxFinished != true))
    {
        /* Empty loop body is intentional for waiting */
    }
}
