/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "board.h"
#include "app.h"
#include "fsl_opamp.h"
#include "fsl_debug_console.h"

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

int main(void)
{
    opamp_config_t sConfig;

    BOARD_InitHardware();

    PRINTF("OPAMP BASIC EXAMPLE.\r\n");

    /*
        psConfig->bEnableLoadCompletionInterrupt = false;
        psConfig->bEnableWriteProtection = false;
        psConfig->eLoadMode = kOPAMP_LoadModeDelayLoad;
        psConfig->ePowerMode = kOPAMP_PowerModeLowPower;
        psConfig->eConfigRegSel   = kOPAMP_ConfigRegSelCFG0;

        psConfig->sConfigSet0.eWorkMode = kOPAMP_WorkModeBufferMode;
        psConfig->sConfigSet0.eNegChannel = kOPAMP_NegChannel0;
        psConfig->sConfigSet0.ePosChannel = kOPAMP_PosChannel0;

        psConfig->sConfigSet1.eWorkMode = kOPAMP_WorkModeBufferMode;
        psConfig->sConfigSet1.eNegChannel = kOPAMP_NegChannel0;
        psConfig->sConfigSet1.ePosChannel = kOPAMP_PosChannel0;

        psConfig->sConfigSet2.eWorkMode = kOPAMP_WorkModeBufferMode;
        psConfig->sConfigSet2.eNegChannel = kOPAMP_NegChannel0;
        psConfig->sConfigSet2.ePosChannel = kOPAMP_PosChannel0;

        psConfig->sConfigSet3.eWorkMode = kOPAMP_WorkModeBufferMode;
        psConfig->sConfigSet3.eNegChannel = kOPAMP_NegChannel0;
        psConfig->sConfigSet3.ePosChannel = kOPAMP_PosChannel0;
     */
    OPAMP_GetDefaultConfig(&sConfig);

    sConfig.eConfigRegSel           = kOPAMP_ConfigRegSelCFG0;
    sConfig.eLoadMode               = kOPAMP_LoadModeImmediatelyLoad;
    sConfig.sConfigSet0.eWorkMode   = kOPAMP_WorkModeInternalGain2X;
    sConfig.sConfigSet0.ePosChannel = kOPAMP_PosChannel0;
    OPAMP_Init(DEMO_OPAMP, &sConfig);

    while (1)
    {
    }
}
