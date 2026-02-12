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
#include "fsl_phylan8741.h"
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
#define EXAMPLE_PHY_ADDRESS 0

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static phy_lan8741_resource_t g_phy_resource;

extern volatile uint32_t appEvent;
extern volatile uint32_t BOARD_SystickCount;

ENET_Type *BOARD_Enet = ENET0;
const phy_operations_t *BOARD_PhyOps = &phylan8741_ops;
uint32_t BOARD_PhySysClock;
uint8_t BOARD_PhyAddress = EXAMPLE_PHY_ADDRESS;
void *BOARD_PhySource = &g_phy_resource;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void MDIO_Init(void)
{
    ENET_SetSMI(BOARD_Enet, BOARD_PhySysClock);
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
    BOARD_InitBootPins();
    BOARD_InitENETPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    RESET_PeripheralReset(kENET0_RST_SHIFT_RSTn);

    /* Use external RMII clock. */
    CLOCK_AttachClk(kNONE_to_ENETRMII);
    CLOCK_EnableClock(s_enetClock[ENET_GetInstance(BOARD_Enet)]);

    /* Reset PHY */
    GPIO_PinWrite(BOARD_INITENETPINS_EPHY_RSTB_GPIO, BOARD_INITENETPINS_EPHY_RSTB_PIN, 0U);
    SDK_DelayAtLeastUs(25000, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    GPIO_PinWrite(BOARD_INITENETPINS_EPHY_RSTB_GPIO, BOARD_INITENETPINS_EPHY_RSTB_PIN, 1U);

    /* Assign MDIO callbacks */
    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;

    BOARD_PhySysClock = CLOCK_GetCoreSysClkFreq();

    /* Initialize MDIO */
    MDIO_Init();

    /* Connect ENET to external PHY over RMII */
    SYSCON->ENET_CTRL = SYSCON_ENET_CTRL_PHY_SEL(0) | SYSCON_ENET_CTRL_PHY_INTF(1);

    SysTick_Config(SystemCoreClock / 1000U);
}

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
void USB1_HS_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(ethNicHandle.deviceHandle);
}
#endif

void USB_DeviceClockInit(void)
{
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };
#endif
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    SPC0->ACTIVE_VDELAY = 0x0500;
    /* Change the power DCDC to 1.8v (By deafult, DCDC is 1.8V), CORELDO to 1.1v (By deafult, CORELDO is 1.0V) */
    SPC0->ACTIVE_CFG &= ~SPC_ACTIVE_CFG_CORELDO_VDD_DS_MASK;
    SPC0->ACTIVE_CFG |= SPC_ACTIVE_CFG_CORELDO_VDD_LVL(0x3);
    /* Wait until it is done */
    while (SPC0->SC & SPC_SC_BUSY_MASK)
        ;
    if (0u == (SCG0->LDOCSR & SCG_LDOCSR_LDOEN_MASK))
    {
        SCG0->TRIM_LOCK = 0x5a5a0001U;
        SCG0->LDOCSR |= SCG_LDOCSR_LDOEN_MASK;
        /* wait LDO ready */
        while (0U == (SCG0->LDOCSR & SCG_LDOCSR_VOUT_OK_MASK))
            ;
    }
    
    MRCC0->MRCC_GLB_CC2_SET = MRCC_MRCC_GLB_CC2_USB1_MASK | MRCC_MRCC_GLB_CC2_USB1_PHY_MASK;
    SCG0->SOSCCFG &= ~(SCG_SOSCCFG_RANGE_MASK | SCG_SOSCCFG_EREFS_MASK);
    /* xtal = 20 ~ 30MHz */
    SCG0->SOSCCFG = (1U << SCG_SOSCCFG_RANGE_SHIFT) | (1U << SCG_SOSCCFG_EREFS_SHIFT);
    SCG0->SOSCCSR |= SCG_SOSCCSR_SOSCEN_MASK;
    while (1)
    {
        if (SCG0->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK)
        {
            break;
        }
    }

    CLOCK_AttachClk(kPHY_CLK_XTAL_to_USBHS);
    CLOCK_AttachClk(kCLK_IN_to_USBHS_PHY);
    CLOCK_EnableClock(kCLOCK_GateUSBHS);
    CLOCK_EnableClock(kCLOCK_GateUSBHS_PHY);
    CLOCK_SetClockDiv(kCLOCK_DivUSBHS_PHY, 1);
    CLOCK_EnableUsbhsPhyPllClock(BOARD_XTAL0_CLK_HZ);
    CLOCK_EnableUsbhsClock();
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);
#endif
}

void USB_DeviceIsrEnable(void)
{
    uint8_t irqNumber;
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    uint8_t usbDeviceEhciIrq[] = USBHS_IRQS;
    irqNumber                  = usbDeviceEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];
#endif

    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ((IRQn_Type)irqNumber);
}

#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle)
{
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    USB_DeviceEhciTaskFunction(deviceHandle);
#endif
}
#endif

void SysTick_Handler(void)
{
    if (!(BOARD_SystickCount++ % APP_ETH_LINK_CHECK_INTERVAL_MS))
    {
        APP_ETH_NIC_EVENT_SET(appEvent, kAPP_CheckLinkChange);
    }
}
