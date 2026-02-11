/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "boot.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_SERIAL_RECOVERY_GPIO_PORT  BOARD_SW1_GPIO_PORT
#define BOARD_SERIAL_RECOVERY_GPIO_PIN   BOARD_SW1_GPIO_PIN

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_BootClockFROHF96M();
    BOARD_InitPins();
    BOARD_InitDebugConsole();
    /* enable clock for GPIO*/
    CLOCK_EnableClock(kCLOCK_Gpio0);
    CLOCK_EnableClock(kCLOCK_Gpio1);

    PRINTF("hello sbl.\r\n");

    (void)sbl_boot_main();

    return 0;
}

void SBL_DisablePeripherals(void)
{
    DbgConsole_Deinit();
}

int SBL_SerialRecovery_gpio_check(void)
{
    if (GPIO_PinRead(GPIO, BOARD_SERIAL_RECOVERY_GPIO_PORT, BOARD_SERIAL_RECOVERY_GPIO_PIN) == 0U)
    {    
        return 1;
    }
    return 0;
}