/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_reset.h"
/*${header:end}*/

/*${variable:start}*/
static trdc_mbc_memory_block_config_t mbcBlockConfig;
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Disable SECVIO as a reset source so access violation will not trigger reset. */
    ITRC0->OUT_SEL[4][0] = 0xAAAAAAAA;
}

void APP_SetTrdcGlobalConfig(void)
{
    TRDC_Init(TRDC);

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

    TRDC_MbcSetMemoryAccessConfig(TRDC, &memAccessConfig, 0U, EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_INDEX);

    /* 2. Set the configuration for the MBC slave memory block that is to be tested */

    (void)memset(&mbcBlockConfig, 0, sizeof(mbcBlockConfig));
    mbcBlockConfig.memoryAccessControlSelect = EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_INDEX;
    mbcBlockConfig.nseEnable                 = false;
    mbcBlockConfig.mbcIdx                    = 0U; /* Only have one MBC */
    mbcBlockConfig.domainIdx                 = 0U; /* Only have one domain */
    mbcBlockConfig.slaveMemoryIdx            = EXAMPLE_TRDC_MBC_SLAVE_INDEX;
    mbcBlockConfig.memoryBlockIdx            = EXAMPLE_TRDC_MBC_MEMORY_INDEX;

    TRDC_MbcSetMemoryBlockConfig(TRDC, &mbcBlockConfig);
}

void APP_SetMbcUnaccessible(void)
{
    /* Set to nse enable to disable access in secure mode. */
    mbcBlockConfig.nseEnable = true;
    TRDC_MbcSetMemoryBlockConfig(TRDC, &mbcBlockConfig);
}

void APP_TouchMbcMemory(void)
{
    /* Touch the memory. */
    (*(volatile uint32_t *)0x1000000);
}

void APP_ResolveMbcAccessError(void)
{
    PRINTF("Resolve access error\r\n");
    /* Set to nse disable to enable access in secure mode. */
    mbcBlockConfig.nseEnable = false;
    TRDC_MbcSetMemoryBlockConfig(TRDC, &mbcBlockConfig);
}
/*${function:end}*/
