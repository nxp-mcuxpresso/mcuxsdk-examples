/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
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
#endif
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
#if configUSE_TICKLESS_IDLE == 2
    lptmr_config_t lptmrConfig;

    CLOCK_EnableClock(kCLOCK_Lptmr0);
    /* Configuration LPTMR  */
    LPTMR_GetDefaultConfig(&lptmrConfig);
    /* Use clock from prescaler */
    lptmrConfig.bypassPrescaler = false;
    /* Prescaler divide 128 */
    lptmrConfig.value = kLPTMR_Prescale_Glitch_6;

    LPTMR_Init(LPTMR0, &lptmrConfig);
    /* Enable timer interrupt */
    LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
    /* Enable at the NVIC */
    EnableIRQ(PWT_LPTMR0_IRQn);
#endif
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

#if configUSE_TICKLESS_IDLE == 2
/*!
 * @brief Interrupt service fuction of LPT timer.
 *
 * This function to call vPortLptmrIsr
 */
void PWT_LPTMR0_IRQHandler(void)
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
    return LPTMR0;
}

/*!
 * @brief Fuction of other timer.
 *
 * This function to return other timer interrupt number
 */

IRQn_Type vPortGetLptmrIrqn(void)
{
    return PWT_LPTMR0_IRQn;
}
#endif
/*${function:end}*/
