/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_power.h"
#include "fsl_irtc.h"
#include "fsl_debug_console.h"
#include "app.h"
#include <stdbool.h>
/*${header:end}*/

/*${variable:start}*/
uint32_t excludeFromDS[2];
uint32_t wakeupFromDS[4];
uint32_t excludeFromPD[1];
uint32_t wakeupFromPD[4];
uint32_t excludeFromDPD[1];
uint32_t wakeupFromDPD[2];
/*${variable:end}*/

/*${function:start}*/
void RTC_IRQHandler(void)
{
}
void BOARD_InitHardware(void)
{
    CLOCK_EnableClock(kCLOCK_Gpio0); /* Enable the clock for GPIO0. */

    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 0u, false);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 1u, true);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    manage_evk_io_optimization();
    BOARD_BootClockFRO12M();
    BOARD_InitDebugConsole();
#if !defined(DONOT_ENABLE_FLASH_PREFETCH)
    /* enable flash prefetch for better performance */
    SYSCON->FMCCR |= SYSCON_FMCCR_PREFEN_MASK;
#endif

    excludeFromDS[0]  = kPDRUNCFG_PD_DCDC | kPDRUNCFG_PD_FRO192M | kPDRUNCFG_PD_FRO32K;
    excludeFromDS[1]  = 0;
    excludeFromPD[0]  = kPDRUNCFG_PD_LDOMEM | kPDRUNCFG_PD_FRO32K;
    excludeFromDPD[0] = kPDRUNCFG_PD_LDOMEM | kPDRUNCFG_PD_FRO32K;

    wakeupFromDS[0]  = WAKEUP_GPIO_INT0_0;
    wakeupFromDS[1]  = 0;
    wakeupFromDS[2]  = 0;
    wakeupFromDS[3]  = 0;
    wakeupFromPD[0]  = WAKEUP_GPIO_GLOBALINT0 | WAKEUP_GPIO_GLOBALINT1;
    wakeupFromPD[1]  = 0;
    wakeupFromPD[2]  = 0;
    wakeupFromPD[3]  = 0;
    wakeupFromDPD[0] = WAKEUP_RTC_ALARM_WAKEUP;
    wakeupFromDPD[1] = 0;
}
void DEMO_PreLowPower(void)
{
    /*!< Nothing to do */
}
void DEMO_PowerDownWakeup(void)
{
    BOARD_BootClockFRO12M();
    BOARD_InitDebugConsole();
}
void DEMO_PreDeepPowerDown(void)
{
    uint32_t sec  = 0;
    uint8_t index = 0;
    irtc_datetime_t date;
    irtc_config_t irtcConfig;

    /* Set a start date time and start RT */
    date.year    = 2021U;
    date.month   = 12U;
    date.day     = 25U;
    date.weekDay = 6U;
    date.hour    = 19U;
    date.minute  = 0;
    date.second  = 0;

    /* Get alarm time from user */
    PRINTF("Please input the number of second to wait for waking up\r\n");
    PRINTF("The second must be positive value(1-59)\r\n");
    while (index != 0x0D)
    {
        index = GETCHAR();
        if ((index >= '0') && (index <= '9'))
        {
            PUTCHAR(index);
            sec = sec * 10 + (index - 0x30U);
        }
    }
    PRINTF("\r\n");

    POWER_EnablePD(kPDRUNCFG_PD_XTAL32K); /*!< Powered down the XTAL 32 kHz RTC oscillator */
    POWER_DisablePD(kPDRUNCFG_PD_FRO32K); /*!< Powered the FRO 32 kHz RTC oscillator */
    CLOCK_AttachClk(kFRO32K_to_OSC32K);   /*!< Switch OSC32K to FRO32K */

    IRTC_GetDefaultConfig(&irtcConfig);

    /* Init RTC */
    IRTC_Init(RTC, &irtcConfig);

    /* Enable the RTC 32KHz oscillator at CFG0 by writing a 0 */
    IRTC_Enable32kClkDuringRegisterWrite(RTC, true);

    /* Clear all Tamper events by writing a 1 to the bits */
    IRTC_ClearTamperStatusFlag(RTC);

    IRTC_SetDatetime(RTC, &date);
    date.second += sec;
    IRTC_SetAlarm(RTC, &date);

    /* Enable RTC alarm interrupt */
    IRTC_EnableInterrupts(RTC, kIRTC_AlarmInterruptEnable);

    /* Enable at the NVIC */
    EnableIRQ(RTC_IRQn);

    PRINTF("System will wakeup at%02ds later\r\n", sec);
}

/*${function:end}*/
