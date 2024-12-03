/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_snvs.h"
#include "fsl_dryice_digital.h"
#include "app.h"

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
    snvs_config_t snvsConfig;
    dryice_digital_config_t dryiceConfig;
    dryice_digital_pin_config_t pinConfig;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    PRINTF("DryIce Digital Example:\r\n");
    PRINTF("Disconnect jumper to generate tamper event.\r\n");

    /* Initialize DryIce Digital module with default config */
    DRYICE_DIGITAL_GetDefaultConfig(DRYICE, &dryiceConfig);
    DRYICE_DIGITAL_Init(DRYICE, &dryiceConfig);

    DRYICE_DIGITAL_PinGetDefaultConfig(DRYICE, &pinConfig);

    pinConfig.pullSelect           = kDRYICE_DIGITAL_PullDown;
    pinConfig.pullEnable           = true;
    pinConfig.pullContinuousEnable = true;
    pinConfig.outputKeepEnable     = true;

    /* Set DryIce Digital tamper pin configuration */
    DRYICE_DIGITAL_SetPinConfig(DRYICE, kDRYICE_DIGITAL_TamperPin0, &pinConfig);

    /* Initialize SNVS module to be able to read tamper pin status */
    SNVS_GetDefaultConfig(&snvsConfig);
    SNVS_Init(SNVS, &snvsConfig);

    SNVS_EnableExternalTamper(SNVS, kSNVS_ExternalTamper1, kSNVS_ExternalTamperActiveLow);

    /* Wait until tamper event on external pin is detected */
    while (SNVS_GetExternalTamperStatus(SNVS, kSNVS_ExternalTamper1) == kSNVS_TamperNotDetected)
    {
    }

    PRINTF("\r\n    TAMPER DETECTED!!!\r\n");
    while (1)
    {
    }
}
