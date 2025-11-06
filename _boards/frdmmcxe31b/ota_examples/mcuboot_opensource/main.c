/*
 * Copyright 2025 NXP
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
#include "fsl_c40_flash.h"
#include "flash_partitioning.h"
#include "boot.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

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
    /* The app triggered a reset, clear the status */
    if (MC_RGM->FES & MC_RGM_FES_SW_FUNC_MASK)
    {
        MC_RGM->FES = MC_RGM_FES_SW_FUNC_MASK;
    }

    BOARD_InitHardware();

    /* Unlock application slots for flash erase/write.
     * On MCXE31B sectors within first 768kB block have super-sector
     * granularity of 64kB.
     */

    /* Either the size of the slot must be aligned to super sector size or it must also clear
       the protection on the last partly used sector. The current setup uses aligned slots
    */

    assert(BOOT_FLASH_ACT_SIZE % MFLASH_SUPER_SECTOR_SIZE == 0);

    for (int i=0; i < BOOT_FLASH_ACT_SIZE/MFLASH_SUPER_SECTOR_SIZE; i++)
    {
        FLASH_SetSectorProtection(NULL, BOOT_FLASH_ACT_APP + i*MFLASH_SUPER_SECTOR_SIZE, false);
        FLASH_SetSectorProtection(NULL, BOOT_FLASH_CAND_APP + i*MFLASH_SUPER_SECTOR_SIZE, false);
    }

    PRINTF("hello sbl.\r\n");

    (void)sbl_boot_main();

    return 0;
}

void SBL_DisablePeripherals(void)
{
    DbgConsole_Deinit();

    // Disable all interrupts
    __disable_irq();
    __asm volatile ("cpsid i");

    // CM7 Cache Management
    // Disable and clean D-Cache
    if (SCB->CCR & SCB_CCR_DC_Msk)
    {
        SCB_DisableDCache();
        SCB_CleanInvalidateDCache();
    }
    // Disable I-Cache
    if (SCB->CCR & SCB_CCR_IC_Msk)
    {
        SCB_DisableICache();
        SCB_InvalidateICache();
    }

    // MPU Management
    #ifdef __MPU_PRESENT
    if (MPU->CTRL & MPU_CTRL_ENABLE_Msk)
    {
        ARM_MPU_Disable();
    }
    #endif

    // FPU Management
    #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    // Disable FPU if it was enabled
    SCB->CPACR &= ~((3UL << 20) | (3UL << 22));  // Clear CP10 and CP11
    #endif

    // Reset NVIC - Clear all pending interrupts
    for (int i = 0; i < 8; i++)
    {
        NVIC->ICER[i] = 0xFFFFFFFF;  // Disable all interrupts
        NVIC->ICPR[i] = 0xFFFFFFFF;  // Clear all pending interrupts
    }

    // Memory barriers - Ensure all operations are completed
    __DSB();  // Data Synchronization Barrier
    __ISB();  // Instruction Synchronization Barrier
}

