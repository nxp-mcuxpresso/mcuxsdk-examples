/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <math.h>
#include "fsl_debug_console.h"
#include "fsl_flexio_a-format.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ENCODER_ID 0x03

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
encoder_A_format encoder;
status_t status;
static bool cmdFlag = false;
flexio_a_format_handle_t g_aformatHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/
void FLEXIO_A_Format_UserCallback(FLEXIO_A_FORMAT_Type *base,
       	                          flexio_a_format_handle_t *handle,
                                  status_t status, void *userData)
{
    userData = userData;

    if (kStatus_FLEXIO_A_FORMAT_TxIdle == status)
    {
//        txBufferFull = false;
//        txOnGoing    = false;
    }

    if (kStatus_FLEXIO_A_FORMAT_RxIdle == status)
    {
        cmdFlag = true;
//        rxBufferEmpty = false;
//        rxOnGoing     = false;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Structure of initialize A-format encoder */
    flexio_a_format_config_t devConfig;
    FLEXIO_A_FORMAT_Type encDev;
    encoder_abs_multi_single_t enc_abs;

    BOARD_InitHardware();

    PRINTF("Encoder A-format example\r\n");

    /* 
     * Config->enableA_Format   = true;
     * Config->enableInDoze     = false;
     * Config->enableInDebug    = true;
     * Config->enableFastAccess = false;
     * Config->baudRate_bps     = kFLEXIO_A_FORMAT_2_5MHZ;
     * Config->userMode         = kFLEXIO_A_FORMAT_USERMODE_ONESHOT;
     */
    FLEXIO_A_Format_GetDefaultConfig(&devConfig);

    encDev.flexioBase                 = BOARD_FLEXIO_BASE;
    encDev.TxPinIndex                 = FLEXIO_A_FORMAT_TX_PIN; 
    encDev.RxPinIndex                 = FLEXIO_A_FORMAT_RX_PIN;
    encDev.DRPinIndex                 = FLEXIO_A_FORMAT_DR_PIN;
    encDev.shifterIndex[0]            = A_FORMAT_TX_SHIFTER_INDEX;
    encDev.shifterIndex[1]            = A_FORMAT_RX_SHIFTER_INDEX;
    encDev.timerIndex[TIMER_TX_INDEX] = A_FORMAT_TX_TIMER_INDEX;
    encDev.timerIndex[TIMER_RX_INDEX] = A_FORMAT_RX_TIMER_INDEX;
    encDev.timerIndex[TIMER_DR_INDEX] = A_FORMAT_DR_TIMER_INDEX;
//    encDev.triggerIn                  = A_FORMAT_TRIGGER_INDEX;

    encoder.controller = &encDev;
    encoder.singleTurnRevolution  = 20;
    encoder.multiTunrRevolution   = 16;
    encoder.single_turn_sign_mask = 0x000FFFFF;
    encoder.multi_turn_sign_mask  = 0x0000FFFF;

    status = FLEXIO_A_Format_Init(&encDev, &devConfig, FLEXIO_CLOCK_FREQUENCY);
    if (status != kStatus_Success)
    {
        return -1;
    }

    FLEXIO_A_Format_TransferCreateHandle(&encDev, &g_aformatHandle, FLEXIO_A_Format_UserCallback, NULL);
    A_Format_ABS_Readout_Multi_Single_IRQ(&encoder, ENCODER_ADDRESS_IT(0x03), &enc_abs);
    while (1)
    {
        if (cmdFlag)
        {
            PRINTF("Multi-turn data: %d, single-turn data: %ld\r\n", enc_abs.multiTurn, enc_abs.singleTurn);
            cmdFlag = false;
            SDK_DelayAtLeastUs(3000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
            A_Format_ABS_Readout_Multi_Single_IRQ(&encoder, ENCODER_ADDRESS_IT(0x03), &enc_abs);
        }
    }
}
