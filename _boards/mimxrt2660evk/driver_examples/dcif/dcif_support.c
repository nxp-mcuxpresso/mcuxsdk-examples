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
#if DEMO_INTERFACE_TYPE == DEMO_INTERFACE_DBI
#include "fsl_st7796s.h"
#include "fsl_dbi.h"
#endif
#include "fsl_debug_console.h"

void BOARD_InitDcifPowerClockReset(void)
{
#if (DEMO_INTERFACE_TYPE == DEMO_INTERFACE_DPI)
    /* For DPI panel one frame need (800+8+8+4)x(480+8+8+4)=410000 pixel clock,
     * a 60Hz frame rate requires 410000*60=24.6M pixel clock. DBI panel uses
     * media pll clock directly, no need to configure pixel clock.
     */
    uint32_t pixClkPerFrame = (DEMO_PANEL_WIDTH + DEMO_HSW + DEMO_HFP + DEMO_HBP) *
        (DEMO_PANEL_HEIGHT + DEMO_VSW + DEMO_VFP + DEMO_VBP);

    /* TODO Use fixed value for now since CLOCK_GetRootClockFreq(kCLOCK_SRC_PERI5)
     * should return 400 MHz but actually returns 200m. */
    clock_root_config_t rootCfg = {
        .mux = kCLOCK_DCPIXEL_ClockRoot_PERI5,
        .div = 400000000U / pixClkPerFrame / DEMO_FRAME_RATE,
    };

    CLOCK_SetRootClock(kCLOCK_Root_MEDIA_dcpixel_fclk, &rootCfg);
#endif

    /* Enable DCIF clock gate. */
    CLOCK_EnableClock(kCLOCK_MEDIA_dcif);

    /* Pulse the DCIF peripheral reset. */
    RESET_PeripheralReset((reset_ip_name_t)kModCon_MEDIA_DCIF);
}

#if DEMO_INTERFACE_TYPE == DEMO_INTERFACE_DPI

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
        .format = kDCIF_DpiRGB888,
        .displayMode = kDCIF_DpiNormal,
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

#else

static status_t DEMO_LcdWriteCommand(void *dbiXferHandle, uint32_t command)
{
    DCIF_Type *dcifBase = (DCIF_Type *)dbiXferHandle;

    DCIF_DbiWriteCommand(dcifBase, (uint8_t)command);

    return kStatus_Success;
}

static status_t DEMO_LcdWriteData(void *dbiXferHandle, void *data, uint32_t len_byte)
{
    DCIF_Type *dcifBase = (DCIF_Type *)dbiXferHandle;

    DCIF_DbiWriteParam(dcifBase, data, len_byte);

    return kStatus_Success;
}

static st7796s_handle_t lcdHandle;
/* The functions used to drive the panel. */
static dbi_xfer_ops_t s_dcifDbiOps = {
    .writeCommand          = DEMO_LcdWriteCommand,
    .writeData             = DEMO_LcdWriteData,
    .writeMemory           = NULL,
    .readMemory            = NULL, /* Don't need read in this project. */
    .setMemoryDoneCallback = NULL, /* Write memory is blocking function, don't need callback. */
};

void DEMO_InitInterface(void)
{
    /* For DBI panel one frame has 480x320=153600 pixels, on D16-RGB565 interface each WR cycle
     * write one pixel. DBI uses pixel clock directly, calculate the WR cycle period.
     */
    uint8_t wrCycle = (uint8_t)(CLOCK_GetRootClockFreq(kCLOCK_Root_MEDIA_dcpixel_fclk) /
        (DEMO_PANEL_WIDTH * DEMO_PANEL_HEIGHT * DEMO_FRAME_RATE));
    uint8_t srcPrd_Ns= (uint8_t)(1000000000U / CLOCK_GetRootClockFreq(kCLOCK_Root_MEDIA_mediapll_clk));

    dcif_dbi_config_t dbiConfig = {
        .format = kDCIF_DbiOutD16RGB565,
        .type = kDCIF_DbiTypeB,
        .signalFlags = 0U,
        .wrHigh = wrCycle / 2U, /* Choose same high and low WR period */
        .wrLow = wrCycle / 2U,
        .csSetup = (15U + srcPrd_Ns - 1U) / srcPrd_Ns,  /* TCS at least 15ns, round up. */
        .csHold = (10U + srcPrd_Ns - 1U) / srcPrd_Ns,   /* TCSH at least 10ns, round up. */
    };

    DCIF_DbiSetConfig(DEMO_DCIF, &dbiConfig);
}

void DEMO_DbiSelectUpdateArea(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y)
{
    ST7796S_SelectArea(&lcdHandle, start_x, start_y, end_x, end_y);
}

void DEMO_UpdateFrame(uint32_t address)
{
    /* Command mode only need 1 frame buffer. */
    (void)address;

    DCIF_DisableInterrupts(DEMO_DCIF, DEMO_DOMAIN, kDCIF_InterruptDbiCommandDone);
    DCIF_DbiWriteCommand(DEMO_DCIF, DBI_CMD_WRITE_MEMORY_START);
    DCIF_DbiWritePixel(DEMO_DCIF);
    DCIF_EnableInterrupts(DEMO_DCIF, DEMO_DOMAIN, kDCIF_InterruptDbiCommandDone);
}

status_t BOARD_InitDisplayInterface(void)
{
    status_t status;

    const st7796s_config_t st7796sConfig = {.driverPreset    = kST7796S_DriverPresetLCDPARS035,
                                            .pixelFormat     = kST7796S_PixelFormatRGB565,
                                            .orientationMode = kST7796S_Orientation270,
                                            .teConfig        = kST7796S_TEDisabled,
                                            .invertDisplay   = true,
                                            .flipDisplay     = true,
                                            .bgrFilter       = true};

    const gpio_pin_config_t config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 1U,
    };
    GPIO_PinInit(BOARD_DBI_PANEL_LCD_RST_GPIO, BOARD_DBI_PANEL_LCD_RST_PIN,
                    &config);
    GPIO_PinInit(BOARD_DBI_PANEL_BLK_GPIO, BOARD_DBI_PANEL_BLK_PIN,
                    &config);

    /* Enable backlight */
    GPIO_PinWrite(BOARD_DBI_PANEL_BLK_GPIO, BOARD_DBI_PANEL_BLK_PIN, 1U);

    /* Reset controller */
    GPIO_PinWrite(BOARD_DBI_PANEL_LCD_RST_GPIO,
                    BOARD_DBI_PANEL_LCD_RST_PIN, 0U);
    SDK_DelayAtLeastUs(10, SystemCoreClock);   /* Delay 10ns. */

    GPIO_PinWrite(BOARD_DBI_PANEL_LCD_RST_GPIO,
                    BOARD_DBI_PANEL_LCD_RST_PIN, 1U);
    SDK_DelayAtLeastUs(5000, SystemCoreClock); /* Delay 5ms. */

    status = ST7796S_Init(&lcdHandle, &st7796sConfig, &s_dcifDbiOps, DEMO_DCIF);

    if (kStatus_Success != status)
    {
        return status;
    }

    ST7796S_EnableDisplay(&lcdHandle, true);

    return kStatus_Success;
}

#endif
