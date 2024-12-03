/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_vref.h"
#include "fsl_power.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    vref_config_t vrefConfig;

    BOARD_InitPins();
    BOARD_BootClockPLL150M();
    BOARD_InitDebugConsole();

    /* Disable VREF power down */
    POWER_DisablePD(kPDRUNCFG_PD_VREF);
    POWER_DisablePD(kPDRUNCFG_PD_CMPBIAS);
    POWER_DisablePD(kPDRUNCFG_PD_HSCMP0);
    POWER_DisablePD(kPDRUNCFG_PD_HSCMP0_DAC);

    VREF_GetDefaultConfig(&vrefConfig);
    /* Initialize the VREF mode. */
    VREF_Init(DEMO_VREF_BASE, &vrefConfig);
    /* Get a 1.8V reference voltage. */
    VREF_SetTrim21Val(DEMO_VREF_BASE, 8U);
}
/*${function:end}*/
