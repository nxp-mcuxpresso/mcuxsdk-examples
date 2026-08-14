/**
 * Copyright 2025 NXP
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
#include "fsl_pcal6524.h"
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
#define EXAMPLE_PHY_ADDRESS BOARD_ENET0_PHY_ADDRESS

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static phy_yt8521_resource_t g_phy_resource;
static pcal6524_handle_t s_pcal6524Handle;

extern volatile uint32_t appEvent;
extern volatile uint32_t BOARD_SystickCount;

ENET_Type *BOARD_Enet                = COMM__ENET;
const phy_operations_t *BOARD_PhyOps = &phyyt8521_ops;
uint32_t BOARD_PhySysClock;
uint8_t BOARD_PhyAddress = EXAMPLE_PHY_ADDRESS;
void *BOARD_PhySource    = &g_phy_resource;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void MDIO_Init(void)
{
    CLOCK_EnableClock(s_enetClock[ENET_GetInstance(BOARD_Enet)]);
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
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_InitENETPins();
    BOARD_Init6524Pins();

    BOARD_PhySysClock = CLOCK_GetRootClockFreq(kCLOCK_Root_COMM_comm_clk);

    /* Reset YT8531 PHY (YT8521 register-compatible driver) via PCAL6524 P2_2 (BOARD_PCAL6524_ETH1_RST_B):
     * drive low for >= 10 ms, then high; PHY requires a further settling
     * delay before MDIO access. */
    BOARD_InitPCAL6524(&s_pcal6524Handle);
    {
        uint32_t rstMask = 1U << BOARD_PCAL6524_ETH1_RST_B;
        (void)PCAL6524_SetDirection(&s_pcal6524Handle, rstMask, kPCAL6524_Output);
        (void)PCAL6524_ClearPins(&s_pcal6524Handle, rstMask);
        SDK_DelayAtLeastUs(10000U, CLOCK_GetRootClockFreq(kCLOCK_Root_CMPT_cpu_clk));
        (void)PCAL6524_SetPins(&s_pcal6524Handle, rstMask);
        SDK_DelayAtLeastUs(30000U, CLOCK_GetRootClockFreq(kCLOCK_Root_CMPT_cpu_clk));
    }

    MDIO_Init();
    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;

    SysTick_Config(SystemCoreClock / 1000U);
}

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
void USBHS_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(ethNicHandle.deviceHandle);
}
#endif
#if (defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U))
void USBFS_IRQHandler(void)
{
    USB_DeviceKhciIsrFunction(ethNicHandle.deviceHandle);
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

    CLOCK_EnableUsbHsClock();
    USB_EhciPhyInit(CONTROLLER_ID, CLOCK_GetRootClockFreq(kCLOCK_Root_COMM_usb0_phyclk), &phyConfig);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    CLOCK_EnableUsbFsClock(kCLOCK_UsbFsSrcUsb1Root);
    /* Crystal-less: trim the FRO against the USB FS (KHCI) frame timing. */
    CLOCK_TrimUsbFroClock(kCLOCK_UsbFroTrimFs);
#endif
}

void USB_DeviceIsrEnable(void)
{
    IRQn_Type irqNumber;

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    IRQn_Type usbDeviceEhciIrq[] = USBHS_IRQS;
    irqNumber                    = usbDeviceEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];

    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority(irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ(irqNumber);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    IRQn_Type usbDeviceKhciIrq[] = USBFS_IRQS;
    irqNumber                    = usbDeviceKhciIrq[CONTROLLER_ID - kUSB_ControllerKhci0];

    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority(irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ(irqNumber);
#endif
}

#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle)
{
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    USB_DeviceEhciTaskFunction(deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    USB_DeviceKhciTaskFunction(deviceHandle);
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
