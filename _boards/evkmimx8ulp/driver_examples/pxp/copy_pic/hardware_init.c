/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_reset.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
#include "fsl_upower.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_BootClockRUN();
    BOARD_InitBootPins();
    BOARD_InitDebugConsole();

    if (BOARD_IsLowPowerBootType() != true) /* not low power boot type */
    {
        BOARD_HandshakeWithUboot();
    }
    else /* low power boot type */
    {
        PRINTF("Pls run the demo with A Core(the demo depend on resource of A Core)\r\n");
        assert(false);
    }

    /* MIPI panel GPIO. */
    CLOCK_EnableClock(kCLOCK_RgpioC);
    CLOCK_EnableClock(kCLOCK_RgpioA);

    /* PCA6416A I2C. */
    CLOCK_SetIpSrc(kCLOCK_Lpi2c0, kCLOCK_Pcc1BusIpSrcSysOscDiv2);
    RESET_PeripheralReset(kRESET_Lpi2c0);
    BOARD_InitPCA6416A(&g_pca6416aHandle);

    /* Reset pxp */
    RESET_PeripheralReset(kRESET_Pxp);

    /* Power on PXP Caches, buffers */
    UPOWER_PowerOnMemPart(kUPOWER_MP0_PXP, 0U);
}
/*${function:end}*/
