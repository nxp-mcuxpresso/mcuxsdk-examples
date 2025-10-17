/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_siul2.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief delay a while.
 */
void delay(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void delay(void)
{
    volatile uint32_t i = SystemCoreClock / 10U;
    while(i != 0U)
    {
        __asm("NOP"); /* delay */
        i--;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    /* Print a note to terminal. */
    PRINTF("\r\n SIUL2 GPIO Driver example\r\n");
    PRINTF("\r\n The LED is blinking.\r\n");

    SIUL2_SetPinOutputBuffer(BOARD_SIUL2_BASE, (BOARD_LED_BLUE_GPIO * 32U + BOARD_LED_GPIO_PIN), true, kPORT_MUX_AS_GPIO);
    SIUL2_PortPinWrite(BOARD_SIUL2_BASE, BOARD_LED_GPIO, BOARD_LED_GPIO_PIN, BOARD_LED_OFF_LOGIC);

    while (1)
    {
        delay();
        SIUL2_PortToggle(BOARD_SIUL2_BASE, BOARD_LED_GPIO, 1u << BOARD_LED_GPIO_PIN);
    }
}
