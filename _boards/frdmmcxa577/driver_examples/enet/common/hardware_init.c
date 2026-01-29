/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_enet.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/
#if BOARD_NETWORK_USE_TENBASET_PHY
phy_tenbaset_resource_t g_phy_resource;
#else
phy_lan8741_resource_t g_phy_resource;
#endif /* BOARD_NETWORK_USE_TENBASET_PHY */

#if BOARD_NETWORK_USE_TENBASET_PHY

static status_t PHY_Wrapped_Init(phy_handle_t *handle, const phy_config_t *config);

const phy_operations_t phy_ops = {.phyInit            = &PHY_Wrapped_Init,
                                  .getAutoNegoStatus  = &PHY_TENBASET_GetAutoNegotiationStatus,
                                  .getLinkStatus      = &PHY_TENBASET_GetLinkStatus,
                                  .getLinkSpeedDuplex = &PHY_TENBASET_GetLinkSpeedDuplex,
                                  .setLinkSpeedDuplex = &PHY_TENBASET_SetLinkSpeedDuplex,
                                  .enableLoopback     = &PHY_TENBASET_EnableLoopback};
#endif /* BOARD_NETWORK_USE_TENBASET_PHY */
/*${variable:end}*/

/*${function:start}*/

#if !BOARD_NETWORK_USE_TENBASET_PHY
static void MDIO_Init(void)
{
    ENET_SetSMI(EXAMPLE_ENET_BASE, EXAMPLE_CLOCK_FREQ);
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(EXAMPLE_ENET_BASE, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(EXAMPLE_ENET_BASE, phyAddr, regAddr, pData);
}
#endif /* !BOARD_NETWORK_USE_TENBASET_PHY */

void BOARD_InitHardware(void)
{
    /* Hardware Initialization. */
    BOARD_InitBootPins();
#if BOARD_NETWORK_USE_TENBASET_PHY
    BOARD_InitTenBaseT1SPins();
    BOARD_BootClockPLL200M();
#else
    BOARD_InitENETPins();
    BOARD_InitBootClocks();
#endif /* BOARD_NETWORK_USE_TENBASET_PHY */
    BOARD_InitDebugConsole();

    RESET_PeripheralReset(kENET0_RST_SHIFT_RSTn);

    /* Use external RMII clock. */
    CLOCK_AttachClk(kNONE_to_ENETRMII);
    CLOCK_EnableClock(s_enetClock[ENET_GetInstance(EXAMPLE_ENET_BASE)]);

#if BOARD_NETWORK_USE_TENBASET_PHY
    CLOCK_AttachClk(kPll1Clk_to_TENBASET_PHY);
    CLOCK_SetClockDiv(kCLOCK_DivTENBASET_PHY0, 2);
    CLOCK_EnableClock(kCLOCK_GateTENBASET_PHY0);
    RESET_PeripheralReset(kT1S0_RST_SHIFT_RSTn);

    /* Enable the interrupt. */
    EnableIRQ(TENBASET_PHY0_IRQn);

    /* Enable ENET0 and TENBASET_PHY0 */
    MRCC0->MRCC_GLB_PR2 |= MRCC_MRCC_GLB_PR2_ENET0_MASK | MRCC_MRCC_GLB_PR2_T1S0_MASK;
    /* Disable RMII */
    MRCC0->MRCC_GLB_PR2 &= ~MRCC_MRCC_GLB_PR2_RMII_MASK;

    /* Connect ENET to internal TENBASET_PHY0 instead of external PHY */
    SYSCON->ENET_CTRL = SYSCON_ENET_CTRL_PHY_SEL(1) | SYSCON_ENET_CTRL_PHY_INTF(0);

    /* Set TENBASET_PHY0 configuration */
    memset(&g_phy_resource, 0, sizeof(phy_tenbaset_resource_t));
    TENBASET_PHY_GetDefaultConfig(&g_phy_resource.config);
    g_phy_resource.base                     = T1S0;
    g_phy_resource.config.plcaConfig.enable = true;
    g_phy_resource.config.plcaConfig.nodeId = 1U;
#else
    /* Reset PHY */
    GPIO_PinWrite(BOARD_INITENETPINS_EPHY_RSTB_GPIO, BOARD_INITENETPINS_EPHY_RSTB_PIN, 0U);
    SDK_DelayAtLeastUs(25000, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    GPIO_PinWrite(BOARD_INITENETPINS_EPHY_RSTB_GPIO, BOARD_INITENETPINS_EPHY_RSTB_PIN, 1U);

    /* Assign MDIO callbacks */
    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;

    /* Initialize MDIO */
    MDIO_Init();

    /* Enable ENET0 and RMII */
    MRCC0->MRCC_GLB_PR2 |= MRCC_MRCC_GLB_PR2_ENET0_MASK | MRCC_MRCC_GLB_PR2_RMII_MASK;
    /* Disable TENBASET_PHY0 */
    MRCC0->MRCC_GLB_PR2 &= ~MRCC_MRCC_GLB_PR2_T1S0_MASK;

    /* Connect ENET to external PHY over RMII */
    SYSCON->ENET_CTRL = SYSCON_ENET_CTRL_PHY_SEL(0) | SYSCON_ENET_CTRL_PHY_INTF(1);
#endif /* BOARD_NETWORK_USE_TENBASET_PHY */
}

#if BOARD_NETWORK_USE_TENBASET_PHY

/* Wrapper for PHY init to enable loopback */
static status_t PHY_Wrapped_Init(phy_handle_t *handle, const phy_config_t *config)
{
    status_t result;

    /* Initialize PHY */
    result = PHY_TENBASET_Init(handle, config);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Enable PCS loopback */
    return PHY_EnableLoopback(handle, kPHY_LocalLoop, kPHY_Speed10M, true);
}
#endif /* BOARD_NETWORK_USE_TENBASET_PHY */
/*${function:end}*/
