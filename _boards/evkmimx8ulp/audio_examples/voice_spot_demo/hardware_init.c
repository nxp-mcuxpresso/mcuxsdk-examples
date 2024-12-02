/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_common.h"
#include "fsl_fusion.h"
#include "fsl_reset.h"
#include "fsl_rgpio.h"
#include "fsl_iomuxc.h"
#include "fsl_reset.h"
#include "fsl_upower.h"
#include "fsl_rtd_cmc.h"
/*${header:end}*/

/*${variable:start}*/
cgc_rtd_sys_clk_config_t g_sysClkConfigRun = {
    .divBus  = 1,                     /* Bus clock divider. */
    .divSlow = 7,                     /* Slow clock divider. */
    .src     = kCGC_RtdSysClkSrcPll0, /* PLL0 main clock source. */
    .divCore = 3,                     /* Core clock divider. */
    .locked  = 0,                     /* Register not locked. */
};

/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_BootClockRUN();

    BOARD_UpdateM33CoreFreq(&g_sysClkConfigRun);

    CLOCK_DeinitPll1();

    BOARD_InitDebugConsole();

    Fusion_Init();
    if (BOARD_IsLowPowerBootType() != true) /* not low power boot type */
    {
        BOARD_HandshakeWithUboot(); /* Must handshake with uboot, unless will get issues(such as: SoC reset all the
                                       time) */
    }
    else                            /* low power boot type */
    {
        BOARD_SetTrdcGlobalConfig();
    }

    CLOCK_SetIpSrc(kCLOCK_Micfil, kCLOCK_FusionMicfilClkSrcFro24);
    RESET_PeripheralReset(kRESET_Micfil);

    UPOWER_PowerOnMemPart(0U, (uint32_t)kUPOWER_MP1_DMA0);
    UPOWER_SetRtdUseDdr(false);

    CLOCK_EnableClock(kCLOCK_Dma0Ch0);
}

void voice_spot_demo_idle_callback(void)
{
    RTDCMC_SetPowerModeProtection(CMC_RTD, kRTDCMC_AllowAllLowPowerModes);
    RTDCMC_EnterLowPowerMode(CMC_RTD, kRTDCMC_SleepMode);
}

/*${function:end}*/
