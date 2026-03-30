/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_power.h"
#include "fsl_gpio.h"
#include "fsl_mipi_dsi.h"
#include "lcdif_support.h"

#if (USE_MIPI_PANEL == MIPI_PANEL_RASPI_7INCH)
#include "fsl_rpi.h"
#endif

#if BOARD_ENABLE_PSRAM_CACHE
/*
 * When PSRAM cache enabled and frame buffer placed in PSRAM, LCDIF reads the
 * cache first, then reads PSRAM if cache missed. Generally the frame buffer
 * is large, cache miss happens frequently, so the LCDIF read performance is low.
 *
 * There are three solutions:
 * 1. Disables the PSRAM cache, LCDIF reads from PSRAM directly.
 * 2. Slow down the frame rate, or use smaller pixel format, or use small resolution.
 * 3. Don't place frame buffer in PSRAM.
 *
 * In the example, solution 1 is used.
 */
#error Please read the comment about PSRAM cache and display refresh rate here
#endif

uint32_t mipiDsiDpiClkFreq_Hz;

uint32_t mipiDsiTxEscClkFreq_Hz;
uint32_t mipiDsiDphyBitClkFreq_Hz;

static status_t BOARD_ReadPanelStatus(uint8_t regAddr, uint8_t *value)
{
    return BOARD_I2C_Receive(BOARD_MIPI_PANEL_TOUCH_I2C_BASEADDR, RPI_ADDR, regAddr, 1U, value, 1U);
}

static status_t BOARD_WritePanelRegister(uint8_t regAddr, uint8_t value)
{
    return BOARD_I2C_Send(BOARD_MIPI_PANEL_TOUCH_I2C_BASEADDR, RPI_ADDR, regAddr, 1, &value, 1);
}


status_t PANEL_DSI_Transfer(dsi_transfer_t *xfer)
{
    return DSI_TransferBlocking(DEMO_MIPI_DSI, xfer);
}

static mipi_dsi_device_t dsiDevice = {
    .virtualChannel = 0,
    .xferFunc       = PANEL_DSI_Transfer,
};

static const rpi_resource_t rpiResource = {
    .dsiDevice     = &dsiDevice,
    .readStatus    = &BOARD_ReadPanelStatus,
    .writeRegister = &BOARD_WritePanelRegister,
};

static display_handle_t rpiHandle = {
    .resource = &rpiResource,
    .ops      = &rpi_ops,
};

void BOARD_InitLcdifClock(void)
{
    POWER_DisablePD(kPDRUNCFG_SHUT_MEDIA_MAINCLK);
    POWER_DisablePD(kPDRUNCFG_APD_LCDIF);
    POWER_DisablePD(kPDRUNCFG_PPD_LCDIF);
    POWER_ApplyPD();

    /*
     * The pixel clock is (height + VSW + VFP + VBP) * (width + HSW + HFP + HBP) * frame rate.
     * Here use the main pll (528MHz) as clock source.
     * Since MIPI DPHY clock use AUDIO pll pfd2 as aource, and its max allowed clock frequency is
     * 532.48 x 18 / 16 = 599.04MHz. To avoid exceed this limit, for RK055AHD091 and RK055MHD091,
     * the frame rate shall be 36.3fps and 37.72fps, which is 37.71MHz pixel clock. For RK055IQH091
     * it's resolution is safe to use 60fps frame rate, which is 35.2mHz pixel clock.
     * For RaspberryPi panel, the frame rate shall be 42fps, which is 19.56MHz pixel clock.
     */
    CLOCK_AttachClk(kMAIN_PLL_PFD2_to_LCDIF);
    CLOCK_SetClkDiv(kCLOCK_DivLcdifClk, 27);

    /* Get lcdif pixel clock frequency. */
    mipiDsiDpiClkFreq_Hz = CLOCK_GetLcdifClkFreq();

    CLOCK_EnableClock(kCLOCK_Lcdif);
    RESET_ClearPeripheralReset(kLCDIF_RST_SHIFT_RSTn);
}


static status_t BOARD_InitLcdPanel(void)
{
    status_t status;

    const display_config_t displayConfig = {
        .resolution   = FSL_VIDEO_RESOLUTION(DEMO_PANEL_WIDTH, DEMO_PANEL_HEIGHT),
        .hsw          = DEMO_HSW,
        .hfp          = DEMO_HFP,
        .hbp          = DEMO_HBP,
        .vsw          = DEMO_VSW,
        .vfp          = DEMO_VFP,
        .vbp          = DEMO_VBP,
        .controlFlags = 0,
        .dsiLanes     = DEMO_MIPI_DSI_LANE_NUM,
    };

    BOARD_MIPIPanelTouch_I2C_Init();

    status = RPI_Init(&rpiHandle, &displayConfig);
    return status;
}

static void BOARD_InitMipiDsiClock(void)
{
    POWER_DisablePD(kPDRUNCFG_PPD_MIPIDSI);
    POWER_DisablePD(kPDRUNCFG_APD_MIPIDSI);
    POWER_DisablePD(kPDRUNCFG_PD_VDD2_MIPI);
    POWER_ApplyPD();

    /* Use PLL PFD1 as clock source, 396m. */
    CLOCK_AttachClk(kMAIN_PLL_PFD1_to_MIPI_DPHYESC_CLK);
    /* RxClkEsc min 60MHz, TxClkEsc 12 to 20MHz. */
    /* RxClkEsc = 396MHz / 6 = 66MHz. */
    CLOCK_SetClkDiv(kCLOCK_DivDphyEscRxClk, 6);
    /* TxClkEsc = 396MHz / 6 / 4 = 16.5MHz. */
    CLOCK_SetClkDiv(kCLOCK_DivDphyEscTxClk, 4);

    mipiDsiTxEscClkFreq_Hz = CLOCK_GetMipiDphyEscTxClkFreq();

    /* The DPHY bit clock must be fast enough to send out the pixels, it should be
     * larger than:
     *
     *         (Pixel clock * bit per output pixel) / number of MIPI data lane
     *
     * DPHY uses AUDIO pll pfd2 as aource, and its max allowed clock frequency is
     * 532.48 x 18 / 16 = 599.04MHz. The MIPI panel supports up to 850MHz bit clock.
     */
    mipiDsiDphyBitClkFreq_Hz = mipiDsiDpiClkFreq_Hz * DEMO_MIPI_DSI_BIT_PER_PIXEL / DEMO_MIPI_DSI_LANE_NUM;
    mipiDsiDphyBitClkFreq_Hz = DEMO_MIPI_DPHY_BIT_CLK_ENLARGE(mipiDsiDphyBitClkFreq_Hz);

    uint8_t div = (uint8_t)((uint64_t)CLOCK_GetAudioPllFreq() * 18U / (uint64_t)mipiDsiDphyBitClkFreq_Hz);

    CLOCK_InitAudioPfd(kCLOCK_Pfd2, div);
    CLOCK_AttachClk(kAUDIO_PLL_PFD2_to_MIPI_DSI_HOST_PHY);
    CLOCK_SetClkDiv(kCLOCK_DivDphyClk, 1);


    mipiDsiDphyBitClkFreq_Hz = CLOCK_GetMipiDphyClkFreq();
}

static void BOARD_SetMipiDsiConfig(void)
{
    dsi_config_t dsiConfig;
    dsi_dphy_config_t dphyConfig;

    const dsi_dpi_config_t dpiConfig = {
        .pixelPayloadSize = DEMO_PANEL_WIDTH,
        .dpiColorCoding   = kDSI_Dpi24Bit,
        .pixelPacket      = kDSI_PixelPacket24Bit,
        .videoMode        = kDSI_DpiNonBurstWithSyncPulse,
        .bllpMode         = kDSI_DpiBllpLowPower,
        .polarityFlags    = kDSI_DpiVsyncActiveLow | kDSI_DpiHsyncActiveLow,
        .hfp              = DEMO_HFP,
        .hbp              = DEMO_HBP,
        .hsw              = DEMO_HSW,
        .vfp              = DEMO_VFP,
        .vbp              = DEMO_VBP,
        .panelHeight      = DEMO_PANEL_HEIGHT,
        .virtualChannel   = 0
    };

    /*
     * dsiConfig.numLanes = 4;
     * dsiConfig.enableNonContinuousHsClk = false;
     * dsiConfig.autoInsertEoTp = true;
     * dsiConfig.numExtraEoTp = 0;
     * dsiConfig.htxTo_ByteClk = 0;
     * dsiConfig.lrxHostTo_ByteClk = 0;
     * dsiConfig.btaTo_ByteClk = 0;
     */
    DSI_GetDefaultConfig(&dsiConfig);
    dsiConfig.numLanes                 = DEMO_MIPI_DSI_LANE_NUM;
    dsiConfig.autoInsertEoTp           = false;
    dsiConfig.enableNonContinuousHsClk = false;

    DSI_GetDphyDefaultConfig(&dphyConfig, mipiDsiDphyBitClkFreq_Hz, mipiDsiTxEscClkFreq_Hz);

    /* Init the DSI module. */
    DSI_Init(DEMO_MIPI_DSI, &dsiConfig);

    /* Init DPHY. There is not DPHY PLL, the ref clock is not used. */
    DSI_InitDphy(DEMO_MIPI_DSI, &dphyConfig, 0);

    /* Init DPI interface. */
    DSI_SetDpiConfig(DEMO_MIPI_DSI, &dpiConfig, DEMO_MIPI_DSI_LANE_NUM, mipiDsiDpiClkFreq_Hz, mipiDsiDphyBitClkFreq_Hz);
}

status_t BOARD_InitDisplayInterface(void)
{
    /* 1. Setup clock. */
    BOARD_InitMipiDsiClock();

    /* 2. Configures peripheral. */
    BOARD_SetMipiDsiConfig();

    /* 3. Configure the panel. */
    return BOARD_InitLcdPanel();
}
