/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_sfa.h"
#include "app.h"
/*${header:end}*/

/*!
 * Porting Guide.
 *
 * To port SFA driver example to other platforms, please recode APP_SetupFastClock() function
 * and APP_SetupSlowClock() function, for these two functions are platform-related.
 *
 * The APP_SetupFastClock() function is used to select the clock which is faster than the SFA reference clock and sets
 * the SFA configuration. In this scenario, it is recommended to select the measure mode as "kSFA_FrequencyMeasurement1"
 * and set cutTarget as "0xFFFFFFFFUL" to avoid CUT counter timeout. 'enableCUTPin' can be set as "True", to monitor the
 * clock signal via SFA_CLK pin.
 *
 * The APP_SetupSlowClock() function is used to select the clock which is slower than the SFA reference clock and sets
 * the SFA configuration. In this scenario, it is recommended to select the measure mode as
 * "kSFA_FrequencyMeasurement0", for the frequency of CUT is slower than the reference clock. 'enableCUTPin' can be set
 * as "True", to monitor the clock signal via SFA_CLK pin.
 */

/*${function:start}*/

/*!
 * @brief Set up the fast clock to be measured.
 */
void APP_SetupFastClock(sfa_config_t *configPtr)
{
    scg_sys_clk_config_t clkConfig;

    CLOCK_GetCurSysClkConfig(&clkConfig);

    switch ((scg_sys_clk_src_t)clkConfig.src)
    {
        case kSCG_SysClkSrcSysOsc:
            CLOCK_SetClkOutSel(kClockClkoutSelSosc);
            break;
        case kSCG_SysClkSrcSirc:
            CLOCK_SetClkOutSel(kClockClkoutSelSirc);
            break;
        case kSCG_SysClkSrcFirc:
            CLOCK_SetClkOutSel(kClockClkoutSelFirc);
            break;
        case kSCG_SysClkSrcRosc:
            CLOCK_SetClkOutSel(kClockClkoutSelScgRtcOsc);
            break;
        default:
            assert(false);
            break;
    }

    configPtr->mode         = kSFA_FrequencyMeasurement1;
    configPtr->cutSelect    = DEMO_CUT_SCG_CLK_OUT_SELECT;
    configPtr->refTarget    = 0x0000FFFFUL;
    configPtr->cutTarget    = 0xFFFFFFFFUL;
    configPtr->prediv       = 2U;
    configPtr->enableCUTPin = true;
}

/*!
 * @brief Set up the slow clock to be measured.
 */
void APP_SetupSlowClock(sfa_config_t *configPtr)
{
    configPtr->mode         = kSFA_FrequencyMeasurement0;
    configPtr->cutSelect    = DEMO_CUT_32KHZ_SELECT;
    configPtr->refTarget    = 0xFFFFFFUL;
    configPtr->cutTarget    = 0xFFUL;
    configPtr->enableCUTPin = true;
}

void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
