/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "mcmgr.h"
#include "fsl_trdc.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Configure core1 secure privileged access to GPIOD peripheral
       to allow LED2_BLUE control from the secondary core application */
    TRDC_SetDacGlobalValid(TRDC);
    trdc_non_processor_domain_assignment_t domain = {
      .domainId = 0U,
      .privilegeAttr = kTRDC_ForcePrivilege,
      .secureAttr = kTRDC_ForceSecure,
      .bypassDomainId = 0U,
    };
    TRDC_SetNonProcessorDomainAssignment(TRDC, 3U, &domain);
}

/*!
 * @brief Application-specific implementation of the SystemInitHook() weak function.
 */
void SystemInitHook(void)
{
    /* Initialize MCMGR - low level multicore management library. Call this
       function as close to the reset entry as possible to allow CoreUp event
       triggering. The SystemInitHook() weak function overloading is used in this
       application. */
    (void)MCMGR_EarlyInit();
}
/*${function:end}*/
