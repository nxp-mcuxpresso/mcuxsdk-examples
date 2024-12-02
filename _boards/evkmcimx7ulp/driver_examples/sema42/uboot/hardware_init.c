/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "app.h"
#include "fsl_xrdc.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void APP_InitDomainConfig(void)
{
    uint32_t i;
    xrdc_periph_access_config_t periConfig;
    xrdc_mem_access_config_t memConfig;
    xrdc_processor_domain_assignment_t domainConfig;
    xrdc_non_processor_domain_assignment_t nonProcDomainConfig;

    const xrdc_periph_t periphAccessible[] = {kXRDC_PeriphLpuart0, kXRDC_PeriphLpuart4, kXRDC_PeriphLpi2c3,
                                              kXRDC_PeriphXrdc0,   kXRDC_PeriphXrdc1,   kXRDC_PeriphXrdc2,
                                              kXRDC_PeriphXrdc3,   kXRDC_PeriphSema42_0};

    XRDC_Init(XRDC);
    XRDC_SetGlobalValid(XRDC, false);

    /* Configure the domain ID for Cortex-M. */
    XRDC_GetDefaultProcessorDomainAssignment(&domainConfig);
    domainConfig.domainId = APP_CORTEX_M_DID;
    XRDC_SetProcessorDomainAssignment(XRDC, kXRDC_MasterCM4Code, 0, &domainConfig);
    XRDC_SetProcessorDomainAssignment(XRDC, kXRDC_MasterCM4System, 0, &domainConfig);

    /* Configure the domain ID for Cortex-A. */
    domainConfig.domainId = APP_CORTEX_A_DID;
    XRDC_SetProcessorDomainAssignment(XRDC, kXRDC_MasterCA7, 0, &domainConfig);

    XRDC_GetDefaultNonProcessorDomainAssignment(&nonProcDomainConfig);
    nonProcDomainConfig.domainId = APP_CORTEX_A_DID;
    XRDC_SetNonProcessorDomainAssignment(XRDC, kXRDC_MasterSdhc0, 0, &nonProcDomainConfig);
    XRDC_SetNonProcessorDomainAssignment(XRDC, kXRDC_MasterSdhc1, 0, &nonProcDomainConfig);

    /*
     * Configure the peripheral policy.
     */
    XRDC_GetPeriphAccessDefaultConfig(&periConfig);
    periConfig.policy[APP_CORTEX_A_DID] = kXRDC_AccessPolicyAll;
    periConfig.policy[APP_CORTEX_M_DID] = kXRDC_AccessPolicyAll;

    for (i = 0; i < ARRAY_SIZE(periphAccessible); i++)
    {
        periConfig.periph = periphAccessible[i];
        XRDC_SetPeriphAccessConfig(XRDC, &periConfig);
    }

    /*
     * Configure the memory policy.
     */
    XRDC_GetMemAccessDefaultConfig(&memConfig);
    memConfig.policy[APP_CORTEX_A_DID] = kXRDC_AccessPolicyAll;
    memConfig.policy[APP_CORTEX_M_DID] = kXRDC_AccessPolicyAll;
    /* SRAM0. */
    memConfig.mem         = kXRDC_MemMrc2_0;
    memConfig.baseAddress = 0x2F000000U;
    memConfig.size        = kXRDC_MemSize128K;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);

    /* SRAM1. */
    memConfig.mem         = kXRDC_MemMrc5_0;
    memConfig.baseAddress = 0x2F020000U;
    memConfig.size        = kXRDC_MemSize128K;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);

    /* QSPI flash. */
    memConfig.mem         = kXRDC_MemMrc1_0;
    memConfig.baseAddress = 0xC0000000U;
    memConfig.size        = kXRDC_MemSize256M;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);
    memConfig.mem         = kXRDC_MemMrc1_1;
    memConfig.baseAddress = 0x04000000U;
    memConfig.size        = kXRDC_MemSize64M;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);
    memConfig.mem         = kXRDC_MemMrc1_2;
    memConfig.baseAddress = 0x08000000U;
    memConfig.size        = kXRDC_MemSize64M;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);

    /* MMDC. */
    memConfig.mem         = kXRDC_MemMrc6_0;
    memConfig.baseAddress = 0x60000000U;
    memConfig.size        = kXRDC_MemSize512M;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);
    memConfig.mem         = kXRDC_MemMrc6_1;
    memConfig.baseAddress = 0x80000000U;
    memConfig.size        = kXRDC_MemSize1G;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);

    XRDC_SetGlobalValid(XRDC, true);
}

void APP_DeinitDomainConfig(void)
{
    XRDC_SetGlobalValid(XRDC, false);
}

void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    CLOCK_SetIpSrc(kCLOCK_Lpuart0, kCLOCK_IpSrcSysOscAsync);
    BOARD_InitDebugConsole();
}
/*${function:end}*/
