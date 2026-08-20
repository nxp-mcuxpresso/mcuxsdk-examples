/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "display_support.h"
#include "fsl_gpio.h"
#include "fsl_power.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_reset.h"
#if (DEMO_PANEL == DEMO_PANEL_LCD_PAR_S035)
#include "fsl_st7796s.h"
#include "fsl_dc_fb_dbi.h"
#include "fsl_dbi_dcif.h"
#elif (DEMO_PANEL == DEMO_PANEL_RK055MHD091A0)
#include "fsl_hx8394.h"
#include "fsl_dc_fb_dcif.h"
#include "fsl_mipi_dsi.h"
#include "fsl_clock.h"
#elif (DEMO_PANEL == DEMO_PANEL_LCM_RGB_5INCH)
#include "fsl_dc_fb_dcif.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Macros for DCIF and interrupt. */
#define DEMO_DCIF        MEDIA__DCIF
#define DEMO_DCIF_IRQn   MEDIA_DCIF_CH0_IRQn
#define DEMO_DCIF_DOMAIN 0

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

#if (DEMO_PANEL == DEMO_PANEL_LCD_PAR_S035)

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void BOARD_InitDcifPowerClockReset(void);
static void BOARD_ConfigDcifDbi(void);
static void BOARD_ResetPanel(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static dc_fb_dbi_handle_t s_dcDbiHandle;

const dc_fb_t g_dc = {
    .ops     = &g_dcFbOpsDbi,
    .prvData = &s_dcDbiHandle,
    .config  = NULL,
};

const st7796s_config_t st7796sConfig = {.driverPreset    = kST7796S_DriverPresetLCDPARS035,
                                        .pixelFormat     = kST7796S_PixelFormatRGB565,
                                        .orientationMode = kST7796S_Orientation270,
                                        .teConfig        = kST7796S_TEDisabled,
                                        .invertDisplay   = true,
                                        .flipDisplay     = true,
                                        .bgrFilter       = true};

static dbi_dcif_prv_data_t s_dcifPrvData = {
    .dcif   = MEDIA__DCIF,
    .domain = DEMO_DCIF_DOMAIN,
    .height = DEMO_PANEL_HEIGHT,
    .width  = DEMO_PANEL_WIDTH,
};

static st7796s_handle_t lcdHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void BOARD_InitDcifPowerClockReset(void)
{
    /* Enable DCIF clock gate. */
    CLOCK_EnableClock(kCLOCK_MEDIA_dcif);

    /* Pulse the DCIF peripheral reset. */
    RESET_PeripheralReset((reset_ip_name_t)kModCon_MEDIA_DCIF);
}

static void BOARD_ConfigDcifDbi(void)
{
    /* For DBI panel one frame has 480x320=153600 pixels, on D16-RGB565 interface each WR cycle
     * write one pixel. So for a 60Hz frame rate the WR frequency should be 153600x60=9.216MHz.
     * DBI uses media pll directly, calculate the half WR cycle period.
     */
    uint8_t wrCycle = (uint8_t)(CLOCK_GetRootClockFreq(kCLOCK_Root_MEDIA_mediapll_clk) /
        (DEMO_PANEL_WIDTH * DEMO_PANEL_HEIGHT * DEMO_FRAME_RATE));
    uint8_t srcPrd_Ns = (uint8_t)(1000000000U / CLOCK_GetRootClockFreq(kCLOCK_Root_MEDIA_mediapll_clk));

    dcif_dbi_config_t dbiConfig = {
        .format = kDCIF_DbiOutD16RGB565,
        .type        = kDCIF_DbiTypeB,
        .signalFlags = 0U,             /* TE disabled. */
        .wrHigh      = wrCycle / 2U,   /* Same high and low WR period. */
        .wrLow       = wrCycle / 2U,
        .csSetup     = (15U + srcPrd_Ns - 1U) / srcPrd_Ns, /* TCS shall >= 15ns, round up. */
        .csHold      = (10U + srcPrd_Ns - 1U) / srcPrd_Ns, /* TCSH shall >= 10ns, round up. */
    };

    /* DBI configuration. */
    DCIF_DbiSetConfig(DEMO_DCIF, &dbiConfig);

    /* Set up DCIF interrupt. */
    NVIC_ClearPendingIRQ(DEMO_DCIF_IRQn);
    NVIC_SetPriority(DEMO_DCIF_IRQn, 3);
    NVIC_EnableIRQ(DEMO_DCIF_IRQn);
}

static void BOARD_ResetPanel(void)
{
    const gpio_pin_config_t resetPinConfig = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 1,
    };

    /* Set LCD_PAR_S035 reset and backlight pins to output (as in dcif_1/basic). */
    GPIO_PinInit(BOARD_DBI_PANEL_LCD_RST_GPIO, BOARD_DBI_PANEL_LCD_RST_PIN, &resetPinConfig);
    GPIO_PinInit(BOARD_DBI_PANEL_BLK_GPIO, BOARD_DBI_PANEL_BLK_PIN, &resetPinConfig);

    /* Turn the backlight on -- without this the panel stays dark even when the
     * frame is transferred correctly. */
    GPIO_PinWrite(BOARD_DBI_PANEL_BLK_GPIO, BOARD_DBI_PANEL_BLK_PIN, 1);

    /* Reset the LCD_PAR_S035 LCD controller. */
    GPIO_PinWrite(BOARD_DBI_PANEL_LCD_RST_GPIO, BOARD_DBI_PANEL_LCD_RST_PIN, 0);
    VIDEO_DelayMs(1); /* Delay 1ms (10ns required). */
    GPIO_PinWrite(BOARD_DBI_PANEL_LCD_RST_GPIO, BOARD_DBI_PANEL_LCD_RST_PIN, 1);
    VIDEO_DelayMs(5); /* Delay 5ms. */
}

status_t BOARD_PrepareDisplayController(void)
{
    status_t status;

    /* 1. Setup DCIF clock/power/reset. */
    BOARD_InitDcifPowerClockReset();

    /* 2. Initialize DCIF. Software-reset the DCIF resgisters. */
    DCIF_Init(DEMO_DCIF);

    /* 3. Create the DCIF DBI XFER interface (configures DISP_SIZE / output). */
    DBI_DCIF_InitController(&(s_dcDbiHandle.dbiIface), &s_dcifPrvData);

    /* 4. Configure DCIF DBI mode. */
    BOARD_ConfigDcifDbi();

    /* 5. Enable the DCIF output BEFORE any DBI command is issued. */
    DCIF_EnableOutput(DEMO_DCIF, true);

    /* 6. Initialize panel. */
    BOARD_ResetPanel();

    status = ST7796S_Init(&lcdHandle, &st7796sConfig, &s_dcDbiHandle.dbiIface);
    if (kStatus_Success != status)
    {
        return status;
    }

    /* 7. Turn the display on (sends DISPON). */
    status = ST7796S_EnableDisplay(&lcdHandle, true);

    return status;
}

void MEDIA_DCIF_CH0_IRQHandler(void)
{
    DBI_DCIF_IRQHandler(&(s_dcDbiHandle.dbiIface));
}

#elif (DEMO_PANEL == DEMO_PANEL_RK055MHD091A0)

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_HSW 40
#define DEMO_HBP 50
#define DEMO_HFP 40
#define DEMO_VSW 2
#define DEMO_VBP 14
#define DEMO_VFP 16
#define DEMO_POL_FLAGS \
    (kDCIF_DpiDataEnableActiveHigh | kDCIF_DpiVsyncActiveLow | kDCIF_DpiHsyncActiveLow | \
     kDCIF_DpiDriveDataOnRisingClkEdge)

/* Definitions for MIPI. */
#define DEMO_MIPI_DSI               (&g_mipiDsi)
#define DEMO_MIPI_DSI_LANE_NUM      2
#define DEMO_MIPI_DSI_BIT_PER_PIXEL 24

#define DEMO_MIPI_DPHY_BIT_CLK_ENLARGE(origin) (((origin) / 96) * 100)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void BOARD_PullPanelResetPin(bool pullUp);
static void BOARD_PullPanelPowerPin(bool pullUp);
static status_t BOARD_DSI_Transfer(dsi_transfer_t *xfer);
static void BOARD_InitDcifPowerClockReset(void);
static void BOARD_DsiHostPhyInit(void);
static status_t BOARD_InitLcdPanel(void);
static void BOARD_DsiVideoModeConfig(void);

/*******************************************************************************
 * Variables mark
 ******************************************************************************/
const MIPI_DSI_Type g_mipiDsi = {
    .host = DSI2_HOST,
    .dpi  = DSI2_HOST_VID_IF,
    .dbi  = DSI2_HOST_DBI_IF,
    .apb  = DSI2_HOST_APB_PKT_IF,
    .dphy = DSI2_TX_PHY,
};

static uint32_t mipiDsiDpiClkFreq_Hz;
static uint32_t mipiDsiTxEscClkFreq_Hz;
static uint32_t mipiDsiRxEscClkFreq_Hz;
static uint32_t mipiDsiDphyBitClkFreq_Hz;
static uint32_t mipiDsiDphyRefClkFreq_Hz;

static mipi_dsi_device_t dsiDevice = {
    .virtualChannel = 0,
    .xferFunc       = BOARD_DSI_Transfer,
};

static const hx8394_resource_t hx8394Resource = {
    .dsiDevice    = &dsiDevice,
    .pullResetPin = BOARD_PullPanelResetPin,
    .pullPowerPin = BOARD_PullPanelPowerPin,
};

static display_handle_t hx8394Handle = {
    .resource = &hx8394Resource,
    .ops      = &hx8394_ops,
};

static dc_fb_dcif_handle_t s_dcFbDcifHandle;

static const dc_fb_dcif_config_t s_dcFbDcifConfig = {
    .dcif          = DEMO_DCIF,
    .width         = DEMO_PANEL_WIDTH,
    .height        = DEMO_PANEL_HEIGHT,
    .hsw           = DEMO_HSW,
    .hfp           = DEMO_HFP,
    .hbp           = DEMO_HBP,
    .vsw           = DEMO_VSW,
    .vfp           = DEMO_VFP,
    .vbp           = DEMO_VBP,
    .polarityFlags = DEMO_POL_FLAGS,
    .domain        = DEMO_DCIF_DOMAIN,
};

const dc_fb_t g_dc = {
    .ops     = &g_dcFbOpsDcif,
    .prvData = &s_dcFbDcifHandle,
    .config  = &s_dcFbDcifConfig,
};

/*******************************************************************************
 * Code
 ******************************************************************************/
static void BOARD_PullPanelResetPin(bool pullUp)
{
    pcal6524_handle_t *h = BOARD_GetPCAL6524Handle();
    const uint32_t mask  = 1U << BOARD_PCAL6524_LCD_RST_B;
    if (pullUp)
    {
        (void)PCAL6524_SetPins(h, mask);
    }
    else
    {
        (void)PCAL6524_ClearPins(h, mask);
    }
}

static void BOARD_PullPanelPowerPin(bool pullUp)
{
    pca9555_handle_t *h = BOARD_GetPCA9555Handle();
    const uint32_t mask  = 1U << BOARD_PCA9555_LCM_PWR_EN2;
    if (pullUp)
    {
        (void)PCA9555_SetPins(h, mask);
    }
    else
    {
        (void)PCA9555_ClearPins(h, mask);
    }
}

static status_t BOARD_DSI_Transfer(dsi_transfer_t *xfer)
{
    return DSI_TransferBlocking(DEMO_MIPI_DSI, xfer);
}

static void BOARD_InitDcifPowerClockReset(void)
{
    /* Pulse the DCIF peripheral reset. */
    RESET_PeripheralReset((reset_ip_name_t)kModCon_MEDIA_DCIF);
    /* Enable DCIF peripheral clock. */
    CLOCK_EnableClock(kCLOCK_MEDIA_dcif);

    /* Pulse the MIPI-DSI peripheral reset. */
    RESET_PeripheralReset((reset_ip_name_t)kModCon_MEDIA_MIPI_DSI);
    /* Enable MIPI-DSI peripheral clock. */
    CLOCK_EnableClock(kCLOCK_MEDIA_mipi_dsi);

    clock_root_config_t rootCfg = {0};

    /* For RK055MHD091 panel, one frame is (720+6+12+24)x(1280+2+16+14)=762x1312=999744 pixel
     * To achieve about 50Hz frame rate, the pixel clock shall be 999744x50=49987200 about 50mHz
     */
    uint32_t pixClkPerFrame = (DEMO_PANEL_WIDTH + DEMO_HSW + DEMO_HFP + DEMO_HBP) *
                                (DEMO_PANEL_HEIGHT + DEMO_VSW + DEMO_VFP + DEMO_VBP);

    rootCfg.mux = kCLOCK_DCPIXEL_ClockRoot_PERI5;
    rootCfg.div = (CLOCK_GetClockSrcFreq(kCLOCK_SRC_PERI5) + (pixClkPerFrame * DEMO_FRAME_RATE) / 2U) /
        (pixClkPerFrame * DEMO_FRAME_RATE);
    rootCfg.sndDiv = 1U;
    CLOCK_SetRootClock(kCLOCK_Root_MEDIA_dcpixel_fclk, &rootCfg);
    CLOCK_PowerOnRootClock(kCLOCK_Root_MEDIA_dcpixel_fclk);

    mipiDsiDpiClkFreq_Hz = CLOCK_GetRootClockFreq(kCLOCK_Root_MEDIA_dcpixel_fclk);

    /* DPHY high speed clock should be >= (pix_clk * bits_per_pixel / lane_num)
     * Enlarge the value to be sure. */
    mipiDsiDphyBitClkFreq_Hz = (mipiDsiDpiClkFreq_Hz * DEMO_MIPI_DSI_BIT_PER_PIXEL / DEMO_MIPI_DSI_LANE_NUM) / 96U * 100U;

    /* MIPI-DSI DPHY escape clock:
     *   - RxClkEsc 100-300 MHz. Set to 200MHz.
     *   - TxClkEsc 12-20 MHz. Set to 20MHz
     */
    rootCfg.mux    = kCLOCK_MIPIDSI_ESCCLK_ClockRoot_PERI5;
    rootCfg.div    = 2U;
    rootCfg.sndDiv = 10U;
    CLOCK_SetRootClock(kCLOCK_Root_MEDIA_mipidsi_escclk_divided, &rootCfg);
    CLOCK_PowerOnRootClock(kCLOCK_Root_MEDIA_mipidsi_escclk_divided);

    mipiDsiRxEscClkFreq_Hz = CLOCK_GetRootClockFreq(kCLOCK_Root_MEDIA_mipidsi_escclk_divided) * rootCfg.sndDiv;
    mipiDsiTxEscClkFreq_Hz = CLOCK_GetRootClockFreq(kCLOCK_Root_MEDIA_mipidsi_escclk_divided);

    /* MIPI-DSI reference clock fixed to be 24MHz, use for DPHY generation. */
    CLOCK_PowerOnRootClock(kCLOCK_Root_MEDIA_mipidsi_refclk);

    mipiDsiDphyRefClkFreq_Hz = CLOCK_GetRootClockFreq(kCLOCK_Root_MEDIA_mipidsi_refclk);
}

static status_t BOARD_InitLcdPanel(void)
{
    const gpio_pin_config_t bl = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 1U,
    };

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

    pcal6524_handle_t *hExp = BOARD_GetPCAL6524Handle();
    pca9555_handle_t  *hPwr = BOARD_GetPCA9555Handle();

    /* Enable reset, power, backlight pins. */
    (void)PCAL6524_SetDirection(hExp, 1U << BOARD_PCAL6524_LCD_RST_B, kPCAL6524_Output);
    (void)PCA9555_SetDirection(hPwr, 1U << BOARD_PCA9555_LCM_PWR_EN2, kPCA9555_Output);
    BOARD_PullPanelResetPin(false);
    BOARD_PullPanelPowerPin(true);
    GPIO_PinInit(BOARD_MIPI_PANEL_BL_GPIO, BOARD_MIPI_PANEL_BL_PIN, &bl);

    return HX8394_Init(&hx8394Handle, &displayConfig);
}

static void BOARD_DsiHostPhyInit(void)
{
    dsi_config_t config;
    DSI_GetDefaultConfig(&config);
    DSI_Init(DEMO_MIPI_DSI, &config);

    dsi_dphy_config_t dphyConfig;
    DSI_GetDphyDefaultConfig(&dphyConfig, mipiDsiDphyBitClkFreq_Hz, mipiDsiTxEscClkFreq_Hz, mipiDsiRxEscClkFreq_Hz);
    DSI_InitDphy(DEMO_MIPI_DSI, &dphyConfig, mipiDsiDphyRefClkFreq_Hz);
}

static void BOARD_DsiVideoModeConfig(void)
{
    dsi_dpi_config_t config = {
        .enable            = true,
        .virtualChannel    = 0U,
        .pixelPacketFormat = kDSI_DpiPixelPacket24Bit,
        .videoMode         = kDSI_DpiBurst,
        .polarityFlags     = kDSI_DpiHsyncActiveLow | kDSI_DpiVsyncActiveLow,
        .bllpMode          = kDSI_DpiBllpVerticalLowPower | kDSI_DpiBllpHorizontalLowPower,
        .overrideTiming    = false, /* Use auto timing detection, no need to set the sync params. */
        .pixelPerPacket    = DEMO_PANEL_WIDTH,
        .panelHeight       = DEMO_PANEL_HEIGHT,
        .hsw               = DEMO_HSW,
        .hfp               = DEMO_HFP,
        .hbp               = DEMO_HBP,
        .vbp               = DEMO_VBP,
        .vfp               = DEMO_VFP,
    };

    DSI_SetDpiConfig(DEMO_MIPI_DSI, &config);
}

status_t BOARD_PrepareDisplayController(void)
{
    status_t status;

    /* 1. Setup clock/power/reset for DCIF and MIPI-DSI. */
    BOARD_InitDcifPowerClockReset();

    /* 2. Init the DSI host and D-PHY. */
    BOARD_DsiHostPhyInit();

    /* 3. Configure the panel (sends DCS init sequence over DSI). */
    status = BOARD_InitLcdPanel();

    /* 4. Configure the DSI DPI (video mode) interface. */
    BOARD_DsiVideoModeConfig();

    if (kStatus_Success == status)
    {
        NVIC_SetPriority(MEDIA_DCIF_CH0_IRQn, 3);
        EnableIRQ(MEDIA_DCIF_CH0_IRQn);
    }

    return kStatus_Success;
}

void MEDIA_DCIF_CH0_IRQHandler(void)
{
    DC_FB_DCIF_IRQHandler(&g_dc);
}

#elif (DEMO_PANEL == DEMO_PANEL_LCM_RGB_5INCH)

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_HSW 4
#define DEMO_HFP 8
#define DEMO_HBP 8
#define DEMO_VSW 4
#define DEMO_VFP 8
#define DEMO_VBP 8
#define DEMO_POL_FLAGS \
    (kDCIF_DpiDataEnableActiveHigh | kDCIF_DpiVsyncActiveLow | kDCIF_DpiHsyncActiveLow | \
        kDCIF_DpiDriveDataOnRisingClkEdge)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void BOARD_InitDcifPowerClockReset(void);
static void BOARD_InitDisplayInterface(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static dc_fb_dcif_handle_t s_dcFbDcifHandle = {0}; /* The handle must be initialized to 0. */

const dc_fb_dcif_config_t s_dcFbDcifConfig = {
    .dcif          = DEMO_DCIF,
    .width         = DEMO_PANEL_WIDTH,
    .height        = DEMO_PANEL_HEIGHT,
    .hsw           = DEMO_HSW,
    .hfp           = DEMO_HFP,
    .hbp           = DEMO_HBP,
    .vsw           = DEMO_VSW,
    .vfp           = DEMO_VFP,
    .vbp           = DEMO_VBP,
    .polarityFlags = DEMO_POL_FLAGS,
    .domain        = DEMO_DCIF_DOMAIN,
    .output        = kDCIF_DpiRGB888,
};

const dc_fb_t g_dc = {
    .ops     = &g_dcFbOpsDcif,
    .prvData = &s_dcFbDcifHandle,
    .config  = &s_dcFbDcifConfig,
};

/*******************************************************************************
 * Code
 ******************************************************************************/
static void BOARD_InitDcifPowerClockReset(void)
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

static void BOARD_InitDisplayInterface(void)
{
    pcal6524_handle_t *power_h = BOARD_GetPCAL6524Handle();
    pca9555_handle_t *back_light_h = BOARD_GetPCA9555Handle();

    (void)PCAL6524_SetDirection(power_h, 1U << BOARD_PCAL6524_LCM_PWR_EN1, kPCAL6524_Output);
    (void)PCAL6524_SetPins(power_h, 1U << BOARD_PCAL6524_LCM_PWR_EN1);
    (void)PCA9555_SetDirection(back_light_h, 1U << BOARD_PCA9555_BL_EN_RGB, kPCA9555_Output);
    (void)PCA9555_SetPins(back_light_h, 1U << BOARD_PCA9555_BL_EN_RGB);
}

status_t BOARD_PrepareDisplayController(void)
{
    BOARD_InitDcifPowerClockReset();

    BOARD_InitDisplayInterface();

    NVIC_SetPriority(MEDIA_DCIF_CH0_IRQn, 3);
    EnableIRQ(MEDIA_DCIF_CH0_IRQn);

    return kStatus_Success;
}

void MEDIA_DCIF_CH0_IRQHandler(void)
{
    DC_FB_DCIF_IRQHandler(&g_dc);
}

#endif
