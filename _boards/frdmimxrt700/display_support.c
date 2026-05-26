/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "display_support.h"
#include "fsl_gpio.h"
#if (DEMO_PANEL_RASPI_7INCH == DEMO_PANEL)
#include "fsl_dc_fb_lcdif.h"
#include "fsl_xspi.h"
#include "fsl_rpi.h"
#include "fsl_mipi_dsi.h"
#elif (DEMO_PANEL_CO5300 == DEMO_PANEL)
#include "fsl_dc_fb_dbi.h"
#include "fsl_dbi_lcdif.h"
#include "fsl_co5300.h"
#include "fsl_mipi_dsi.h"
#endif
#include "fsl_power.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
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

#if (DEMO_PANEL_RASPI_7INCH == DEMO_PANEL)

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

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define DEMO_LCDIF_HSW 20
#define DEMO_LCDIF_HFP 70
#define DEMO_LCDIF_HBP 23
#define DEMO_LCDIF_VSW 2
#define DEMO_LCDIF_VFP 7
#define DEMO_LCDIF_VBP 21

#define DEMO_LCDIF_POL_FLAGS \
    (kLCDIF_DataEnableActiveHigh | kLCDIF_VsyncActiveLow | kLCDIF_HsyncActiveLow | kLCDIF_DriveDataOnRisingClkEdge)

#define DEMO_LCDIF    LCDIF

/* Definitions for MIPI. */
#define DEMO_MIPI_DSI          MIPI_DSI_HOST
#define DEMO_MIPI_DSI_LANE_NUM 1
#define DEMO_MIPI_DSI_BIT_PER_PIXEL 24

/* Here the desired DPHY bit clock multiplied by ( 9 / 8 = 1.125) to ensure
 * it is fast enough.
 */
#define DEMO_MIPI_DPHY_BIT_CLK_ENLARGE(origin) (((origin) / 8) * 9)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static status_t BOARD_ReadPanelStatus(uint8_t regAddr, uint8_t *status);
static status_t BOARD_WritePanelRegister(uint8_t regAddr, uint8_t value);
static void BOARD_InitLcdifClock(void);
static void BOARD_InitMipiDsiClock(void);
static status_t BOARD_DSI_Transfer(dsi_transfer_t *xfer);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint32_t mipiDsiTxEscClkFreq_Hz;
static uint32_t mipiDsiDphyBitClkFreq_Hz;
static uint32_t mipiDsiDpiClkFreq_Hz;

static mipi_dsi_device_t dsiDevice = {
    .virtualChannel = 0,
    .xferFunc       = BOARD_DSI_Transfer,
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

static dc_fb_lcdif_handle_t s_dcFbLcdifHandle;

static const dc_fb_lcdif_config_t s_dcFbLcdifConfig = {
    .lcdif         = DEMO_LCDIF,
    .width         = DEMO_PANEL_WIDTH,
    .height        = DEMO_PANEL_HEIGHT,
    .hsw           = DEMO_LCDIF_HSW,
    .hfp           = DEMO_LCDIF_HFP,
    .hbp           = DEMO_LCDIF_HBP,
    .vsw           = DEMO_LCDIF_VSW,
    .vfp           = DEMO_LCDIF_VFP,
    .vbp           = DEMO_LCDIF_VBP,
    .polarityFlags = DEMO_LCDIF_POL_FLAGS,
    .outputFormat  = kLCDIF_Output24Bit,
};

const dc_fb_t g_dc = {
    .ops     = &g_dcFbOpsLcdif,
    .prvData = &s_dcFbLcdifHandle,
    .config  = &s_dcFbLcdifConfig,
};

/*******************************************************************************
 * Code
 ******************************************************************************/
static status_t BOARD_ReadPanelStatus(uint8_t regAddr, uint8_t *value)
{
    return BOARD_I2C_Receive(BOARD_MIPI_PANEL_TOUCH_I2C_BASEADDR, RPI_ADDR, regAddr, 1U, value, 1U);
}

static status_t BOARD_WritePanelRegister(uint8_t regAddr, uint8_t value)
{
    return BOARD_I2C_Send(BOARD_MIPI_PANEL_TOUCH_I2C_BASEADDR, RPI_ADDR, regAddr, 1, &value, 1);
}

static status_t BOARD_DSI_Transfer(dsi_transfer_t *xfer)
{
    return DSI_TransferBlocking(DEMO_MIPI_DSI, xfer);
}

static void BOARD_InitLcdifClock(void)
{
    POWER_DisablePD(kPDRUNCFG_SHUT_MEDIA_MAINCLK);
    POWER_DisablePD(kPDRUNCFG_APD_LCDIF);
    POWER_DisablePD(kPDRUNCFG_PPD_LCDIF);
    POWER_ApplyPD();

    /*
     * The pixel clock is (height + VSW + VFP + VBP) * (width + HSW + HFP + HBP) * frame rate.
     * Here use the main pll (528MHz) as clock source.
     * For RaspberryPi panel, the frame rate shall be 42fps, which is 19.56MHz pixel clock.
     */
    CLOCK_AttachClk(kMAIN_PLL_PFD2_to_LCDIF);
    CLOCK_SetClkDiv(kCLOCK_DivLcdifClk, 27);

    /* Get lcdif pixel clock frequency. */
    mipiDsiDpiClkFreq_Hz = CLOCK_GetLcdifClkFreq();

    CLOCK_EnableClock(kCLOCK_Lcdif);
    RESET_ClearPeripheralReset(kLCDIF_RST_SHIFT_RSTn);
}

static void BOARD_InitMipiDsiClock(void)
{
    uint8_t div;

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

    div = (uint8_t)((uint64_t)CLOCK_GetAudioPllFreq() * 18U / (uint64_t)mipiDsiDphyBitClkFreq_Hz);

    CLOCK_InitAudioPfd(kCLOCK_Pfd2, div);

    CLOCK_AttachClk(kAUDIO_PLL_PFD2_to_MIPI_DSI_HOST_PHY);

    CLOCK_SetClkDiv(kCLOCK_DivDphyClk, 1);

    mipiDsiDphyBitClkFreq_Hz = CLOCK_GetMipiDphyClkFreq();
}

static status_t BOARD_InitLcdPanel(void)
{
    status_t status;

    const display_config_t displayConfig = {
        .resolution   = FSL_VIDEO_RESOLUTION(DEMO_PANEL_WIDTH, DEMO_PANEL_HEIGHT),
        .hsw          = DEMO_LCDIF_HSW,
        .hfp          = DEMO_LCDIF_HFP,
        .hbp          = DEMO_LCDIF_HBP,
        .vsw          = DEMO_LCDIF_VSW,
        .vfp          = DEMO_LCDIF_VFP,
        .vbp          = DEMO_LCDIF_VBP,
        .controlFlags = 0,
        .dsiLanes     = DEMO_MIPI_DSI_LANE_NUM,
    };

    BOARD_MIPIPanelTouch_I2C_Init();

    status = RPI_Init(&rpiHandle, &displayConfig);

    return status;
}

static void BOARD_InitMipiDsiConfig(void)
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
        .hfp              = DEMO_LCDIF_HFP,
        .hbp              = DEMO_LCDIF_HBP,
        .hsw              = DEMO_LCDIF_HSW,
        .vfp              = DEMO_LCDIF_VFP,
        .vbp              = DEMO_LCDIF_VBP,
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

static status_t BOARD_InitDisplayInterface(void)
{
    /* 1. Setup clock. */
    BOARD_InitMipiDsiClock();

    /* 2. Configures peripheral. */
    BOARD_InitMipiDsiConfig();

    /* 3. Configure the panel. */
    return BOARD_InitLcdPanel();
}

void LCDIF_IRQHandler(void)
{
    DC_FB_LCDIF_IRQHandler(&g_dc);
}

/*
 * With the default configuration, when frame buffer is placed in PSRAM,
 * LCDIF might underflow if some other masters are writing to PSRAM.
 * This function improve the LCDIF bandwidth to make sure it doesn't underflow.
 */
static void BOARD_ImproveLcdifBandwidth(void)
{
/* Allocate 2K AHB buffer for LCDIF. */
#define XSPI_LCDIF_AHB_BUF_SIZE (2 * 1024)

    xspi_ahbBuffer_config_t lcdifBufferConfig = {
        .bufferSize            = XSPI_LCDIF_AHB_BUF_SIZE / 8,
        .enaPri.enablePriority = false, /* Must use false for A0 chip. No limitaion for B0 chip. */
        .masterId              = 0x1dU, /* LCDIF */
        .ptrSubBuffer0Config   = NULL,
        .ptrSubBuffer1Config   = NULL,
        .ptrSubBuffer2Config   = NULL,
        .ptrSubBuffer3Config   = NULL,
    };
    xspi_ahbBuffer_config_t buffer1Config = {
        .bufferSize            = 0,
        .enaPri.enablePriority = false,
        .masterId              = 0xFFU, /* Not used. */
        .ptrSubBuffer0Config   = NULL,
        .ptrSubBuffer1Config   = NULL,
        .ptrSubBuffer2Config   = NULL,
        .ptrSubBuffer3Config   = NULL,
    };
    xspi_ahbBuffer_config_t buffer2Config = {
        .bufferSize            = 0,
        .enaPri.enablePriority = false,
        .masterId              = 0xFFU, /* Not used. */
        .ptrSubBuffer0Config   = NULL,
        .ptrSubBuffer1Config   = NULL,
        .ptrSubBuffer2Config   = NULL,
        .ptrSubBuffer3Config   = NULL,
    };
    xspi_ahbBuffer_config_t buffer3Config = {
        .bufferSize = ((4 * 1024) - XSPI_LCDIF_AHB_BUF_SIZE) / 8, /* Totally 4K, Left part shared by other masters */
        .enaPri.enableAllMaster = true,
        .masterId               = 0x0U,
        .ptrSubBuffer0Config    = NULL,
        .ptrSubBuffer1Config    = NULL,
        .ptrSubBuffer2Config    = NULL,
        .ptrSubBuffer3Config    = NULL,
    };

    if (XSPI_SetAhbBufferConfig(XSPI2, &lcdifBufferConfig, &buffer1Config, &buffer2Config, &buffer3Config) !=
        kStatus_Success)
    {
        PRINTF("XSPI AHB buffer reconfiguration failed.\r\n");
        while (1)
        {
        }
    }

    /* Let LCDIF read has higher priority from MEDIA AXI_SWITCH */
    NIC_MEDIA1->ASIB[7].READ_QOS = NIC_READ_QOS_READ_QOS(1);
}

status_t BOARD_PrepareDisplayController(void)
{
    status_t status;

    BOARD_ImproveLcdifBandwidth();

    BOARD_InitLcdifClock();

    status = BOARD_InitDisplayInterface();

    if (kStatus_Success != status)
    {
        PRINTF("ERROR: Display interface initialization failed\r\n");
    }

    NVIC_SetPriority(LCDIF_IRQn, 3);
    EnableIRQ(LCDIF_IRQn);

    return status;
}

#elif (DEMO_PANEL_CO5300 == DEMO_PANEL)

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Definitions for MIPI. */
#define DEMO_MIPI_DSI          MIPI_DSI_HOST
#define DEMO_MIPI_DSI_LANE_NUM 1

/* Macros for LCDIF and interrupt. */
#define DEMO_DBI_LCDIF      LCDIF
#define DEMO_DBI_LCDIF_IRQn LCDIF_IRQn

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void BOARD_PullPanelResetPin(bool pullUp);
static void BOARD_PullPanelPowerPin(bool pullUp);
static void BOARD_InitMipiDsiClock(void);
static void BOARD_InitMipiDsi(void);
static status_t BOARD_DSI_Transfer(dsi_transfer_t *xfer);
static void BOARD_InitMipiPanelTEPin(void);
static void BOARD_InitLcdifPowerReset(void);
static void BOARD_InitLcdif(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint32_t mipiDsiTxEscClkFreq_Hz;
static uint32_t mipiDsiDphyBitClkFreq_Hz;

static mipi_dsi_device_t dsiDevice = {
    .virtualChannel = 0,
    .xferFunc       = BOARD_DSI_Transfer,
};

static const co5300_resource_t co5300Resource = {
    .dsiDevice    = &dsiDevice,
    .pullResetPin = BOARD_PullPanelResetPin,
    .pullPowerPin = BOARD_PullPanelPowerPin,
};

static display_handle_t co5300Handle = {
    .resource = &co5300Resource,
    .ops      = &co5300_ops,
};

static display_config_t displayConfig = {
    .resolution   = FSL_VIDEO_RESOLUTION(DEMO_PANEL_WIDTH, DEMO_PANEL_HEIGHT),
    .hsw          = 0,
    .hfp          = 0,
    .hbp          = 0,
    .vsw          = 0,
    .vfp          = 0,
    .vbp          = 0,
    .controlFlags = 0,
    .dsiLanes     = DEMO_MIPI_DSI_LANE_NUM,
    .pixelFormat  = DEMO_BUFFER_PIXEL_FORMAT,
};

static dbi_lcdif_prv_data_t s_lcdifPrvData;

static dc_fb_dbi_handle_t s_dcDbiHandle = {
    .useTEPin = true,
};

const dc_fb_t g_dc = {
    .ops     = &g_dcFbOpsDbi,
    .prvData = &s_dcDbiHandle,
    .config  = NULL,
};

/*******************************************************************************
 * Code
 ******************************************************************************/

static void BOARD_PullPanelResetPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_MIPI_RST_GPIO, BOARD_MIPI_RST_PIN, 1);
    }
    else
    {
        GPIO_PinWrite(BOARD_MIPI_RST_GPIO, BOARD_MIPI_RST_PIN, 0);
    }
}

static void BOARD_PullPanelPowerPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_MIPI_POWER_GPIO, BOARD_MIPI_POWER_PIN, 1);
    }
    else
    {
        GPIO_PinWrite(BOARD_MIPI_POWER_GPIO, BOARD_MIPI_POWER_PIN, 0);
    }
}

static status_t BOARD_DSI_Transfer(dsi_transfer_t *xfer)
{
    return DSI_TransferBlocking(DEMO_MIPI_DSI, xfer);
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

    /* When using LCDIF, with 279.53MHz DBI source clock and 16bpp format, a 14 cycle period requires a 279.53MHz / 14 *
       16 = 319.46Mhz DPHY clk source. Considering the DCS packaging cost, the MIPI DPHY speed shall be ***SLIGHTLY***
       larger than the DBI interface speed. DPHY uses AUDIO_PLL_PFD2 which is 532.48MHz as source, the frequency is
       532.48 * 18 / 30 = 319.49MHz, which meets the requirement. */
    CLOCK_InitAudioPfd(kCLOCK_Pfd2, 30);

    CLOCK_AttachClk(kAUDIO_PLL_PFD2_to_MIPI_DSI_HOST_PHY);

    CLOCK_SetClkDiv(kCLOCK_DivDphyClk, 1);
    mipiDsiDphyBitClkFreq_Hz = CLOCK_GetMipiDphyClkFreq();
}

static void BOARD_InitMipiDsi(void)
{
    dsi_config_t dsiConfig;
    dsi_dphy_config_t dphyConfig;

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
    dsiConfig.numLanes       = DEMO_MIPI_DSI_LANE_NUM;
    dsiConfig.autoInsertEoTp = true;

    DSI_GetDphyDefaultConfig(&dphyConfig, mipiDsiDphyBitClkFreq_Hz, mipiDsiTxEscClkFreq_Hz);

    /* Init the DSI module. */
    DSI_Init(DEMO_MIPI_DSI, &dsiConfig);

    /* Init DPHY. There is not DPHY PLL, the ref clock is not used. */
    DSI_InitDphy(DEMO_MIPI_DSI, &dphyConfig, 0);

    DSI_SetDbiPixelFormat(DEMO_MIPI_DSI, kDSI_DbiRGB565);
}

static status_t BOARD_InitLcdPanel(void)
{
    const gpio_pin_config_t pinConfig = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 0,
    };

    GPIO_PinInit(BOARD_MIPI_POWER_GPIO, BOARD_MIPI_POWER_PIN, &pinConfig);
    GPIO_PinInit(BOARD_MIPI_RST_GPIO, BOARD_MIPI_RST_PIN, &pinConfig);

    BOARD_InitMipiPanelTEPin();

    return kStatus_Success;
}

static void BOARD_InitMipiPanelTEPin(void)
{
    const gpio_pin_config_t tePinConfig = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic  = 0,
    };

    /*
     * TE pin configure method:
     *
     * The TE pin interrupt is like this:
     *
     *            VSYNC
     *         +--------+
     *         |        |
     *         |        |
     * --------+        +----------------
     *
     * 1. If one frame send time is shorter than one frame refresh time, then set
     *    TE pin interrupt at the start of VSYNC.
     * 2. If one frame send time is longer than one frame refresh time, and shorter
     *    than two frames refresh time, then set TE pin interrupt at the end of VSYNC.
     * 3. If one frame send time is longer than two frame refresh time, tearing effect
     *    could not be removed.
     *
     * For RM67162 @60Hz frame rate, frame refresh time is 16.7 ms. After test,
     * one frame send time is shorter than one frame refresh time. So TE interrupt is
     * set to start of VSYNC.
     */

    GPIO_PinInit(BOARD_MIPI_TE_GPIO, BOARD_MIPI_TE_PIN, &tePinConfig);

    GPIO_SetPinInterruptConfig(BOARD_MIPI_TE_GPIO, BOARD_MIPI_TE_PIN, kGPIO_InterruptRisingEdge);
    GPIO_SetPinInterruptChannel(BOARD_MIPI_TE_GPIO, BOARD_MIPI_TE_PIN, kGPIO_InterruptOutput0);

    NVIC_SetPriority(BOARD_MIPI_TE_GPIO_IRQn, 3);
    EnableIRQ(BOARD_MIPI_TE_GPIO_IRQn);
}

static void BOARD_InitLcdifPowerReset(void)
{
    POWER_DisablePD(kPDRUNCFG_SHUT_MEDIA_MAINCLK);
    POWER_DisablePD(kPDRUNCFG_APD_LCDIF);
    POWER_DisablePD(kPDRUNCFG_PPD_LCDIF);
    POWER_ApplyPD();

    CLOCK_EnableClock(kCLOCK_Lcdif);
    RESET_ClearPeripheralReset(kLCDIF_RST_SHIFT_RSTn);
}

static void BOARD_InitLcdif(void)
{
    lcdif_dbi_config_t dbiConfig;
    lcdif_panel_config_t config;

    /* DBI uses MEDIA MAIN clock. Change the PFD2 divide to 17. Then the DBI source frequency shall be 528 * 18 / 17 / 2
     * = 279.53MHz. */
    CLOCK_InitMainPfd(kCLOCK_Pfd2, 17);
    CLOCK_SetClkDiv(kCLOCK_DivMediaMainClk, 2U);
    CLOCK_AttachClk(kMAIN_PLL_PFD2_to_MEDIA_MAIN);

    /* DBI configurations. */
    LCDIF_DbiModeGetDefaultConfig(&dbiConfig);
    dbiConfig.acTimeUnit      = 0;
#if (defined(FSL_FEATURE_LCDIF_HAS_DBIX_POLARITY) && FSL_FEATURE_LCDIF_HAS_DBIX_POLARITY)
    dbiConfig.reversePolarity = true;
#endif
    dbiConfig.writeWRPeriod   = 14U;
    /* With 279.53MHz source and 16bpp format, a 14 cycle period requires a 279.53MHz / 14 * 16 = 319.46Mhz DPHY clk
     * source. */
    if (DEMO_BUFFER_PIXEL_FORMAT == kVIDEO_PixelFormatRGB565)
    {
        dbiConfig.format = kLCDIF_DbiOutD16RGB565;
    }
    else
    {
        dbiConfig.format = kLCDIF_DbiOutD16RGB888Option1;
    }
    dbiConfig.type            = kLCDIF_DbiTypeB;
    dbiConfig.writeCSAssert   = 1;
    dbiConfig.writeCSDeassert = 4;
    dbiConfig.writeWRAssert   = (dbiConfig.writeWRPeriod - 1U) / 2U; /* Asset at the middle. */
    dbiConfig.writeWRDeassert = (dbiConfig.writeWRPeriod - 1U);      /* Deassert at the end */

    LCDIF_Init(DEMO_DBI_LCDIF);
    LCDIF_DbiModeSetConfig(DEMO_DBI_LCDIF, 0, &dbiConfig);

    LCDIF_PanelGetDefaultConfig(&config);
    /* For ZC143AC72MIPI panel, when the source pixel format is RGB565, the endian shall be swapped and the component order on bus shall be BGR
       to show the piture properly. */
    if (DEMO_BUFFER_PIXEL_FORMAT == kVIDEO_PixelFormatRGB565)
    {
        config.endian = kLCDIF_HalfWordSwap;
    }

    LCDIF_SetPanelConfig(DEMO_DBI_LCDIF, 0, &config);

    NVIC_ClearPendingIRQ(DEMO_DBI_LCDIF_IRQn);
    NVIC_SetPriority(DEMO_DBI_LCDIF_IRQn, 3);
    NVIC_EnableIRQ(DEMO_DBI_LCDIF_IRQn);
}

status_t BOARD_PrepareDisplayController(void)
{
    /* 1. Initialize LCDIF and MIPI-DSI. */
    /* Initialize clock, power and reset. */
    BOARD_InitMipiDsiClock();

    BOARD_InitLcdifPowerReset();

    /* Configures peripheral. */
    BOARD_InitMipiDsi();

    BOARD_InitLcdif();

    /* 2. Create the LCDIF DBI XFER interface. */
    DBI_LCDIF_InitController(&(s_dcDbiHandle.dbiIface), &s_lcdifPrvData, DEMO_DBI_LCDIF, DEMO_MIPI_DSI);

    /* 3. Initialize the panel. */
    BOARD_InitLcdPanel();
    return DISPLAY_Init(&co5300Handle, &displayConfig);
}

void LCDIF_IRQHandler(void)
{
    DBI_LCDIF_IRQHandler(&(s_dcDbiHandle.dbiIface));
}

/* Smart panel TE pin IRQ handler. */
void BOARD_DisplayTEPinHandler(void)
{
    DC_FB_DBI_TE_IRQHandler(&g_dc);
}

#endif
