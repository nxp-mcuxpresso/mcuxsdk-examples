/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board_fusionf1.h"
#include "fsl_clock.h"
#include "app.h"
#include "fsl_trdc.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
}

uint8_t APP_GetDSPCoreDomainID(void)
{
    return TRDC_GetCurrentMasterDomainId(TRDC);
}

/*${function:end}*/
