/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_xrdc.h"
#include "fsl_debug_console.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_pit.h"
/*${header:end}*/

/*${function:start}*/

/* The memory for MRC test, here use the DTCM Backdoor. */
#define TEST_MEM_REGION_START       0x21000000U
#define TEST_MEM_REGION_END         0x2101FFFFU
#define TEST_MEM_REGION_MRC         kXRDC_MemMrc1_1
#define TEST_MEM_REGION_CONTROLLER  kXRDC_MemController1

/* Peripheral */
#define TEST_PERIH                  kXRDC_PeriphPit0
#define TEST_PERIH_CONTROLLER       kXRDC_PeriphController0

#define APP_DOMAIN_ID 1

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

void APP_SetXrdcConfig(void)
{
    uint32_t i;
    xrdc_periph_access_config_t periConfig;
    xrdc_processor_domain_assignment_t domainConfig;
    xrdc_mem_access_config_t memConfig;

    const xrdc_periph_t periphAccessible[] = {kXRDC_PeriphLpuart5};

    XRDC_Init(XRDC);
    XRDC_SetGlobalValid(XRDC, false);

    /* Configure the domain ID for current core. */
    XRDC_GetDefaultProcessorDomainAssignment(&domainConfig);
    domainConfig.domainId = APP_DOMAIN_ID;
    /*
     * Current core might have many masters to access the memory and peripherals.
     * In the example, all masters are set with the same domain configuration.
     */
    XRDC_SetProcessorDomainAssignment(XRDC, kXRDC_MasterCM7, 0, &domainConfig);

    /*
     * Configure the peripheral policy.
     */
    XRDC_GetPeriphAccessDefaultConfig(&periConfig);
    periConfig.policy[APP_DOMAIN_ID] = kXRDC_AccessPolicyAll;

    for (i = 0; i < ARRAY_SIZE(periphAccessible); i++)
    {
        periConfig.periph = periphAccessible[i];
        XRDC_SetPeriphAccessConfig(XRDC, &periConfig);
    }

    /*
     * Configure the memory policy.
     */
    XRDC_GetMemAccessDefaultConfig(&memConfig);
    memConfig.policy[APP_DOMAIN_ID] = kXRDC_AccessPolicyAll;

    /* SRAM */
    memConfig.mem         = kXRDC_MemMrc1_0;
    memConfig.baseAddress = 0x20400000;
    memConfig.endAddress  = 0x205FFFFF;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);

    /* Flash memory */
    memConfig.mem         = kXRDC_MemMrc0_0;
    memConfig.baseAddress = 0x00400000;
    memConfig.endAddress  = 0x00BFFFFF;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);

    /* QSPI */
    memConfig.mem         = kXRDC_MemMrc2_0;
    memConfig.baseAddress = 0x68000000;
    memConfig.endAddress  = 0x6FFFFFFF;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);
}

void APP_CheckAndResolvePeriphAccessError(xrdc_error_t *error)
{
    /* Controlled by PAC0 */
    if (error->controller == TEST_PERIH_CONTROLLER)
    {
        PRINTF("Violent access at address: 0x%8X\r\n", error->address);

        xrdc_periph_access_config_t periConfig;
        XRDC_GetPeriphAccessDefaultConfig(&periConfig);
        periConfig.periph                = TEST_PERIH;
        periConfig.policy[APP_DOMAIN_ID] = kXRDC_AccessPolicyAll;
        XRDC_SetPeriphAccessConfig(XRDC, &periConfig);
    }
}

void APP_CheckAndResolveMemoryAccessError(xrdc_error_t *error)
{
    if (error->controller == TEST_MEM_REGION_CONTROLLER)
    {
        PRINTF("Violent access at address: 0x%8X\r\n", error->address);

        xrdc_mem_access_config_t memConfig;
        XRDC_GetMemAccessDefaultConfig(&memConfig);
        memConfig.mem                   = TEST_MEM_REGION_MRC;
        memConfig.baseAddress           = TEST_MEM_REGION_START;
        memConfig.endAddress            = TEST_MEM_REGION_END;
        memConfig.policy[APP_DOMAIN_ID] = kXRDC_AccessPolicyAll;
        XRDC_SetMemAccessConfig(XRDC, &memConfig);
    }
}

void APP_SetPeriphUnaccessible(void)
{
    xrdc_periph_access_config_t periConfig;

    XRDC_GetPeriphAccessDefaultConfig(&periConfig);
    periConfig.periph                = TEST_PERIH;
    periConfig.policy[APP_DOMAIN_ID] = kXRDC_AccessPolicyNone;
    XRDC_SetPeriphAccessConfig(XRDC, &periConfig);
}

void APP_SetMemoryUnaccessible(void)
{
    xrdc_mem_access_config_t memConfig;

    XRDC_GetMemAccessDefaultConfig(&memConfig);
    memConfig.mem                   = TEST_MEM_REGION_MRC;
    memConfig.baseAddress           = TEST_MEM_REGION_START;
    memConfig.endAddress            = TEST_MEM_REGION_END;
    memConfig.policy[APP_DOMAIN_ID] = kXRDC_AccessPolicyNone;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);
}

void APP_TouchPeriph(void)
{
    pit_config_t config;

    PIT_GetDefaultConfig(&config);

    PIT_Init(PIT_0, &config);
}

void APP_TouchMemory(void)
{
    /* Touch the memory. */
    (*(volatile uint32_t *)TEST_MEM_REGION_START) = (*(volatile uint32_t *)TEST_MEM_REGION_START);
}
/*${function:end}*/
