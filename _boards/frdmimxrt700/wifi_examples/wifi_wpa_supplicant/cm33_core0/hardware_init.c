/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"
#include "fsl_gpio.h"
#include "fsl_cache.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitWIFIPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    BOARD_InitAHBSC();

    /* Enable TRNG clock required by ELS PKC PSA crypto driver */
    GlikeyWriteEnable(GLIKEY3, 1U);                                     /* Enable SYSCON0_SEC_CLK_CTRL write */
    SYSCON0->SEC_CLK_CTRL |= SYSCON0_SEC_CLK_CTRL_TRNG_REFCLK_EN_MASK; /* Enable TRNG REF CLOCK */
    CLOCK_AttachClk(kFRO1_DIV2_to_TRNG);                                /* Max 96MHz with 1.0V normal supply */
    CLOCK_SetClkDiv(kCLOCK_DivTrngClk, 1U);

    /* Disable XCACHEs for ELS PKC DMA operations */
    XCACHE_DisableCache(XCACHE0);
    XCACHE_DisableCache(XCACHE1);

    __DSB();
    __ISB();
}
/*${function:end}*/
