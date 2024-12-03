/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_rgpio.h"
#include "fsl_mipi_dsi.h"
#include "fsl_rm67191.h"
#include "fsl_adp5585.h"
#include "lcdifv3_support.h"

uint32_t mipiDsiTxEscClkFreq_Hz;
uint32_t mipiDsiDphyBitClkFreq_Hz;
uint32_t mipiDsiDphyRefClkFreq_Hz;
uint32_t mipiDsiDpiClkFreq_Hz;
static adp5585_handle_t adpHandle;
uint32_t mediaApbClkFreq_Hz;
uint32_t testByteClkFreq_Hz;
uint32_t phyRefClkFreq_Hz;
uint32_t phyByteClkFreq_Hz;
uint32_t mediaPixClkFreq_Hz;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
status_t RM67191_DSI_Transfer(dsi_transfer_t *xfer);
static void RM67191_PullResetPin(bool pullUp);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static mipi_dsi_device_t dsiDevice = {
    .virtualChannel = 0,
    .xferFunc       = RM67191_DSI_Transfer,
};

static const rm67191_resource_t rm67191Resource = {
    .dsiDevice    = &dsiDevice,
    .pullResetPin = RM67191_PullResetPin,
};

static display_handle_t rm67191Handle = {
    .resource = &rm67191Resource,
    .ops      = &rm67191_ops,
};
status_t RM67191_DSI_Transfer(dsi_transfer_t *xfer)
{
    return DSI_TransferBlocking(MIPI_DSI, xfer);
}

static void RM67191_PullResetPin(bool pullUp)
{
    if (pullUp)
    {
        ADP5585_SetPins(&adpHandle, (1 << DSI_RESET_PIN));
    }
    else
    {
        ADP5585_ClearPins(&adpHandle, (1 << DSI_RESET_PIN));
    }
}

AT_NONCACHEABLE_SECTION_ALIGN(
    uint8_t s_frameBuffer[DEMO_BUFFER_COUNT][DEMO_PANEL_HEIGHT][DEMO_PANEL_WIDTH][DEMO_BUFFER_BYTE_PER_PIXEL],
    DEMO_FB_ALIGN);

AT_NONCACHEABLE_SECTION_ALIGN(
    uint32_t s_pictureBitmap[DEMO_BUFFER_COUNT][DEMO_PANEL_HEIGHT][DEMO_PANEL_WIDTH][DEMO_BUFFER_BYTE_PER_PIXEL],
    DEMO_FB_ALIGN);

AT_NONCACHEABLE_SECTION_ALIGN(uint32_t s_updatePicFlag[DEMO_BUFFER_COUNT], DEMO_FB_ALIGN);

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
    /* Init the resource for RM67191. I.MX93 uses ADP5585 GPIO expander  */
    const clock_root_config_t lpi2c2ClkCfg = {.clockOff = false, .mux = kCLOCK_LPI2C2_ClockRoot_MuxOsc24M, .div = 1};
    CLOCK_SetRootClock(BOARD_ADP5585_I2C_CLOCK_ROOT, &lpi2c2ClkCfg);
    BOARD_InitADP5585(&adpHandle);
    ADP5585_SetDirection(&adpHandle, (1 << DSI_ENABLE_PIN), kADP5585_Output);
    ADP5585_SetDirection(&adpHandle, (1 << DSI_RESET_PIN), kADP5585_Output);
    ADP5585_SetDirection(&adpHandle, (1 << DSI_PWM_PIN), kADP5585_Output);
    ADP5585_SetPins(&adpHandle, (1 << DSI_RESET_PIN));
    ADP5585_SetPins(&adpHandle, (1 << DSI_PWM_PIN));
    ADP5585_SetPins(&adpHandle, (1 << DSI_ENABLE_PIN));

    if (USE_MIPI_PANEL == X_MX8_DSI_OLED1)
    {
        status = RM67191_Init(&rm67191Handle, &displayConfig);
    }
    /* Disable the MIPI DSI command mode */
    DSI_EnableCommandMode(MIPI_DSI, false);

    return status;
}

static void BOARD_SetMipiDsiConfig(void)
{
    uint32_t m;
    uint32_t n;
    dsi_config_t dsiConfig;
    dsiConfig.mode                    = kDSI_CommandMode;
    dsiConfig.packageFlags            = kDSI_DpiEnableBta | kDSI_DpiEnableEcc | kDSI_DpiEnableCrc;
    dsiConfig.enableNoncontinuousClk  = false;
    dsiConfig.HsRxDeviceReady_ByteClk = 0U;
    dsiConfig.lpRxDeviceReady_ByteClk = 0U;
    dsiConfig.HsTxDeviceReady_ByteClk = 0U;
    dsiConfig.lpTxDeviceReady_ByteClk = 0U;
    DSI_Init(MIPI_DSI, &dsiConfig);

    const dsi_dpi_config_t dpiConfig = {.pixelPayloadSize = DEMO_PANEL_WIDTH,
                                        .virtualChannel   = 0U,
                                        .colorCoding      = kDSI_DpiRGB16Bit,
                                        .enableAck        = false,
                                        .enablelpSwitch   = true,
                                        .pattern          = kDSI_PatternDisable,
                                        .videoMode        = kDSI_DpiNonBurstWithSyncPulse,
                                        .polarityFlags    = kDSI_DpiVsyncActiveLow | kDSI_DpiHsyncActiveLow,
                                        .hfp              = DEMO_HFP,
                                        .hbp              = DEMO_HBP,
                                        .hsw              = DEMO_HSW,
                                        .vfp              = DEMO_VFP,
                                        .vbp              = DEMO_VBP,
                                        .vsw              = DEMO_VSW,
                                        .panelHeight      = DEMO_PANEL_HEIGHT};

    DSI_SetDpiConfig(MIPI_DSI, &dpiConfig, 4U);
    /* Calculate byte count per second. */
    phyByteClkFreq_Hz = DEMO_PANEL_WIDTH * DEMO_PANEL_HEIGHT * DEMO_BUFFER_BYTE_PER_PIXEL * FRAME_RATE;
    dsi_command_config_t cmdConfig;
    /* Default clock value is 20Mhz */
    cmdConfig.escClkFreq_Hz = ESCCLK;
    cmdConfig.btaTo_Ns      = BTA;
    cmdConfig.hsTxTo_Ns     = HSTX;
    cmdConfig.lpRxTo_Ns     = LPRX;
    DSI_SetCommandModeConfig(MIPI_DSI, &cmdConfig, phyByteClkFreq_Hz);

    /* Get the divider value to set to the mediamix block. */
    DSI_DphyGetPllDivider(&m, &n, phyRefClkFreq_Hz, phyByteClkFreq_Hz);

    /* MEDIAMIX */
    /* Clear the bit to reset the clock logic */
    BLK_CTRL_MEDIAMIX->CLK_RESETN.RESET &=
        ~(MEDIAMIX_BLK_CTRL_RESET_dsi_apb_en_MASK | MEDIAMIX_BLK_CTRL_RESET_ref_clk_en_MASK);
    BLK_CTRL_MEDIAMIX->CLK_RESETN.RESET |=
        (MEDIAMIX_BLK_CTRL_RESET_dsi_apb_en_MASK | MEDIAMIX_BLK_CTRL_RESET_ref_clk_en_MASK);
    /*             |-cam_clk_en
     *             | |-pxp_axi_en
     *             | |       |-isi_apb_en
     *             | |       |
     * 0x78c = 0b 0111 1000 1100
     *              |  |    |
     *              |  |    |
     *              |  |    -isi_proc_en
     *              |  pxp_apb_en
     *              |-csi_apb_en
     */
    BLK_CTRL_MEDIAMIX->CLK_RESETN.CLK = MEDIAMIX_BLK_CTRL_RESET_cam_clk_en(1) | MEDIAMIX_BLK_CTRL_RESET_csi_apb_en(1) |
                                        MEDIAMIX_BLK_CTRL_RESET_pxp_axi_en(1) | MEDIAMIX_BLK_CTRL_RESET_pxp_apb_en(1) |
                                        MEDIAMIX_BLK_CTRL_RESET_isi_proc_en(1) | MEDIAMIX_BLK_CTRL_RESET_isi_apb_en(1);
    /*
     * PROP_CNTRL: 0xd << 20 = 0b 1101
     * VCO_CNTRL: 0xf<< 14
     * N: n << 10
     * M: m << 0
     */
    BLK_CTRL_MEDIAMIX->MIPI.DSI_W0 = MEDIAMIX_BLK_CTRL_DSI_W0_PROP_CNTRL(0xd) |
                                     MEDIAMIX_BLK_CTRL_DSI_W0_VCO_CNTRL(0xf) | MEDIAMIX_BLK_CTRL_DSI_W0_N(n) |
                                     MEDIAMIX_BLK_CTRL_DSI_W0_M(m);
    /*
     * 0x41 = 0b 0000 0100 0001
     *              |       |||
     *              |       |GMP_CNTRL
     *              ---|-----
     *                 CPBIAS_CNTRL
     */
    BLK_CTRL_MEDIAMIX->MIPI.DSI_W1 =
        MEDIAMIX_BLK_CTRL_DSI_W1_CPBIAS_CNTRL(0x10) | MEDIAMIX_BLK_CTRL_DSI_W1_GMP_CNTRL(1);
    uint32_t SRCBaseAddr                             = SRC_ADDR;
    uint32_t SRCIsoControlOffset                     = SRC_ISO_CTRL_OFFSET;
    *(uint32_t *)(SRCBaseAddr + SRCIsoControlOffset) = 0U;

    dsi_dphy_config_t phyConfig;
    DSI_GetDefaultDphyConfig(&phyConfig, phyByteClkFreq_Hz, 4U);
    DSI_InitDphy(MIPI_DSI, &phyConfig);

    /*
     * CONT_EN: 1<<18
     * CLKSEL: 1<<6(0b01: clock generation)
     */
    BLK_CTRL_MEDIAMIX->MIPI.DSI = MEDIAMIX_BLK_CTRL_DSI_shadow_clear(1) | MEDIAMIX_BLK_CTRL_DSI_CLKSEL(1);
    /*
     * updatpll: 1<<17
     *                       ---CLKSEL
     *                       ||
     * 0x285c = 0b 0010 1000 0101 1100
     *              |      |   |     |
     *              |      |   |-----|
     *              |      |      |
     *              --------      CFGCLKFREQRANGE
     *                 |
     *                 -HSFREQRANGE
     */
    BLK_CTRL_MEDIAMIX->MIPI.DSI = MEDIAMIX_BLK_CTRL_DSI_updatepll(1) | MEDIAMIX_BLK_CTRL_DSI_HSFREQRANGE(0x28) |
                                  MEDIAMIX_BLK_CTRL_DSI_CLKSEL(1) | MEDIAMIX_BLK_CTRL_DSI_CFGCLKFREQRANGE(0x1c);

    status_t result = DSI_PowerUp(MIPI_DSI);
    if (result != 0U)
    {
        PRINTF("DSI PHY init failed.\r\n");
    }
}

status_t BOARD_InitDisplayInterface(void)
{
    /* kCLOCK_Root_MediaAxi = 69 */
    const clock_root_config_t mediaAxiClkCfg = {
        .clockOff = false, .mux = kCLOCK_MEDIAAXI_ClockRoot_MuxSysPll1Pfd1, .div = 2};
    /* mediaAxiClkFreq_Hz value is 400Mhz */
    CLOCK_SetRootClock(kCLOCK_Root_MediaAxi, &mediaAxiClkCfg);

    /* kCLOCK_Root_MediaApb = 70 */
    const clock_root_config_t mediaApbClkCfg = {
        .clockOff = false, .mux = kCLOCK_MEDIAAPB_ClockRoot_MuxSysPll1Pfd1Div2, .div = 3};
    CLOCK_SetRootClock(kCLOCK_Root_MediaApb, &mediaApbClkCfg);
    /* mediaApbClkFreq_Hz value is 133.33Mhz */
    mediaApbClkFreq_Hz = CLOCK_GetIpFreq(kCLOCK_Root_MediaApb);

    /* kCLOCK_Root_MipiTestByte = 74 */
    const clock_root_config_t mipiTestByteClkCfg = {
        .clockOff = false, .mux = kCLOCK_MIPITESTBYTE_ClockRoot_MuxSysPll1Pfd0, .div = 50};
    CLOCK_SetRootClock(kCLOCK_Root_MipiTestByte, &mipiTestByteClkCfg);
    /* testByteClkFreq_Hz value is 20Mhz */
    testByteClkFreq_Hz = CLOCK_GetIpFreq(kCLOCK_Root_MipiTestByte);

    /* Enable video PHY reference clock. */
    /* kCLOCK_Root_MipiPhyCfg = 75 */
    const clock_root_config_t mipiPhyClkCfg = {
        .clockOff = false, .mux = kCLOCK_MIPIPHYCFG_ClockRoot_MuxOsc24M, .div = 1};
    CLOCK_SetRootClock(kCLOCK_Root_MipiPhyCfg, &mipiPhyClkCfg);
    /* phyRefClkFreq_Hz value is 24Mhz */
    phyRefClkFreq_Hz = CLOCK_GetIpFreq(kCLOCK_Root_MipiPhyCfg);

    /*
     * Configure VIDEOPLL clock source frequenccy for kCLOCK_Root_MediaDispPix
     *
     * VCO = (24MHz / rdiv) * (mfi + (mfn / mfd))  = 2,904,000,000 Hz
     * Output = VCO / odiv = 484,000,000 Hz
     */
    const fracn_pll_init_t g_vidioPllCfg = {.rdiv = 1, .mfi = 121, .mfn = 0, .mfd = 1, .odiv = 6};

    CLOCK_PllInit(VIDEOPLL, &g_vidioPllCfg);

    /* kCLOCK_Root_MediaDispPix = 72 */
    const clock_root_config_t mediaDisPixClkCfg = {
        .clockOff = false, .mux = kCLOCK_MEDIADISPPIX_ClockRoot_MuxVideoPll1Out, .div = 4 /* linux */
    };
    CLOCK_SetRootClock(kCLOCK_Root_MediaDispPix, &mediaDisPixClkCfg);
    /* use phy ref clk since it's same as OSC */
    /* mediaPixClkFreq_Hz value is 121Mhz */
    mediaPixClkFreq_Hz = (phyRefClkFreq_Hz / g_vidioPllCfg.rdiv) *
                         (g_vidioPllCfg.mfi + (g_vidioPllCfg.mfn / g_vidioPllCfg.mfd)) / g_vidioPllCfg.odiv /
                         mediaDisPixClkCfg.div;
    BOARD_SetMipiDsiConfig();

    return BOARD_InitLcdPanel();
}
