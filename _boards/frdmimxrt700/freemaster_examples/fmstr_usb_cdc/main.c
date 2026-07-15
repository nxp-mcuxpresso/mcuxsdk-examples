/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FreeMASTER Communication Driver - Example Application
 */

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "board.h"
#include "pin_mux.h"

#include "fsl_common.h"
#include "fsl_debug_console.h"

#include "usb_device_config.h"

#include "freemaster.h"
#include "freemaster_usb.h"
#include "freemaster_example.h"

#include "usb.h"
#include "usb_phy.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! Note: All global variables accessed by FreeMASTER are defined in a shared
//! freemaster_example.c file

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    uint32_t usbClockFreq             = 24000000;

#if (defined(MIMXRT798S_cm33_core0_SERIES) || defined(MIMXRT798S_hifi4_SERIES) || defined(MIMXRT798S_ezhv_SERIES))
    /* Board initialization core 0*/
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();

    BOARD_InitDebugConsole();
    BOARD_InitAHBSC();
#endif

    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };

    /* Power on COM VDDN domain for USB */
    POWER_DisablePD(kPDRUNCFG_DSR_VDDN_COM);

    /* Power on USB0 SRAM array */
    POWER_DisablePD(kPDRUNCFG_APD_USB0_SRAM);
    POWER_DisablePD(kPDRUNCFG_PPD_USB0_SRAM);
    /* Apply the config */
    POWER_ApplyPD();

    /* Enable the USBPHY0 CLOCK */
    SYSCON4->USBPHY0_CLK_ACTIVE |= SYSCON4_USBPHY0_CLK_ACTIVE_IPG_CLK_ACTIVE_MASK;
    CLOCK_AttachClk(k32KHZ_WAKE_to_USB);
    CLOCK_AttachClk(kOSC_CLK_to_USB_24MHZ);
    CLOCK_EnableClock(kCLOCK_Usb0);
    CLOCK_EnableClock(kCLOCK_UsbphyRef);
    RESET_PeripheralReset(kUSB0_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSBPHY0_RST_SHIFT_RSTn);
    CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, usbClockFreq);
    CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, usbClockFreq);
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL_SYS_CLK_HZ, &phyConfig);

    /* FreeMASTER communication layer initialization */
    FMSTR_ExampleUsbInit();

    /* This example uses shared code from FreeMASTER generic example application */
    FMSTR_Example_Init();

    PRINTF(
        "\nFreeMASTER USB Example.\n"
        "Connect using USB Serial Device COM port.\n\n");

    while(1)
    {
        // FreeMASTER example increments several variables periodically,
        // use the FreeMASTER PC Host tool to visualize the variables
        FMSTR_Example_Poll();
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
/////////////////////////////////////////////////////////////////////////////////
