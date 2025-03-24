/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include <stdbool.h>
#include "FreeRTOS.h"
#include "fsl_power.h"
#if configUSE_TICKLESS_IDLE == 2
#include "fsl_rtc.h"
#include "app.h"
#endif
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    POWER_Init();

    CLOCK_EnableClock(kCLOCK_Gpio0);
    GPIO_PortInit(GPIO, 0);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM0);
    CLOCK_EnableClock(kCLOCK_FlexComm0);

#if configUSE_TICKLESS_IDLE == 2
    POWER_PeripheralPowerOn(kPOWERCFG_FRO32K);
    CLOCK_Select32kOscClkSrc(kCLOCK_Osc32kClockSrc_FRO);
    RTC_Init(RTC);
    RTC_EnableWakeupTimer(RTC, true);
    EnableIRQ(RTC_IRQn);
#endif

    BOARD_InitPins();
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
 * @brief Fuction of RTC timer.
 *
 * This function to return RTC timer base address
 */

RTC_Type *vPortGetRtcBase(void)
{
    return TICKLESS_RTC_BASE_PTR;
}

/*!
 * @brief Fuction of RTC timer.
 *
 * This function to return RTC timer interrupt number
 */

IRQn_Type vPortGetRtcIrqn(void)
{
    return TICKLESS_RTC_IRQn;
}
#endif
/*${function:end}*/
