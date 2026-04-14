/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_edma_soc.h"
#include "fsl_xbar.h"
/*${header:end}*/

/*${function:start}*/

void BOARD_InitHardware(void)
{
    /* Init xbara module. */
    XBAR_Init(kXBAR_DSC1);

    /* Configure the XBARA signal connections: route LPTMR1 trigger to ADC1 HW trigger 0. */
    XBAR_SetSignalsConnection(kXBAR1_InputLptmr1TriggerDelay, kXBAR1_OutputAdc12HwTrig0);

    BOARD_CommonSetting();
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitADCPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}

/*${function:end}*/
