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
#include "fsl_iomuxc.h"

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
    clock_root_config_t rootCfg = {.mux = 4, .div = 10}; /* Generate 50M RMII root clock. */
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

/* Board-specific JL1111 RMII fix-up over MDIO, after PHY reset. RMSR is the
 * vendor RMII Mode Setting Register (page 7, reg 0x10).
 *   - clear RMII_CLK_DIR (bit 12): keep the PHY as the 50 MHz RMII clock source
 *   - clear RMII_RX_REVERSE (bit 4): flip the RX sampling edge */
static void BOARD_ConfigJL1111Rmii(void)
{
    const uint8_t phyAddr = EXAMPLE_PHY_ADDRESS;
    const uint8_t PAGE_SELECT = 0x1FU; /* Page Select Register */
    const uint8_t RMII_PAGE = 0x07U;
    const uint8_t RMSR_REG = 0x10U; /* RMII Mode Setting Register */

    const uint16_t RMSR_CLK_DIR = 0x1000U;    /* bit 12: 1 = REF_CLK input (must be 0 here) */
    const uint16_t RMSR_RX_REVERSE = 0x0010U; /* bit 4 : RX clock edge select */

    uint16_t rmsr = 0U;

    (void)MDIO_Write(phyAddr, PAGE_SELECT, RMII_PAGE);
    (void)MDIO_Read(phyAddr, RMSR_REG, &rmsr);

    rmsr = (uint16_t)(rmsr & ~RMSR_CLK_DIR);
    rmsr = (uint16_t)(rmsr & ~RMSR_RX_REVERSE);

    (void)MDIO_Write(phyAddr, RMSR_REG, rmsr);
    (void)MDIO_Write(phyAddr, PAGE_SELECT, 0x00U);
}

/* Board fix-up of the ENET_QOS config, called by the shared lwIP QoS port when
 * LWIP_ENET_FLEXIBLE_CONFIGURATION is set. The default config is RGMII 1000M,
 * which is invalid and hangs ENET_QOS_Init() */
void BOARD_ENETFlexibleConfigure(enet_qos_config_t *config)
{
    config->miiMode = kENET_QOS_RmiiMode;
    config->miiSpeed = kENET_QOS_MiiSpeed100M;
    config->miiDuplex = kENET_QOS_MiiFullDuplex;
}

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
    IOMUXC_GPR->GPR6 |= IOMUXC_GPR_GPR6_ENET_QOS_REF_CLK_DIR_MASK;
    /* REF_CLK_DIR=OUT: MAC drives the 50 MHz RMII clock, SION loops it back to
     * clock the MAC RX. Taking REF_CLK as input hangs PHY init on this board. */
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

    NVIC_SetPriority(ENET_QOS_IRQn, ENET_PRIORITY);

    MDIO_Init();
    g_phy_resource.read = MDIO_Read;
    g_phy_resource.write = MDIO_Write;

    BOARD_ConfigJL1111Rmii();
}

/*${function:end}*/
