/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include <stdlib.h>

#include "fsl_debug_console.h"
#include "fsl_dac.h"
#include "fsl_power.h"

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

/*!
 * @brief Main function
 */
int main(void)
{
    dac_config_t config;
    BOARD_InitHardware();

    POWER_DisablePD(kPDRUNCFG_PD_DAC);

    PRINTF("\r\nDAC sine wave example.\r\n");

    /*Sin wave*/
    DAC_GetDefaultConfig(&config);
    config.sin_cfg.amp       = 0xffff;
    config.sin_cfg.dc_offset = 1;
    config.sin_cfg.freq      = 0xccc;
    config.sin_cfg.en        = kDAC_SinWaveEnable;
    config.sign_inv          = 1;
    config.output            = kDAC_BufferOutAlignLeft;
    config.trg_cfg.mode      = kDAC_TriggerModeContinueMode;
    DAC_Init(DEMO_DAC_BASE, &config);

    DAC_Enable(DEMO_DAC_BASE, true);

    while (1)
    {
    }
}
