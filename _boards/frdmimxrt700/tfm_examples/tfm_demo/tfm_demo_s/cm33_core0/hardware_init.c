/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_cache.h"
#include "board.h"
#include "app.h"


/*${header:end}*/
/*${function:start}*/

void BOARD_InitHardware(void)
{

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    
    GlikeyWriteEnable(GLIKEY3, 1U);                                    /* Enable SYSCON0_SEC_CLK_CTRL write */
    SYSCON0->SEC_CLK_CTRL |= SYSCON0_SEC_CLK_CTRL_TRNG_REFCLK_EN_MASK; /* Enable TRNG REF CLOCK */

    CLOCK_AttachClk(kFRO1_DIV2_to_TRNG);                               /* Clock attach FRO1 */
    CLOCK_SetClkDiv(kCLOCK_DivTrngClk, 1U);

    XCACHE_DisableCache(XCACHE0);
    XCACHE_DisableCache(XCACHE1);

    __DSB();
    __ISB();
}
/*${function:end}*/
