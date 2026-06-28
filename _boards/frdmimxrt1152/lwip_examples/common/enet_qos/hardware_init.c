/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "app.h"
#include "fsl_enet.h"
#include "fsl_enet_qos.h"
/*${header:end}*/

/*${variable:start}*/
phy_jl1111_resource_t g_phy_resource;
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
    ENET_SetSMI(ENET_1G, EXAMPLE_CLOCK_FREQ, false);
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(ENET_1G, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(ENET_1G, phyAddr, regAddr, pData);
}

void BOARD_InitHardware(void)
{
    /* Hardware Initialization. */
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitENET_QOSPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitModuleClock();

    IOMUXC_GPR->GPR6 &= ~IOMUXC_GPR_GPR6_ENET_QOS_RGMII_EN_MASK; /* Use RMII connection to the 100M PHY. */

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
}
/*${function:end}*/
