/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include <stdbool.h>
#include "clock_config.h"
#include "stdbool.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Select the clock source for the LPI2C0 module as fast internal RC oscillator */
    CLOCK_SetIpSrc(kCLOCK_Lpi2c0, kCLOCK_IpSrcFircAsync);

/* Set IRQ priority for freertos_lpi2c */
#if (I2C_MASTER_SLAVE == isMASTER)
    NVIC_SetPriority(EXAMPLE_I2C_MASTER_IRQN, 3);
#elif (I2C_MASTER_SLAVE == isSLAVE)
    NVIC_SetPriority(EXAMPLE_I2C_SLAVE_IRQN, 2);
#endif
}
/*${function:end}*/
