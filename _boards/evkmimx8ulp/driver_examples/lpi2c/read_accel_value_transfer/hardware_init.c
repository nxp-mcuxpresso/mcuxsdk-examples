/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_rgpio.h"
#include "fsl_iomuxc.h"
#include "fsl_reset.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_I2C_ReleaseBus(0);
    BOARD_I2C_ConfigurePins();

    BOARD_InitDebugConsole();

    CLOCK_SetIpSrc(kCLOCK_Lpi2c0, kCLOCK_Pcc1BusIpSrcLpo); /* Choose LPO1MHz */
    RESET_PeripheralReset(kRESET_Lpi2c0);

    if (BOARD_IsLowPowerBootType() != true) /* not low power boot type */
    {
        BOARD_HandshakeWithUboot(); /* Must handshake with uboot, unless will get issues(such as: SoC reset all the
                                       time) */
    }
    else                            /* low power boot type */
    {
        BOARD_SetTrdcGlobalConfig();
    }
}


/*${function:end}*/
