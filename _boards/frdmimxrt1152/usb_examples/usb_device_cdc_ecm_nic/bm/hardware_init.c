/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "board.h"
#include "clock_config.h"
#include "fsl_enet.h"
#include "fsl_phyyt8521.h"
#include "pin_mux.h"
#include "usb_device_config.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_descriptor.h"
#include "usb_eth_adapter.h"
#include "usb_phy.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Address of PHY interface. */
#define EXAMPLE_PHY_ADDRESS BOARD_ENET1_PHY_ADDRESS

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static phy_yt8521_resource_t g_phy_resource;

extern volatile uint32_t appEvent;
extern volatile uint32_t BOARD_SystickCount;

ENET_Type *BOARD_Enet = ENET_1G;
const phy_operations_t *BOARD_PhyOps = &phyyt8521_ops;
uint32_t BOARD_PhySysClock;
uint8_t BOARD_PhyAddress = EXAMPLE_PHY_ADDRESS;
void *BOARD_PhySource = &g_phy_resource;

/*******************************************************************************
 * Code
 ******************************************************************************/
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
    (void)CLOCK_EnableClock(s_enetClock[ENET_GetInstance(BOARD_Enet)]);
    ENET_SetSMI(BOARD_Enet, BOARD_PhySysClock, false);
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(BOARD_Enet, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(BOARD_Enet, phyAddr, regAddr, pData);
}

void BOARD_InitHardware(void)
{
    /* Hardware Initialization. */
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_Init6524Pins();
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

    BOARD_PhySysClock = CLOCK_GetRootClockFreq(kCLOCK_Root_Bus);

    MDIO_Init();
    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;
    
    SysTick_Config(SystemCoreClock / 1000U);
}

void USB_OTG1_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(ethNicHandle.deviceHandle);
}

void USB_DeviceClockInit(void)
{
    uint32_t usbClockFreq;
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };

    usbClockFreq = 24000000;
    CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, usbClockFreq);
    CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, usbClockFreq);

    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);
}

void USB_DeviceIsrEnable(void)
{
    uint8_t irqNumber;

    uint8_t usbDeviceEhciIrq[] = USBHS_IRQS;
    irqNumber                  = usbDeviceEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];

    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ((IRQn_Type)irqNumber);
}

#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle)
{
    USB_DeviceEhciTaskFunction(deviceHandle);
}
#endif

void SysTick_Handler(void)
{
    if (!(BOARD_SystickCount++ % APP_ETH_LINK_CHECK_INTERVAL_MS))
    {
        APP_ETH_NIC_EVENT_SET(appEvent, kAPP_CheckLinkChange);
    }
}
