/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "app.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_trdc.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_InitCANPins();

    /*
     * Override M-EDMA3 Ch0/1 to Secure Privileged so it can access HSP
     * peripherals through MAIN_TRDC MBC1 (e.g. kTRDC_MAIN_MBC_HSP_FLEXCAN1).
     * BOARD_ConfigTRDC sets PA=0b00 (Force User) which is denied by the
     * Secure-Privileged GLBAC policy that ELE programs in MAIN_MBC1 DOM0,
     * causing CH0_ES[SBE] when EDMA3 reads the FlexCAN Enhanced Rx FIFO.
     * SA=0b00 (Force Secure) + PA=0b01 (Force Privileged) matches the policy.
     */
    MAIN__TRDC->MDA_DFMT1[kTRDC_MAIN_MasterMEDMA3Ch0_1].MDA_W_DFMT1[0] =
        TRDC_MDA_W_DFMT1_DID(0) | TRDC_MDA_W_DFMT1_SA(0U) | TRDC_MDA_W_DFMT1_PA(1U) | TRDC_MDA_W_DFMT1_VLD_MASK;
}
/*${function:end}*/
