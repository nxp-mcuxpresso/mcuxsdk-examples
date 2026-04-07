/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_rgpio.h"
#include "fsl_adp5585.h"
#include "fsl_pcal6524.h"
#include "display_support.h"
#include "sm_platform.h"
#include "fsl_video_common.h"
#include "fsl_display.h"
#include "fsl_lpi2c.h"
#if DPU_EXAMPLE_DI == DPU_DI_MIPI
static adp5585_handle_t adpHandle;
#if !APP_DISPLAY_EXTERNAL_CONVERTOR
#include "fsl_mipi_dsi.h"
#include "fsl_rm692c9.h"
#else
#include "fsl_mipi_dsi.h"
#include "fsl_adv7535.h"
#endif
#elif DPU_EXAMPLE_DI == DPU_DI_LVDS
static pcal6524_handle_t pcalHandle;
#include "fsl_ldb.h"
#if APP_DISPLAY_EXTERNAL_CONVERTOR
#include "fsl_it6263.h"
#endif
#endif
uint32_t mipiDsiDpiClkFreq_Hz;
uint32_t phyRefClkFreq_Hz;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void DPU_IRQHandler(void);
#if (DPU_EXAMPLE_DI == DPU_DI_MIPI)
#if !APP_DISPLAY_EXTERNAL_CONVERTOR
status_t RM692c9_DSI_Transfer(dsi_transfer_t *xfer);
static void RM692c9_PullResetPin(bool pullUp);
#endif

#elif (DPU_EXAMPLE_DI == DPU_DI_LVDS)
#if APP_DISPLAY_EXTERNAL_CONVERTOR
static void IT6263_PullResetPin(bool pullUp);
#endif
#endif /* DPU_EXAMPLE_DI */

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if (DPU_EXAMPLE_DI == DPU_DI_MIPI)
#if !APP_DISPLAY_EXTERNAL_CONVERTOR

static mipi_dsi_device_t dsiDevice = {
    .virtualChannel = 0,
    .xferFunc       = RM692c9_DSI_Transfer,
};

static const rm692c9_resource_t rm692c9Resource = {
    .dsiDevice    = &dsiDevice,
    .pullResetPin = RM692c9_PullResetPin,
};

static display_handle_t rm692c9Handle = {
    .resource = &rm692c9Resource,
    .ops      = &rm692c9_ops,
};
#endif

#elif (DPU_EXAMPLE_DI == DPU_DI_LVDS)
#if APP_DISPLAY_EXTERNAL_CONVERTOR
    static it6263_resource_t it6263Resource = {
        .i2cAddr        = 0x98,
        .pullResetPin   = IT6263_PullResetPin,
        .i2cSendFunc    = BOARD_Display_I2C_Send,
        .i2cReceiveFunc = BOARD_Display_I2C_Receive,
    };

    static display_handle_t it6263Handle = {
        .resource = &it6263Resource,
        .ops      = &it6263_ops,
    };
#endif
#endif /* DPU_EXAMPLE_DI  */

#if (DPU_EXAMPLE_DI == DPU_DI_MIPI)
#if !APP_DISPLAY_EXTERNAL_CONVERTOR
status_t RM692c9_DSI_Transfer(dsi_transfer_t *xfer)
{
    return DSI_TransferBlocking(MIPI_DSI, xfer);
}

static void RM692c9_PullResetPin(bool pullUp)
{
    if (pullUp)
    {
        ADP5585_SetPins(&adpHandle, (1 << DSICSI_RST_SYNC));
    }
    else
    {
        ADP5585_ClearPins(&adpHandle, (1 << DSICSI_RST_SYNC));
    }
}
#endif

#elif (DPU_EXAMPLE_DI == DPU_DI_LVDS)
#if APP_DISPLAY_EXTERNAL_CONVERTOR
static void IT6263_PullResetPin(bool pullUp)
{
    if (pullUp)
    {
        PCAL6524_SetPins(&pcalHandle, (1 << LVDS1_RST));
    }
    else
    {
        PCAL6524_ClearPins(&pcalHandle, (1 << LVDS1_RST));
    }
}
#endif
#endif /* DPU_EXAMPLE_DI */

/*******************************************************************************
 * Codes
 ******************************************************************************/
/*
 * The DPU output interrupts are:
 *
 * INT_OUT [0] - common
 * INT_OUT [1] - display 0 A
 * INT_OUT [2] - display 0 B (a duplicate to be sent to Coretex-M core)
 * INT_OUT [3] - display 1 A
 * INT_OUT [4] - display 1 B
 * INT_OUT [5] - Reserved
 * INT_OUT [6] - Reserved
 * INT_OUT [7] - BLIT
 *
 * Here use display 0 B and display 1 B.
 */

#if (0 == APP_DPU_DISPLAY_INDEX)
void DISP_IRQSTEER1_DriverIRQHandler(void)
{
    DPU_IRQHandler();
    __DSB();
}
#else
void DISP_IRQSTEER3_DriverIRQHandler(void)
{
    DPU_IRQHandler();
    __DSB();
}
#endif

void DISP_IRQSTEER7_DriverIRQHandler(void)
{
    DPU_IRQHandler();
    __DSB();
}

void BOARD_InitDpuInterrupt(void)
{
#if (0 == APP_DPU_DISPLAY_INDEX)
    /* Display engine stream 0 */
    DPU_IRQSTEER->CHN_MASK[13] = 0x10249U;
    (void)EnableIRQ(DISP_IRQSTEER1_IRQn);
#else
    /* Display engine stream 1 */
    DPU_IRQSTEER->CHN_MASK[9] = 0x10249U;
    (void)EnableIRQ(DISP_IRQSTEER3_IRQn);
#endif
    /* Blit engine. */
    DPU_IRQSTEER->CHN_MASK[1] = 0x7U;
    (void)EnableIRQ(DISP_IRQSTEER7_IRQn);
}

void APP_InitPixelLink(void)
{
#if (DPU_EXAMPLE_DI == DPU_DI_MIPI)
#if (0 == APP_DPU_DISPLAY_INDEX)
     CAMERA__DSI_MASTER_CSR->DSI_PIXEL_LINK_CONTROL = CAMERA_DSI_MASTER_CSR_DSI_PIXEL_LINK_CONTROL_Pixel_link_sel(0x0);
     DISPLAY__BLK_CTRL_DISPLAYMIX->PIXEL_LINK_CTRL = (DISPLAY_BLK_CTRL_DISPLAYMIX_PIXEL_LINK_CTRL_PL0_enable(0x1) | DISPLAY_BLK_CTRL_DISPLAYMIX_PIXEL_LINK_CTRL_PL0_valid(0x1));
#else
     CAMERA__DSI_MASTER_CSR->DSI_PIXEL_LINK_CONTROL = CAMERA_DSI_MASTER_CSR_DSI_PIXEL_LINK_CONTROL_Pixel_link_sel(0x1);
     DISPLAY__BLK_CTRL_DISPLAYMIX->PIXEL_LINK_CTRL = (DISPLAY_BLK_CTRL_DISPLAYMIX_PIXEL_LINK_CTRL_PL1_enable(0x1) | DISPLAY_BLK_CTRL_DISPLAYMIX_PIXEL_LINK_CTRL_PL1_valid(0x1));
#endif
#elif (DPU_EXAMPLE_DI == DPU_DI_LVDS)
    /* The default pixel interleaver is bypass mode, do not need extra setting for LVDS */
#endif
}

void BOARD_PrepareDisplay(void)
{
#if (DPU_EXAMPLE_DI == DPU_DI_MIPI)
    hal_clk_t hal_videopll1vcoCLKCfg = {
        .clk_id = hal_clock_videopll1ctl,
        .clk_round_opt = hal_clk_round_auto,
        .ratel = 4008000000,
        .rateu = 0,
    };

    hal_clk_t hal_videopll1CLKCfg = {
        .clk_id = hal_clock_videopll1,
        .clk_round_opt = hal_clk_round_auto,
        .ratel = 446333333,
        .rateu = 0,
    };

    hal_clk_t hal_disp1pixlCLKCfg = {
        .clk_id        = hal_clock_disp1pix,
        .pclk_id       = hal_clock_videopll1,
        .div           = 3,
        .enable_clk    = true,
        .clk_round_opt = hal_clk_round_auto,
    };

    hal_clk_t hal_mipiphycfgCLKCfg = {
        .clk_id        = hal_clock_mipiPhyCfg,
        .pclk_id       = hal_clock_osc24m,
        .div           = 1,
        .enable_clk    = true,
        .clk_round_opt = hal_clk_round_auto,
    };

    hal_clk_t hal_mipiphypllbypassCLKCfg = {
        .clk_id        = hal_clock_mipiPhyPllBypass,
        .pclk_id       = hal_clock_videopll1,
        .div           = 1,
        .enable_clk    = true,
        .clk_round_opt = hal_clk_round_auto,
    };

    hal_clk_t hal_mipiphypllrefCLKCfg = {
        .clk_id        = hal_clock_mipiPhyPllRef,
        .pclk_id       = hal_clock_osc24m,
        .div           = 1,
        .enable_clk    = true,
        .clk_round_opt = hal_clk_round_auto,
    };

    hal_clk_t hal_mipitestbyteCLKCfg = {
        .clk_id        = hal_clock_mipiTestByte,
        .pclk_id       = hal_clock_videopll1,
        .div           = 1,
        .enable_clk    = true,
        .clk_round_opt = hal_clk_round_auto,
    };

    HAL_ClockSetPllClk(&hal_videopll1vcoCLKCfg);
    HAL_ClockSetPllClk(&hal_videopll1CLKCfg);
    HAL_ClockEnable(&hal_videopll1vcoCLKCfg);
    HAL_ClockEnable(&hal_videopll1CLKCfg);
    HAL_ClockSetRootClk(&hal_disp1pixlCLKCfg);
    mipiDsiDpiClkFreq_Hz = HAL_ClockGetIpFreq(hal_clock_disp1pix);
    HAL_ClockSetRootClk(&hal_mipiphycfgCLKCfg);
    HAL_ClockSetRootClk(&hal_mipiphypllbypassCLKCfg);
    HAL_ClockSetRootClk(&hal_mipiphypllrefCLKCfg);
    phyRefClkFreq_Hz = HAL_ClockGetIpFreq(hal_clock_mipiPhyPllRef);
    HAL_ClockSetRootClk(&hal_mipitestbyteCLKCfg);

#elif (DPU_EXAMPLE_DI == DPU_DI_LVDS)
#if !APP_DISPLAY_EXTERNAL_CONVERTOR
    hal_clk_t hal_ldbpllvcoCLKCfg = {
        .clk_id = hal_clock_ldbpllctl,
        .clk_round_opt = hal_clk_round_auto,
        .ratel = 2986200000,
        .rateu = 0,
    };

    hal_clk_t hal_ldbpllCLKCfg = {
        .clk_id = hal_clock_ldbpll,
        .clk_round_opt = hal_clk_round_auto,
        .ratel = 497700000,
        .rateu = 0,
    };
#else
    hal_clk_t hal_ldbpllvcoCLKCfg = {
        .clk_id = hal_clock_ldbpllctl,
        .clk_round_opt = hal_clk_round_auto,
        .ratel = 4158000000,
        .rateu = 0,
    };

    hal_clk_t hal_ldbpllCLKCfg = {
        .clk_id = hal_clock_ldbpll,
        .clk_round_opt = hal_clk_round_auto,
        .ratel = 1039500000,
        .rateu = 0,
    };
#endif
    HAL_ClockSetPllClk(&hal_ldbpllvcoCLKCfg);
    HAL_ClockSetPllClk(&hal_ldbpllCLKCfg);
    HAL_ClockEnable(&hal_ldbpllvcoCLKCfg);
    HAL_ClockEnable(&hal_ldbpllCLKCfg);
#endif
    BOARD_InitDpuInterrupt();
}

void BOARD_InitLcdPanel(void)
{
#if (DPU_EXAMPLE_DI == DPU_DI_MIPI)
    const display_config_t displayConfig = {
        .resolution   = FSL_VIDEO_RESOLUTION(APP_PANEL_WIDTH, APP_PANEL_HEIGHT),
        .hsw          = APP_HSW,
        .hfp          = APP_HFP,
        .hbp          = APP_HBP,
        .vsw          = APP_VSW,
        .vfp          = APP_VFP,
        .vbp          = APP_VBP,
        .controlFlags = 0,
        .dsiLanes     = APP_MIPI_DSI_LANE_NUM,
    };
    /* Init the DSI related resource. I.MX95 uses ADP5585 GPIO expander  */
    hal_clk_t hal_lpi2cClkCfg = {
        .clk_id = hal_clock_lpi2c2,
        .pclk_id = hal_clock_osc24m,
        .div = 1, /* 24Mhz for lpi2c */
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    HAL_ClockSetRootClk(&hal_lpi2cClkCfg);
    BOARD_InitADP5585(&adpHandle);
    ADP5585_SetDirection(&adpHandle, (1 << DSICSI_EN_PWDN), kADP5585_Output);
    ADP5585_SetDirection(&adpHandle, (1 << DSICSI_RST_SYNC), kADP5585_Output);

    ADP5585_SetPins(&adpHandle, (1 << DSICSI_EN_PWDN));
    ADP5585_SetPins(&adpHandle, (1 << DSICSI_RST_SYNC));
#if !APP_DISPLAY_EXTERNAL_CONVERTOR
    if (DPU_EXAMPLE_DI == DPU_DI_MIPI)
    {
        RM692c9_Init(&rm692c9Handle, &displayConfig);
    }
#else
    static adv7535_resource_t adv7535Resource;

    static display_handle_t advhandle = {
        .resource = &adv7535Resource,
        .ops      = &adv7535_ops,
    };
    /* Init the resource for adv7535. */
    adv7535Resource.i2cAddr        = 0x3D;
    adv7535Resource.i2cSendFunc    = BOARD_Display_I2C_Send;
    adv7535Resource.i2cReceiveFunc = BOARD_Display_I2C_Receive;

    ADV7535_Init(&advhandle, &displayConfig);
#endif
    /* Disable the MIPI DSI command mode */
    DSI_EnableCommandMode(MIPI_DSI, false);

    return;
#endif
}

void BOARD_InitDisplayInterface(void)
{
#if (DPU_EXAMPLE_DI == DPU_DI_MIPI)
    uint16_t phy_hsfreqrange;
    uint32_t phyByteClkFreq_Hz;
    uint32_t dataRateFreq_Hz;
    dsi_config_t dsiConfig;
    dsi_dphy_config_t phyConfig;
    dsiConfig.mode                    = kDSI_CommandMode;
    dsiConfig.packageFlags            = kDSI_DpiEnableBta | kDSI_DpiEnableEcc | kDSI_DpiEnableCrc;
    dsiConfig.enableNoncontinuousClk  = false;
    dsiConfig.HsRxDeviceReady_ByteClk = 0U;
    dsiConfig.lpRxDeviceReady_ByteClk = 0U;
    dsiConfig.HsTxDeviceReady_ByteClk = 0U;
    dsiConfig.lpTxDeviceReady_ByteClk = 0U;
    DSI_Init(MIPI_DSI, &dsiConfig);

    const dsi_dpi_config_t dpiConfig = {.pixelPayloadSize = APP_PANEL_WIDTH,
                                        .virtualChannel   = 0U,
                                        .colorCoding      = kDSI_DpiRGB24Bit,
                                        .enableAck        = false,
                                        .enablelpSwitch   = true,
                                        .pattern          = kDSI_PatternDisable,
                                        .videoMode        = kDSI_DpiNonBurstWithSyncPulse,
                                        .polarityFlags    = kDSI_DpiVsyncActiveLow | kDSI_DpiHsyncActiveLow,
                                        .hfp              = APP_HFP,
                                        .hbp              = APP_HBP,
                                        .hsw              = APP_HSW,
                                        .vfp              = APP_VFP,
                                        .vbp              = APP_VBP,
                                        .vsw              = APP_VSW,
                                        .panelHeight      = APP_PANEL_HEIGHT};

    DSI_SetDpiConfig(MIPI_DSI, &dpiConfig, APP_MIPI_DSI_LANE_NUM);

    /* Calculate data rate per line */
    dataRateFreq_Hz = mipiDsiDpiClkFreq_Hz * BPP / APP_MIPI_DSI_LANE_NUM;
    phyByteClkFreq_Hz = dataRateFreq_Hz * APP_MIPI_DSI_LANE_NUM / 8;
    DSI_GetDefaultDphyConfig(&phyConfig, phyByteClkFreq_Hz, APP_MIPI_DSI_LANE_NUM);
    DSI_InitDphy(MIPI_DSI, &phyConfig);
    phy_hsfreqrange = Pll_Set_Hs_Freqrange(dataRateFreq_Hz);
    /* cfg hsfreqrange */
    CAMERA__DSI_OR_CSI_PHY_CSR->COMBO_PHY_FREQ_CONTROL = CAMERA_DSI_OR_CSI_PHY_CSR_COMBO_PHY_FREQ_CONTROL_Phy_hsfreqrange(phy_hsfreqrange)|CAMERA_DSI_OR_CSI_PHY_CSR_COMBO_PHY_FREQ_CONTROL_Phy_cfgclkfreqrange(0x1CU);

    /* Clear PHY state. */
    CAMERA__DSI_OR_CSI_PHY_CSR->COMBO_PHY_MODE_CONTROL = 0x3U;

    DSI_ConfigDphy(MIPI_DSI, phyRefClkFreq_Hz, dataRateFreq_Hz);
    status_t result = DSI_PowerUp(MIPI_DSI);
    if (result != 0U)
    {
        PRINTF("DSI PHY init failed.\r\n");
    }

    BOARD_InitLcdPanel();
#elif DPU_EXAMPLE_DI == DPU_DI_LVDS
    
#if !APP_DISPLAY_EXTERNAL_CONVERTOR
    BOARD_InitPCAL6524(&pcalHandle);
    PCAL6524_SetDirection(&pcalHandle, (1 << BOARD_PCAL6524_EXT_5V0_PWR_EN), kPCAL6524_Output);
    PCAL6524_SetPins(&pcalHandle, (1 << BOARD_PCAL6524_EXT_5V0_PWR_EN));
    /* LVDS configuration */
    LDB_Init(APP_LDB, APP_DPU_DISPLAY_INDEX, LDB_DUAL_PANEL, LVDS_SPWG);

#else
    display_config_t displayConfig = {
        .resolution    = FSL_VIDEO_RESOLUTION(APP_PANEL_WIDTH, APP_PANEL_HEIGHT),
        .hsw           = APP_HSW,
        .hfp           = APP_HFP,
        .hbp           = APP_HBP,
        .vsw           = APP_VSW,
        .vfp           = APP_VFP,
        .vbp           = APP_VBP,
        .controlFlags  = 0,
    };
    /* Init the DSI related resource. I.MX95 uses ADP5585 GPIO expander  */
    hal_clk_t hal_lpi2cClkCfg = {
        .clk_id = hal_clock_lpi2c2,
        .pclk_id = hal_clock_osc24m,
        .div = 1, /* 24Mhz for lpi2c */
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    HAL_ClockSetRootClk(&hal_lpi2cClkCfg);

    hal_clk_t hal_lpi2c4ClkCfg = {
        .clk_id = hal_clock_lpi2c4,
        .pclk_id = hal_clock_osc24m,
        .div = 1, /* 24Mhz for lpi2c */
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    HAL_ClockSetRootClk(&hal_lpi2c4ClkCfg);
    BOARD_InitPCAL6524(&pcalHandle);
    PCAL6524_SetDirection(&pcalHandle, (1 << BOARD_PCAL6524_LCD3_nRST), kPCAL6524_Output);
    PCAL6524_SetPins(&pcalHandle, (1 << BOARD_PCAL6524_LCD3_nRST));

    BOARD_LPI2C_Init(LPI2C4, HAL_ClockGetIpFreq(hal_clock_lpi2c4));
    if (kStatus_Success != IT6263_Init(&it6263Handle, &displayConfig))
    {
        PRINTF("Error: Failed to init the IT6263 convert card\r\n");
    }

    IT6263_Start(&it6263Handle);

    /* IT623 default pixel map is JEIDA standard */
    LDB_Init(APP_LDB, APP_DPU_DISPLAY_INDEX, LDB_DUAL_PANEL, LVDS_JEIDA);
#endif
    /* Select LDBPLL/7 as Display 0 clock source */
#if (0 == APP_DPU_DISPLAY_INDEX)
    DISPLAY__BLK_CTRL_DISPLAYMIX->DISPLAY_ENGINES_CLOCK_CONTROL =
        DISPLAY_BLK_CTRL_DISPLAYMIX_DISPLAY_ENGINES_CLOCK_CONTROL_DSIP_CLK0_SEL(0x2);
#else
    DISPLAY__BLK_CTRL_DISPLAYMIX->DISPLAY_ENGINES_CLOCK_CONTROL =
        DISPLAY_BLK_CTRL_DISPLAYMIX_DISPLAY_ENGINES_CLOCK_CONTROL_DSIP_CLK1_SEL(0x2);
#endif
#endif
}
