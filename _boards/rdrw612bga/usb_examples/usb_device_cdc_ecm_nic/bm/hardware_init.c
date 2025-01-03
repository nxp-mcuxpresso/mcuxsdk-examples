/**
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "board.h"
#include "clock_config.h"
#include "fsl_enet.h"
#include "fsl_phyksz8081.h"
#include "pin_mux.h"
#include "usb_device_config.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_descriptor.h"
#include "usb_eth_adapter.h"
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
static phy_ksz8081_resource_t g_phy_resource;

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_InitModuleClock(void)
{
    /* Set 50MHz output clock required by PHY. */
    CLOCK_EnableClock(kCLOCK_TddrMciEnetClk);
}

ENET_Type *BOARD_GetExampleEnetBase(void)
{
    return ENET;
}

const phy_operations_t *BOARD_GetPhyOps(void)
{
    return &phyksz8081_ops;
}

void *BOARD_GetPhyResource(void)
{
    return &g_phy_resource;
}

uint32_t BOARD_GetPhySysClock(void)
{
    return CLOCK_GetMainClkFreq();
}

uint8_t BOARD_GetPhyAddress(void)
{
    return EXAMPLE_PHY_ADDRESS;
}

static void MDIO_Init(void)
{
    uint32_t i = ENET_GetInstance(BOARD_GetExampleEnetBase());

    (void)CLOCK_EnableClock(s_enetClock[i]);
    (void)CLOCK_EnableClock(s_enetExtraClock[i]);
    ENET_SetSMI(BOARD_GetExampleEnetBase(), BOARD_GetPhySysClock(), false);
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(BOARD_GetExampleEnetBase(), phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(BOARD_GetExampleEnetBase(), phyAddr, regAddr, pData);
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitModuleClock();

    ENET_ResetHardware();

    GPIO_PinWrite(GPIO, 0U, 21U, 0U);
    SDK_DelayAtLeastUs(1000000, CLOCK_GetCoreSysClkFreq());
    GPIO_PinWrite(GPIO, 0U, 21U, 1U);

    MDIO_Init();
    g_phy_resource.read = MDIO_Read;
    g_phy_resource.write = MDIO_Write;
}

void USBHS_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(ethNicHandle.deviceHandle);
}

void USB_DeviceClockInit(void)
{
    /* reset USB */
    RESET_PeripheralReset(kUSB_RST_SHIFT_RSTn);

    /* enable usb clock */
    CLOCK_EnableClock(kCLOCK_Usb);

    /* enable usb phy clock */
    CLOCK_EnableUsbhsPhyClock();
}

void USB_DeviceIsrEnable(void)
{
    uint8_t irqNumber;

    uint8_t usbDeviceEhciIrq[] = USBHS_IRQS;
    irqNumber = usbDeviceEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];

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
