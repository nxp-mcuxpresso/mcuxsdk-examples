/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_xrdc.h"
/*${header:end}*/

/*${function:start}*/
void APP_InitDomain(void)
{
    /*
     * This function assigns core 0 (CM4) to domain 0, assigns core 1 (CM0+)
     * to domain 1. Then sets the necessary memory and peripheral permission.
     */

    uint32_t i;
    xrdc_periph_access_config_t periConfig;
    xrdc_processor_domain_assignment_t domainConfig;
    xrdc_mem_access_config_t memConfig;

    const xrdc_periph_t periphAccessible[] = {
        kXRDC_PeriphLpuart0, kXRDC_PeriphWdog0,   kXRDC_PeriphXrdcMgr, kXRDC_PeriphXrdcMdac, kXRDC_PeriphXrdcPac,
        kXRDC_PeriphXrdcMrc, kXRDC_PeriphSema420, kXRDC_PeriphSema421, kXRDC_PeriphWdog1,    kXRDC_PeriphPcc0,
        kXRDC_PeriphPcc1,    kXRDC_PeriphMua,     kXRDC_PeriphMub};

    XRDC_Init(XRDC);
    XRDC_SetGlobalValid(XRDC, false);

    /* Assign CM4 to domain 0. */
    XRDC_GetDefaultProcessorDomainAssignment(&domainConfig);
    domainConfig.domainId = 0;
    XRDC_SetProcessorDomainAssignment(XRDC, kXRDC_MasterCM4CodeBus, 0, &domainConfig);
    XRDC_SetProcessorDomainAssignment(XRDC, kXRDC_MasterCM4SystemBus, 0, &domainConfig);

    /* Assign CM0+ to domain 1. */
    domainConfig.domainId = 1;
    XRDC_SetProcessorDomainAssignment(XRDC, kXRDC_MasterCM0P, 0, &domainConfig);

    /*
     * Configure the peripheral policy.
     */
    XRDC_GetPeriphAccessDefaultConfig(&periConfig);
    /* Access permission for domain 0. */
    periConfig.policy[0] = kXRDC_AccessPolicyAll;
    /* Access permission for domain 1. */
    periConfig.policy[1] = kXRDC_AccessPolicyAll;

    for (i = 0; i < ARRAY_SIZE(periphAccessible); i++)
    {
        periConfig.periph = periphAccessible[i];
        XRDC_SetPeriphAccessConfig(XRDC, &periConfig);
    }

    /* Configure the memory policy. */
    XRDC_GetMemAccessDefaultConfig(&memConfig);
    /* CM4 flash is CM4 code region. */
    memConfig.mem         = kXRDC_MemMrc0_0;
    memConfig.baseAddress = 0x00000000U;
    memConfig.endAddress  = 0x000FFFFFU;
    memConfig.codeRegion  = kXRDC_MemCodeRegion1;
    memConfig.policy[0]   = kXRDC_AccessFlagsAlt4;
    memConfig.policy[1]   = kXRDC_AccessFlagsNone;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);

    /* CM0+ flash is CM0+ code region. */
    memConfig.mem         = kXRDC_MemMrc1_0;
    memConfig.baseAddress = 0x01000000U;
    memConfig.endAddress  = 0x0103FFFFU;
    memConfig.codeRegion  = kXRDC_MemCodeRegion1;
    memConfig.policy[0]   = kXRDC_AccessFlagsNone;
    memConfig.policy[1]   = kXRDC_AccessFlagsAlt4;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);

    /* CM4 ITCM SRAM, code might be placed here. */
    memConfig.mem         = kXRDC_MemMrc0_1;
    memConfig.baseAddress = 0x08000000U;
    memConfig.endAddress  = 0x0800FFFFU;
    memConfig.codeRegion  = kXRDC_MemCodeRegion1;
    memConfig.policy[0]   = kXRDC_AccessFlagsAlt4;
    memConfig.policy[1]   = kXRDC_AccessFlagsNone;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);

    /* Boot ROM. */
    memConfig.mem         = kXRDC_MemMrc0_2;
    memConfig.baseAddress = 0x08800000U;
    memConfig.endAddress  = 0x0880BFFFU;
    memConfig.codeRegion  = kXRDC_MemCodeRegion1;
    memConfig.policy[0]   = kXRDC_AccessFlagsAlt4;
    memConfig.policy[1]   = kXRDC_AccessFlagsAlt4;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);

    /* CM0+ TCM SRAM. */
    memConfig.mem         = kXRDC_MemMrc1_1;
    memConfig.baseAddress = 0x09000000U;
    memConfig.endAddress  = 0x0901FFFFU;
    memConfig.codeRegion  = kXRDC_MemCodeRegion1;
    memConfig.policy[0]   = kXRDC_AccessFlagsNone;
    memConfig.policy[1]   = kXRDC_AccessFlagsAlt4;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);

    /* CM0+ CTI, for debugger. */
    memConfig.mem         = kXRDC_MemMrc1_2;
    memConfig.baseAddress = 0xF0006000U;
    memConfig.endAddress  = 0xF0006FFFU;
    memConfig.codeRegion  = kXRDC_MemCodeRegion0;
    memConfig.policy[0]   = kXRDC_AccessFlagsNone;
    memConfig.policy[1]   = kXRDC_AccessFlagsAlt4;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);

    /* CM4 DTCM SRAM, data region. */
    memConfig.mem         = kXRDC_MemMrc0_3;
    memConfig.baseAddress = 0x20000000U;
    memConfig.endAddress  = 0x2001FFFFU;
    memConfig.codeRegion  = kXRDC_MemCodeRegion0;
    memConfig.policy[0]   = kXRDC_AccessFlagsAlt4;
    memConfig.policy[1]   = kXRDC_AccessFlagsNone;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);

    memConfig.mem         = kXRDC_MemMrc0_4;
    memConfig.baseAddress = 0x20020000U;
    memConfig.endAddress  = 0x2003FFFFU;
    memConfig.codeRegion  = kXRDC_MemCodeRegion0;
    memConfig.policy[0]   = kXRDC_AccessFlagsAlt4;
    memConfig.policy[1]   = kXRDC_AccessFlagsNone;
    XRDC_SetMemAccessConfig(XRDC, &memConfig);

    XRDC_SetGlobalValid(XRDC, true);
}

void APP_DeinitDomain(void)
{
    XRDC_SetGlobalValid(XRDC, false);
}

static void BOARD_InitLedPin(void)
{
    const gpio_pin_config_t config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 1,
    };

    GPIO_PinInit(BOARD_LED1_GPIO, BOARD_LED1_GPIO_PIN, &config);
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitLedPin();
}
/*${function:end}*/
