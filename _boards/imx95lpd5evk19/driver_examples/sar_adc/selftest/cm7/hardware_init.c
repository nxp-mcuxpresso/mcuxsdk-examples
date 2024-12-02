/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "board.h"
#include "sm_platform.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    uint32_t channel = SM_PLATFORM_A2P;
    uint32_t ctrlId = 6U;
    uint32_t numVal = 1U;
    uint32_t val = 0x80U;
    
    SM_Platform_Init();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* ADC receives a bandgap reference voltage from BBSM domain, this is used as the ADC self-test reference voltage.
     */
    if (SCMI_ERR_SUCCESS != SCMI_MiscControlSet(channel, ctrlId, numVal, &val))
    {
        PRINTF("\r\n Enable adc self test reference voltage failed.");
        assert(false);
    }
} 
/*${function:end}*/
