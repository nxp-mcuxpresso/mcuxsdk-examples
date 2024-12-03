/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "peripherals.h"
#include "fsl_sim.h"
/*${header:end}*/

void DemoShowResetReason(void);
void DemoShowBootMode(void);
void DemoShowBootloaderBasedOnResetType(void);
void DemoModeSwitch(char ch);

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    BOARD_InitDebugConsole();

    EnableIRQWithPriority(BOARD_DEBUG_UART_RX_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);
    EnableIRQWithPriority(BOARD_DEBUG_UART_TX_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);
    EnableIRQWithPriority(BOARD_DEBUG_UART_RX_ERR_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);
    EnableIRQWithPriority(BOARD_DEBUG_UART_TX_IDLE_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);
    SetIRQBasePriority(0);
}

void DemoShowResetReason(void)
{
    uint16_t rstReason = SIM_GetResetStatusFlags(SIM);

    PRINTF("Reset Reason: ");
    if (0U != (rstReason & kSIM_PowerONResetFlag))
    {
        PRINTF("Power on");
    }
    else if (0U != (rstReason & kSIM_ExternalResetFlag))
    {
        PRINTF("External pin");
    }
    else if (0U != (rstReason & kSIM_COPLossOfReferenceResetFlag))
    {
        PRINTF("COP loss of reference");
    }
    else if (0U != (rstReason & kSIM_COPCPUTimeOutResetFlag))
    {
        PRINTF("COP time out");
    }
    else if (0U != (rstReason & kSIM_SofwareResetFlag))
    {
        PRINTF("Software reset");
    }
    else if (0U != (rstReason & kSIM_COPWindowTimeOutResetFlag))
    {
        PRINTF("COP window time out");
    }
    PRINTF("\r\n");
}

/*******************************************************************
 * For MC56F83xxx, the boot type is determined by
 * bootCfg = FOPT[7:6} & (~SIM->BOOT_MODE_OVERRIDE[1:0]).
 * If bootCfg is 0b11, the MCU (re)boot from bootloader,
 * otherwise MCU (re)boot from flash.
 ******************************************************************/

void DemoShowBootMode(void)
{
    uint8_t fcfBootCfg = (FTFE->FOPT & 0xC0) >> 6;
    uint8_t bootCfg    = (fcfBootCfg & (~(SIM->BOOT_MODE_OVERRIDE))) & 0x03;

    PRINTF("Boot from   : ");

    if (0x03 == bootCfg)
    {
        PRINTF("bootloader");
    }
    else
    {
        PRINTF("flash");
    }
    PRINTF("\r\n");
}

/*
 * When FCF set 0b11 to FOPT[7:6], below code shows how to
 * enter bootloader based on reset type. That POR always boot from
 * bootloader, and application code could determine MCU boot type(bootloader/flash)
 * based on reset type. It is NOT possible to boot from flash for POR when
 * FOPT[7:6] = 0b11.
 * When FCF doesn't set 0b11 to FOPT[7:6], application code is impossible to set
 * boot type to bootloader.
 *
 * Please be noted that when use a debugger(say MC56F81000-EVK on board multilink),
 * it use reset pin to reset the target mcu.
 */
void DemoShowBootloaderBasedOnResetType(void)
{
    uint16_t rstReason = SIM_GetResetStatusFlags(SIM);
    if (0xC0 == (FTFE->FOPT & 0xC0))
    {
        if (0U != (rstReason & kSIM_ExternalResetFlag))
        {
            PRINTF("External pin reset, reboot from bootloader\r\n");
            SIM_SetBootOverRide(SIM, kSIM_BootFromRomOrFlash);
            SIM_TriggerSoftwareReset(SIM);
        }
    }
    else
    {
        PRINTF("FCF set boot from flash by mandatory!\r\n");
    }

    /* Default set boot from flash. */
    SIM_SetBootOverRide(SIM, kSIM_BootFromFlash);
}

/*
 * Below code demos how to set boot type(bootloader/flash) via software reset
 * when FCF set 0b11 to FOPT[7:6].
 */
void DemoModeSwitch(char ch)
{
    switch (ch)
    {
        case 'f':
        case 'F':
        case 'n':
        case 'N':
            PRINTF("MC56F83xxx doesn't support mode switch!\r\n");
            break;

        case 'b':
            PRINTF("Software reset, reboot from flash\r\n");
            SIM_SetBootOverRide(SIM, kSIM_BootFromFlash);
            SIM_TriggerSoftwareReset(SIM);
            break;

        case 'B':
            if (0xC0 == (FTFE->FOPT & 0xC0))
            {
                PRINTF("Software reset, reboot from bootloader\r\n");
                SIM_SetBootOverRide(SIM, kSIM_BootFromRomOrFlash);
                SIM_TriggerSoftwareReset(SIM);
            }
            else
            {
                PRINTF("FCF set boot from flash by mandatory, failed to reboot!\r\n");
            }
            break;

        default:
            PRINTF("%c is not supported!\r\n", ch);
    }
}

/*${function:end}*/
