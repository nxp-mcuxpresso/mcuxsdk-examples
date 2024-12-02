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
#include "fsl_reset.h"
#include "fsl_trdc.h"
/*${header:end}*/

/*${variable:start}*/
static trdc_mbc_memory_block_config_t mbcBlockConfig;
/*${variable:end}*/

/*${function:start}*/
void APP_SetTrdcAccessible(void)
{
    /* Configure in GLIKEY to make the TRDC accessiable. */
    *(volatile uint32_t *)0x40091D00 = 0x00060000;
    *(volatile uint32_t *)0x40091D00 = 0x0002000F;
    *(volatile uint32_t *)0x40091D00 = 0x0001000F;
    *(volatile uint32_t *)0x40091D04 = 0x00290000;
    *(volatile uint32_t *)0x40091D00 = 0x0002000F;
    *(volatile uint32_t *)0x40091D04 = 0x00280000;
    *(volatile uint32_t *)0x40091D00 = 0x0000000F;
}

void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    APP_SetTrdcAccessible();
}

void APP_SetTrdcGlobalConfig(void)
{
    TRDC_Init(EXAMPLE_TRDC_INSTANCE);

    /* Make the all flash region accessiable. */
    *(volatile uint32_t *)0x4008E020 = 0x00007777;
    *(volatile uint32_t *)0x4008E040 = 0x00000000;
    *(volatile uint32_t *)0x4008E044 = 0x00000000;
    *(volatile uint32_t *)0x4008E048 = 0x00000000;
    *(volatile uint32_t *)0x4008E04C = 0x00000000;
    *(volatile uint32_t *)0x4008E050 = 0x00000000;
    *(volatile uint32_t *)0x4008E054 = 0x00000000;
    *(volatile uint32_t *)0x4008E058 = 0x00000000;
    *(volatile uint32_t *)0x4008E05C = 0x00000000;

    /* 1. Set control policies for MBC access control configuration registers */
    trdc_memory_access_control_config_t memAccessConfig;
    (void)memset(&memAccessConfig, 0, sizeof(memAccessConfig));

    memAccessConfig.nonsecureUsrX  = 1U;
    memAccessConfig.nonsecureUsrW  = 1U;
    memAccessConfig.nonsecureUsrR  = 1U;
    memAccessConfig.nonsecurePrivX = 1U;
    memAccessConfig.nonsecurePrivW = 1U;
    memAccessConfig.nonsecurePrivR = 1U;
    memAccessConfig.secureUsrX     = 1U;
    memAccessConfig.secureUsrW     = 1U;
    memAccessConfig.secureUsrR     = 1U;
    memAccessConfig.securePrivX    = 1U;
    memAccessConfig.securePrivW    = 1U;
    memAccessConfig.securePrivR    = 1U;

    TRDC_MbcSetMemoryAccessConfig(EXAMPLE_TRDC_INSTANCE, &memAccessConfig, 0U, EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_INDEX);

    memAccessConfig.securePrivX    = 0U;
    memAccessConfig.securePrivW    = 0U;
    memAccessConfig.securePrivR    = 0U;
    memAccessConfig.nonsecurePrivX = 0U;
    memAccessConfig.nonsecurePrivW = 0U;
    memAccessConfig.nonsecurePrivR = 0U;
    memAccessConfig.secureUsrX     = 0U;
    memAccessConfig.secureUsrW     = 0U;
    memAccessConfig.secureUsrR     = 0U;
    memAccessConfig.nonsecureUsrX  = 0U;
    memAccessConfig.nonsecureUsrW  = 0U;
    memAccessConfig.nonsecureUsrR  = 0U;

    TRDC_MbcSetMemoryAccessConfig(EXAMPLE_TRDC_INSTANCE, &memAccessConfig, 0U, EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_INDEX_NO_ACCESS);

    /* 2. Set the configuration for the MBC slave memory block that is to be tested */

    (void)memset(&mbcBlockConfig, 0, sizeof(mbcBlockConfig));
    mbcBlockConfig.memoryAccessControlSelect = EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_INDEX;
    mbcBlockConfig.nseEnable                 = false;
    mbcBlockConfig.mbcIdx                    = 0U; /* Only have one MBC */
    mbcBlockConfig.domainIdx                 = 0U; /* Only have one domain */
    mbcBlockConfig.slaveMemoryIdx            = EXAMPLE_TRDC_MBC_SLAVE_INDEX;
    mbcBlockConfig.memoryBlockIdx            = EXAMPLE_TRDC_MBC_MEMORY_INDEX;

    TRDC_MbcSetMemoryBlockConfig(EXAMPLE_TRDC_INSTANCE, &mbcBlockConfig);
}

void APP_SetMbcUnaccessible(void)
{
    /* Use policy that can't access the memory region. */
    mbcBlockConfig.memoryAccessControlSelect = EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_INDEX_NO_ACCESS;
    TRDC_MbcSetMemoryBlockConfig(EXAMPLE_TRDC_INSTANCE, &mbcBlockConfig);
}

void APP_TouchMbcMemory(void)
{
    /* Touch the memory. */
    (*(volatile uint32_t *)(8192U * EXAMPLE_TRDC_MBC_MEMORY_INDEX));
}

void APP_ResolveMbcAccessError(void)
{
    PRINTF("Resolve access error\r\n");
    /* Use policy that can access the memory region. */
    mbcBlockConfig.memoryAccessControlSelect = EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_INDEX;
    TRDC_MbcSetMemoryBlockConfig(EXAMPLE_TRDC_INSTANCE, &mbcBlockConfig);
}
/*${function:end}*/
