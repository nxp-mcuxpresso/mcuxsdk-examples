/*
 * Copyright 2024-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @brief Board-level display support for FRDM-IMXRT700 with Waveshare 5inch DSI LCD.
 *
 * The Waveshare 5inch DSI LCD (800x480, ILI6122 DSI bridge, GT911 touch) connects
 * via the board's MIPI DSI connector. The panel operates in DSI video mode (burst)
 * with 2 data lanes at ~500 Mbps/lane.
 *
 * Pin mapping (from pin_mux.c BOARD_InitMipiPanelPins):
 *   RST   : GPIO3_4
 *   POWER : GPIO1_10
 *   BL    : GPIO1_14
 *   TE    : GPIO3_5  (not used by this panel, kept for compatibility)
 *   Touch : LPI2C8   (GT911, addr 0x5D or 0x14)
 */

#include "display_support.h"
#include "fsl_gpio.h"
#include "fsl_mipi_dsi.h"
#include "fsl_mipi_dsi_cmd.h"
#include "fsl_dc_fb_lcdif.h"
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Waveshare 5inch DSI LCD timing (800x480, ILI6122 bridge)
 *
 * Pixel clock: ~33.3 MHz  (800+210+46+10) * (480+22+23+10) * 60 ≈ 33.26 MHz
 * DSI bit clock per lane: pixel_clk * 24bpp / 2 lanes * 1.125 margin ≈ 450 Mbps
 ******************************************************************************/
#define DEMO_HSW  10U   /* Horizontal sync width */
#define DEMO_HFP  46U   /* Horizontal front porch */
#define DEMO_HBP  210U  /* Horizontal back porch */
#define DEMO_VSW  10U   /* Vertical sync width */
#define DEMO_VFP  22U   /* Vertical front porch */
#define DEMO_VBP  23U   /* Vertical back porch */

/* MIPI DSI */
#define DEMO_MIPI_DSI           MIPI_DSI_HOST
#define DEMO_MIPI_DSI_LANE_NUM  2U

/* DPHY bit clock: enlarge by 9/8 for margin */
#define DEMO_MIPI_DPHY_BIT_CLK_ENLARGE(origin) (((origin) / 8U) * 9U)

/* LCDIF polarity flags for DSI video mode */
#define DEMO_LCDIF_POL_FLAGS \
    (kLCDIF_DataEnableActiveHigh | kLCDIF_VsyncActiveLow | \
     kLCDIF_HsyncActiveLow | kLCDIF_DriveDataOnFallingClkEdge)

#define DEMO_LCDIF LCDIF

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void BOARD_PullPanelResetPin(bool pullUp);
static void BOARD_PullPanelPowerPin(bool pullUp);
static void BOARD_SetPanelBacklight(bool on);
static status_t BOARD_DSI_Transfer(dsi_transfer_t *xfer);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* LCDIF DC framebuffer driver */
static dc_fb_lcdif_handle_t s_dcFbLcdifHandle;

static const dc_fb_lcdif_config_t s_dcFbLcdifConfig = {
    .lcdif        = DEMO_LCDIF,
    .width        = DEMO_BUFFER_WIDTH,
    .height       = DEMO_BUFFER_HEIGHT,
    .hsw          = DEMO_HSW,
    .hfp          = DEMO_HFP,
    .hbp          = DEMO_HBP,
    .vsw          = DEMO_VSW,
    .vfp          = DEMO_VFP,
    .vbp          = DEMO_VBP,
    .polarityFlags = DEMO_LCDIF_POL_FLAGS,
    .outputFormat  = kLCDIF_Output24Bit,
};

const dc_fb_t g_dc = {
    .ops     = &g_dcFbOpsLcdif,
    .prvData = &s_dcFbLcdifHandle,
    .config  = &s_dcFbLcdifConfig,
};

/* MIPI DSI device handle */
static mipi_dsi_device_t s_dsiDevice = {
    .virtualChannel = 0U,
    .xferFunc       = BOARD_DSI_Transfer,
};

/*******************************************************************************
 * Code
 ******************************************************************************/

static void BOARD_PullPanelResetPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_MIPI_RST_GPIO, BOARD_MIPI_RST_PIN, 1U);
    }
    else
    {
        GPIO_PinWrite(BOARD_MIPI_RST_GPIO, BOARD_MIPI_RST_PIN, 0U);
    }
}

static void BOARD_PullPanelPowerPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_MIPI_PWR_GPIO, BOARD_MIPI_PWR_PIN, 1U);
    }
    else
    {
        GPIO_PinWrite(BOARD_MIPI_PWR_GPIO, BOARD_MIPI_PWR_PIN, 0U);
    }
}

static void BOARD_SetPanelBacklight(bool on)
{
    GPIO_PinWrite(BOARD_MIPI_BL_GPIO, BOARD_MIPI_BL_PIN, on ? 1U : 0U);
}

static status_t BOARD_DSI_Transfer(dsi_transfer_t *xfer)
{
    return DSI_TransferBlocking(DEMO_MIPI_DSI, xfer);
}

/*
 * @brief Initialize GPIO output pins for panel control.
 */
static void BOARD_InitPanelGpio(void)
{
    gpio_pin_config_t pinConfig = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 0U,
    };

    /* RST: GPIO3_4 */
    GPIO_PinInit(BOARD_MIPI_RST_GPIO, BOARD_MIPI_RST_PIN, &pinConfig);
    /* POWER: GPIO1_10 */
    GPIO_PinInit(BOARD_MIPI_PWR_GPIO, BOARD_MIPI_PWR_PIN, &pinConfig);
    /* BL: GPIO1_14 */
    GPIO_PinInit(BOARD_MIPI_BL_GPIO, BOARD_MIPI_BL_PIN, &pinConfig);
}

/*
 * @brief Reset the Waveshare 5inch DSI panel.
 *
 * The ILI6122 DSI bridge requires a clean power-on reset sequence:
 *   1. Assert POWER
 *   2. Assert RST low for >=10ms
 *   3. Deassert RST high
 *   4. Wait >=120ms for panel init
 */
static void BOARD_ResetPanel(void)
{
    BOARD_PullPanelPowerPin(true);
    SDK_DelayAtLeastUs(10000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY); /* 10 ms */

    BOARD_PullPanelResetPin(false);
    SDK_DelayAtLeastUs(10000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY); /* 10 ms */

    BOARD_PullPanelResetPin(true);
    SDK_DelayAtLeastUs(120000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY); /* 120 ms */
}

/*
 * @brief Configure MIPI DSI host and DPHY clocks for the Waveshare 5inch panel.
 *
 * Pixel clock ≈ 33.26 MHz → DPHY bit clock ≈ 450 Mbps/lane (2 lanes, 24bpp).
 *
 * Clock sources on MIMXRT798S:
 *  - DPHY clock  : MEDIA_VDD2_BASE → MIPI_DSI_HOST_PHY
 *  - DPHY ESC    : MEDIA_VDD2_BASE → MIPI_DPHYESC_CLK  (must be ≤20 MHz)
 *
 * Returns the actual DPHY HS bit clock achieved (Hz), needed by DSI_SetDpiConfig.
 */
static uint32_t BOARD_InitMipiDsiClock(void)
{
    uint32_t desiredBitClk_Hz = 450000000U; /* 450 Mbps/lane */
    uint32_t escClkFreq_Hz;
    dsi_dphy_config_t dphyConfig;

    /* Attach and divide DPHY clock */
    CLOCK_AttachClk(kMEDIA_VDD2_BASE_to_MIPI_DSI_HOST_PHY);
    /* Attach and divide DPHY escape clock: target ~10 MHz (≤20 MHz).
     * MIMXRT798S has separate RX and TX escape clock dividers. */
    CLOCK_AttachClk(kMEDIA_VDD2_BASE_to_MIPI_DPHYESC_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivDphyEscRxClk, 10U); /* ~10 MHz */
    CLOCK_SetClkDiv(kCLOCK_DivDphyEscTxClk, 10U); /* ~10 MHz */

    escClkFreq_Hz = CLOCK_GetMipiDphyEscRxClkFreq();

    /* Compute D-PHY timing from desired bit clock and escape clock */
    DSI_GetDphyDefaultConfig(&dphyConfig, desiredBitClk_Hz, escClkFreq_Hz);

    /* Initialize D-PHY — returns actual bit clock achieved */
    uint32_t actualBitClk_Hz = DSI_InitDphy(DEMO_MIPI_DSI, &dphyConfig, 0U);
    PRINTF("MIPI DSI DPHY bit clock: %u Hz\r\n", actualBitClk_Hz);

    return actualBitClk_Hz;
}

/*
 * @brief Send DSI commands to initialize the ILI6122 bridge on the Waveshare panel.
 *
 * The ILI6122 is a DSI-to-DPI bridge — it accepts a small set of DCS commands
 * to configure the output interface. For the Waveshare 5inch panel the bridge
 * is pre-configured by hardware strapping and typically requires no software
 * initialization. We send a DCS soft-reset and sleep-out sequence to be safe.
 */
static status_t BOARD_InitPanel(void)
{
    status_t status = kStatus_Success;
    dsi_transfer_t xfer = {0};
    uint8_t txData[4];

    /* DCS soft reset */
    txData[0]          = 0x01U; /* DCS_SOFT_RESET */
    xfer.virtualChannel = 0U;
    xfer.txDataType    = kDSI_TxDataDcsShortWrNoParam;
    xfer.txData        = txData;
    xfer.txDataSize    = 1U;
    status = BOARD_DSI_Transfer(&xfer);
    if (kStatus_Success != status)
    {
        return status;
    }
    SDK_DelayAtLeastUs(120000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY); /* 120 ms */

    /* DCS sleep out */
    txData[0]          = 0x11U; /* DCS_EXIT_SLEEP_MODE */
    xfer.txDataType    = kDSI_TxDataDcsShortWrNoParam;
    xfer.txDataSize    = 1U;
    status = BOARD_DSI_Transfer(&xfer);
    if (kStatus_Success != status)
    {
        return status;
    }
    SDK_DelayAtLeastUs(120000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY); /* 120 ms */

    /* DCS display on */
    txData[0]          = 0x29U; /* DCS_SET_DISPLAY_ON */
    xfer.txDataType    = kDSI_TxDataDcsShortWrNoParam;
    xfer.txDataSize    = 1U;
    status = BOARD_DSI_Transfer(&xfer);

    return status;
}

/*
 * @brief Prepare the LCDIF + MIPI DSI display controller.
 *        Called by the MPP HAL before g_dc.ops->init().
 */
void BOARD_PrepareDisplayController(void)
{
    status_t status;
    uint32_t dsiHsBitClk_Hz;

    /* 1. Configure GPIO pins for panel control */
    BOARD_InitPanelGpio();

    /* 2. Configure MIPI DSI clocks — returns actual DPHY HS bit clock */
    dsiHsBitClk_Hz = BOARD_InitMipiDsiClock();

    /* 3. Initialize MIPI DSI host peripheral */
    dsi_config_t dsiConfig;
    DSI_GetDefaultConfig(&dsiConfig);
    dsiConfig.numLanes       = DEMO_MIPI_DSI_LANE_NUM;
    dsiConfig.enableNonContinuousHsClk = false;
    dsiConfig.autoInsertEoTp = true;
    DSI_Init(DEMO_MIPI_DSI, &dsiConfig);

    /* 4. Power on and reset the panel */
    BOARD_ResetPanel();

    /* 5. Send panel init commands over DSI LP mode */
    status = BOARD_InitPanel();
    if (kStatus_Success != status)
    {
        PRINTF("ERROR: Panel init failed: %d\r\n", (int)status);
    }

    /* 6. Switch DSI to HS video mode (LCDIF drives pixel stream).
     *
     * DSI_SetDpiConfig takes 5 args: base, config, numLanes,
     * dpiPixelClkFreq_Hz, dsiHsBitClkFreq_Hz.
     * vfp/vbp ARE valid fields of dsi_dpi_config_t (vertical porch lines).
     */
    dsi_dpi_config_t dpiConfig = {
        .pixelPayloadSize = DEMO_BUFFER_WIDTH,
        .dpiColorCoding   = kDSI_Dpi24Bit,
        .pixelPacket      = kDSI_PixelPacket24Bit,
        .videoMode        = kDSI_DpiBurst,
        .bllpMode         = kDSI_DpiBllpLowPower,
        .polarityFlags    = kDSI_DpiVsyncActiveLow | kDSI_DpiHsyncActiveLow,
        .hfp              = DEMO_HFP,
        .hbp              = DEMO_HBP,
        .hsw              = DEMO_HSW,
        .vfp              = DEMO_VFP,
        .vbp              = DEMO_VBP,
        .panelHeight      = DEMO_BUFFER_HEIGHT,
        .virtualChannel   = 0U,
    };
    DSI_SetDpiConfig(DEMO_MIPI_DSI, &dpiConfig, DEMO_MIPI_DSI_LANE_NUM,
                     CLOCK_GetLcdifClkFreq(), dsiHsBitClk_Hz);

    /* 7. Enable backlight */
    BOARD_SetPanelBacklight(true);
}

/* TE pin handler — not used by Waveshare 5inch (no TE signal), kept for API compat */
void BOARD_DisplayTEPinHandler(void)
{
}
