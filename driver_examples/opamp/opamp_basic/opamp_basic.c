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
#include "fsl_debug_console.h"
#include "fsl_opamp.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
void OPAMP_Configuration(void);
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
    BOARD_InitHardware();
    PRINTF("OPAMP BASIC EXAMPLE!\r\n");
    OPAMP_Configuration();
    while (1)
    {
    }
}

void OPAMP_Configuration(void)
{
    opamp_config_t config;
    /*
     *  config->enable        = false;
     *  config->enablePosADCSw1 = false;
     *  config->mode          = kOPAMP_LowNoiseMode;
     *  config->trimOption    = kOPAMP_TrimOptionDefault;
     *  config->intRefVoltage = kOPAMP_IntRefVoltVddaDiv2;
     *  config->enablePosADCSw1 = false;
     *  config->enablePosADCSw2 = false;
     *  config->posRefVoltage = kOPAMP_PosRefVoltVrefh3;
     *  config->posGain       = kOPAMP_PosGainReserved;
     *  config->negGain       = kOPAMP_NegGainBufferMode;
     *  config->enableRefBuffer = false;
     *  config->PosInputChannelSelection  = kOPAMP_PosInputChannel0
     *  config->enableTriggerMode = false;
     *  config->enableOutputSwitch = true;
     */
    OPAMP_GetDefaultConfig(&config);
    config.posGain = kOPAMP_PosGainNonInvertDisableBuffer2X;
    config.negGain = kOPAMP_NegGainInvert1X;
    config.enable  = true;
    OPAMP_Init(DEMO_OPAMP_BASEADDR, &config);
}
