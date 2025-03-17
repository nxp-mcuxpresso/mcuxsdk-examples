/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_clock.h"
#include "fsl_power.h"
#include "fsl_mu.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

 /*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/

bool APP_SecondaryCoreCallback(power_low_power_mode_t targetPowerMode, void *ptrPowerConfig, void *userData)
{
    char *const g_modeNameArray[] = APP_POWER_MODE_NAME;
    
    PRINTF("Primary Core Request to enter: %s\r\n", g_modeNameArray[(uint32_t)targetPowerMode]);
    
    (void)ptrPowerConfig;
    (void)userData;
    return true;
}


void MU_B_RX_IRQHandler(void)
{
    MU_DisableInterrupts(APP_MU, kMU_Tx3EmptyInterruptEnable);
    Power_MuMessageCallback(MU_ReceiveMsgNonBlocking(APP_MU, APP_MU_CHANNEL), APP_MU_CHANNEL);
}

int main(void)
{
    BOARD_InitHardware();
    PRINTF("\r\n###########################  Power Mode Switch Demo Secondary Core Boot  ###########################\r\n");

    while(1)
    {
    }
}
