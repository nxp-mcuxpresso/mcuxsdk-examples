/*
 * Copyright 2017-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_device_registers.h"
#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "FreeRTOS.h"
#if configUSE_TICKLESS_IDLE == 2
#include "fsl_lptmr.h"
#include "app.h"
#include "fsl_intmux.h"

#endif
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
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
    LPTMR_Init(LPTMR2, &lptmrConfig);
    /* Enable timer interrupt */
    LPTMR_EnableInterrupts(LPTMR2, kLPTMR_TimerInterruptEnable);
    /* Enable at the NVIC */
    EnableIRQ(LPTMR2_IRQn);
#endif
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    INTMUX_Init(INTMUX1);
    /* Enable PORTA intmux source on INTMUX channel 0 */
    INTMUX_EnableInterrupt(INTMUX1, 0, PORTA_IRQn);
}

#if configUSE_TICKLESS_IDLE == 2
/*!
 * @brief Interrupt service fuction of LPT timer.
 *
 * This function to call vPortLptmrIsr
 */
void LPTMR2_IRQHandler(void)
{
    vPortLptmrIsr();
}

/*!
 * @brief Fuction of other timer.
 *
 * This function to return other timer base address
 */

LPTMR_Type *vPortGetLptrmBase(void)
{
    return LPTMR2;
}

/*!
 * @brief Fuction of other timer.
 *
 * This function to return other timer interrupt number
 */

IRQn_Type vPortGetLptmrIrqn(void)
{
    return LPTMR2_IRQn;
}
#endif
/*${function:end}*/
