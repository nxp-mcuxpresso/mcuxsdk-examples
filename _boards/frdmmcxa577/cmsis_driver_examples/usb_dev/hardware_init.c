/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * hardware_init.c - Board-level hardware initialization for FRDM-MCXA577
 * CMSIS-Driver Validation (USB Device) example.
 *
 * For GCC (newlib): _write syscall routes printf/vprintf to LPUART1.
 * For IAR (DLib):   __write hook routes printf/vprintf to LPUART1.
 * SDK_DEBUGCONSOLE=0 keeps standard newlib printf intact.
 */

#include <stdint.h>
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "usb.h"
#include "usb_phy.h"
#include "fsl_lpuart.h"
#include "board.h"
#include "cmsis_usbd_validation.h"

/* Timeout counts for SPC/SCG busy-wait loops (iterations, not real time).
 * Sized for the slowest expected clock after BOARD_InitBootClocks(). */
#define USB_SPC_BUSY_TIMEOUT_COUNT   (100000U)
#define USB_SCG_LDO_TIMEOUT_COUNT    (100000U)
#define USB_SCG_SOSC_TIMEOUT_COUNT   (100000U)

/*---------------------------------------------------------------------------
 * USB_InitClockSource - configure USB clock source, mux, and dividers.
 * Called from BOARD_InitHardware() before the CMSIS USB driver is powered.
 * Returns 0 on success, -1 on timeout.
 *---------------------------------------------------------------------------*/
static int USB_InitClockSource(void)
{
    uint32_t timeout;

    /* Raise core LDO voltage to support USB HS operation. */
    SPC0->ACTIVE_VDELAY  = 0x0500U;
    SPC0->ACTIVE_CFG    &= ~SPC_ACTIVE_CFG_CORELDO_VDD_DS_MASK;
    SPC0->ACTIVE_CFG    |= SPC_ACTIVE_CFG_CORELDO_VDD_LVL(0x3U);
    timeout = USB_SPC_BUSY_TIMEOUT_COUNT;
    while ((SPC0->SC & SPC_SC_BUSY_MASK) != 0U)
    {
        if (--timeout == 0U)
        {
            return -1;
        }
    }

    /* Enable SCG internal LDO required by the USB PHY PLL. */
    if (0U == (SCG0->LDOCSR & SCG_LDOCSR_LDOEN_MASK))
    {
        SCG0->TRIM_LOCK  = 0x5a5a0001U;
        SCG0->LDOCSR    |= SCG_LDOCSR_LDOEN_MASK;
        timeout = USB_SCG_LDO_TIMEOUT_COUNT;
        while (0U == (SCG0->LDOCSR & SCG_LDOCSR_VOUT_OK_MASK))
        {
            if (--timeout == 0U)
            {
                return -1;
            }
        }
    }

    /* Enable MRCC USB clock gates so the PHY reference path is active. */
    MRCC0->MRCC_GLB_CC2_SET = MRCC_MRCC_GLB_CC2_USB1_MASK | MRCC_MRCC_GLB_CC2_USB1_PHY_MASK;

    /* Enable SOSC (crystal oscillator) if not already valid; leave it
     * running unchanged if a prior driver already started it. */
    if (0U == (SCG0->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK))
    {
        SCG0->SOSCCFG &= ~(SCG_SOSCCFG_RANGE_MASK | SCG_SOSCCFG_EREFS_MASK);
        SCG0->SOSCCFG  = (1U << SCG_SOSCCFG_RANGE_SHIFT) | (1U << SCG_SOSCCFG_EREFS_SHIFT);
        SCG0->SOSCCSR |= SCG_SOSCCSR_SOSCEN_MASK;
        timeout = USB_SCG_SOSC_TIMEOUT_COUNT;
        while (0U == (SCG0->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK))
        {
            if (--timeout == 0U)
            {
                return -1;
            }
        }
    }

    /* Route XTAL to USBHS PHY and configure the divider. */
    CLOCK_AttachClk(kPHY_CLK_XTAL_to_USBHS);
    CLOCK_AttachClk(kCLK_IN_to_USBHS_PHY);
    CLOCK_SetClockDiv(kCLOCK_DivUSBHS_PHY, 1U);

    CLOCK_EnableClock(kCLOCK_GateUSBHS);
    CLOCK_EnableClock(kCLOCK_GateUSBHS_PHY);
    if (!CLOCK_EnableUsbhsPhyPllClock(BOARD_XTAL0_CLK_HZ))
    {
        return -1;
    }
    if (!CLOCK_EnableUsbhsClock())
    {
        return -1;
    }
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };
    if (USB_EhciPhyInit(kUSB_ControllerEhci0, BOARD_XTAL0_CLK_HZ, &phyConfig) != kStatus_USB_Success)
    {
        return -1;
    }

    return 0;
}

/*---------------------------------------------------------------------------
 * Board hardware initialization
 *---------------------------------------------------------------------------*/
void BOARD_InitHardware(void)
{
    lpuart_config_t cfg;

    BOARD_InitDEBUG_UARTPins();
    BOARD_InitBootClocks();

    /* LPUART1 clock setup - BOARD_BootClockPLL240M leaves LPUART1 with no clock.
     * Match SDK board.h: kFRO_LF_DIV (~12 MHz), divider=1, then release reset. */
    CLOCK_AttachClk(kFRO_LF_DIV_to_LPUART1);
    CLOCK_SetClockDiv(kCLOCK_DivLPUART1, 1U);
    CLOCK_EnableClock(kCLOCK_GateLPUART1);
    RESET_ReleasePeripheralReset(kLPUART1_RST_SHIFT_RSTn);

    /* Init LPUART1 at 115200 8N1, source clock 12 MHz */
    LPUART_GetDefaultConfig(&cfg);
    cfg.baudRate_Bps = 115200U;
    cfg.enableTx     = true;
    cfg.enableRx     = false;
    LPUART_Init(LPUART1, &cfg, 12000000U);

    /* USB HS clock source, mux, and divider setup.
     * The CMSIS USB driver (fsl_usbd_ehci_cmsis.c) only opens/closes the
     * peripheral clock gates; all upstream configuration is done here,
     * matching the pattern used by cmsis_lpspi and cmsis_lpuart. */
    (void)USB_InitClockSource();
}

/*---------------------------------------------------------------------------
 * main - initialize hardware then hand off to the RTOS application
 *---------------------------------------------------------------------------*/
int main(void)
{
    BOARD_InitHardware();
    return (int)app_main();
}

/*---------------------------------------------------------------------------
 * UART output hook - routes printf/vprintf output to LPUART1
 * GCC newlib  : _write(fd, buf, count)
 * IAR DLib    : __write(handle, buf, size)
 *---------------------------------------------------------------------------*/
#ifdef __ICCARM__
#include <LowLevelIOInterface.h>
size_t __write(int handle, const unsigned char *buf, size_t size)
{
    (void)handle;
    LPUART_WriteBlocking(LPUART1, buf, size);
    return size;
}
#else
int _write(int fd, const char *buf, int count)
{
    (void)fd;
    LPUART_WriteBlocking(LPUART1, (const uint8_t *)buf, (size_t)count);
    return count;
}
#endif
