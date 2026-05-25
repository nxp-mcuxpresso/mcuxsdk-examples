/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "boot.h"

#include "fsl_dcp.h"
#include "fsl_trng.h"
#include <stdio.h>

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
#ifdef CONFIG_BOOT_MODE_FLASH_REMAP

#define IOMUXC_GPR_GPR30_REG 0x400AC078 /* Flash remapping start address  */
#define IOMUXC_GPR_GPR31_REG 0x400AC07C /* Flash remapping end address    */
#define IOMUXC_GPR_GPR32_REG 0x400AC080 /* Flash remapping offset address */

void SBL_EnableRemap(uint32_t start_addr, uint32_t end_addr, uint32_t off)
{
    uint32_t *remap_start  = (uint32_t *)IOMUXC_GPR_GPR30_REG;
    uint32_t *remap_end    = (uint32_t *)IOMUXC_GPR_GPR31_REG;
    uint32_t *remap_offset = (uint32_t *)IOMUXC_GPR_GPR32_REG;

    *remap_start  = start_addr;
    *remap_end    = end_addr;
    *remap_offset = off;
}

void SBL_DisableRemap(void)
{
    uint32_t *remap_start  = (uint32_t *)IOMUXC_GPR_GPR30_REG;
    uint32_t *remap_end    = (uint32_t *)IOMUXC_GPR_GPR31_REG;
    uint32_t *remap_offset = (uint32_t *)IOMUXC_GPR_GPR32_REG;

    /* Disable offset first! */
    *remap_offset = 0;
    *remap_start  = 0;
    *remap_end    = 0;
}
#endif

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

#if defined(MCUBOOT_DIRECT_XIP) && defined(CONFIG_BOOT_MODE_FLASH_REMAP)
    /* Make sure flash remapping function is disabled before running the
     * bootloader application .
     */
    PRINTF("Disabling flash remapping function\n");
    SBL_DisableRemap();
#endif

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
