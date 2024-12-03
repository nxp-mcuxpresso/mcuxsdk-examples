/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

#include "fsl_dryice.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
static const uint8_t g_privateKey[] = "This is my password! Very secret"; /* 32 bytes plus 1 byte zero termination */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function.
 * @details Compute CRC checksum and print it to terminal.
 */
int main(void)
{
    DRY_Type *base = DRY;
    status_t status;
    uint32_t flags;
    dryice_config_t myConfig;
    uint8_t myKey[32] = {0};

    /* Init hardware*/
    BOARD_InitHardware();

    PRINTF("DryIce Peripheral Driver Example\r\n\r\n");

    DRYICE_Init(base);

    flags  = 0;
    status = DRYICE_GetStatusFlags(base, &flags);
    if ((status == kStatus_Success) && (kDRYICE_StatusTamperFlag & flags))
    {
        /* if Tamper Acknowledge is clear, we need to issue chip reset to make it set */
        if (0 == (kDRYICE_StatusTamperAcknowledgeFlag & flags))
        {
            /* request software reset */
            SCB->AIRCR = 0x05FA0004;
            while (1)
            {
            }
        }

        /* DTF and TAF are set so we can clear */
        status = DRYICE_ClearStatusFlags(base, kDRYICE_StatusTamperFlag | kDRYICE_StatusTamperAcknowledgeFlag);
    }

    /* enable the DryIce clock (32768 Hz within DryIce) and prescaler */
    if (status == kStatus_Success)
    {
        /*
         * myConfig.innerClockAndPrescalerEnable = true;
         * myConfig.tamperForceSystemResetEnable = false;
         * myConfig.updateMode = kDRYICE_StatusLockWithTamper;
         * myConfig.clockSourceActiveTamper0 = kDRYICE_ClockType1Hz;
         * myConfig.clockSourceActiveTamper1 = kDRYICE_ClockType1Hz;
         * myConfig.tamperHysteresisSelect = kDRYICE_Hysteresis305mV;
         * myConfig.tamperPassiveFilterEnable = false;
         * myConfig.tamperDriveStrength = kDRYICE_DriveStrengthLow;
         * myConfig.tamperSlewRate = kDRYICE_SlewRateSlow;
         * myConfig.secureRegisterFile = kDRYICE_VbatRegisterResetWithTamperOrInterrupt;
         * myConfig.prescaler = 0;
         */
        DRYICE_GetDefaultConfig(base, &myConfig);
        status = DRYICE_SetConfig(base, &myConfig);
    }

    /* store private key */
    if (status == kStatus_Success)
    {
        status = DRYICE_WriteKey(base, g_privateKey, sizeof(g_privateKey) - 1u);
    }

    /* disable write access to Secure Key Registers until next chip reset */
    if (status == kStatus_Success)
    {
        PRINTF("writing Secure Key = %s\r\n", g_privateKey);

        DRYICE_DisableAccess(base, kDRYICE_NoRegister, kDRYICE_AllSecureKeys, kDRYICE_Write);

        /* force Tamper Detect Flag to be set. */
        status = DRYICE_ForceTamper(base);
    }

    /* when Tamper Detect Flag is set by DRYICE_ForceTamper(), Secure Key storage is held in reset */
    if (status == kStatus_Success)
    {
        status = DRYICE_GetKey(base, myKey, ARRAY_SIZE(myKey));
    }

    /* myKey[] is zero */
    if (status == kStatus_Success)
    {
        for (int i = 0; i < ARRAY_SIZE(myKey); i++)
        {
            if (myKey[i])
            {
                status = kStatus_Fail;
                break;
            }
        }
    }

    if (status == kStatus_Success)
    {
        PRINTF("Secure Key erased upon Tamper Detection\r\n");
    }
    else
    {
        PRINTF("An error has occurred!\r\n");
    }

    while (1)
    {
    }
}
