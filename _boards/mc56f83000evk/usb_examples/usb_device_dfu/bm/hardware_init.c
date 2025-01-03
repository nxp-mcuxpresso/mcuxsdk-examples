/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_device_registers.h"
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "fsl_pit.h"
#include "dfu_app.h"
#include "dfu_timer.h"
#include "pin_mux.h"
#include "peripherals.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${macro:start}*/
#define USB_IRQ_HANDLER ivINT_USB

#define DEMO_PIT_BASEADDR        PIT0
#define PIT0_IRQ_ROLLOVR_HANDLER ivINT_PIT0_ROLLOVR

/* Set PIT Clock source divider to 2048, the value of two macros below should be aligned. */
#define PIT_CLOCK_SOURCE_DIVIDER kPIT_PrescalerDivBy2048
#define PIT_SOURCE_CLOCK         (CLOCK_GetFreq(kCLOCK_BusClk) / (1U << 11))
#define PIT_IRQ_ID               PIT0_ROLLOVR_IRQn
#define PIT_IRQ_PRIO             1
/*${macro:end}*/

extern usb_device_dfu_app_struct_t g_UsbDeviceDfu;
void USB_DeviceClockInit();
void USB_DeviceIsrEnable(void);

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_USB);

    SetIRQBasePriority(0);
}

void DFU_TimerHWInit()
{
    pit_config_t pitConfig;

    /*
     * psConfig->ePrescaler = kPIT_PrescalerDivBy1;
     * psConfig->bEnableInterrupt = false;
     * psConfig->bEnableSlaveMode = false;
     * psConfig->bEnableTimer = false;
     * psConfig->eClockSource = kPIT_CountBusClock;
     * psConfig->u16PeriodCount = 0xFFFFU;
     */
    PIT_GetDefaultConfig(&pitConfig);
    pitConfig.ePrescaler       = PIT_CLOCK_SOURCE_DIVIDER;
    pitConfig.bEnableInterrupt = true;

    /* Initialize pit module */
    PIT_Init(DEMO_PIT_BASEADDR, &pitConfig);

    /* Set timer period to 1 second */
    PIT_SetTimerPeriod(DEMO_PIT_BASEADDR, MSEC_TO_COUNT(1, PIT_SOURCE_CLOCK));

    /* Enable IRQ with priority */
    EnableIRQWithPriority(PIT_IRQ_ID, PIT_IRQ_PRIO);

    /* Start PIT timer */
    PIT_StopTimer(DEMO_PIT_BASEADDR);
}

void HW_TimerControl(uint8_t enable)
{
    if (enable)
    {
        /* Enable counter */
        PIT_StartTimer(DEMO_PIT_BASEADDR);
    }
    else
    {
        /* Disable counter */
        PIT_StopTimer(DEMO_PIT_BASEADDR);
    }
}

#pragma interrupt alignsp saveall
void PIT0_IRQ_ROLLOVR_HANDLER(void);
void PIT0_IRQ_ROLLOVR_HANDLER(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(DEMO_PIT_BASEADDR);

    DFU_TimerISR();
}
#pragma interrupt off

void USB_DeviceClockInit(void)
{
    CLOCK_EnableUsbfs0Clock();
}

void USB_DeviceIsrEnable(void)
{
    EnableIRQWithPriority(USB_IRQn, 3);
}

#pragma interrupt alignsp saveall
void USB_IRQ_HANDLER(void);
void USB_IRQ_HANDLER(void)
{
    USB_DeviceKhciIsrFunction(g_UsbDeviceDfu.deviceHandle);
}
#pragma interrupt off

/*${function:end}*/
