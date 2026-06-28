/*
 * Copyright 2026 NXP
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
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_Init6524Pins();       /* LPI2C4 SCL/SDA + INT GPIO for PCAL6524 */
    BOARD_InitSDHCPins();       /* USDHC1 pins for SDIO to MAYA-W276 */
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
