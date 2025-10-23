/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "fsl_power.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{   
    /* Release peripheral reset */
    RESET_ReleasePeripheralReset(kOSTIMER0_RST_SHIFT_RSTn);

    /* Enable clock */
    CLOCK_EnableClock(kCLOCK_GateOSTIMER0);

    BOARD_InitBootClocks();
    BOARD_InitBootPins();
    BOARD_InitDebugConsole();
}

#if defined(__ICCARM__) /* IAR Workbench */
  #pragma location = "rpmsg_sh_mem_section"
  power_handle_t powerHandle;
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION) /* Keil MDK */
power_handle_t powerHandle __attribute__((section("rpmsg_sh_mem_section")));
#elif defined(__GNUC__)
power_handle_t powerHandle __attribute__((section(".noinit.$rpmsg_sh_mem")));
#else
#error "RPMsg: Please provide your definition of rpmsg_lite_base[]!"
#endif

/* Enter deep sleep mode. */
void EXAMPLE_EnterDeepSleep()
{
    while (OSTIMER0->OSEVENT_CTRL & OSTIMER_OSEVENT_CTRL_MATCH_WR_RDY_MASK);
    power_drv_config_t powerDrvConfig = {
        .muChannelId = 0U,
        .noSyncCM0P = true,
    };
    (void)Power_CreateHandle(&powerHandle, &powerDrvConfig);
    
    power_ds_config_t dsConfig;

    Power_EnterDeepSleep(&dsConfig);
}
/* Enable OSTIMER IRQ under deep mode */
void EXAMPLE_EnableDeepSleepIRQ(void)
{
    EnableIRQ(EXAMPLE_OSTIMER_IRQn);
}
/*${function:end}*/