/*
 * Copyright 2023-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#ifdef CONFIG_USB_CDC_ACM
#include "fsl_device_registers.h"
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "virtual_com.h"
#endif /* CONFIG_USB_CDC_ACM */

#include "pin_mux.h"
#include "board.h"
#include "board_isp.h"
#include "clock_config.h"
#include "hal_power.h"
#include "hal_clock.h"
#include "display_support.h"
#include "sm_platform.h"
#include "fsl_debug_console.h"
#include "fsl_irqsteer.h"


#ifdef CONFIG_ISPSDK_ETH
#include "app.h"
#include "fsl_netc_endpoint.h"
#include "fsl_netc_mdio.h"
#include "fsl_netc_phy_wrapper.h"
#include "fsl_msgintr.h"
#endif

/*${header:end}*/


#ifdef CONFIG_ISPSDK_ETH
/*${macro:start}*/
/*${macro:end}*/

/*${variable:start}*/
static status_t ENETC0_PHY_Init(phy_handle_t *phy_handle, const phy_config_t *config);
static status_t ENETC2_PHY_Init(phy_handle_t *phy_handle, const phy_config_t *config);

static netc_mdio_handle_t s_mdio_handle;
static netc_mdio_handle_t s_emdio_handle;

phy_aqr113c_resource_t g_phy_aqr113c_resource;
const phy_operations_t g_app_phy_aqr113c_ops = {.phyInit             = ENETC2_PHY_Init,
                                                .phyWrite            = NULL,
                                                .phyRead             = NULL,
                                                .phyWriteC45         = PHY_AQR113C_Write,
                                                .phyReadC45          = PHY_AQR113C_Read,
                                                .getAutoNegoStatus   = PHY_AQR113C_GetAutoNegotiationStatus,
                                                .getLinkStatus       = PHY_AQR113C_GetLinkStatus,
                                                .getLinkSpeedDuplex  = PHY_AQR113C_GetLinkSpeedDuplex,
                                                .setLinkSpeedDuplex  = PHY_AQR113C_SetLinkSpeedDuplex,
                                                .enableLoopback      = PHY_AQR113C_EnableLoopback,
                                                .enableLinkInterrupt = NULL,
                                                .clearInterrupt      = NULL};

phy_rtl8211f_resource_t g_phy_rtl8211f_resource;
const phy_operations_t g_app_phy_rtl8211f_ops = {.phyInit             = ENETC0_PHY_Init,
                                                 .phyWrite            = PHY_RTL8211F_Write,
                                                 .phyRead             = PHY_RTL8211F_Read,
                                                 .getAutoNegoStatus   = PHY_RTL8211F_GetAutoNegotiationStatus,
                                                 .getLinkStatus       = PHY_RTL8211F_GetLinkStatus,
                                                 .getLinkSpeedDuplex  = PHY_RTL8211F_GetLinkSpeedDuplex,
                                                 .setLinkSpeedDuplex  = PHY_RTL8211F_SetLinkSpeedDuplex,
                                                 .enableLoopback      = PHY_RTL8211F_EnableLoopback,
                                                 .enableLinkInterrupt = PHY_RTL8211F_EnableLinkInterrupt,
                                                 .clearInterrupt      = PHY_RTL8211F_ClearInterrupt};
/*${variable:end}*/
#endif
/*${function:start}*/

#ifdef CONFIG_ISPSDK_ETH
static status_t ENETC0_MDIO_Init(void)
{
    netc_mdio_config_t mdioConfig = {
        .isPreambleDisable = false,
        .isNegativeDriven  = false,
        .srcClockHz        = HAL_ClockGetIpFreq(hal_clock_enet),
    };

    /* EMDIO init */
    mdioConfig.mdio.type = kNETC_EMdio;
    return  NETC_MDIOInit(&s_emdio_handle, &mdioConfig);
}

static status_t ENETC0_EMDIOWrite(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return NETC_MDIOWrite(&s_emdio_handle, phyAddr, regAddr, data);
}

static status_t ENETC0_EMDIORead(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return NETC_MDIORead(&s_emdio_handle, phyAddr, regAddr, pData);
}

static status_t ENETC0_PHY_Init(phy_handle_t *phy_handle, const phy_config_t *config)
{
    status_t result            = kStatus_Success;
    pcal6408_handle_t handle;

    /* MDIO init */
    result = ENETC0_MDIO_Init();
    if (result != kStatus_Success)
    {
        return result;
    }

    /* For a complete PHY reset of RTL8211FDI-CG, this pin must be asserted low for at least 10ms. And
     * wait for a further 72ms(for internal circuits settling time) before accessing the PHY register */
    BOARD_InitPCAL6408_I2C5(&handle);
    PCAL6408_SetDirection(&handle, (1 << BOARD_PCAL6408_ENET1_RST_B), kPCAL6408_Output);
    PCAL6408_ClearPins(&handle, (1 << BOARD_PCAL6408_ENET1_RST_B));
    SDK_DelayAtLeastUs(20000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    PCAL6408_SetPins(&handle, (1 << BOARD_PCAL6408_ENET1_RST_B));
    SDK_DelayAtLeastUs(80000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    /* Initialize PHY */
    result = PHY_RTL8211F_Init(phy_handle, config);
    return result;
}

static status_t ENETC2_MDIO_Init(void)
{
    status_t result = kStatus_Success;

    netc_mdio_config_t mdioConfig = {
        .isPreambleDisable = false,
        .isNegativeDriven  = false,
        .srcClockHz        = HAL_ClockGetIpFreq(hal_clock_enet),
    };

    /* EMDIO init */
    mdioConfig.mdio.type = kNETC_EMdio;
    result               = NETC_MDIOInit(&s_emdio_handle, &mdioConfig);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Internal MDIO init */
    ENETC2_PCI_HDR_TYPE0->PCI_CFH_CMD |=
        (ENETC_PCI_TYPE0_PCI_CFH_CMD_MEM_ACCESS_MASK | ENETC_PCI_TYPE0_PCI_CFH_CMD_BUS_MASTER_EN_MASK);

    mdioConfig.mdio.type = kNETC_InternalMdio;
    mdioConfig.mdio.port = kNETC_ENETC2EthPort;
    result               = NETC_MDIOInit(&s_mdio_handle, &mdioConfig);
    return result;
}

static status_t ENETC2_EMDIOC45Write(uint8_t portAddr, uint8_t devAddr, uint16_t regAddr, uint16_t data)
{
    return NETC_MDIOC45Write(&s_emdio_handle, portAddr, devAddr, regAddr, data);
}

static status_t ENETC2_EMDIOC45Read(uint8_t portAddr, uint8_t devAddr, uint16_t regAddr, uint16_t *pData)
{
    return NETC_MDIOC45Read(&s_emdio_handle, portAddr, devAddr, regAddr, pData);
}

static status_t ENETC2_PHY_Init(phy_handle_t *phy_handle, const phy_config_t *config)
{
    status_t result            = kStatus_Success;
    pcal6408_handle_t handle;

    /* MDIO init */
    result = ENETC2_MDIO_Init();
    if (result != kStatus_Success)
    {
        return result;
    }

    /* PHY WRAPPER Init */
    NETC_PHYInit(&s_mdio_handle, kNETC_XGMII10G);

    /* Power up and reset */
    BOARD_InitPCAL6408_I2C5(&handle);
    PCAL6408_SetDirection(&handle, (1 << BOARD_PCAL6408_AQR_PWR_EN), kPCAL6408_Output);
    PCAL6408_SetPins(&handle, (1 << BOARD_PCAL6408_AQR_PWR_EN));
    SDK_DelayAtLeastUs(80000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    PCAL6408_SetDirection(&handle, (1 << BOARD_PCAL6408_AQR113C_RST_B_3V3), kPCAL6408_Output);
    PCAL6408_ClearPins(&handle, (1 << BOARD_PCAL6408_AQR113C_RST_B_3V3));
    SDK_DelayAtLeastUs(20000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    PCAL6408_SetPins(&handle, (1 << BOARD_PCAL6408_AQR113C_RST_B_3V3));
    SDK_DelayAtLeastUs(80000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    /* Initialize PHY */
    result = PHY_AQR113C_Init(phy_handle, config);

    return result;
}

#endif
void BOARD_InitHardware(void)
{
    /* clang-format off */
    hal_clk_t hal_dispapbCLKCfg = {
        .clk_id = hal_clock_dispapb,
        .pclk_id = hal_clock_syspll1dfs1div2,
        .div = 3, /* Source clock value 400Mhz, the current freq 133Mhz */
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    hal_clk_t hal_dispaxiCLKCfg = {
        .clk_id = hal_clock_dispaxi,
        .pclk_id = hal_clock_syspll1dfs2,
        .div = 1, /* AXI clock 800Mhz */
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    hal_clk_t hal_dispocramCLKCfg = {
        .clk_id = hal_clock_dispocram,
        .pclk_id = hal_clock_syspll1dfs2,
        .div = 2, /* Ocram clocck value 333Mhz*/
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    hal_pwr_s_t pwrst = {
        .did = HAL_POWER_PLATFORM_MIX_SLICE_IDX_DISPLAY,
        .st = hal_power_state_on,
    };

    hal_pwr_s_t campwrst = {
        .did = HAL_POWER_PLATFORM_MIX_SLICE_IDX_CAMERA,
        .st = hal_power_state_on,
    };

    hal_clk_t hal_lpi2cClkCfg = {
        .clk_id = hal_clock_lpi2c2,
        .pclk_id = hal_clock_osc24m,
        .div = 1, /* 24Mhz for lpi2c */
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
        .rate = 24000000UL,
    };

    hal_clk_t hal_lpi3cClkCfg = {
        .clk_id = hal_clock_lpi2c3,
        .pclk_id = hal_clock_osc24m,
        .div = 1, /* 24Mhz for lpi2c */
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
        .rate = 24000000UL,
    };

    /* IMX95_CLK_CAM_ISI expected 667000000*/
    hal_clk_t hal_camisiClkCfg = {
        .clk_id = hal_clock_camIsi,
        .pclk_id = hal_clock_syspll1dfs1,
        .div = 6,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    /* MIPI CAM APB expected 133330000hz */
    hal_clk_t hal_camapbClkCfg = {
        .clk_id = hal_clock_camApb,
        .pclk_id = hal_clock_syspll1dfs1div2,
        .div = 30,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    /* MIPI DPHY expected 24mhz */
    hal_clk_t hal_mipiphyClkCfg = {
        .clk_id = hal_clock_mipiPhyCfg,
        .pclk_id = hal_clock_osc24m,
        .div = 1,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    /* MIPI DPHY expected 24mhz */
    hal_clk_t hal_mipiphytestClkCfg = {
        .clk_id = hal_clock_mipiTestByte,
        .pclk_id = hal_clock_osc24m,
        .div = 1,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    /* MIPI DPHY expected 24mhz */
    hal_clk_t hal_mipiphypllbypassClkCfg = {
        .clk_id = hal_clock_mipiPhyPllBypass,
        .pclk_id = hal_clock_osc24m,
        .div = 1,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    hal_clk_t hal_mipiphypllRefClkCfg = {
        .clk_id = hal_clock_mipiPhyPllRef,
        .pclk_id = hal_clock_osc24m,
        .div = 1,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    /* provide CSI_CLK to connected sensor */
    hal_clk_t hal_ccmclk1ClkCfg = {
        .clk_id = hal_clock_ccmcko1,
        .pclk_id = hal_clock_osc24m,
        .div = 1, /* 24Mhz for lpi2c */
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };

#ifdef CONFIG_ISPSDK_ETH

    /* enetClk 666.66MHz */
    hal_clk_t hal_enetclk = {
        .clk_id = hal_clock_enet,
        .pclk_id = hal_clock_syspll1dfs2,
        .div = 1,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    /* enetRefClk 250MHz */
    hal_clk_t hal_enetrefclk = {
        .clk_id = hal_clock_enetref,
        .pclk_id = hal_clock_syspll1dfs0,
        .div = 4,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    /* enetTimer1Clk 100MHz */
    hal_clk_t hal_enettimer1clk = {
        .clk_id = hal_clock_enettimer1,
        .pclk_id = hal_clock_syspll1dfs0div2,
        .div = 5,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    /* NETCMIX power up */
    hal_pwr_s_t pwrst_eth = {
        .did = HAL_POWER_PLATFORM_MIX_SLICE_IDX_NETC,
        .st = hal_power_state_on,
    };
    /* lpi2c5Clk 24MHz */
    hal_clk_t hal_lpi2c5ClkCfg = {
        .clk_id = hal_clock_lpi2c5,
        .pclk_id = hal_clock_osc24m,
        .div = 1,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    /* lpi2c7Clk 24MHz */
    hal_clk_t hal_lpi2c7ClkCfg = {
        .clk_id = hal_clock_lpi2c7,
        .pclk_id = hal_clock_osc24m,
        .div = 1,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
#endif
    /* clang-format on */

    SM_Platform_Init();

    /* Power on the Displaymix */
    HAL_PowerSetState(&pwrst);
    while (HAL_PowerGetState(&pwrst));

    HAL_PowerSetState(&campwrst);
    while (HAL_PowerGetState(&campwrst));
#ifdef CONFIG_ISPSDK_ETH
	/* Power up NETCMIX */
    HAL_PowerSetState(&pwrst_eth);
    while(HAL_PowerGetState(&pwrst_eth));
#endif

    BOARD_ConfigMPU_ISP();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    HAL_ClockSetRootClk(&hal_dispapbCLKCfg);
    HAL_ClockSetRootClk(&hal_dispaxiCLKCfg);
    HAL_ClockSetRootClk(&hal_dispocramCLKCfg);
    HAL_ClockSetRootClk(&hal_lpi2cClkCfg);
    HAL_ClockSetRootClk(&hal_lpi3cClkCfg);

    HAL_ClockSetRootClk(&hal_camisiClkCfg);
    HAL_ClockSetRootClk(&hal_camapbClkCfg);
    HAL_ClockSetRootClk(&hal_mipiphyClkCfg);
    HAL_ClockSetRootClk(&hal_mipiphytestClkCfg);
    HAL_ClockSetRootClk(&hal_mipiphypllbypassClkCfg);
    HAL_ClockSetRootClk(&hal_mipiphypllRefClkCfg);
    HAL_ClockSetRootClk(&hal_ccmclk1ClkCfg);
#ifdef CONFIG_ISPSDK_ETH
    HAL_ClockSetRootClk(&hal_enetclk);
    HAL_ClockSetRootClk(&hal_enetrefclk);
    HAL_ClockSetRootClk(&hal_enettimer1clk);
    HAL_ClockSetRootClk(&hal_lpi2c5ClkCfg);
    HAL_ClockSetRootClk(&hal_lpi2c7ClkCfg);
#endif

    BOARD_PrepareDisplay();

    IRQSTEER_Init(IRQSTEER);

    NVIC_SetPriority(LPI2C2_IRQn, 5);
    NVIC_SetPriority(LPI2C3_IRQn, 5);
    NVIC_SetPriority(ISI_IRQn, 5);
    for (int i=0; i < FSL_FEATURE_IRQSTEER_MASTER_COUNT; i++)
    {
        NVIC_SetPriority(IRQSTEER_0_IRQn+i, 5);
    }
    // EnableIRQ(DMA5_2_0_1_IRQn); // eDMA IRQ
    NVIC_SetPriority(DMA5_2_0_1_IRQn, 5);

#ifdef CONFIG_USB_CDC_ACM
    /* USB init */
    pcal6524_handle_t handle;

    BOARD_InitPCAL6524(&handle);
    PCAL6524_SetDirection(&handle, (1 << BOARD_PCAL6524_USB2_PWR_EN), kPCAL6524_Output);
    PCAL6524_ClearPins(&handle, (1 << BOARD_PCAL6524_USB2_PWR_EN));
    SDK_DelayAtLeastUs(20000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    PCAL6524_SetPins(&handle, (1 << BOARD_PCAL6524_USB2_PWR_EN));
#endif
#ifdef CONFIG_ISPSDK_ETH
    pcal6524_handle_t handle1;
    pcal6408_handle_t handle2;
    /* Enable 156.25MHz clock to 10G ETH_CLKIN_P/ETH_CLKIN_N */
    BOARD_InitPCAL6524(&handle1);
    PCAL6524_SetDirection(&handle1, (1 << BOARD_PCAL6524_SI5332_RST), kPCAL6524_Output);
    PCAL6524_ClearPins(&handle1, (1 << BOARD_PCAL6524_SI5332_RST));
    SDK_DelayAtLeastUs(100000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    BOARD_InitPCAL6408_I2C5(&handle2);
    PCAL6408_SetDirection(&handle2, (1 << BOARD_PCAL6408_ETH_CLK_EN), kPCAL6408_Output);
    PCAL6408_ClearPins(&handle2, (1 << BOARD_PCAL6408_ETH_CLK_EN));
    SDK_DelayAtLeastUs(100000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    /* Protocol configure */
    BLK_CTRL_NETCMIX->CFG_LINK_MII_PROT = 0x00000522;
    BLK_CTRL_NETCMIX->CFG_LINK_PCS_PROT_2 = 0x00000040;

    /* Unlock the IERB. It will warm reset whole NETC. */
    NETC_PRIV->NETCRR &= ~NETC_PRIV_NETCRR_LOCK_MASK;
    while ((NETC_PRIV->NETCRR & NETC_PRIV_NETCRR_LOCK_MASK) != 0U)
    {
    }

    /* Lock the IERB. */
    NETC_PRIV->NETCRR |= NETC_PRIV_NETCRR_LOCK_MASK;
    while ((NETC_PRIV->NETCSR & NETC_PRIV_NETCSR_STATE_MASK) != 0U)
    {
    }

    IRQSTEER_EnableInterrupt(IRQSTEER, MSGINTR2_IRQn);

    g_phy_rtl8211f_resource.write = ENETC0_EMDIOWrite;
    g_phy_rtl8211f_resource.read  = ENETC0_EMDIORead;

    g_phy_aqr113c_resource.write = ENETC2_EMDIOC45Write;
    g_phy_aqr113c_resource.read  = ENETC2_EMDIOC45Read;
#endif
}

#ifdef CONFIG_USB_CDC_ACM
void USB2_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(s_cdcVcom.deviceHandle);
}

void USB_DeviceClockInit(void)
{
}

void USB_DeviceIsrEnable(void)
{
    uint8_t irqNumber;

    irqNumber                = USB2_IRQn;
/* USB_DEVICE_CONFIG_EHCI */

/* Install isr, set priority, and enable IRQ. */
#if defined(__GIC_PRIO_BITS)
    GIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
#else
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
#endif
    EnableIRQ((IRQn_Type)irqNumber);
}
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle)
{
    USB_DeviceEhciTaskFunction(deviceHandle);
}
#endif

#endif /* CONFIG_USB_CDC_ACM */
/*${function:end}*/
