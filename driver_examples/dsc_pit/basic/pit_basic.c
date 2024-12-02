/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_pit.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#ifndef PIT_IRQ_PRIO
#define PIT_IRQ_PRIO 0
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile bool pitIsrFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
#if defined(DSC_INTERRUPT_WEAK) && (DSC_INTERRUPT_WEAK == 1U)
/*
 * DSC common double weak ISR handler.
 *
 * The IRQ handler could/should be defined as normal function, and its called function is normal function.
 */
#pragma interrupt alignsp saveall
void PIT_LED_HANDLER(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(DEMO_PIT_BASEADDR);
    pitIsrFlag = true;
}
#pragma interrupt off
#elif defined(DSC_INTERRUPT_ON) && (DSC_INTERRUPT_ON == 1U)
void ISR_INTERRUPT_ON_SUBROUTINE(void);
void ISR_INTERRUPT_ON_SUBROUTINE(void)
{
#pragma interrupt alignsp called
    pitIsrFlag = true;
}

/*
 * DSC interrupt on, which enables the option to compile all C routines as interrupt routines, the routine preserves
 * register values by saving and restoring the registers that are used by the routine, not care about subroutines.
 *
 * This method (interrupt on) is not general, unless the user knows well about the compiler and interrupt mechanism.
 */
#pragma interrupt on
void ivINT_PIT0_ROLLOVR(void);
void ivINT_PIT0_ROLLOVR(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(DEMO_PIT_BASEADDR);
    /*
     * In this mode, this routine preserves register values by saving and restoring the registers that are used by the
     * routine, but subroutine maybe doesn't save and restore register values correctly, so you should guarantee that
     * subroutine save and restore register values correctly by "called" or "saveall".
     */
    ISR_INTERRUPT_ON_SUBROUTINE();
}
#pragma interrupt off
#elif defined(DSC_INTERRUPT_SAVEALL) && (DSC_INTERRUPT_SAVEALL == 1U)
void ISR_INTERRUPT_SAVEALL_SUBROUTINE(void)
{
    pitIsrFlag = true;
}

/*
 * DSC interrupt save all, which preserves register values by saving and restoring all registers by calling
 * the INTERRUPT_SAVEALL and INTERRUPT _ RESTOREALL routines in the Runtime Library.
 *
 * Its called function could/should be normal function. And this method is general for no timing critical ISR
 * and recommend to use this method.
 */
#pragma interrupt alignsp saveall
void ivINT_PIT0_ROLLOVR(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(DEMO_PIT_BASEADDR);

    /*
     * Subroutine can be any type, such as saveall/called/default, and all routine context will be saved and restored
     * like function calling.
     */
    ISR_INTERRUPT_SAVEALL_SUBROUTINE();
}
#pragma interrupt off
#elif defined(DSC_INTERRUPT_FAST) && (DSC_INTERRUPT_FAST == 1U)
/*
 * DSC interrupt fast, fast interrupt processing has lower overhead than normal interrupts processing and should be used
 * for all low-latency time-critical interrupts; this interrupt method tend to fast and simple ISR process and it tend
 * to no subroutine called.
 *
 * Its called function should be add #pragma interrupt called. And it is not recommend to call subroutine in fast ISR
 * handler, better to finish the operation within fast ISR handler and don't do any complex operation, because it is
 * also could be considered as subroutine call.
 */
#pragma interrupt fast
void ivINT_PIT0_ROLLOVR(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(DEMO_PIT_BASEADDR);
    pitIsrFlag = true;
}
#pragma interrupt off
#endif

/*!
 * @brief Main function
 */
int main(void)
{
    pit_config_t pitConfig;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    /* Initialize and enable LED */
    LED_INIT();

    /*
     *    psConfig->ePrescaler = kPIT_PrescalerDivBy1;
     *    psConfig->bEnableInterrupt = false;
     *    psConfig->bEnableSlaveMode = false;
     *    psConfig->bEnableTimer = false;
     *    psConfig->eClockSource = kPIT_CountClockSource0;
     *    psConfig->u16PeriodCount = 0xFFFFU;
     */
    PIT_GetDefaultConfig(&pitConfig);
    pitConfig.eClockSource = PIT_CLOCK_SOURCE;
    pitConfig.ePrescaler   = PIT_CLOCK_SOURCE_DIVIDER;
#if defined(FSL_FEATURE_PIT_32BIT_COUNTER) && FSL_FEATURE_PIT_32BIT_COUNTER
    pitConfig.u32PeriodCount = PIT_PERIOD_COUNT;
#else
    pitConfig.u16PeriodCount = PIT_PERIOD_COUNT;
#endif
    pitConfig.bEnableInterrupt = true;
    pitConfig.bEnableTimer     = true;

    /* Initialize pit module */
    PIT_Init(DEMO_PIT_BASEADDR, &pitConfig);

    /* Enable IRQ with priority */
    EnableIRQWithPriority(PIT_IRQ_ID, PIT_IRQ_PRIO);

#if defined(DSC_INTERRUPT_FAST) && (DSC_INTERRUPT_FAST == 1U)
    INTC_SetFastIRQVectorHandler0(PIT_VECTOR_NUM, ivINT_PIT0_ROLLOVR);
#endif

    /* Start PIT timer */
    PRINTF("\r\nStarting PIT timer ...");
    PIT_StartTimer(DEMO_PIT_BASEADDR);

    while (true)
    {
        /* Check whether occur interrupt and toggle LED */
        if (true == pitIsrFlag)
        {
            PRINTF("\r\n PIT timer interrupt is occurred!");
            LED_TOGGLE();
            pitIsrFlag = false;
        }
    }
}
