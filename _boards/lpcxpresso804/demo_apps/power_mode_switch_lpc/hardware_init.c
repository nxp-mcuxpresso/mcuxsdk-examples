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
#include "fsl_iocon.h"
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
    /*POWER_EnableLPOInDpdMode(true);*/

    wktConfig.clockSource = kWKT_LowPowerClockSource;

    /* Init wkt module */
    WKT_Init(WKT, &wktConfig);

    /* Clear Pending Interrupt */
    NVIC_ClearPendingIRQ(WKT_IRQn);
    /* Enable at the NVIC */
    EnableIRQ(WKT_IRQn);
}
static void DEMO_InitResetPin(void)
{
    SWM_SetFixedPinSelect(SWM0, kSWM_RESETN, true);
}

/*
 * Setup a GPIO input pin as wakeup source.
 */
static void DEMO_InitWakeupPin(void)
{
    uint32_t i;
    uint32_t modefunc;

    gpio_pin_config_t gpioPinConfigStruct;

    /* Set SW pin as GPIO input. */
    gpioPinConfigStruct.pinDirection = kGPIO_DigitalInput;
    GPIO_PinInit(DEMO_USER_WAKEUP_KEY_GPIO, DEMO_USER_WAKEUP_KEY_PORT, DEMO_USER_WAKEUP_KEY_PIN, &gpioPinConfigStruct);

    SYSCON_AttachSignal(SYSCON, kPINT_PinInt0, DEMO_PINT_PIN_INT0_SRC);

    /* Configure the interrupt for SW pin. */
    PINT_Init(PINT);
    PINT_PinInterruptConfig(PINT, kPINT_PinInt0, kPINT_PinIntEnableFallEdge, pint_intr_callback);
    PINT_EnableCallback(PINT); /* Enable callbacks for PINT */

    /* Set all pin inactive mode(disable pull up/pull down) */
    for (i = 0; i < 30; i++)
    {
        modefunc = (IOCON->PIO[i] & (~IOCON_PIO_MODE_MASK)) | IOCON_PIO_MODE(0UL);
        IOCON_PinMuxSet(IOCON, (uint8_t)i, modefunc);
    }
    /* Set all pin output and high Voltage, except P0_12, P0_13 */
    gpioPinConfigStruct.pinDirection = kGPIO_DigitalOutput;
    gpioPinConfigStruct.outputLogic  = 1UL;
    for (i = 0; i <= 30; i++)
    {
        if (i != BOARD_LED_GREEN_GPIO_PIN && i != BOARD_S1_GPIO_PIN)
        {
            GPIO_PinInit(GPIO, 0UL, i, &gpioPinConfigStruct);
        }
    }
}

void BOARD_InitHardware(void)
{
    /* Select the main clock as source clock of USART0 (debug console) */
    CLOCK_Select(BOARD_DEBUG_USART_CLK_ATTACH);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Init existed wake up source */
    DEMO_InitWakeupPin();
    DEMO_InitResetPin();
    DEMO_InitWkt();

    /* Disable BOD reset, Disable BOD reset before de-powering BOD */
    SYSCON->BODCTRL  = (SYSCON->BODCTRL & (~SYSCON_BODCTRL_BODRSTENA_MASK)) | SYSCON_BODCTRL_BODRSTENA(0UL);
    SYSCON->PDRUNCFG = (SYSCON->PDRUNCFG & (~SYSCON_PDSLEEPCFG_BOD_PD_MASK)) | SYSCON_PDSLEEPCFG_BOD_PD(1UL);
}

void DEMO_PreEnterLowPower(void)
{
    /* switch main clock source to FRO18M */
    POWER_DisablePD(kPDRUNCFG_PD_FRO_OUT);
    POWER_DisablePD(kPDRUNCFG_PD_FRO);
    CLOCK_SetMainClkSrc(kCLOCK_MainClkSrcFro);
    CLOCK_SetFroOscFreq(kCLOCK_FroOscOut18M);
    /*CLOCK_SetFroOutClkSrc(kCLOCK_FroSrcFroOsc);*/

    /*
     * system osc power down
     * application should decide if more part need to power down to achieve better power consumption
     */
    CLOCK_DisableClock(kCLOCK_Iocon);
    CLOCK_DisableClock(kCLOCK_Uart0);
}

void DEMO_LowPowerWakeup(void)
{
    /* clock configurations restore */
    BOARD_InitBootClocks();

    CLOCK_EnableClock(kCLOCK_Iocon);
    CLOCK_EnableClock(kCLOCK_Uart0);
}
/*${function:end}*/
