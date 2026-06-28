/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_enet.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/
phy_yt8521_resource_t g_phy_resource;
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitModuleClock(void)
{
    const clock_sys_pll1_config_t sysPll1Config = {
        .pllDiv2En = true,
    };
    CLOCK_InitSysPll1(&sysPll1Config);

    clock_root_config_t rootCfg = {.mux = 4, .div = 4}; /* Generate 125M root clock. */
    CLOCK_SetRootClock(kCLOCK_Root_Enet2, &rootCfg);
}

static void MDIO_Init(void)
{
    (void)CLOCK_EnableClock(s_enetClock[ENET_GetInstance(EXAMPLE_ENET)]);
    ENET_SetSMI(EXAMPLE_ENET, EXAMPLE_CLOCK_FREQ, false);
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(EXAMPLE_ENET, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(EXAMPLE_ENET, phyAddr, regAddr, pData);
}

void BOARD_InitHardware(void)
{
    /* Hardware Initialization. */
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitENET_1GPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitModuleClock();

    IOMUXC_GPR->GPR5 |= IOMUXC_GPR_GPR5_ENET1G_RGMII_EN_MASK; /* Enable RGMII TX clock output on TX_CLK pad. */

    pcal6524_handle_t handle;

    BOARD_InitPCAL6524(&handle);
    PCAL6524_SetDirection(&handle, (1UL << BOARD_PCAL6524_ETH0_RST_B), kPCAL6524_Output);
    PCAL6524_ClearPins(&handle, (1UL << BOARD_PCAL6524_ETH0_RST_B));
    SDK_DelayAtLeastUs(10000U, CLOCK_GetFreq(kCLOCK_CpuClk));
    PCAL6524_SetPins(&handle, (1UL << BOARD_PCAL6524_ETH0_RST_B));
    SDK_DelayAtLeastUs(150000U, CLOCK_GetFreq(kCLOCK_CpuClk));

    EnableIRQ(ENET_1G_MAC0_Tx_Rx_1_IRQn);
    EnableIRQ(ENET_1G_MAC0_Tx_Rx_2_IRQn);

    MDIO_Init();
    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;
}
/*${function:end}*/
