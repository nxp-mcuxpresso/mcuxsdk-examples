/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "boot.h"
#include "fsl_cache.h"
#include "flash_partitioning.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

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
    BOARD_InitHardware();
   
#ifdef CONFIG_BOOT_USE_PSA_CRYPTO
    /* Disable system cache */
    XCACHE_DisableCache(XCACHE0);
    /* flush pipeline */
    __DSB();
    __ISB();
#endif

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
    return 0;
}
