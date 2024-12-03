/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "board.h"
#include "fsl_phy.h"
#include "fsl_gpio.h"
#include "pin_mux.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/
phy_rtl8211f_resource_t g_phy_resource;
/*${variable:end}*/

/*${function:start}*/
static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_QOS_MDIOWrite(EXAMPLE_ENET_QOS_BASE, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_QOS_MDIORead(EXAMPLE_ENET_QOS_BASE, phyAddr, regAddr, pData);
}

void BOARD_InitHardware(void)
{
    /* M7 has its local cache and enabled by default,
     * need to set smart subsystems (0x28000000 ~ 0x3FFFFFFF)
     * non-cacheable before accessing this address region */
    BOARD_InitMemory();

    /* Board specific RDC settings */
    BOARD_RdcInit();

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRootDivider(kCLOCK_RootEnetAxi, 1U, 1U);
    CLOCK_SetRootMux(kCLOCK_RootEnetAxi, kCLOCK_EnetAxiRootmuxSysPll2Div4); /* SYSTEM PLL2 divided by 4: 250Mhz */

    CLOCK_SetRootDivider(kCLOCK_RootEnetQos, 1U, 1U);
    CLOCK_SetRootMux(kCLOCK_RootEnetQos, kCLOCK_EnetQosRootmuxSysPll2Div8); /* SYSTEM PLL2 divided by 8: 125Mhz */

    CLOCK_SetRootDivider(kCLOCK_RootEnetQosTimer, 1U, 1U);
    CLOCK_SetRootMux(kCLOCK_RootEnetQosTimer,
                     kCLOCK_EnetQosTimerRootmuxSysPll2Div10); /* SYSTEM PLL2 divided by 10: 100Mhz */

    IOMUXC_GPR->GPR1 |=
        IOMUXC_GPR_GPR1_GPR_ENET_QOS_CLK_GEN_EN_MASK | IOMUXC_GPR_GPR1_IOMUXC_GPR_ENET_QOS_RGMII_EN_MASK |
        IOMUXC_GPR_GPR1_IOMUXC_GPR_ENET1_RGMII_EN_MASK; /* Set this bit to enable ENET_QOS clock generation. */

    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
    GPIO_PinInit(GPIO4, 22, &gpio_config);
    /* For a complete PHY reset of RTL8211FDI-CG, this pin must be asserted low for at least 10ms. And
     * wait for a further 30ms(for internal circuits settling time) before accessing the PHY register */
    GPIO_WritePinOutput(GPIO4, 22, 0);
    SDK_DelayAtLeastUs(10000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    GPIO_WritePinOutput(GPIO4, 22, 1);
    SDK_DelayAtLeastUs(30000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    ENET_QOS_SetSMI(EXAMPLE_ENET_QOS_BASE, CORE_CLK_FREQ);
    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;
}

void BOARD_UpdateENETModuleClock(enet_qos_mii_speed_t miiSpeed)
{
    switch (miiSpeed)
    {
        case kENET_QOS_MiiSpeed1000M:
            /* Generate 125M root clock for 1000Mbps. */
            CLOCK_SetRootDivider(kCLOCK_RootEnetQos, 1U, 1U);
            CLOCK_SetRootMux(kCLOCK_RootEnetQos,
                             kCLOCK_EnetQosRootmuxSysPll2Div8); /* SYSTEM PLL2 divided by 8: 125Mhz */
            break;
        case kENET_QOS_MiiSpeed100M:
            /* Generate 25M root clock for 100Mbps. */
            CLOCK_SetRootDivider(kCLOCK_RootEnetQos, 1U, 2U);
            CLOCK_SetRootMux(kCLOCK_RootEnetQos,
                             kCLOCK_EnetQosRootmuxSysPll2Div20); /* SYSTEM PLL2 divided by 20: 50Mhz */
            break;
        case kENET_QOS_MiiSpeed10M:
            /* Generate 2.5M root clock for 10Mbps. */
            CLOCK_SetRootDivider(kCLOCK_RootEnetQos, 4U, 5U);
            CLOCK_SetRootMux(kCLOCK_RootEnetQos,
                             kCLOCK_EnetQosRootmuxSysPll2Div20); /* SYSTEM PLL2 divided by 20: 50Mhz */
            break;
        default:
            /* Generate 125M root clock. */
            CLOCK_SetRootDivider(kCLOCK_RootEnetQos, 1U, 1U);
            CLOCK_SetRootMux(kCLOCK_RootEnetQos,
                             kCLOCK_EnetQosRootmuxSysPll2Div8); /* SYSTEM PLL2 divided by 8: 125Mhz */
            break;
    }
}
void ENET_QOS_EnableClock(bool enable)
{
    IOMUXC_GPR->GPR1 = (IOMUXC_GPR->GPR1 & (~IOMUXC_GPR_GPR1_GPR_ENET_QOS_CLK_GEN_EN_MASK)) |
                       IOMUXC_GPR_GPR1_GPR_ENET_QOS_CLK_GEN_EN(enable);
}
void ENET_QOS_SetSYSControl(enet_qos_mii_mode_t miiMode)
{
    IOMUXC_GPR->GPR1 |= (miiMode << 16U);
}
/*${function:end}*/
