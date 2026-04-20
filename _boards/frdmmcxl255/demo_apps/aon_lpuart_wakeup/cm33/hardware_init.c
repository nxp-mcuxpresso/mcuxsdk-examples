/*
 * Copyright 2026 NXP
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

#include "fsl_power.h"
#include "fsl_cmc.h"
#include "fsl_mu.h"
#include "fsl_smm.h"

#include "fsl_reset.h"
#include "fsl_pmu.h"

// #include "fsl_rtc.h"
/*${header:end}*/

/*${function:start}*/

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
    /* Assert CM0+ reset first.  After DPD1/DPD2 wakeup (full reboot) the AON
     * domain was never power-cycled, so CM0+ may still be running.  We must
     * reset it before overwriting its code image. */
    AON__CGU->RST_SUB_BLK &= ~CGU_RST_SUB_BLK_CM0P_RST_REL_MASK;
    for (uint8_t i = 0U; i < 100; i++)
    {
        __NOP();
    }
    /* Release CM0+ from reset. */
    AON__CGU->RST_SUB_BLK |= CGU_RST_SUB_BLK_CM0P_RST_REL_MASK;
    for (uint8_t i = 0U; i < 100; i++)
    {
        __NOP();
    }
}

void BOARD_Init96MClocksBoot(void)
{
    CLOCK_SetRoscMonitorMode(kSCG_RoscMonitorDisable);
    CLOCK_SetupFROHFClocking(96000000U, 0U);
    CLOCK_SetupFRO12MClocking();
    CLOCK_EnableClock(kCLOCK_GateAonAPB);

    CLOCK_AttachClk(kFIRC_to_MAIN_CLK);
    CLOCK_AttachClk(kFIRC_to_FRO_HF);
    CLOCK_AttachClk(kCPU_CLK_to_SYSTICK);
    CLOCK_AttachClk(kFRO16K_to_OSTIMER0);
    CLOCK_AttachClk(kSLOW_CLK_to_CLKOUT);
    CLOCK_AttachClk(kFRO_HF_DIV_to_ADC0);
    CLOCK_AttachClk(kCLK_1M_to_UTICK0);
    CLOCK_AttachClk(kFRO12M_to_CMP0);
    CLOCK_AttachClk(kFRO12M_to_PERIPH_GROUP0);
    CLOCK_AttachClk(kFRO_HF_DIV_to_PERIPH_GROUP1);

    CLOCK_SetClockDiv(kCLOCK_DivAHBCLK, 1U);
    CLOCK_SetClockDiv(kCLOCK_DivFRO_HF_DIV, 1U);
    CLOCK_SetClockDiv(kCLOCK_DivWWDT0, 1U);
    CLOCK_SetClockDiv(kCLOCK_DivCMP0_FUNC, 1U);
    CLOCK_SetClockDiv(kCLOCK_DivTRACE, 1U);
    CLOCK_SetClockDiv(kCLOCK_DivADC0, 1U);
    CLOCK_SetClockDiv(kCLOCK_DivPeriphGroup0, 1U);
    CLOCK_SetClockDiv(kCLOCK_DivPeriphGroup1, 1U);

    CLOCK_EnableClock(kCLOCK_GatePERIPH_GROUP0);
    CLOCK_EnableClock(kCLOCK_GatePERIPH_GROUP1);

    SystemCoreClock = 96000000U;

    FMU0->FCTRL = (FMU0->FCTRL & ~FMU_FCTRL_RWSC_MASK) | FMU_FCTRL_RWSC(2);
}

void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_Init96MClocksBoot();
    BOARD_InitDebugConsole();

    Power_ClearLpPowerSettings();

    PMU_DoHandshakeBetweenPMUAndPAC(AON__PMU);

    CMC_EnableDebugOperation(CMC, false);

    EnableIRQ(MU_A_RX_IRQn);
    MU_EnableInterrupts(APP_MU, (kMU_Rx0FullInterruptEnable));
}

/*${function:end}*/
