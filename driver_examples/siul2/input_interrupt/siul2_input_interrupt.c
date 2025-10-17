/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "fsl_siul2.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Whether the SW button is pressed */
volatile bool g_ButtonPress = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Interrupt service fuction of switch.
 *
 * This function toggles the LED
 */
void BOARD_SW_IRQ_HANDLER(void)
{
    SIUL2_ClearExtDmaInterruptStatusFlags(BOARD_SIUL2_BASE, 1U << BOARD_SW_EIRQ);
    /* Change state of button. */
    g_ButtonPress = true;
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    /* Print a note to terminal. */
    PRINTF("\r\n SIUL2 GPIO Driver example\r\n");
    PRINTF("\r\n Press %s to turn on/off a LED \r\n", BOARD_SW_NAME);

    /* Init input switch EIRQ. */
    SIUL2_SetGlitchFilterPrescaler(BOARD_SIUL2_BASE, 1U);
    SIUL2_EnableExtInterrupt(BOARD_SIUL2_BASE, BOARD_SW_EIRQ,
                             kSIUL2_InterruptFallingEdge, 2U);
    EnableIRQ(BOARD_SW_IRQ);

    /* Init output LED GPIO. */
    SIUL2_SetPinDirection(BOARD_SIUL2_BASE,
                          (BOARD_LED_GPIO * 32U + BOARD_LED_GPIO_PIN),
                          kPORT_OUT);

    while (1)
    {
        if (g_ButtonPress)
        {
            PRINTF(" %s is pressed \r\n", BOARD_SW_NAME);
            /* Toggle LED. */
            SIUL2_PortToggle(BOARD_SIUL2_BASE, BOARD_LED_GPIO, 1U << BOARD_LED_GPIO_PIN);
            /* Reset state of button. */
            g_ButtonPress = false;
        }
    }
}
