/*
 * Copyright 2023, 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"
#include "board.h"

#ifdef MCMGR_USED
#include "mcmgr.h"
#endif /* MCMGR_USED */

void BOARD_Init()
{
    /* Init uart. */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM4);
    CLOCK_EnableClock(kCLOCK_LPFlexComm4);
    CLOCK_EnableClock(kCLOCK_LPUart4);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);

    /* init I2C7 clocks */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM7);
    CLOCK_EnableClock(kCLOCK_LPFlexComm7);
    CLOCK_EnableClock(kCLOCK_LPI2c7);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom7Clk, 1u);

    BOARD_InitPins();
    BOARD_InitBootClocks();
#ifndef DISABLE_CORE0_CONSOLE
    BOARD_InitDebugConsole();
#endif
    BOARD_Camera_I2C_Init();

    /* Enable caching of flash memory */
    SYSCON->LPCAC_CTRL = SYSCON->LPCAC_CTRL & ~SYSCON_LPCAC_CTRL_DIS_LPCAC_MASK;
    SYSCON->NVM_CTRL = SYSCON->NVM_CTRL & ~SYSCON_NVM_CTRL_DIS_FLASH_DATA_MASK;
}

#ifdef MCMGR_USED
#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void)
{
    uint32_t image_size;
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    image_size = (uint32_t)&Image$$CORE1_REGION$$Length;
#elif defined(__ICCARM__)
#pragma section = "__core1_image"
    image_size = (uint32_t)__section_end("__core1_image") - (uint32_t)&core1_image_start;
#elif defined(__GNUC__)
    image_size = (uint32_t)core1_image_size;
#endif
    return image_size;
}
#endif /* CORE1_IMAGE_COPY_TO_RAM */

/*!
 * @brief Application-specific implementation of the SystemInitHook() weak function.
 */
void SystemInitHook(void)
{
    /* Initialize MCMGR - low level multicore management library. Call this
       function as close to the reset entry as possible to allow CoreUp event
       triggering. The SystemInitHook() weak function overloading is used in this
       application. */
    (void)MCMGR_EarlyInit();
}
/*${function:end}*/
#endif /* MCMGR_USED */
