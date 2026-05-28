/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_rgpio.h"
#include "fsl_adp5585.h"
#include "display_support.h"
#include "fsl_video_common.h"
#include "fsl_display.h"
static adp5585_handle_t adpHandle;
#if DPU_EXAMPLE_DI == DPU_DI_MIPI
#include "fsl_mipi_dsi2_dwc_dphy.h"
#if !APP_DISPLAY_EXTERNAL_CONVERTOR
#include "fsl_rm692c9.h"
#else
#include "fsl_adv7535.h"
#endif
#elif DPU_EXAMPLE_DI == DPU_DI_LVDS
#include "fsl_ldb.h"
#if (DEMO_PANEL == DEMO_PANEL_MX9_LVDS_LD)
#include "fsl_pi4io6408.h"
static pi4io6408_handle_t pi4io_handle;
#endif
#if APP_DISPLAY_EXTERNAL_CONVERTOR
#include "fsl_it6263.h"
#endif
#endif
uint32_t mipiDsiTxEscClkFreq_Hz;
uint32_t mipiDsiDphyBitClkFreq_Hz;
uint32_t mipiDsiDphyRefClkFreq_Hz;
uint32_t mipiDsiDpiClkFreq_Hz;
uint32_t voutClkFreq_Hz;

uint32_t mediaApbClkFreq_Hz;
uint32_t testByteClkFreq_Hz;
uint32_t phyRefClkFreq_Hz;
uint32_t phyByteClkFreq_Hz;
uint32_t mediaPixClkFreq_Hz;
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
    return DSI_TransferBlocking(DSI_MAIN, DSI_HOST, DSI_INT, xfer);
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
        ADP5585_SetPins(&adpHandle, (1 << LVDS1_RST));
    }
    else
    {
        ADP5585_ClearPins(&adpHandle, (1 << LVDS1_RST));
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
    DPU_IRQSTEER->CHN_MASK[13] = 0x3810249U;
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

#if (DEMO_PANEL == DEMO_PANEL_MX9_LVDS_LD)
void BOARD_ConfigCm0Csr(void)
{
    /* Set the clock gating of CM0P and LPCAC as not gated */
    DISPLAY__CM0_CSR->CM0P_CLOCK_GATING &= ~DISPLAY_CM0_CSR_CM0P_CLOCK_GATING_DISP_CM0P_CLK_MASK;
    DISPLAY__CM0_CSR->CM0P_ADDR_OFFSET2 = 0x4B30000;
    DISPLAY__CM0_CSR->CM0P_CPUWAIT = DISPLAY_CM0_CSR_CM0P_CPUWAIT_RST_MASK | DISPLAY_CM0_CSR_CM0P_CPUWAIT_CPW_MASK;
    /* Controls CM0P CPUWAIT input signal as running */
    DISPLAY__CM0_CSR->CM0P_CPUWAIT &= ~DISPLAY_CM0_CSR_CM0P_CPUWAIT_CPW_MASK;
}
#endif

void APP_InitPixelLink(void)
{
#if (DPU_EXAMPLE_DI == DPU_DI_MIPI)
#if (0 == APP_DPU_DISPLAY_INDEX)
     DISPLAY__DSI_CSR->DSI_PXL_LINK_CNTL = DISPLAY_DSI_CSR_DSI_PXL_LINK_CNTL_PXL_LINK_SEL(0x0);
     DISPLAY__DSI_CSR->DSI_PHY_MODE_CONTROL |= DISPLAY_DSI_CSR_DSI_PHY_MODE_CONTROL_SHUTDOWN_N_MASK | DISPLAY_DSI_CSR_DSI_PHY_MODE_CONTROL_TEST_STOP_CLK_EN_MASK;
     DISPLAY__BLK_CTRL_DISPLAYMIX->PIXEL_LINK_CTRL = (DISPLAY_BLK_CTRL_DISPLAYMIX_PIXEL_LINK_CTRL_PL0_enable(0x1) | DISPLAY_BLK_CTRL_DISPLAYMIX_PIXEL_LINK_CTRL_PL0_valid(0x1));
#else
     DISPLAY__DSI_CSR->DSI_PXL_LINK_CNTL = DISPLAY_DSI_CSR_DSI_PXL_LINK_CNTL_PXL_LINK_SEL(0x1);
     DISPLAY__BLK_CTRL_DISPLAYMIX->PIXEL_LINK_CTRL = (DISPLAY_BLK_CTRL_DISPLAYMIX_PIXEL_LINK_CTRL_PL1_enable(0x1) | DISPLAY_BLK_CTRL_DISPLAYMIX_PIXEL_LINK_CTRL_PL1_valid(0x1));
#endif
#elif (DPU_EXAMPLE_DI == DPU_DI_LVDS)
    /* The default pixel interleaver is bypass mode, do not need extra setting for LVDS */
#endif
}

void BOARD_PrepareDisplay(void)
{
#if (DPU_EXAMPLE_DI == DPU_DI_MIPI)
    clk_t videopll1vcoCLKCfg = {
        .clkId = kCLOCK_videopll1ctl,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 4009500000,
    };

    clk_t videopll1CLKCfg = {
        .clkId = kCLOCK_videopll1,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 445500000,
    };

    clk_t disp1pixlCLKCfg = {
        .clkId = kCLOCK_disp1pix,
        .pclkId = kCLOCK_videopll1,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 148500000,
    };

    clk_t mipiphycfgCLKCfg = {
        .clkId = kCLOCK_camPhyCfg,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000,
    };

    clk_t mipiphypllbypassCLKCfg = {
        .clkId = kCLOCK_mipiPhyPllBypass,
        .pclkId = kCLOCK_videopll1,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 445500000,
    };

    clk_t mipitestbyteCLKCfg = {
        .clkId = kCLOCK_mipiTestByte,
        .pclkId = kCLOCK_videopll1,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 445500000,
    };

    CLOCK_SetRate(&videopll1vcoCLKCfg);
    CLOCK_EnableClock(videopll1vcoCLKCfg.clkId);
    CLOCK_SetRate(&videopll1CLKCfg);
    CLOCK_EnableClock(videopll1CLKCfg.clkId);
    voutClkFreq_Hz = CLOCK_GetRate(videopll1CLKCfg.clkId);
    CLOCK_SetParent(&disp1pixlCLKCfg);
    CLOCK_SetRate(&disp1pixlCLKCfg);
    CLOCK_EnableClock(disp1pixlCLKCfg.clkId);
    mipiDsiDpiClkFreq_Hz = CLOCK_GetRate(disp1pixlCLKCfg.clkId);
    CLOCK_SetRate(&mipiphycfgCLKCfg);
    CLOCK_EnableClock(mipiphycfgCLKCfg.clkId);
    CLOCK_SetParent(&mipiphypllbypassCLKCfg);
    CLOCK_SetRate(&mipiphypllbypassCLKCfg);
    CLOCK_SetParent(&mipitestbyteCLKCfg);
    CLOCK_SetRate(&mipitestbyteCLKCfg);
#elif (DPU_EXAMPLE_DI == DPU_DI_LVDS)
#if !APP_DISPLAY_EXTERNAL_CONVERTOR
#if (DEMO_PANEL == DEMO_PANEL_MX9_LVDS_LD)
    clk_t ldbpllvcoCLKCfg = {
        .clkId = kCLOCK_ldbpllctl,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 2564800000,
    };

    clk_t ldbpllCLKCfg = {
        .clkId = kCLOCK_ldbpll,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 641200000,
    };

    clk_t spi9CLKCfg = {
        .clkId = kCLOCK_displpspi,
        .pclkId = kCLOCK_syspll1dfs0div2,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 20000000,
    };
#else
    clk_t ldbpllvcoCLKCfg = {
        .clkId = kCLOCK_ldbpllctl,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 2986200000,
    };

    clk_t ldbpllCLKCfg = {
        .clkId = kCLOCK_ldbpll,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 497700000,
    };

#endif
#else
    clk_t ldbpllvcoCLKCfg = {
        .clkId = kCLOCK_ldbpllctl,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 4158000000,
    };

    clk_t ldbpllCLKCfg = {
        .clkId = kCLOCK_ldbpll,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 1039500000,
    };
#endif
    CLOCK_SetRate(&ldbpllvcoCLKCfg);
    CLOCK_EnableClock(ldbpllvcoCLKCfg.clkId);
    CLOCK_SetRate(&ldbpllCLKCfg);
    CLOCK_EnableClock(ldbpllCLKCfg.clkId);
#if (DEMO_PANEL == DEMO_PANEL_MX9_LVDS_LD)
    CLOCK_SetParent(&spi9CLKCfg);
    CLOCK_SetRate(&spi9CLKCfg);
    CLOCK_EnableClock(spi9CLKCfg.clkId);
    EnableIRQ(LPSPI9_IRQn);
#endif
    /* Select LVDS related pin by using ADP5585 */
    BOARD_InitADP5585(&adpHandle);
    ADP5585_SetDirection(&adpHandle, (1 << LVDS1_RST), kADP5585_Output);
    ADP5585_SetDirection(&adpHandle, (1 << LVDS1_EN), kADP5585_Output);
    ADP5585_SetDirection(&adpHandle, (1 << LVDS0_RST), kADP5585_Output);
    ADP5585_SetDirection(&adpHandle, (1 << LVDS0_EN), kADP5585_Output);

    ADP5585_SetPins(&adpHandle, (1 << LVDS1_RST));
    ADP5585_SetPins(&adpHandle, (1 << LVDS1_EN));
    ADP5585_SetPins(&adpHandle, (1 << LVDS0_RST));
    ADP5585_SetPins(&adpHandle, (1 << LVDS0_EN));
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
    /* Init the DSI related resource. I.MX952 uses ADP5585 GPIO expander  */
    clk_t lpi2cClkCfg = {
        .clkId = kCLOCK_lpi2c2,
        .rate = 1000000UL, /* 24Mhz for lpi2c */
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    CLOCK_SetRate(&lpi2cClkCfg);
    CLOCK_EnableClock(lpi2cClkCfg.clkId);
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

    return;
#endif
}

void BOARD_InitDisplayInterface(void)
{
#if (DPU_EXAMPLE_DI == DPU_DI_MIPI)

    dsi_config_t dsiConfig;
    DSI_GetDefaultConfig(&dsiConfig);
    DSI_Init(DSI_MAIN, DSI_HOST, DSI_INT, &dsiConfig);

    /* IPI setting is based on video mode only */
    /* Set MIPI DSI IPI config */
    const dsi_ipi_config_t ipiConfig = {.pixelPayloadSize      = APP_PANEL_WIDTH,
                                        .dsi_ipi_lane          = kDSI_IPI1Lane,
                                        .dsi_ipi_mapping       = kDSI_IPIDpiConfig1,
                                        .ipiColor.depth        = kDSI_IPIdepth8bits,
                                        .ipiColor.format       = kDSI_IPIRGB,
                                        .dsi_fmt               = kDSI_Format_RGB888,
                                        .ipi_fifo_depth_value  = APP_IPI_PIXEL_DEPTH,
                                        .hfp                   = APP_HFP,
                                        .hbp                   = APP_HBP,
                                        .hsa                   = APP_HSW,
                                        .hactive               = APP_PANEL_WIDTH,
                                        .hline                 = APP_PANEL_WIDTH + APP_HSW + APP_HBP + APP_HFP,
                                        .vfp                   = APP_VFP,
                                        .vbp                   = APP_VBP,
                                        .vsa                   = APP_VSW,
                                        .vactive               = APP_PANEL_HEIGHT,
    };
    phyByteClkFreq_Hz = voutClkFreq_Hz * 2 / 8U;
    float ratio = (float)phyByteClkFreq_Hz / (float)mipiDsiDpiClkFreq_Hz;
    DSI_PHY->PHY_IPI_RATIO_MAN_CFG     = MIPI_ConvertFloat(ratio, 6, 16);
    DSI_PHY->PHY_SYS_RATIO_MAN_CFG     = MIPI_ConvertFloat(ratio, 6, 16);
    DSI_ConfigHorizontalParams(DSI_IPI, &ipiConfig, ratio);
    DSI_SetIpiConfig(DSI_IPI, &ipiConfig);
    uint32_t dataRateFreq_Hz = voutClkFreq_Hz;
    uint32_t phyRefClkFreq_Hz = 24000000U;
    uint32_t bndwidth_Hz = voutClkFreq_Hz * 2;
    uint32_t lpclk_hz = mipiDsiDpiClkFreq_Hz / 8;
    DSI_StartupTxStaticSetting(APP_DPU_DPHY);
    DSI_DphyTxDynamicSetting(APP_DPU_DPHY, bndwidth_Hz, lpclk_hz);
    DSI_ConfigDphy(DSI_CSR, phyRefClkFreq_Hz, dataRateFreq_Hz, &ipiConfig);

    /* Set MIPI DSI PHY config */
    dsi_phy_config_t phyConfig;
    phyConfig.phymode = kDSI_DPHY;
    phyConfig.enableNoncontinuousClk = false;
    phyConfig.ppiwidth = kDSI_PPI8BITS;
    phyConfig.numLanes = APP_MIPI_DSI_LANE_NUM -1U;
    phyConfig.lp2hs_time = 0x10000U;
    phyConfig.hs2lp_time = 0x10000U;
    phyConfig.esccmd_time = 0x90000U;
    phyConfig.escbyte_time = 0xe0000U;
    phyConfig.lptx_clkdiv = 4U;
    phyConfig.tolerance_time = 0U;
    phyConfig.cal_time = 0U;
    phyConfig.ulps_wakeuptime = 0U;

    DSI_SetPhyConfig(DSI_PHY, &phyConfig);

    status_t result = DSI_PowerUp(DSI_MAIN, DSI_PHY);
    if (result != 0U)
    {
        PRINTF("DSI PHY init failed.\r\n");
    }

    DSI_ClearPpiInterfaceControl(DSI_CSR);
    dsiConfig.operatemode = kDSI_CommandMode;
    DSI_SetOperateMode(DSI_MAIN, DSI_HOST, &dsiConfig);
    BOARD_InitLcdPanel();
    /* Enter video mode */
    dsiConfig.operatemode = kDSI_VideoMode;
    DSI_SetOperateMode(DSI_MAIN, DSI_HOST, &dsiConfig);

#elif DPU_EXAMPLE_DI == DPU_DI_LVDS

#if !APP_DISPLAY_EXTERNAL_CONVERTOR
#if (DEMO_PANEL == DEMO_PANEL_MX9_LVDS_LD)
    BOARD_InitPI4IO6408_I2C3(&pi4io_handle);
    PI4IO6408_EnableAllPinOutput(&pi4io_handle);
    PI4IO6408_SetDirection(&pi4io_handle, (1 << 3), kPI4IO6408_Output);
    PI4IO6408_SetPins(&pi4io_handle, (1 << 3));
    SDK_DelayAtLeastUs(100, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    PI4IO6408_SetDirection(&pi4io_handle, (1 << 1), kPI4IO6408_Output);
    PI4IO6408_SetPins(&pi4io_handle, (1 << 1));
    SDK_DelayAtLeastUs(100, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    PI4IO6408_SetDirection(&pi4io_handle, (1 << 2), kPI4IO6408_Output);
    PI4IO6408_SetPins(&pi4io_handle, (1 << 2));
    SDK_DelayAtLeastUs(100, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    PI4IO6408_SetDirection(&pi4io_handle, (1 << 4), kPI4IO6408_Output);
    PI4IO6408_SetPins(&pi4io_handle, (1 << 4));
    SDK_DelayAtLeastUs(100, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
#endif
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

    clk_t lpi2cClkCfg = {
        .clkId = kCLOCK_lpi2c2,
        .rate = 24000000UL, /* 24Mhz for lpi2c */
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    CLOCK_SetRate(&lpi2cClkCfg);
    CLOCK_EnableClock(lpi2cClkCfg.clkId);

    BOARD_InitADP5585(&adpHandle);
    ADP5585_SetDirection(&adpHandle, (1 << LVDS1_RST), kADP5585_Output);
    ADP5585_SetDirection(&adpHandle, (1 << LVDS1_EN), kADP5585_Output);
    ADP5585_SetPins(&adpHandle, (1 << LVDS1_RST));
    ADP5585_SetPins(&adpHandle, (1 << LVDS1_EN));

    if (kStatus_Success != IT6263_Init(&it6263Handle, &displayConfig))
    {
        PRINTF("Error: Failed to init the IT6263 convert card\r\n");
    }

    IT6263_Start(&it6263Handle);

    /* IT623 default pixel map is JEIDA standard */
    LDB_Init(APP_LDB, APP_DPU_DISPLAY_INDEX, LDB_DUAL_PANEL, LVDS_JEIDA);
#endif

#if (0 == APP_DPU_DISPLAY_INDEX)
    /* Select LDBPLL/7 as Display 0 clock source */
    DISPLAY__BLK_CTRL_DISPLAYMIX->DISPLAY_ENGINES_CLOCK_CONTROL = DISPLAY_BLK_CTRL_DISPLAYMIX_DISPLAY_ENGINES_CLOCK_CONTROL_DSIP_CLK0_SEL(0x2);
#else
    /* Select LDBPLL/7 as Display 1 clock source */
    DISPLAY__BLK_CTRL_DISPLAYMIX->DISPLAY_ENGINES_CLOCK_CONTROL = DISPLAY_BLK_CTRL_DISPLAYMIX_DISPLAY_ENGINES_CLOCK_CONTROL_DSIP_CLK1_SEL(0x2);
#endif
#endif
}
