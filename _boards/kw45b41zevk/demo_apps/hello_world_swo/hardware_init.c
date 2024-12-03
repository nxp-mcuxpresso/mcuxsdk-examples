/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
extern volatile bool g_userPress;

void BOARD_SW_IRQ_HANDLER(void)
{
    /* Clear external interrupt flag. */
    GPIO_GpioClearInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);

    /* Change state of button. */
    g_userPress = true;
    SDK_ISR_EXIT_BARRIER;
}

void BOARD_InitKey(void)
{
    /* Define the init structure for the input switch pin */
    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput,
        0,
    };

    /* Init input switch GPIO. */
    GPIO_SetPinInterruptConfig(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, kGPIO_InterruptFallingEdge);
    EnableIRQ(BOARD_SW_IRQ);
    GPIO_PinInit(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, &sw_config);
}

void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitKey();

    /* Write KEY value (0xC5ACCE55) to Lock Access Register to unlock the write access to ATB funnel by CPU. */
    (*(unsigned int *)0xE0044FB0) = 0xC5ACCE55;
    /* As documented in the RM, there is a FUNNEL between the CM33 ITM output
     * and the CM3 (radio) ITM output before the SWO pin output.
     * The FUNNEL registers are located on the PPB bus at 0xE0044000.
     * Configure the FUNNEL register to enable the SWO pin output to CM33 ITM output. */
    (*(unsigned int *)0xE0044000) |= 0x1;
}

void BOARD_InitDebugConsoleSWO(unsigned int port, unsigned int baudrate)
{
    DbgConsole_Init(port, baudrate, kSerialPort_Swo, SystemCoreClock);
}
/*${function:end}*/
