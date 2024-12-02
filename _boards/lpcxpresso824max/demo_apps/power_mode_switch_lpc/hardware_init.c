/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include <stdbool.h>
#include "app.h"
#include "fsl_power.h"
#include "fsl_pint.h"
#include "fsl_wkt.h"
#include "fsl_swm.h"
#include "fsl_syscon.h"
/*${header:end}*/

/*${function:start}*/

/*
 * Callback function when wakeup key is pressed.
 */
static void pint_intr_callback(pint_pin_int_t pintr, uint32_t pmatch_status)
{
    /* do nothing here */
}

void DEMO_InitWkt(void)
{
    wkt_config_t wktConfig;

    POWER_EnableLPO(true);
    POWER_EnableLPOInDeepPowerDownMode(true);

    wktConfig.clockSource = kWKT_LowPowerClockSource;

    /* Init wkt module */
    WKT_Init(WKT, &wktConfig);

    /* Clear Pending Interrupt */
    NVIC_ClearPendingIRQ(WKT_IRQn);
    /* Enable at the NVIC */
    EnableIRQ(WKT_IRQn);
}

void DEMO_InitResetPin(void)
{
    SWM_SetFixedPinSelect(SWM0, kSWM_RESETN, true);
}

/*
 * Setup a GPIO input pin as wakeup source.
 */
void DEMO_InitWakeupPin(void)
{
    gpio_pin_config_t gpioPinConfigStruct;

    /* Set SW pin as GPIO input. */
    gpioPinConfigStruct.pinDirection = kGPIO_DigitalInput;
    GPIO_PinInit(DEMO_USER_WAKEUP_KEY_GPIO, DEMO_USER_WAKEUP_KEY_PORT, DEMO_USER_WAKEUP_KEY_PIN, &gpioPinConfigStruct);

    SYSCON_AttachSignal(SYSCON, kPINT_PinInt0, DEMO_PINT_PIN_INT0_SRC);

    /* Configure the interrupt for SW pin. */
    PINT_Init(PINT);
    PINT_PinInterruptConfig(PINT, kPINT_PinInt0, kPINT_PinIntEnableFallEdge, pint_intr_callback);
    PINT_EnableCallback(PINT); /* Enable callbacks for PINT */
}

void BOARD_InitHardware(void)
{
    /* Enable clock of uart0. */
    CLOCK_EnableClock(kCLOCK_Uart0);
    /* Ser DIV of uart0. */
    CLOCK_SetClkDivider(kCLOCK_DivUsartClk, 1U);

    BOARD_InitBootPins();
    BOARD_BootClockPll24M();
    BOARD_InitDebugConsole();

    /* Init existed wake up source */
    DEMO_InitWakeupPin();
    DEMO_InitResetPin();
    DEMO_InitWkt();
}

void DEMO_PreEnterLowPower(void)
{
    /* switch main clock source to FRO18M */
    POWER_DisablePD(kPDRUNCFG_PD_IRC_OUT);
    POWER_DisablePD(kPDRUNCFG_PD_IRC);
    CLOCK_SetMainClkSrc(kCLOCK_MainClkSrcIrc);

    /*
     * system osc power down
     * application should decide if more part need to power down to achieve better power consumption
     */
    POWER_EnablePD(kPDRUNCFG_PD_SYSOSC);
    CLOCK_DisableClock(kCLOCK_Iocon);
    CLOCK_DisableClock(kCLOCK_Uart0);
}

void DEMO_LowPowerWakeup(void)
{
    /* clock configurations restore */
    BOARD_BootClockPll24M();

    CLOCK_EnableClock(kCLOCK_Iocon);
    CLOCK_EnableClock(kCLOCK_Uart0);
}
/*${function:end}*/
