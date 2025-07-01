/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "FreeRTOSConfig.h"
#include "fsl_lptmr.h"
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "board.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitBUTTONsPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

#if configUSE_TICKLESS_IDLE == 2
    CLOCK_SetClockDiv(kCLOCK_DivLPTMR0, 1u);
    CLOCK_AttachClk(kFRO_LF_DIV_to_LPTMR0);

    CLOCK_SetupFRO16KClocking(kCLKE_16K_SYSTEM | kCLKE_16K_COREMAIN);

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
    lptmrConfig.value           = kLPTMR_Prescale_Glitch_3;
    /* Initialize the LP timer */
    LPTMR_Init(LPTMR0, &lptmrConfig);
    /* Enable timer interrupt */
    LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
    /* Enable at the NVIC */
    EnableIRQ(LPTMR0_IRQn);
#endif
}

#if configUSE_TICKLESS_IDLE == 2
void vPortLptmrIsr(void);
/*!
 * @brief Interrupt service fuction of LPT timer.
 *
 * This function to call vPortLptmrIsr
 */
void LPTMR0_IRQHandler(void)
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
    return LPTMR0;
}

/*!
 * @brief Fuction of LPT timer.
 *
 * This function to return LPT timer interrupt number
 */

IRQn_Type vPortGetLptmrIrqn(void)
{
    return LPTMR0_IRQn;
}
#endif
/*${function:end}*/
