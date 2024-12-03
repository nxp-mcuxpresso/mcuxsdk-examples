/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include <stdbool.h>
#include "FreeRTOS.h"
#include "fsl_power.h"
#if configUSE_TICKLESS_IDLE == 2
#include "fsl_rtc.h"
#include "app.h"
#endif
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* enable clock for IOCON */
    CLOCK_EnableClock(kCLOCK_Iocon);

    /* enable clock for GPIO*/
    CLOCK_EnableClock(kCLOCK_Gpio0);

    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM0);

#if configUSE_TICKLESS_IDLE == 2

    /* enable clock for UTICK*/
    CLOCK_EnableClock(kCLOCK_Rtc);

    /* Enable the RTC 32K Oscillator */
    SYSCON->RTCOSCCTRL |= SYSCON_RTCOSCCTRL_EN_MASK;

    /* Initialize RTC timer */
    RTC_Init(RTC);

    /* enable RTC interrupt */
    RTC_EnableInterrupts(RTC, RTC_CTRL_WAKE1KHZ_MASK);
    EnableIRQ(RTC_IRQn);
#endif

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

#if configUSE_TICKLESS_IDLE == 2
/*!
 * @brief Interrupt service fuction of LPT timer.
 *
 * This function to call vPortRtcIsr
 */
void RTC_IRQHandler(void)
{
    vPortRtcIsr();
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Fuction of other timer.
 *
 * This function to return other timer base address
 */

RTC_Type *vPortGetRtcBase(void)
{
    return RTC;
}

/*!
 * @brief Fuction of other timer.
 *
 * This function to return other timer interrupt number
 */

IRQn_Type vPortGetRtcIrqn(void)
{
    return RTC_IRQn;
}
#endif
/*${function:end}*/
