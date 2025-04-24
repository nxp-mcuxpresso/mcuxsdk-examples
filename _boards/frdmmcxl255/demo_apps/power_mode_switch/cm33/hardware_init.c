/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

#include "fsl_wuu.h"
#include "fsl_power.h"
#include "fsl_cmc.h"
#include "fsl_mu.h"
/*${header:end}*/

/*${function:start}*/
void WUU0_IRQHandler(void)
{
//    uint32_t externalPinFlag;
//    externalPinFlag = WUU_GetExternalWakeUpPinsFlag(WUU0);
//
//    if ((externalPinFlag & (1UL << 7UL)) != 0UL)
//    {
//        WUU_ClearExternalWakeUpPinsFlag(WUU0, externalPinFlag);
//    }
//
//    Power_DisableWakeupSource(externalPinFlag);
}

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
#endif

void APP_BootCore1(void)
{
    AON__CGU->RST_SUB_BLK |= CGU_RST_SUB_BLK_CM0P_RST_REL_MASK;
    for(uint8_t i = 0U; i < 100; i++)
    {
        __NOP();
    }
}

void BOARD_InitHardware(void)
{
    BOARD_InitSWD_DEBUGPins();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CMC_EnableDebugOperation(CMC, false);
    
    CLOCK_EnableClock(kCLOCK_GateAonPORT);
    CLOCK_EnableClock(kCLOCK_GateAonAPB);
    CLOCK_EnableClock(kCLOCK_GateAonGPIO);
    CLOCK_EnableClock(kCLOCK_GateAonSYS);
    CLOCK_EnableClock(kCLOCK_GateAonUART);
    CLOCK_EnableClock(kCLOCK_GateAonI2C);
        
    EnableIRQ(MU_A_RX_IRQn);
    MU_EnableInterrupts(APP_MU, (kMU_Rx0FullInterruptEnable));
}

/*${function:end}*/
