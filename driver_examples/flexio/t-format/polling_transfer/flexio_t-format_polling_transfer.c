/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <math.h>
#include "fsl_debug_console.h"
#include "fsl_flexio_t-format.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MODE_POLL  0
#define MODE_INT   1
#define MODE       MODE_POLL

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
encoder_T_format encoder;
status_t status;

/*******************************************************************************
 * Code
 ******************************************************************************/
#if (MODE == MODE_INT)
static bool cmdFlag = false;
void FLEXIO_T_Format_UserCallback(FLEXIO_T_FORMAT_Type *base,
       	                          flexio_t_format_handle_t *handle,
                                  status_t status, void *userData)
{
    userData = userData;

    if (kStatus_FLEXIO_T_FORMAT_TxIdle == status)
    {
//        txBufferFull = false;
//        txOnGoing    = false;
    }

    if (kStatus_FLEXIO_T_FORMAT_RxIdle == status)
    {
        cmdFlag = true;
//        rxBufferEmpty = false;
//        rxOnGoing     = false;
    }
}
#endif

/*!
 * @brief Main function
 */
int main(void)
{
    /* Structure of initialize T-format encoder */
    flexio_t_format_config_t devConfig;
    FLEXIO_T_FORMAT_Type encDev;
    encoder_all_info_t enc_abs, abs_save;
    uint8_t enc_id;
    int8_t temp = 0, temp_save = 0;
    uint64_t time = 0;

    BOARD_InitHardware();

    PRINTF("Encoder T-format example\r\n");

    /* 
     * Config->enableT_Format   = true;
     * Config->enableInDoze     = false;
     * Config->enableInDebug    = true;
     * Config->enableFastAccess = false;
     * Config->baudRate_bps     = kFLEXIO_A_FORMAT_2_5MHZ;
     * Config->userMode         = kFLEXIO_A_FORMAT_USERMODE_ONESHOT;
     */
    FLEXIO_T_Format_GetDefaultConfig(&devConfig);

    encDev.flexioBase      = BOARD_FLEXIO_BASE;
    encDev.TxPinIndex      = FLEXIO_T_FORMAT_TX_PIN;
    encDev.RxPinIndex      = FLEXIO_T_FORMAT_RX_PIN;
    encDev.DRPinIndex      = FLEXIO_T_FORMAT_DR_PIN;
    encDev.shifterIndex[0] = T_FORMAT_TX_SHIFTER_INDEX;
    encDev.shifterIndex[1] = T_FORMAT_RX_SHIFTER_INDEX;
    encDev.timerIndex[0]   = T_FORMAT_TX_TIMER_INDEX;
    encDev.timerIndex[1]   = T_FORMAT_RX_TIMER_INDEX;
    encDev.timerIndex[2]   = T_FORMAT_DR_TIMER_INDEX;
//    encDev.triggerIn       = T_FORMAT_TRIGGER_INDEX;

    encoder.controller = &encDev;
    encoder.singleTurnRevolution  = 17;
    encoder.multiTunrRevolution   = 16;
    encoder.single_turn_sign_mask = 0x0001FFFF;
    encoder.multi_turn_sign_mask  = 0x0000FFFF;

    status = FLEXIO_T_Format_Init(&encDev, &devConfig, FLEXIO_CLOCK_FREQUENCY);
    if (status != kStatus_Success)
    {
        return -1;
    }

#if (MODE == MODE_INT)
    FLEXIO_T_Format_TransferCreateHandle(&encDev, &g_aformatHandle, FLEXIO_T_Format_UserCallback, NULL);
    T_Format_ABS_Readout_Multi_Single_IRQ(&encoder, ENCODER_ADDRESS_IT(0x03), &enc_abs);
    while (1)
    {
        if (cmdFlag)
        {
            PRINTF("Multi-turn data: %d, single-turn data: %ld\r\n", enc_abs.multiTurn, enc_abs.singleTurn);
            cmdFlag = false;
        }
        SDK_DelayAtLeastUs(3000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        T_Format_ABS_Readout_Multi_Single_IRQ(&encoder, ENCODER_ADDRESS_IT(0x03), &enc_abs);
    }
#endif

#if (MODE == MODE_POLL)
    status = T_Format_Get_Encoder_ID(&encoder, &enc_id);
    if (status == kStatus_FLEXIO_T_FORMAT_FrameErr)
    {
        PRINTF("[T-format] The frame of getting encoder ID is error!\r\n");
    }
    else
    {
        PRINTF("[T-format] Encoder ID: 0x%02X\r\n\t%s\r\n", enc_id, T_Format_GetStatusFlag(status));
    }

    status = T_Format_Readout_ABS_ABM(&encoder, &enc_abs);
    if (status == kStatus_FLEXIO_T_FORMAT_FrameErr)
    {
        PRINTF("[T-format] The frame of getting ABS and ABM is error!\r\n");
    }
    else
    {
        PRINTF("[T-format] Multi-turn data: %d, single-turn data: %d\r\n\t%s\r\n", enc_abs.multiTurn, enc_abs.singleTurn, T_Format_GetStatusFlag(status));
    }
    status = T_Format_Get_Temperature(&encoder, &temp);
    if (status == kStatus_Success)
    {
        PRINTF("[T-format] Temperature: %d\r\n", temp);
    }

    abs_save  = enc_abs;
    temp_save = temp;
    while (1)
    {
        /* Delay at least 90 ms. */
        SDK_DelayAtLeastUs(90000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        time++;
	status = T_Format_Readout_ABS_ABM(&encoder, &enc_abs);
        SDK_DelayAtLeastUs(10000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        if (status == kStatus_FLEXIO_T_FORMAT_FrameErr)
        {
            PRINTF("[%.2fs] The frame of getting ABS and ABM is error!\r\n", time/10.0);
        }
	T_Format_Get_Temperature(&encoder, &temp);
        if (/*(abs((int)enc_abs.multiTurn - (int)abs_save.multiTurn) > 10) || */(abs(enc_abs.singleTurn - abs_save.singleTurn) > 500) ||
            (abs(temp - temp_save) > 1)) {
            PRINTF("[%.2fs] Encoder ID: 0x%X\n\r\t Multi-turn data: %d, single-turn data: %d\r\n\t Temperature: %d\r\n", time/10.0, enc_id, enc_abs.multiTurn, enc_abs.singleTurn, temp);
            abs_save = enc_abs;
            temp_save = temp;
        }
    }
#endif
}
