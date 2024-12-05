/*
 * Copyright 2024 NXP
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
#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
#include "fsl_lpi2c.h"
#endif
//#include "fsl_irqsteer.h"
//#include "fsl_dpu_irqsteer.h"
#if DPU_EXAMPLE_DI == DPU_DI_MIPI
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
uint32_t mipiDsiTxEscClkFreq_Hz;
uint32_t mipiDsiDphyBitClkFreq_Hz;
uint32_t mipiDsiDphyRefClkFreq_Hz;
uint32_t mipiDsiDpiClkFreq_Hz;

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
    return DSI_TransferBlocking(MIPI_DSI, xfer);
}

static void RM692c9_PullResetPin(bool pullUp)
{
    if (pullUp)
    {
        /* Do not have pin select in schematic, ignore this */
    }
    else
    {
        /* Do not have pin select in schematic, ignore this */
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
    HAL_ClockSetRootClk(&hal_disp1pixlCLKCfg);
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

    /* Select LVDS1 pin by using ADP5585 */
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
    /* Init the DSI related resource. I.MX95 uses ADP5585 GPIO expander  */

#if !APP_DISPLAY_EXTERNAL_CONVERTOR
    if (DPU_EXAMPLE_DI == DPU_DI_MIPI)
    {
        RM692c9_Init(&rm692c9Handle, &displayConfig);
    }
#else
    static adv7535_resource_t adv7535Resource;
    lpi2c_master_config_t lpi2cConfig = {0};
    static display_handle_t advhandle = {
        .resource = &adv7535Resource,
        .ops      = &adv7535_ops,
    };
    hal_clk_t hal_lpi2cclkCfg = {
        .clk_id = hal_clock_lpi2c6,
        .pclk_id = hal_clock_osc24m,
        .div = 24, /* 24Mhz for lpi2c */
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    HAL_ClockSetRootClk(&hal_lpi2cclkCfg);
    /* Init the resource for adv7535. */
    LPI2C_MasterGetDefaultConfig(&lpi2cConfig);
    LPI2C_MasterInit(LPI2C6, &lpi2cConfig, 1000000U);
    adv7535Resource.i2cSendFunc    = BOARD_Display_I2C_Send;
    adv7535Resource.i2cReceiveFunc = BOARD_Display_I2C_Receive;
    adv7535Resource.i2cAddr        = 0x3D;

    ADV7535_Init(&advhandle, &displayConfig);
#endif
    /* Disable the MIPI DSI command mode */
    DSI_EnableCommandMode(MIPI_DSI, false);

    return;
#endif
}

#if (DPU_EXAMPLE_DI == DPU_DI_MIPI)
status_t MIPI_DSI_PowerUp(MIPI_DSI_Type *base)
{
    uint32_t waitTimes = 0x1000U;
    base->PWR_UP       = 1UL;
    base->PHY_RSTZ     = 0xFU;
    /* Wait for the PHY lock state to set. */
    while (((base->PHY_STATUS & MIPI_DSI_PHY_STATUS_phy_lock_MASK) == 0U) && (0U != waitTimes))
    {
        waitTimes--;
    }

    if (waitTimes == 0U)
    {
        return kStatus_Timeout;
    }

    waitTimes = 0x1000U;
    /* Wait for the PHY stopstateclklane to set. */
    while (((base->PHY_STATUS & MIPI_DSI_PHY_STATUS_phy_stopstate0lane_MASK) == 0U) && (0U != waitTimes))
    {
        waitTimes--;
    }

    if (waitTimes == 0U)
    {
        return kStatus_Timeout;
    }
    else
    {
        return kStatus_Success;
    }
}

static void tx_dphy_write_control(uint16_t testcode, uint8_t testwrite)
{
    /* Writing the 4-it testcode MSBs */
    MIPI_DSI->PHY_TST_CTRL0 = 0U;
    MIPI_DSI->PHY_TST_CTRL1 = 0U;
    MIPI_DSI->PHY_TST_CTRL1 = 0x00010000U;
    MIPI_DSI->PHY_TST_CTRL0 = 0x2U;
    MIPI_DSI->PHY_TST_CTRL1 = 0x00010000U;

    MIPI_DSI->PHY_TST_CTRL0 = 0U;
    MIPI_DSI->PHY_TST_CTRL1 = 0U;
    MIPI_DSI->PHY_TST_CTRL1 = (testcode >> 8);
    MIPI_DSI->PHY_TST_CTRL0 = 0x2U;

    MIPI_DSI->PHY_TST_CTRL0 = 0U;
    MIPI_DSI->PHY_TST_CTRL1 = 0x00010000U;
    MIPI_DSI->PHY_TST_CTRL0 = 0x2U;
    MIPI_DSI->PHY_TST_CTRL1 = (0x00010000 | (testcode & 0xFF));
    MIPI_DSI->PHY_TST_CTRL0 = 0U;
    MIPI_DSI->PHY_TST_CTRL1 = 0U;

    MIPI_DSI->PHY_TST_CTRL1 = testwrite;
    MIPI_DSI->PHY_TST_CTRL0 = 0x2U;
}
#endif

void BOARD_InitDisplayInterface(void)
{
#if (DPU_EXAMPLE_DI == DPU_DI_MIPI)
    dsi_config_t dsiConfig;
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
    MIPI_DSI->VID_HSA_TIME   = APP_HSW * MIPI_DSI_BPP / MIPI_DSI_DIV;
    MIPI_DSI->VID_HBP_TIME   = APP_HBP * MIPI_DSI_BPP / MIPI_DSI_DIV;
    MIPI_DSI->VID_HLINE_TIME = (APP_PANEL_WIDTH + APP_HSW + APP_HBP + APP_HFP) * MIPI_DSI_BPP / MIPI_DSI_DIV;

    /* Clear PHY state. */
    MIPI_DSI->PHY_RSTZ = 0U;
    MIPI_DSI->PWR_UP   = 0UL;
    CAMERA__DSI_OR_CSI_PHY_CSR->COMBO_PHY_MODE_CONTROL = 0x3U;

    CAMERA__DSI_CSI_COMBO_COMPLEX_CSI1__CSI->PHY_TEST_CTRL0 = 0x1U;

    MIPI_DSI->PHY_TST_CTRL0 = MIPI_DSI_PHY_TST_CTRL0_phy_testclr_MASK;
    SDK_DelayAtLeastUs(100000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    CAMERA__DSI_CSI_COMBO_COMPLEX_CSI1__CSI->PHY_TEST_CTRL0 = 0x0U;

    MIPI_DSI->PHY_TST_CTRL0 = 0U;
    CAMERA__DSI_OR_CSI_PHY_CSR->COMBO_PHY_FREQ_CONTROL = 0x290000U;

    /* cfg TX PHY registers */
    tx_dphy_write_control(0x16A, 0x3);  /* pll_mpll_prog_rw = 2'b11 */
    tx_dphy_write_control(0x1AB, 0x6);  /* cb_sel_vref_lprx_rw = 2'b10 */
    tx_dphy_write_control(0x1AA, 0x53); /* cb_sel_vrefcd_lprx_rw = 2'b10 */
    tx_dphy_write_control(0x402, 0x2);  /* txclk_term_lowcap_lp00_en_ovr_en = 1'b1, txclk_term_lowcap_lp00_en_ovr = 1'b0 */

    /* set the value of slew rate calibration for 1.5Gbps or below */
    tx_dphy_write_control(0x272, 0x0);  /* sr_range=0, 0x272 */
    tx_dphy_write_control(0x271, 0x7);  /* sr_osc_freq_target[11:8], 0x271 */
    tx_dphy_write_control(0x270, 0xd0); /* sr_osc_freq_target[7:0], 0x270 */
    /* enable slew rate calibration */
    tx_dphy_write_control(0x272, 0x10); /* sr_range=0,sr_sel_tester_rw=2'b01(slew rate on), 0x272 */

    /* cfg hsfreqrange */
    CAMERA__DSI_OR_CSI_PHY_CSR->COMBO_PHY_FREQ_CONTROL = 0x29001CU;
    /* Configure the PLL */
    /* set PLL M */
    tx_dphy_write_control(0x17A, (165 >> 8));   /* pll_m_ovr_rw[9:8]=0, 0x17A */
                                                  /* tx_dphy_read_control(0x17A) */
    tx_dphy_write_control(0x179, (165 & 0xFF)); /* pll_m_ovr_rw[7:0]='hC6, 0x179 */
                                                  /* tx_dphy_read_control(0x179) */
    /* set PLL N */
    /* enable PLL divider and PLL feedback multiplication parameter overrides */
    tx_dphy_write_control(0x178, (0x1 << 7 | (8 << 3))); /* pll_n_ovr_en_rw =1, pll_n_ovr_rw[3:0]=7, 0x178 */
    /* configure VCO control and PLL charge pump */
    tx_dphy_write_control(0x17B, 0x93); /* pll_vco_cntrl_ovr_en_rw =1, pll_vco_cntrl_ovr_rw[5:0]=9, pll_m_ovr_en_rw=1, 0x17B */
    tx_dphy_write_control(0x15E, 0x10); /* pll_cpbias_cntrl_rw[6:0]='h10, 0x15E */
    tx_dphy_write_control(0x162, 0x1);  /* pll_gmp_cntrl_rw[1:0]='b01,pll_int_cntrl_rw[5:0]=0, 0x162 */
    tx_dphy_write_control(0x16E, 0xd);  /* pll_prop_cntrl_rw[5:0]='hd, 0x16E */
    /* Configure PLL lock fields */
    tx_dphy_write_control(0x173, 0x2);  /* pll_th1_rw[7:0]=2; 0x173 */
    tx_dphy_write_control(0x174, 0x0);  /* pll_th1_rw[9:8]=0;0x174 */
    tx_dphy_write_control(0x175, 0x60); /* pll_th2_rw[7:0] ='h60, 0x175 */
    tx_dphy_write_control(0x176, 0x3);  /* pll_th3_rw[7:0]=3, 0x176 */
    tx_dphy_write_control(0x166, 0x4);  /* pll_lock_sel_rw=1, 0x166 */
                                        /* tx_dphy_read_control(0x166) */
    /* Power on PLL */
    tx_dphy_write_control(0x16E, 0xcd); /* pll_prop_cntrl_rw[5:0]='hd,pll_pwron_ovr_rw=1,pll_pwron_ovr_en_rw=1, 0x16E */
                                        /* tx_dphy_read_control(0x16E) */
    /* Enable the PLL right and left side buffers */
    tx_dphy_write_control(0x17C, 0x3); /* pll_clkouten_right_rw =1, pll_clkouten_left_rw=1, 0x17c */
                                       /* tx_dphy_read_control(0x17C) */
    CAMERA__DSI_OR_CSI_PHY_CSR->COMBO_PHY_TEST_MODE_CONTROL = 0x2000U;
    SDK_DelayAtLeastUs(100000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    MIPI_DSI->PHY_IF_CFG = (3 | (0 << 2) | (40 << 8) | (0 << 16));
    MIPI_DSI->PHY_RSTZ = (0 | (0 << 1) | (1 << 2) | (0 << 3));
    SDK_DelayAtLeastUs(100000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    MIPI_DSI->PHY_RSTZ = (1 | (0 << 1) | (1 << 2) | (0 << 3));
    SDK_DelayAtLeastUs(100000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    status_t result = MIPI_DSI_PowerUp(MIPI_DSI);
    if (result != 0U)
    {
        PRINTF("DSI PHY init failed.\r\n");
    }

    MIPI_DSI->LPCLK_CTRL |= 0x1U;

    BOARD_InitLcdPanel();
#elif DPU_EXAMPLE_DI == DPU_DI_LVDS
#if !APP_DISPLAY_EXTERNAL_CONVERTOR
    /* LVDS configuration */
    LDB_Init(APP_LDB, APP_DPU_DISPLAY_INDEX, LDB_DUAL_PANEL);

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
    adp5585_handle_t adpHandle;
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

    LDB_Init(APP_LDB, APP_DPU_DISPLAY_INDEX, LDB_DUAL_PANEL);
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
