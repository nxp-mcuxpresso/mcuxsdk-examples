/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "display_support.h"
#include "fsl_gpio.h"
#include "fsl_mipi_dsi.h"
#if (DEMO_PANEL_RK055AHD091 == DEMO_PANEL)
#include "fsl_rm68200.h"
#elif (DEMO_PANEL_RK055IQH091 == DEMO_PANEL)
#include "fsl_rm68191.h"
#elif (DEMO_PANEL_RK055MHD091 == DEMO_PANEL)
#include "fsl_hx8394.h"
#elif (DEMO_PANEL_RASPI_7INCH == DEMO_PANEL)
#include "fsl_rpi.h"
#endif
#include "pin_mux.h"
#include "board.h"
#include "fsl_debug_console.h"

#if (DEMO_DISPLAY_CONTROLLER == DEMO_DISPLAY_CONTROLLER_LCDIFV2)
#include "fsl_dc_fb_lcdifv2.h"
#else
#include "fsl_dc_fb_elcdif.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*
 * RK055AHD091 panel
 */

#if (DEMO_PANEL == DEMO_PANEL_RK055AHD091)
#define DEMO_HSW 8
#define DEMO_HFP 32
#define DEMO_HBP 32
#define DEMO_VSW 2
#define DEMO_VFP 16
#define DEMO_VBP 14

#elif (DEMO_PANEL_RK055IQH091 == DEMO_PANEL)

#define DEMO_HSW 2
#define DEMO_HFP 32
#define DEMO_HBP 30
#define DEMO_VSW 2
#define DEMO_VFP 16
#define DEMO_VBP 14

#elif (DEMO_PANEL_RK055MHD091 == DEMO_PANEL)

#define DEMO_HSW 6
#define DEMO_HFP 12
#define DEMO_HBP 24
#define DEMO_VSW 2
#define DEMO_VFP 16
#define DEMO_VBP 14

#elif (DEMO_PANEL_RASPI_7INCH == DEMO_PANEL)

#define DEMO_HSW 20
#define DEMO_HFP 70
#define DEMO_HBP 23
#define DEMO_VSW 2
#define DEMO_VFP 7
#define DEMO_VBP 21

#endif

#if (DEMO_DISPLAY_CONTROLLER == DEMO_DISPLAY_CONTROLLER_LCDIFV2)

#define DEMO_LCDIF_POL_FLAGS                                                             \
    (kLCDIFV2_DataEnableActiveHigh | kLCDIFV2_VsyncActiveLow | kLCDIFV2_HsyncActiveLow | \
     kLCDIFV2_DriveDataOnFallingClkEdge)

#define DEMO_LCDIF LCDIFV2

#else

#define DEMO_LCDIF_POL_FLAGS \
    (kELCDIF_DataEnableActiveHigh | kELCDIF_VsyncActiveLow | kELCDIF_HsyncActiveLow | kELCDIF_DriveDataOnFallingClkEdge)

#define DEMO_LCDIF LCDIF

#endif

/* Definitions for MIPI. */
#define DEMO_MIPI_DSI (&g_mipiDsi)
#if (DEMO_PANEL_RASPI_7INCH != DEMO_PANEL)
#define DEMO_MIPI_DSI_LANE_NUM 2
#else
#define DEMO_MIPI_DSI_LANE_NUM 1
#endif

/*
 * The DPHY bit clock must be fast enough to send out the pixels, it should be
 * larger than:
 *
 *         (Pixel clock * bit per output pixel) / number of MIPI data lane
 *
 * Here the desired DPHY bit clock multiplied by ( 9 / 8 = 1.125) to ensure
 * it is fast enough.
 */
#define DEMO_MIPI_DPHY_BIT_CLK_ENLARGE(origin) (((origin) / 8) * 9)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if (DEMO_PANEL_RASPI_7INCH == DEMO_PANEL)
static status_t BOARD_ReadPanelStatus(uint8_t regAddr, uint8_t *status);
static status_t BOARD_WritePanelRegister(uint8_t regAddr, uint8_t value);
#else
static void BOARD_PullPanelResetPin(bool pullUp);
static void BOARD_PullPanelPowerPin(bool pullUp);
#endif
static void BOARD_InitLcdifClock(void);
static void BOARD_InitMipiDsiClock(void);
static status_t BOARD_DSI_Transfer(dsi_transfer_t *xfer);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint32_t mipiDsiTxEscClkFreq_Hz;
static uint32_t mipiDsiDphyBitClkFreq_Hz;
static uint32_t mipiDsiDphyRefClkFreq_Hz;
static uint32_t mipiDsiDpiClkFreq_Hz;

const MIPI_DSI_Type g_mipiDsi = {
    .host = DSI_HOST,
    .apb  = DSI_HOST_APB_PKT_IF,
    .dpi  = DSI_HOST_DPI_INTFC,
    .dphy = DSI_HOST_DPHY_INTFC,
};

#if (DEMO_PANEL == DEMO_PANEL_RK055AHD091)

static mipi_dsi_device_t dsiDevice = {
    .virtualChannel = 0,
    .xferFunc       = BOARD_DSI_Transfer,
};

static const rm68200_resource_t rm68200Resource = {
    .dsiDevice    = &dsiDevice,
    .pullResetPin = BOARD_PullPanelResetPin,
    .pullPowerPin = BOARD_PullPanelPowerPin,
};

static display_handle_t rm68200Handle = {
    .resource = &rm68200Resource,
    .ops      = &rm68200_ops,
};

#elif (DEMO_PANEL == DEMO_PANEL_RK055MHD091)

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

#elif (DEMO_PANEL_RASPI_7INCH == DEMO_PANEL)

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

#else

static mipi_dsi_device_t dsiDevice = {
    .virtualChannel = 0,
    .xferFunc       = BOARD_DSI_Transfer,
};

static const rm68191_resource_t rm68191Resource = {
    .dsiDevice    = &dsiDevice,
    .pullResetPin = BOARD_PullPanelResetPin,
    .pullPowerPin = BOARD_PullPanelPowerPin,
};

static display_handle_t rm68191Handle = {
    .resource = &rm68191Resource,
    .ops      = &rm68191_ops,
};

#endif

#if (DEMO_DISPLAY_CONTROLLER == DEMO_DISPLAY_CONTROLLER_LCDIFV2)

static dc_fb_lcdifv2_handle_t s_dcFbLcdifv2Handle = {0};

static const dc_fb_lcdifv2_config_t s_dcFbLcdifv2Config = {
    .lcdifv2       = DEMO_LCDIF,
    .width         = DEMO_PANEL_WIDTH,
    .height        = DEMO_PANEL_HEIGHT,
    .hsw           = DEMO_HSW,
    .hfp           = DEMO_HFP,
    .hbp           = DEMO_HBP,
    .vsw           = DEMO_VSW,
    .vfp           = DEMO_VFP,
    .vbp           = DEMO_VBP,
    .polarityFlags = DEMO_LCDIF_POL_FLAGS,
    .lineOrder     = kLCDIFV2_LineOrderRGB,
/* CM4 is domain 1, CM7 is domain 0. */
#if (__CORTEX_M <= 4)
    .domain = 1,
#else
    .domain = 0,
#endif
};

const dc_fb_t g_dc = {
    .ops     = &g_dcFbOpsLcdifv2,
    .prvData = &s_dcFbLcdifv2Handle,
    .config  = &s_dcFbLcdifv2Config,
};

#else

dc_fb_elcdif_handle_t s_dcFbElcdifHandle = {0}; /* The handle must be initialized to 0. */

const dc_fb_elcdif_config_t s_dcFbElcdifConfig = {
    .elcdif        = DEMO_LCDIF,
    .width         = DEMO_PANEL_WIDTH,
    .height        = DEMO_PANEL_HEIGHT,
    .hsw           = DEMO_HSW,
    .hfp           = DEMO_HFP,
    .hbp           = DEMO_HBP,
    .vsw           = DEMO_VSW,
    .vfp           = DEMO_VFP,
    .vbp           = DEMO_VBP,
    .polarityFlags = DEMO_LCDIF_POL_FLAGS,
#if (!DEMO_USE_XRGB8888) && (DEMO_USE_LUT8)
    .dataBus       = kELCDIF_DataBus8Bit,
#else
    .dataBus = kELCDIF_DataBus24Bit,
#endif
};

const dc_fb_t g_dc = {
    .ops     = &g_dcFbOpsElcdif,
    .prvData = &s_dcFbElcdifHandle,
    .config  = &s_dcFbElcdifConfig,
};
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
#if (DEMO_PANEL_RASPI_7INCH == DEMO_PANEL)
static status_t BOARD_ReadPanelStatus(uint8_t regAddr, uint8_t *value)
{
    return BOARD_LPI2C_Receive(BOARD_MIPI_PANEL_TOUCH_I2C_BASEADDR, RPI_ADDR, regAddr, 1U, value, 1U);
}

static status_t BOARD_WritePanelRegister(uint8_t regAddr, uint8_t value)
{
    return BOARD_LPI2C_Send(BOARD_MIPI_PANEL_TOUCH_I2C_BASEADDR, RPI_ADDR, regAddr, 1, &value, 1);
}
#else
static pcal6524_handle_t s_pcal6524Handle;
static bool s_pcal6524Initialized = false;
volatile bool g_touchIntFlag      = false;

#if PCAL6524_CALLBACK_PER_PIN
static void BOARD_TouchPCAL6524Callback(uint8_t pin, bool pinState, void *userData)
{
    g_touchIntFlag = true;
}
#else
static void BOARD_TouchPCAL6524Callback(uint32_t intPins, uint32_t pinStates, void *userData)
{
    if (intPins & (1U << BOARD_PCAL6524_CTP_INT))
    {
        g_touchIntFlag = true;
    }
}
#endif

static void BOARD_EnsurePCAL6524Init(void)
{
    if (!s_pcal6524Initialized)
    {
        BOARD_InitPCAL6524(&s_pcal6524Handle);
        PCAL6524_SetDirection(&s_pcal6524Handle,
                              (1 << BOARD_PCAL6524_LCD_RST_B) | (1 << BOARD_PCAL6524_Backlight_CTL) |
                                  (1 << BOARD_PCAL6524_LCM_PWR_EN) | (1 << BOARD_PCAL6524_CTP_RST_B),
                              kPCAL6524_Output);

        PCAL6524_SetPinInterruptConfig(&s_pcal6524Handle, BOARD_PCAL6524_CTP_INT, kPCAL6524_IntEdgeFalling);
#if PCAL6524_CALLBACK_PER_PIN
        PCAL6524_InstallPinCallback(&s_pcal6524Handle, BOARD_PCAL6524_CTP_INT, BOARD_TouchPCAL6524Callback, NULL);
#else
        PCAL6524_InstallCallback(&s_pcal6524Handle, BOARD_TouchPCAL6524Callback, NULL);
#endif

        s_pcal6524Initialized = true;
    }
}

static void BOARD_PullPanelResetPin(bool pullUp)
{
    BOARD_EnsurePCAL6524Init();
    if (pullUp)
    {
        PCAL6524_SetPins(&s_pcal6524Handle, (1 << BOARD_PCAL6524_LCD_RST_B));
    }
    else
    {
        PCAL6524_ClearPins(&s_pcal6524Handle, (1 << BOARD_PCAL6524_LCD_RST_B));
    }
}

static void BOARD_PullPanelPowerPin(bool pullUp)
{
    BOARD_EnsurePCAL6524Init();
    if (pullUp)
    {
        PCAL6524_SetPins(&s_pcal6524Handle, (1 << BOARD_PCAL6524_LCM_PWR_EN));
    }
    else
    {
        PCAL6524_ClearPins(&s_pcal6524Handle, (1 << BOARD_PCAL6524_LCM_PWR_EN));
    }
}

void BOARD_PullMIPIPanelTouchResetPin(bool pullUp)
{
    BOARD_EnsurePCAL6524Init();
    if (pullUp)
    {
        PCAL6524_SetPins(&s_pcal6524Handle, (1 << BOARD_PCAL6524_CTP_RST_B));
    }
    else
    {
        PCAL6524_ClearPins(&s_pcal6524Handle, (1 << BOARD_PCAL6524_CTP_RST_B));
    }
}

/* mode: 0 = pull down (output low), 1 = pull up (output high), 2 = input */
void BOARD_ConfigMIPIPanelTouchIntPin(uint8_t mode)
{
    BOARD_EnsurePCAL6524Init();
    if (mode == 2U)
    {
        PCAL6524_SetDirection(&s_pcal6524Handle, (1 << BOARD_PCAL6524_CTP_INT), kPCAL6524_Input);
    }
    else
    {
        if (mode == 0U)
        {
            PCAL6524_ClearPins(&s_pcal6524Handle, (1 << BOARD_PCAL6524_CTP_INT));
        }
        else
        {
            PCAL6524_SetPins(&s_pcal6524Handle, (1 << BOARD_PCAL6524_CTP_INT));
        }
        PCAL6524_SetDirection(&s_pcal6524Handle, (1 << BOARD_PCAL6524_CTP_INT), kPCAL6524_Output);
    }
}

void BOARD_PCAL6524_ProcessInterrupt(void)
{
    PCAL6524_InterruptHandler(&s_pcal6524Handle);
}

#endif
static status_t BOARD_DSI_Transfer(dsi_transfer_t *xfer)
{
    return DSI_TransferBlocking(DEMO_MIPI_DSI, xfer);
}

static void BOARD_InitLcdifClock(void)
{
    /*
     * The pixel clock is (height + VSW + VFP + VBP) * (width + HSW + HFP + HBP) * frame rate.
     *
     * Set div to 19 for 28MHz pixel clock.
     */
    const clock_root_config_t lcdifClockConfig = {
        .clockOff = false,
        .mux      = 4, /*!< PLL_528. */
        .div = 19,
    };

#if (DEMO_DISPLAY_CONTROLLER == DEMO_DISPLAY_CONTROLLER_LCDIFV2)
    CLOCK_SetRootClock(kCLOCK_Root_Lcdifv2, &lcdifClockConfig);

    mipiDsiDpiClkFreq_Hz = CLOCK_GetRootClockFreq(kCLOCK_Root_Lcdifv2);

#else

    CLOCK_SetRootClock(kCLOCK_Root_Lcdif, &lcdifClockConfig);

    mipiDsiDpiClkFreq_Hz = CLOCK_GetRootClockFreq(kCLOCK_Root_Lcdif);
#endif
}

static void BOARD_InitMipiDsiClock(void)
{
    uint32_t mipiDsiEscClkFreq_Hz;

    /* RxClkEsc max 60MHz, TxClkEsc 12 to 20MHz. */
    /* RxClkEsc = 528MHz / 11 = 48MHz. */
    /* TxClkEsc = 528MHz / 11 / 4 = 16MHz. */
    const clock_root_config_t mipiEscClockConfig = {
        .clockOff = false,
        .mux      = 4, /*!< PLL_528. */
        .div      = 11,
    };

    CLOCK_SetRootClock(kCLOCK_Root_Mipi_Esc, &mipiEscClockConfig);

    mipiDsiEscClkFreq_Hz = CLOCK_GetRootClockFreq(kCLOCK_Root_Mipi_Esc);

    const clock_group_config_t mipiEscClockGroupConfig = {
        .clockOff = false, .resetDiv = 2, .div0 = 2, /* TX esc clock. */
    };

    CLOCK_SetGroupConfig(kCLOCK_Group_MipiDsi, &mipiEscClockGroupConfig);

    mipiDsiTxEscClkFreq_Hz = mipiDsiEscClkFreq_Hz / 3;

    /* DPHY reference clock, use OSC 24MHz clock. */
    const clock_root_config_t mipiDphyRefClockConfig = {
        .clockOff = false,
        .mux      = 1, /*!< OSC_24M. */
        .div      = 1,
    };

    CLOCK_SetRootClock(kCLOCK_Root_Mipi_Ref, &mipiDphyRefClockConfig);

    mipiDsiDphyRefClkFreq_Hz = BOARD_XTAL0_CLK_HZ;
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

#if (DEMO_PANEL != DEMO_PANEL_RASPI_7INCH)
    BOARD_EnsurePCAL6524Init();
#endif

#if (DEMO_PANEL == DEMO_PANEL_RK055AHD091)
    status = RM68200_Init(&rm68200Handle, &displayConfig);
#elif (DEMO_PANEL_RK055MHD091 == DEMO_PANEL)
    status               = HX8394_Init(&hx8394Handle, &displayConfig);
#elif (DEMO_PANEL_RASPI_7INCH == DEMO_PANEL)
    status = RPI_Init(&rpiHandle, &displayConfig);
#else
    status = RM68191_Init(&rm68191Handle, &displayConfig);
#endif

#if (DEMO_PANEL != DEMO_PANEL_RASPI_7INCH)
    if (status == kStatus_Success)
    {
        PCAL6524_SetPins(&s_pcal6524Handle, (1 << BOARD_PCAL6524_Backlight_CTL));
    }
#endif

    return status;
}

static void BOARD_SetMipiDsiConfig(void)
{
    dsi_config_t dsiConfig;
    dsi_dphy_config_t dphyConfig;

    const dsi_dpi_config_t dpiConfig = {
        .pixelPayloadSize = DEMO_PANEL_WIDTH,
        .dpiColorCoding   = kDSI_Dpi24Bit,
        .pixelPacket      = kDSI_PixelPacket24Bit,
#if (DEMO_PANEL == DEMO_PANEL_RASPI_7INCH)
        .videoMode = kDSI_DpiNonBurstWithSyncPulse,
#else
                      .videoMode = kDSI_DpiBurst,
#endif
        .bllpMode       = kDSI_DpiBllpLowPower,
        .polarityFlags  = kDSI_DpiVsyncActiveLow | kDSI_DpiHsyncActiveLow,
        .hfp            = DEMO_HFP,
        .hbp            = DEMO_HBP,
        .hsw            = DEMO_HSW,
        .vfp            = DEMO_VFP,
        .vbp            = DEMO_VBP,
        .panelHeight    = DEMO_PANEL_HEIGHT,
        .virtualChannel = 0
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
    dsiConfig.numLanes = DEMO_MIPI_DSI_LANE_NUM;
#if (DEMO_PANEL == DEMO_PANEL_RASPI_7INCH)
    dsiConfig.autoInsertEoTp           = false;
    dsiConfig.enableNonContinuousHsClk = false;
#else
    dsiConfig.autoInsertEoTp = true;
#endif

    /* Init the DSI module. */
    DSI_Init(DEMO_MIPI_DSI, &dsiConfig);

    /* Init DPHY.
     *
     * The DPHY bit clock must be fast enough to send out the pixels, it should be
     * larger than:
     *
     *         (Pixel clock * bit per output pixel) / number of MIPI data lane
     *
     * Here the desired DPHY bit clock multiplied by ( 9 / 8 = 1.125) to ensure
     * it is fast enough.
     *
     * Note that the DSI output pixel is 24bit per pixel.
     */
    mipiDsiDphyBitClkFreq_Hz = mipiDsiDpiClkFreq_Hz * (24 / DEMO_MIPI_DSI_LANE_NUM);
    mipiDsiDphyBitClkFreq_Hz = DEMO_MIPI_DPHY_BIT_CLK_ENLARGE(mipiDsiDphyBitClkFreq_Hz);
    DSI_GetDphyDefaultConfig(&dphyConfig, mipiDsiDphyBitClkFreq_Hz, mipiDsiTxEscClkFreq_Hz);

    mipiDsiDphyBitClkFreq_Hz = DSI_InitDphy(DEMO_MIPI_DSI, &dphyConfig, mipiDsiDphyRefClkFreq_Hz);

    /* Init DPI interface. */
    DSI_SetDpiConfig(DEMO_MIPI_DSI, &dpiConfig, DEMO_MIPI_DSI_LANE_NUM, mipiDsiDpiClkFreq_Hz, mipiDsiDphyBitClkFreq_Hz);
}

status_t BOARD_InitDisplayInterface(void)
{
    CLOCK_EnableClock(kCLOCK_Video_Mux);

#if (DEMO_DISPLAY_CONTROLLER == DEMO_DISPLAY_CONTROLLER_LCDIFV2)
    /* LCDIF v2 output to MIPI DSI. */
    VIDEO_MUX->VID_MUX_CTRL.SET = VIDEO_MUX_VID_MUX_CTRL_MIPI_DSI_SEL_MASK;
#else
    /* ELCDIF output to MIPI DSI. */
    VIDEO_MUX->VID_MUX_CTRL.CLR = VIDEO_MUX_VID_MUX_CTRL_MIPI_DSI_SEL_MASK;
#endif

    /* 1. Power on and isolation off. */
    PGMC_BPC4->BPC_POWER_CTRL |= (PGMC_BPC_BPC_POWER_CTRL_PSW_ON_SOFT_MASK | PGMC_BPC_BPC_POWER_CTRL_ISO_OFF_SOFT_MASK);

    /* 2. Assert MIPI reset. */
    IOMUXC_GPR->GPR62 &=
        ~(IOMUXC_GPR_GPR62_MIPI_DSI_PCLK_SOFT_RESET_N_MASK | IOMUXC_GPR_GPR62_MIPI_DSI_ESC_SOFT_RESET_N_MASK |
          IOMUXC_GPR_GPR62_MIPI_DSI_BYTE_SOFT_RESET_N_MASK | IOMUXC_GPR_GPR62_MIPI_DSI_DPI_SOFT_RESET_N_MASK);

    /* 3. Setup clock. */
    BOARD_InitMipiDsiClock();

    /* 4. Deassert PCLK and ESC reset. */
    IOMUXC_GPR->GPR62 |=
        (IOMUXC_GPR_GPR62_MIPI_DSI_PCLK_SOFT_RESET_N_MASK | IOMUXC_GPR_GPR62_MIPI_DSI_ESC_SOFT_RESET_N_MASK);

    /* 5. Configures peripheral. */
    BOARD_SetMipiDsiConfig();

    /* 6. Deassert BYTE and DBI reset. */
    IOMUXC_GPR->GPR62 |=
        (IOMUXC_GPR_GPR62_MIPI_DSI_BYTE_SOFT_RESET_N_MASK | IOMUXC_GPR_GPR62_MIPI_DSI_DPI_SOFT_RESET_N_MASK);

    /* 7. Configure the panel. */
    return BOARD_InitLcdPanel();
}

#if (DEMO_DISPLAY_CONTROLLER == DEMO_DISPLAY_CONTROLLER_LCDIFV2)
void LCDIFv2_IRQHandler(void)
{
    DC_FB_LCDIFV2_IRQHandler(&g_dc);
}
#else
void eLCDIF_IRQHandler(void)
{
    DC_FB_ELCDIF_IRQHandler(&g_dc);
}
#endif

status_t BOARD_VerifyDisplayClockSource(void)
{
    status_t status;
    uint32_t srcClkFreq;

    /*
     * In this implementation, the SYSPLL2 (528M) clock is used as the source
     * of LCDIFV2 pixel clock and MIPI DSI ESC clock. The OSC24M clock is used
     * as the MIPI DSI DPHY PLL reference clock. This function checks the clock
     * source are valid. OSC24M is always valid, so only verify the SYSPLL2.
     */
    srcClkFreq = CLOCK_GetPllFreq(kCLOCK_PllSys2);
    if (528 != (srcClkFreq / 1000000))
    {
        status = kStatus_Fail;
    }
    else
    {
        status = kStatus_Success;
    }

    return status;
}

status_t BOARD_PrepareDisplayController(void)
{
    status_t status;

    status = BOARD_VerifyDisplayClockSource();

    if (status != kStatus_Success)
    {
        PRINTF("Error: Invalid display clock source.\r\n");
        return status;
    }

    BOARD_InitLcdifClock();

    status = BOARD_InitDisplayInterface();

    if (kStatus_Success != status)
    {
        PRINTF("ERROR: Display interface initialization failed\r\n");
    }

#if (DEMO_DISPLAY_CONTROLLER == DEMO_DISPLAY_CONTROLLER_LCDIFV2)
    NVIC_ClearPendingIRQ(LCDIFv2_IRQn);
    NVIC_SetPriority(LCDIFv2_IRQn, 3);
    EnableIRQ(LCDIFv2_IRQn);
#else
    NVIC_ClearPendingIRQ(eLCDIF_IRQn);
    NVIC_SetPriority(eLCDIF_IRQn, 3);
    EnableIRQ(eLCDIF_IRQn);
#endif

    return status;
}
