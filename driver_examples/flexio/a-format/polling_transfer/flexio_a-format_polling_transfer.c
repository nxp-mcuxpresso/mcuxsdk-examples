/*
 * Copyright 2024 NXP
 * All rights reserved.
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

#define MODE_POLL  0
#define MODE_INT   1
#define MODE       MODE_POLL

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
encoder_A_format encoder;
status_t status;
static bool cmdFlag = false;

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
    encoder_abs_multi_single_t enc_abs[4], abs_save;
#if MODE == MODE_POLL
    uint32_t enc_id = 1;
    float temp, temp_save;
    uint64_t time = 0;
    encoder_status_t statusData;
    encoder_eeprom_t eeprom = {
        .address = 0x01,
        .data = 0x5A2B
    };
#endif

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

#if (MODE == MODE_INT)
    FLEXIO_A_Format_TransferCreateHandle(&encDev, &g_aformatHandle, FLEXIO_A_Format_UserCallback, NULL);
    A_Format_ABS_Readout_Multi_Single_IRQ(&encoder, ENCODER_ADDRESS_IT(0x03), &enc_abs);
    while (1)
    {
        if (cmdFlag)
        {
            PRINTF("Multi-turn data: %d, single-turn data: %ld\r\n", enc_abs.multiTurn, enc_abs.singleTurn);
            cmdFlag = false;
        }
        SDK_DelayAtLeastUs(3000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        A_Format_ABS_Readout_Multi_Single_IRQ(&encoder, ENCODER_ADDRESS_IT(0x03), &enc_abs);
    }
#endif

#if (MODE == MODE_POLL)
    A_Format_Memory_Write(&encoder, ENCODER_ADDRESS_IT(0x03), &eeprom);
    memset(&eeprom.data, 0, 2);
    SDK_DelayAtLeastUs(30000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);//30ms
    A_Format_Memory_Read(&encoder, ENCODER_ADDRESS_IT(0x03), &eeprom);
    PRINTF("EEPROM Addr: %d, EEPROM Data: 0x%X\r\n", eeprom.address, eeprom.data);
    
    A_Format_Readout_Encoder_status(&encoder, ENCODER_ADDRESS_IT(0x03), &statusData);
    A_Format_Set_ID(&encoder, ENCODER_ADDRESS_IT(0x03), 0x005ABCDE);
    status = A_Format_Get_ID(&encoder, ENCODER_ADDRESS_IT(0x03), &enc_id);
    if (status != kStatus_Success)
    {
    }
    PRINTF("Encoder ID: %d\r\n", enc_id);

    status = A_Format_ABS_Readout_Multi_Single(&encoder, ENCODER_ADDRESS_IT(0x03), &enc_abs[3]);
//    status = A_Format_Set_Encoder_Address_1to1(&encoder, ENCODER_ADDRESS_IT(0x03));
    if (status != kStatus_Success)
    {
//        DiagnosisInfoOutput(status);
//        return status;
    }
    PRINTF("Multi-turn data: %d, single-turn data: %ld\r\n", enc_abs[3].multiTurn, enc_abs[3].singleTurn);

    abs_save  = enc_abs[3];
    temp_save = temp;
    while (1)
    {
        /* Delay at least 90 ms. */
        SDK_DelayAtLeastUs(90000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        time++;
        A_Format_ABS_Readout_Multi_Single(&encoder, ENCODER_ADDRESS_IT(0x03), &enc_abs[3]);
        SDK_DelayAtLeastUs(10000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        A_Format_Get_Temperature(&encoder, ENCODER_ADDRESS_IT(0x03), &temp);
        if ((abs((int)enc_abs[3].multiTurn - (int)abs_save.multiTurn) > 10) || //(fabs(enc_abs.singleTurn - abs_save.singleTurn) > 10) ||
            (fabs(temp - temp_save) > 0.1)) {
            PRINTF("[%.2fs] Encoder ID: 0x%X\n\r\t Multi-turn data: %d, single-turn data: %ld\r\n\t Temperature: %f\r\n", time/10.0, enc_id, enc_abs[3].multiTurn, enc_abs[3].singleTurn, temp);
            abs_save = enc_abs[3];
            temp_save = temp;
        }
    }
#endif
}
