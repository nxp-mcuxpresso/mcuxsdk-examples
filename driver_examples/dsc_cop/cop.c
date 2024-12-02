/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_cop.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#ifndef COP_IRQ_PRIO
#define COP_IRQ_PRIO 0
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint8_t copRefreshCount = 0;
volatile bool copIsrFlag         = false;
/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief ISR for CMP interrupt function.
 */
#pragma interrupt alignsp saveall
void DEMO_COP_IRQ_HANDLER(void);
void DEMO_COP_IRQ_HANDLER(void)
{
    COP_Refresh(DEMO_COP_BASE);
    copIsrFlag = true;
    copRefreshCount++;
}
#pragma interrupt off

/*!
 * @brief Main function
 */
int main(void)
{
    cop_config_t configCop;

    /* Init hardware */
    BOARD_InitHardware();

    /* Check if COP Time-out reset occurred */
    if (SIM->RSTAT & SIM_RSTAT_COP_CPU_MASK)
    {
        PRINTF("Reset due to COP timeout\r\n");
        COP_Enable(DEMO_COP_BASE, false);
        PRINTF("COP example ends!\r\n");
    }
    else
    {
        /* Initialize and enable LED */
        LED_INIT();
        /* If COP reset is not occurred, enables COP */
        PRINTF("\r\nCOP example start!\r\n");
        /*
         *   psConfig->bEnableWriteProtect = false;
         *   psConfig->bEnableWait = false;
         *   psConfig->bEnableStop = false;
         *   psConfig->bEnableLossOfReference = false;
         *   psConfig->bEnableInterrupt = false;
         *   psConfig->bEnableCOP = false;
         *   psConfig->ePrescaler = kCOP_ClockPrescalerDivide1;
         *   psConfig->u16TimeoutCount = 0xFFFFU;
         *   psConfig->u16WindowCount = 0xFFFFU;
         *   psConfig->u16InterruptCount = 0xFFU;
         *   psConfig->eClockSource = kCOP_RoscClockSource;
         */
        COP_GetDefaultConfig(&configCop);
        /* Select clock source and configure source divider. */
        configCop.eClockSource = COP_SOURCE_CLOCK;
        configCop.ePrescaler   = COP_SOURCE_DIVIDER;
        /* Set Timeout period to 4 second. */
        configCop.u16TimeoutCount = COP_TIMEOUT_COUNT;
        /* Set Interrupt period to 2 second. */
        configCop.u16InterruptCount = COP_INTERRUPT_COUNT;
        /* Enable interrupt. */
        configCop.bEnableInterrupt = true;
        /* Enable COP module. */
        configCop.bEnableCOP = true;
        EnableIRQWithPriority(COP_IRQ_ID, COP_IRQ_PRIO);
        COP_Init(DEMO_COP_BASE, &configCop);

        /* Feed COP for 5 times */
        while (copRefreshCount < 5)
        {
            /* Check whether occur interrupt and toggle LED */
            if (true == copIsrFlag)
            {
                PRINTF("\r\nCOP interrupt is occurred, COP is serviced!");
                LED_TOGGLE();
                copIsrFlag = false;
            }
        };

        /* Disable interrupt.*/
        COP_DisableInterrupt(DEMO_COP_BASE);
        /* Wait for timeout reset */
        PRINTF("\r\nInterrupt disabled, COP will timeout and chip will be reset\r\n");
    }
    while (1)
    {
    }
}
