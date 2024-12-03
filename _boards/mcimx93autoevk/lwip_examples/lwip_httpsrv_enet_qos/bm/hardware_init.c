/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "app.h"
#include "fsl_enet_qos.h"
/*${header:end}*/

/*${variable:start}*/
phy_rtl8211f_resource_t g_phy_resource;
/*${variable:end}*/

/*${function:start}*/
void ENET_QOS_EnableClock(bool enable)
{
    BLK_CTRL_WAKEUPMIX->GPR =
        (BLK_CTRL_WAKEUPMIX->GPR & (~BLK_CTRL_WAKEUPMIX_GPR_ENABLE_MASK)) | BLK_CTRL_WAKEUPMIX_GPR_ENABLE(enable);
}
void ENET_QOS_SetSYSControl(enet_qos_mii_mode_t miiMode)
{
    BLK_CTRL_WAKEUPMIX->GPR |= BLK_CTRL_WAKEUPMIX_GPR_MODE(miiMode);
}

static void MDIO_Init(void)
{
    /* Set SMI first. */
    CLOCK_EnableClock(s_enetqosClock[ENET_QOS_GetInstance(ENET_QOS)]);
    ENET_QOS_SetSMI(ENET_QOS, EXAMPLE_CLOCK_FREQ);
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_QOS_MDIOWrite(ENET_QOS, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_QOS_MDIORead(ENET_QOS, phyAddr, regAddr, pData);
}

void BOARD_InitHardware(void)
{
    /* clang-format off */

    /* enetqosSysClk 250MHz */
    const clock_root_config_t enetqosSysClkCfg = {
        .clockOff = false,
	.mux = kCLOCK_WAKEUPAXI_ClockRoot_MuxSysPll1Pfd0, // 1000MHz
	.div = 4
    };

    /* enetqosClk 250MHz (For 125MHz TX_CLK ) */
    const clock_root_config_t enetqosClkCfg = {
        .clockOff = false,
	.mux = kCLOCK_ENET_ClockRoot_MuxSysPll1Pfd0Div2, // 500MHz
	.div = 2
    };

    const clock_root_config_t lpi2cClkCfg = {
        .clockOff = false,
	.mux = 0, // 24MHz oscillator source
	.div = 1
    };
    /* clang-format on */

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRootClock(ENET_QOS_SYSTEM_CLOCK_ROOT, &enetqosSysClkCfg);
    CLOCK_SetRootClock(ENET_QOS_CLOCK_ROOT, &enetqosClkCfg);
    CLOCK_EnableClock(ENET_QOS_CLOCK_GATE);
    CLOCK_SetRootClock(BOARD_PCAL6524_I2C_CLOCK_ROOT, &lpi2cClkCfg);
    CLOCK_EnableClock(BOARD_PCAL6524_I2C_CLOCK_GATE);

    /* For a complete PHY reset of RTL8211FDI-CG, this pin must be asserted low for at least 10ms. And
     * wait for a further 30ms(for internal circuits settling time) before accessing the PHY register */
    pcal6524_handle_t handle;
    BOARD_InitPCAL6524_A(&handle);
    PCAL6524_SetDirection(&handle, (1 << BOARD_PCAL6524_A_ENET1_NRST), kPCAL6524_Output);
    PCAL6524_ClearPins(&handle, (1 << BOARD_PCAL6524_A_ENET1_NRST));
    SDK_DelayAtLeastUs(10000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    PCAL6524_SetPins(&handle, (1 << BOARD_PCAL6524_A_ENET1_NRST));
    SDK_DelayAtLeastUs(30000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    EnableIRQ(ENET_QOS_IRQn);
    NVIC_SetPriority(ENET_QOS_IRQn, ENET_PRIORITY);

    MDIO_Init();
    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;
}
/*${function:end}*/
