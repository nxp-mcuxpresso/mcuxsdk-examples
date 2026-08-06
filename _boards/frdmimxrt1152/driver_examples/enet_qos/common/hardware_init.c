/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "PERI_ENET_QOS.h"
#include "fsl_enet.h"
#include "fsl_enet_qos.h"
#include "fsl_phy.h"
#include "fsl_iomuxc.h"
#include "fsl_phyjl1111.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/
phy_jl1111_resource_t g_phy_resource;
phy_operations_t g_board_phy_ops;
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitModuleClock(void)
{
    const clock_sys_pll1_config_t sysPll1Config = {
        .pllDiv2En = true,
    };
    CLOCK_InitSysPll1(&sysPll1Config);

    clock_root_config_t rootCfg = {.mux = 4, .div = 10};   /* Generate 50M RMII root clock. */
    CLOCK_SetRootClock(kCLOCK_Root_Enet_Qos, &rootCfg);
    CLOCK_SetRootClock(kCLOCK_Root_Enet_Timer3, &rootCfg); /* Generate 50M PTP REF clock. */
}

void ENET_QOS_EnableClock(bool enable)
{
    IOMUXC_GPR->GPR6 =
        (IOMUXC_GPR->GPR6 & (~IOMUXC_GPR_GPR6_ENET_QOS_CLKGEN_EN_MASK)) | IOMUXC_GPR_GPR6_ENET_QOS_CLKGEN_EN(enable);
}

void ENET_QOS_SetSYSControl(enet_qos_mii_mode_t miiMode)
{
    IOMUXC_GPR->GPR6 =
        (IOMUXC_GPR->GPR6 & (~IOMUXC_GPR_GPR6_ENET_QOS_INTF_SEL_MASK)) | IOMUXC_GPR_GPR6_ENET_QOS_INTF_SEL(miiMode);
}

static void MDIO_Init(void)
{
    /* RT1152 routes the shared PHY MDC/MDIO pins to the ENET_1G SMI controller. */
    (void)CLOCK_EnableClock(s_enetClock[ENET_GetInstance(ENET_1G)]);
    ENET_SetSMI(ENET_1G, CORE_CLK_FREQ, false);
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(ENET_1G, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(ENET_1G, phyAddr, regAddr, pData);
}


#if defined(EXAMPLE_PHY_LOOPBACK_ENABLE)
/* GetLinkStatus override that enables ENET_QOS MAC internal loopback.
 * Must be set after ENET_QOS_Init, because that clears any LM bit set beforehand. */
static status_t BOARD_PHY_GetLinkStatus(phy_handle_t *handle, bool *status)
{
    ENET_QOS->MAC_CONFIGURATION |= ENET_QOS_MAC_CONFIGURATION_LM_MASK;
    return PHY_JL1111_GetLinkStatus(handle, status);
}

static status_t BOARD_PHY_EnableLoopback(phy_handle_t *handle, phy_loop_t mode, phy_speed_t speed, bool enable)
{
    if (speed > kPHY_Speed100M)
    {
        speed = kPHY_Speed100M;
    }
    return PHY_JL1111_EnableLoopback(handle, mode, speed, enable);
}
#endif

void BOARD_InitHardware(void)
{
    /* Hardware Initialization. */
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_Init6524Pins();
    BOARD_InitENET_QOSPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitModuleClock();

    IOMUXC_GPR->GPR6 &= ~IOMUXC_GPR_GPR6_ENET_QOS_RGMII_EN_MASK; /* Use RMII connection to the 100M PHY. */
    IOMUXC_GPR->GPR6 |= IOMUXC_GPR_GPR6_ENET_QOS_REF_CLK_DIR_MASK; /* REF_CLK = output */

    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B2_20_ENET_QOS_REF_CLK, 1U);

    /* JL1111BI datasheet minimum reset timing:
     * - assert reset low for at least 200 ns
     * - after reset deassertion, wait at least 5 ms before first SMI access
     */
    pcal6524_handle_t handle;

    BOARD_InitPCAL6524(&handle);
    PCAL6524_SetDirection(&handle, (1UL << BOARD_PCAL6524_ENET_QOS_RST_B), kPCAL6524_Output);
    PCAL6524_ClearPins(&handle, (1UL << BOARD_PCAL6524_ENET_QOS_RST_B));
    SDK_DelayAtLeastUs(1U, CLOCK_GetFreq(kCLOCK_CpuClk));
    PCAL6524_SetPins(&handle, (1UL << BOARD_PCAL6524_ENET_QOS_RST_B));
    SDK_DelayAtLeastUs(5000U, CLOCK_GetFreq(kCLOCK_CpuClk));

    MDIO_Init();
    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;
    g_board_phy_ops = phyjl1111_ops;
#if defined(EXAMPLE_PHY_LOOPBACK_ENABLE)
    g_board_phy_ops.enableLoopback = BOARD_PHY_EnableLoopback;
    g_board_phy_ops.getLinkStatus = BOARD_PHY_GetLinkStatus;
#endif
}
/*${function:end}*/
