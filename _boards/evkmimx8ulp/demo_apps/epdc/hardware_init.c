/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "fsl_reset.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_BootClockRUN();
    BOARD_InitDebugConsolePins();
    BOARD_InitDebugConsole();

    /* Enable GPIOA clock for panel front light power on. */
    CLOCK_EnableClock(kCLOCK_RgpioA);

    /* PCA6416A I2C. */
    CLOCK_SetIpSrc(kCLOCK_Lpi2c0, kCLOCK_Pcc1BusIpSrcSysOscDiv2);
    RESET_PeripheralReset(kRESET_Lpi2c0);
    BOARD_InitPCA6416A(&g_pca6416aHandle);

    /* EPDC I2C. */
    CLOCK_SetIpSrc(kCLOCK_Lpi2c1, kCLOCK_Pcc1BusIpSrcSysOscDiv2);
    RESET_PeripheralReset(kRESET_Lpi2c1);

    if (BOARD_IsLowPowerBootType() != true) /* not low power boot type */
    {
        BOARD_HandshakeWithUboot();
    }
    else /* low power boot type */
    {
        PRINTF("Pls run the demo with A Core(the demo depend on resource of A Core)\r\n");
        assert(false);
    }

    SDK_DelayAtLeastUs(4000000U,
                       SystemCoreClock); /* wait 4 seconds to make sure reset display components(EPDC, PXP) by mcore */

    /*
     * TODO: Allocate PXP, EPDC to real time domain, otherwise interrupt does not happen.
     * Need to add API in SIM module for this operation.
     */
    SIM_SEC->SYSCTRL0 &= ~SIM_SEC_SYSCTRL0_LPAV_MASTER_CTRL(1); /* Allocate LPAV to RTD */
    SIM_SEC->LPAV_MASTER_ALLOC_CTRL =
        0; /* Allocate all of masters(HIFI4, EPDC, MIPI DSI, DCNANO, GPU3D, GPU2D, PXP) of LPAV to RTD */

    BOARD_InitBootPins();
}
/*${function:end}*/
