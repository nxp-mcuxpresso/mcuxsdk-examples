/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "app.h"
#include "fsl_trdc.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${variable:start}*/
uint32_t mrc_start_addr[2] = {0x4000000, 0x40000000};
uint32_t mrc_end_addr[2]   = {0x5000000, 0x50000000};
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}

void APP_SetTrdcGlobalConfig(void)
{
    /* Ungate TRDC MRC, MBC and DAC PCC */
    TRDC_Init(TRDC);

    /* 1. Get the hardware configuration of the TRDC module */
    trdc_hardware_config_t hwConfig;
    TRDC_GetHardwareConfig(TRDC, &hwConfig);

    /* 2. Set master domain ID for processor master */
    trdc_processor_domain_assignment_t pDomainAssignment;
    TRDC_GetDefaultProcessorDomainAssignment(&pDomainAssignment);
    pDomainAssignment.domainId = APP_CORTEX_M_DID;

    TRDC_SetProcessorDomainAssignment(TRDC, &pDomainAssignment);

    TRDC_SetDacGlobalValid(TRDC);
}

void APP_InitDomainConfig(void)
{
    APP_SetTrdcGlobalConfig();
}

void APP_DeinitDomainConfig(void)
{
    TRDC_Deinit(TRDC);
}
/*${function:end}*/
