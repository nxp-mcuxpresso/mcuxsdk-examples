// /*
//  * Copyright 2020, 2025-2026 NXP
//  * All rights reserved.
//  *
//  * SPDX-License-Identifier: BSD-3-Clause
//  */

// #include "app.h"
// #include "pin_mux.h"
// #include "fsl_debug_console.h"
// #include "display_support.h"
// #include "camera_support.h"
// #include "board.h"
// #include "fsl_soc_src.h"

// #ifdef MCMGR_USED
// #include "mcmgr.h"
// #endif /* MCMGR_USED */

// void BOARD_Init()
// {
//     BOARD_ConfigMPU();
// #ifdef RPMSG_USED
// #if defined(__USE_SHMEM)
//     extern uint32_t __RPMSG_SH_MEM_START[];
//     extern uint32_t __RPMSG_SH_MEM_SIZE[];
//     uint32_t rpmsgShmemStart = (uint32_t)__RPMSG_SH_MEM_START;
//     uint32_t rpmsgShmemSize  = (uint32_t)__RPMSG_SH_MEM_SIZE;
// #endif

//     /* Disable MPU */
//     ARM_MPU_Disable();

// #ifdef USE_SDRAM
// #if defined(CACHE_MODE_WRITE_THROUGH) && CACHE_MODE_WRITE_THROUGH
//     /* Region 9 setting: Memory with Normal type, not shareable, write trough */
//     MPU->RBAR = ARM_MPU_RBAR(1, 0x80000000U);
//     MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 0, 0, ARM_MPU_REGION_SIZE_64MB);
// #else
//     /* Region 9 setting: Memory with Normal type, not shareable, outer/inner write back */
//     MPU->RBAR = ARM_MPU_RBAR(1, 0x80000000U);
//     MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_64MB);
// #endif
// #endif

// #if defined(__USE_SHMEM)
//     int i = 0;
	
//     while ((rpmsgShmemSize >> i) > 0x1U)
//     {
//         i++;
//     }
	
//     if (i != 0)
//     {
//         /* The MPU region size should be 2^N, 5<=N<=32, region base should be multiples of size. */
//         assert(!(rpmsgShmemStart % rpmsgShmemSize));
//         assert(rpmsgShmemSize == (uint32_t)(1 << i));
//         assert(i >= 5);
	
//         /* Region 4 setting: Memory with Normal type, not shareable, non-cacheable */
//         MPU->RBAR = ARM_MPU_RBAR(9, rpmsgShmemStart);
//         MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, i - 1);
//     }
// #endif

//     /* Enable MPU */
//     ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_HFNMIENA_Msk);
// #endif

//     BOARD_InitBootPins();
//     BOARD_InitLpuartPins();
//     BOARD_BootClockRUN();
// #ifndef DISABLE_CORE0_CONSOLE
//     BOARD_InitDebugConsole();
// #endif
//     BOARD_InitMipiCameraPins();
// #ifdef OPENH264
//     BOARD_InitSDCARD();
// #endif /* OPENH264 */
//     /*
//      * Reset the displaymix, otherwise during debugging, the
//      * debugger may not reset the display, then the behavior
//      * is not right.
//      */
//     SRC_AssertSliceSoftwareReset(SRC, kSRC_DisplaySlice);
//     BOARD_InitMipiPanelPins();
//     BOARD_MIPIPanelTouch_I2C_Init();
// }

// #ifdef MCMGR_USED
// #ifdef CORE1_IMAGE_COPY_TO_RAM
// uint32_t get_core1_image_size(void)
// {
//     uint32_t image_size;
// #if defined(__CC_ARM) || defined(__ARMCC_VERSION)
//     image_size = (uint32_t)&Image$$CORE1_REGION$$Length;
// #elif defined(__ICCARM__)
// #pragma section = "__core1_image"
//     image_size = (uint32_t)__section_end("__core1_image") - (uint32_t)&core1_image_start;
// #elif defined(__GNUC__)
//     image_size = (uint32_t)core1_image_size;
// #endif
//     return image_size;
// }
// #endif /* CORE1_IMAGE_COPY_TO_RAM */

// /*!
//  * @brief Application-specific implementation of the SystemInitHook() weak function.
//  */
// void SystemInitHook(void)
// {
//     /* Initialize MCMGR - low level multicore management library. Call this
//        function as close to the reset entry as possible to allow CoreUp event
//        triggering. The SystemInitHook() weak function overloading is used in this
//        application. */
//     (void)MCMGR_EarlyInit();
// }
// /*${function:end}*/
// #endif /* MCMGR_USED */

/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "display_support.h"
#include "camera_support.h"
#include "board.h"
#include "fsl_soc_src.h"
/*${header:end}*/

/*!
 * @brief Resets display controller.
 */
static void BOARD_ResetDisplayMix(void)
{
    /*
     * Reset the displaymix, otherwise during debugging, the
     * debugger may not reset the display, then the behavior
     * is not right.
     */
    SRC_AssertSliceSoftwareReset(SRC, kSRC_DisplaySlice);
    while (kSRC_SliceResetInProcess == SRC_GetSliceResetState(SRC, kSRC_DisplaySlice))
    {
    }
}

/*${function:start}*/
void BOARD_Init(void)
{
    BOARD_ConfigMPU();

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /*
     * Emit a boot marker as soon as the debug console (LPUART1 on the MCU-Link
     * VCOM) is up. The display controller bring-up below (BOARD_PrepareDisplayController)
     * uses no-timeout blocking MIPI-DSI / LPI2C transfers to the panel and the
     * PCAL6524 IO-expander. If the selected panel (DEMO_PANEL) is not present or
     * does not ACK, those calls hang forever inside BOARD_Init(), before the
     * application's first PRINTF -> the console appears "dead" even though the
     * UART is correctly configured. Printing here guarantees at least this line
     * reaches the terminal and pinpoints the hang to the display init.
     */
    PRINTF("\r\n[BOARD_Init] console up (LPUART1). Starting display init...\r\n");

    //incorrect - BOARD_InitMipiCameraPins();
#ifndef SKIP_DISPLAY_INIT
    BOARD_ResetDisplayMix();
    if (BOARD_PrepareDisplayController() != kStatus_Success)
    {
        /* Do not spin here: keep the console/app alive so the failure is visible. */
        PRINTF("[BOARD_Init] WARNING: display controller init failed; continuing.\r\n");
    }
#else
    PRINTF("[BOARD_Init] display init skipped (SKIP_DISPLAY_INIT).\r\n");
#endif
}
/*${function:end}*/