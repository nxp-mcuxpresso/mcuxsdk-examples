/*
 * Copyright 2025 NXP
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
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    
    //BOARD_InitXSPI1Pins();
    CLOCK_AttachClk(kMAIN_PLL_PFD2_to_XSPI1);
    CLOCK_SetClkDiv(kCLOCK_DivXspi1Clk, 1u);     /*400MHz*/ 

//    BOARD_InitAHBSC();
    GlikeyWriteEnable(GLIKEY3, 1U);                                    /* Enable SYSCON0_SEC_CLK_CTRL write */
    SYSCON0->SEC_CLK_CTRL |= SYSCON0_SEC_CLK_CTRL_TRNG_REFCLK_EN_MASK; /* Enable TRNG REF CLOCK */

    CLOCK_AttachClk(kFRO1_DIV2_to_TRNG);                               /* Max 96MHZ with 1.0V nomral supply. */
    CLOCK_SetClkDiv(kCLOCK_DivTrngClk, 1U);

    XCACHE_DisableCache(XCACHE0);
    XCACHE_DisableCache(XCACHE1);

    __DSB();
    __ISB();
}
/*${function:end}*/
