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
#include "fsl_rdc.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    sdma_config_t sdmaConfig;

    /* Only configure the RDC if RDC peripheral write access is allowed. */
    if ((0x1U & RDC_GetPeriphAccessPolicy(RDC, kRDC_Periph_RDC, RDC_GetCurrentMasterDomainId(RDC))) != 0U)
    {
        /*set SDMA1 PERIPH to M7 Domain(DID=1),due to UART not be accessible by DID=0 by default*/
        rdc_domain_assignment_t assignment = {0};
        assignment.domainId                = BOARD_DOMAIN_ID;
        RDC_SetMasterDomainAssignment(RDC, kRDC_Master_SDMA1_PERIPH, &assignment);
    }

    /* M7 has its local cache and enabled by default,
     * need to set smart subsystems (0x28000000 ~ 0x3FFFFFFF)
     * non-cacheable before accessing this address region */
    BOARD_InitMemory();

    /* Board specific RDC settings */
    BOARD_RdcInit();

    BOARD_InitBootPins();
    BOARD_BootClockRUN();

    /* Init the SDMA module */
    SDMA_GetDefaultConfig(&sdmaConfig);
    SDMA_Init(EXAMPLE_UART_DMA_BASEADDR, &sdmaConfig);
}

uint32_t UART4_GetFreq(void)
{
    return BOARD_DEBUG_UART_CLK_FREQ;
}
/*${function:end}*/
