/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_gpio.h"
#include "veneer_table.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    while (1)
    {
        /* If access to SW2 and LED_RED is allowed from secure world by SW1, LED will light if SW2 is pressed */
      
        if (GPIO_PinRead(DEMO_SW2_GPIO, DEMO_SW2_GPIO_PIN))
        {
            LED_RED_OFF();
        }
        else
        {
            LED_RED_ON();
        }
    }
}
