/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    clock_audio_pll_config_t audioPllConfig = {0};

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Init audio pll */
    audioPllConfig.loopDivider = 30;
    audioPllConfig.postDivider = 2;
    audioPllConfig.denominator = 100;
    audioPllConfig.numerator   = 72;
    CLOCK_InitAudioPll(&audioPllConfig);

    /* Set spdif clock root source */
    CLOCK_SetMux(kCLOCK_SpdifMux, 0);
    /* Set spdif pre-div */
    CLOCK_SetDiv(kCLOCK_Spdif0PreDiv, 0);
    CLOCK_SetDiv(kCLOCK_Spdif0Div, 0);
}
/*${function:end}*/
