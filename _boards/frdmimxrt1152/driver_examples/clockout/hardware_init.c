/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_clock.h"
/*${header:end}*/

/*${function:start}*/
clock_pll_ss_config_t sysPll1SsConfig_BOARD_BootClockRUN =
    {
        .stop = 32768,                            /* Stop value to get frequency change */
        .step = 1,                                /* Step value to get frequency change */
    };

const clock_sys_pll1_config_t sysPll1Config_BOARD_BootClockRUN =
    {
        .pllDiv2En = 0,                           /* Enable Sys Pll1 divide-by-2 clock or not */
        .pllDiv5En = 1,                           /* Enable Sys Pll1 divide-by-5 clock or not */
        .ss = &sysPll1SsConfig_BOARD_BootClockRUN,/* Spread spectrum parameter */
        .ssEnable = true,                         /* Enable spread spectrum or not */
    };

const clock_audio_pll_config_t audioPllConfig = {
    .loopDivider = 32,   /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
    .postDivider = 1,    /* Divider after the PLL, should only be 0, 1, 2, 3, 4, 5 */
    .numerator   = 768,  /* 30 bit numerator of fractional loop divider. */
    .denominator = 1000, /* 30 bit denominator of fractional loop divider */
};

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitCLOCKOUTPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Enable audio pll clock */
    CLOCK_InitAudioPll(&audioPllConfig);
    /* Enable syspll1 clock */
    CLOCK_InitSysPll1(&sysPll1Config_BOARD_BootClockRUN);;
      
}
/*${function:end}*/
