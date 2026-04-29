/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "mflash_drv.h"
#include "boot.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_SERIAL_RECOVERY_GPIO_PORT  BOARD_SW2_GPIO
#define BOARD_SERIAL_RECOVERY_GPIO_PIN   BOARD_SW2_GPIO_PIN 

/*******************************************************************************
 * Types
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */

int main(void)
{
    BOARD_InitHardware();

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
    if (GPIO_PinRead(BOARD_SERIAL_RECOVERY_GPIO_PORT, BOARD_SERIAL_RECOVERY_GPIO_PIN) == 0U)
    {    
        return 1;
    }
    return 0;
}
