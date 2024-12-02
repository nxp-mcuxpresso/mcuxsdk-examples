/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
#include "fsl_edma.h"
#include "app.h"
#include "fsl_flexspi.h"
#include "fsl_reset.h"
#include "fsl_upower.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_InitPsRamPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    status_t status = BOARD_InitPsRam();
    if (status != kStatus_Success)
    {
        assert(false);
    }

    UPOWER_PowerOnMemPart(0U, (uint32_t)kUPOWER_MP1_DMA0);
    CLOCK_EnableClock(EXAMPLE_TX_DMA_CHANNEL_CLOCK);
    CLOCK_EnableClock(EXAMPLE_RX_DMA_CHANNEL_CLOCK);
    if (BOARD_IsLowPowerBootType() != true) /* not low power boot type */
    {
        BOARD_HandshakeWithUboot(); /* Must handshake with uboot, unless will get issues(such as: SoC reset all the
                                       time) */
        /* Delay 1s wait uboot complete cache flush */
        SDK_DelayAtLeastUs(1000 * 1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    }
    else /* low power boot type */
    {
        BOARD_SetTrdcGlobalConfig();
    }
}

/*${function:end}*/
