/*
 * Copyright 2022-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_clock.h"
#include "FreeRTOSConfig.h"
#if configUSE_TICKLESS_IDLE == 2
#include "fsl_lptmr.h"
#include "app.h"
#endif
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootClocks();
    BOARD_InitSWD_DEBUGPins();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitDebugConsole();
    BOARD_InitBUTTONsPins();
    RESET_ReleasePeripheralReset(kAonLPTMR_RST_SHIFT_RSTn);

#if configUSE_TICKLESS_IDLE == 2
    lptmr_config_t lptmrConfig;

    /* Configuration LPTMR  */
    /*
     * lptmrConfig.timerMode = kLPTMR_TimerModeTimeCounter;
     * lptmrConfig.pinSelect = kLPTMR_PinSelectInput_0;
     * lptmrConfig.pinPolarity = kLPTMR_PinPolarityActiveHigh;
     * lptmrConfig.enableFreeRunning = false;
     * lptmrConfig.bypassPrescaler = true;
     * lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1;
     * lptmrConfig.value = kLPTMR_Prescale_Glitch_0;
     */
    LPTMR_GetDefaultConfig(&lptmrConfig);
    /* Set prescaller to reach the 100Hz frequency that is expected by the FreeRTOS tickless implementation, see
     * configLPTMR_CLOCK_HZ */
    lptmrConfig.bypassPrescaler = false;
    lptmrConfig.value           = kLPTMR_Prescale_Glitch_10; /* 2.5MHz / 2048 = 1.2kHz */
    /* Initialize the LP timer */
    LPTMR_Init(AON__LPTMR0, &lptmrConfig);
    /* Enable timer interrupt */
    LPTMR_EnableInterrupts(AON__LPTMR0, kLPTMR_TimerInterruptEnable);
    /* Enable at the NVIC */
    EnableIRQ(LPTMR_AON_IRQn);
#endif
}

#if configUSE_TICKLESS_IDLE == 2
/*!
 * @brief Interrupt service fuction of LPT timer.
 *
 * This function to call vPortLptmrIsr
 */
void LPTMR_AON_IRQHandler(void)
{
    vPortLptmrIsr();
}

/*!
 * @brief Fuction of LPT timer.
 *
 * This function to return LPT timer base address
 */

LPTMR_Type *vPortGetLptrmBase(void)
{
    return AON__LPTMR0;
}

/*!
 * @brief Fuction of LPT timer.
 *
 * This function to return LPT timer interrupt number
 */

IRQn_Type vPortGetLptmrIrqn(void)
{
    return LPTMR_AON_IRQn;
}
#endif
/*${function:end}*/
