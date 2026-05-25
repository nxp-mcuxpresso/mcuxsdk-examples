/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "boot.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

#include "fsl_dcp.h"
#include "fsl_trng.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_SERIAL_RECOVERY_GPIO_PORT  BOARD_USER_BUTTON_GPIO
#define BOARD_SERIAL_RECOVERY_GPIO_PIN   BOARD_USER_BUTTON_GPIO_PIN
   
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
    /* Init board hardware. */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    PRINTF("hello sbl.\r\n");

    (void)sbl_boot_main();

    return 0;
}

void SBL_DisablePeripherals(void)
{
    DbgConsole_Deinit();
    DCP_Deinit(DCP);
    TRNG_Deinit(TRNG);
    SCB_DisableDCache();
    SCB_DisableICache();
    ARM_MPU_Disable();
}

int SBL_SerialRecovery_gpio_check(void)
{
    if (GPIO_PinRead(BOARD_SERIAL_RECOVERY_GPIO_PORT, BOARD_SERIAL_RECOVERY_GPIO_PIN) == 0U)
    {    
        return 1;
    }
    return 0;
}
