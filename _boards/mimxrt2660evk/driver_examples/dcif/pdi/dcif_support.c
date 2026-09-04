/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "dcif_support.h"
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_dcif.h"
#include "fsl_reset.h"
#include "fsl_debug_console.h"

void BOARD_InitDcifPowerClockReset(void)
{
    /* For DPI panel one frame need (800+8+8+4)x(480+8+8+4)=410000 pixel clock,
     * a 60Hz frame rate requires 410000*60=24.6M pixel clock. DBI panel uses
     * media pll clock directly, no need to configure pixel clock.
     */
    uint32_t pixClkPerFrame = (DEMO_PANEL_WIDTH + DEMO_HSW + DEMO_HFP + DEMO_HBP) *
        (DEMO_PANEL_HEIGHT + DEMO_VSW + DEMO_VFP + DEMO_VBP);

    clock_root_config_t rootCfg = {
        .mux = kCLOCK_DCPIXEL_ClockRoot_PERI5,
        .div = CLOCK_GetClockSrcFreq(kCLOCK_SRC_PERI5) / pixClkPerFrame / DEMO_FRAME_RATE,
    };

    CLOCK_SetRootClock(kCLOCK_Root_MEDIA_dcpixel_fclk, &rootCfg);

    /* Enable DCIF clock gate. */
    CLOCK_EnableClock(kCLOCK_MEDIA_dcif);

    /* Pulse the DCIF peripheral reset. */
    RESET_PeripheralReset((reset_ip_name_t)kModCon_MEDIA_DCIF);
}

void DEMO_InitInterface(void)
{
    /* DPI configuration. */
    dcif_dpi_config_t dpiConfig = {
        .hsw = DEMO_HSW,
        .hfp = DEMO_HFP,
        .hbp = DEMO_HBP,
        .vsw = DEMO_VSW,
        .vfp = DEMO_VFP,
        .vbp = DEMO_VBP,
        .polarityFlags = DEMO_POL_FLAGS,
        .format        = kDCIF_DpiRGB888,
        .displayMode   = kDCIF_DpiNormal,
        .frameFetch    = kDCIF_DpiVswBegin,
        .txFifoFill    = kDCIF_DpiVbpEnd,
        .enableBackground = true,
    };

    DCIF_DpiSetConfig(DEMO_DCIF, &dpiConfig);
}

void DEMO_UpdateFrame(uint32_t address)
{
    if (address != 0U)
    {
        DCIF_SetLayerAddr(DEMO_DCIF, 0U, address);
    }

    DCIF_TriggerLayerShadowLoad(DEMO_DCIF, 0U);
}

status_t BOARD_InitDisplayInterface(void)
{
    pcal6524_handle_t *power_h = BOARD_GetPCAL6524Handle();
    pca9555_handle_t *back_light_h = BOARD_GetPCA9555Handle();

    (void)PCAL6524_SetDirection(power_h, 1U << BOARD_PCAL6524_LCM_PWR_EN1, kPCAL6524_Output);
    (void)PCAL6524_SetPins(power_h, 1U << BOARD_PCAL6524_LCM_PWR_EN1);
    (void)PCA9555_SetDirection(back_light_h, 1U << BOARD_PCA9555_BL_EN_RGB, kPCA9555_Output);
    (void)PCA9555_SetPins(back_light_h, 1U << BOARD_PCA9555_BL_EN_RGB);

    return kStatus_Success;
}
