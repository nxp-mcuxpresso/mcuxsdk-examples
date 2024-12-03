/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_i2c.h"
#include "app.h"
/*${header:end}*/

extern i2c_slave_handle_t g_s_handle;
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}

void I2C0_I2C1_IRQHandler(void)
{
    I2C_SlaveTransferHandleIRQ(EXAMPLE_I2C_SLAVE_BASEADDR, &g_s_handle);
    __DSB();
}

/*${function:end}*/
