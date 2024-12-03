/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_aon_gpio.h"
#include "fsl_ipmq.h"
#include "fsl_aon_common.h"
#include "fsl_common.h"
#include "app.h"
#include "pin_mux.h"
#include "board.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint32_t outputVal = 0U;
volatile bool isFinished = true;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Interrupt service fuction of switch.
 *
 * This function set the interrupt success status.
 */
static void gpio_callback()
{
    isFinished = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("\r\n AON_GPIO Driver example\r\n");

    /* Set gpio callback function. */
    g_ipmCabllbacks.gpioIntA = gpio_callback;
    g_ipmCabllbacks.gpioIntB = gpio_callback;

    AON_GPIO_PinConfig(DEMO_GPIO_TEST_PIN, DEMO_GPIO_AON_PIN_DIRECTION, 1U, \
                   DEMO_GPIO_AON_INT_SELECT, DEMO_GPIO_AON_INTCONFIG, DEMO_USER_DATA);

    while (1)
    {
        if(isFinished)
        {
            isFinished = false;
            AON_GPIO_PinRead(DEMO_GPIO_TEST_PIN, &outputVal, DEMO_USER_DATA);
            if(outputVal == 1U)
            {
                /* set pin output logic level 0. */
                AON_GPIO_PinWrite(DEMO_GPIO_TEST_PIN, 0U, DEMO_USER_DATA);
                PRINTF("\r\n Set pin output logic value as low.  \r\n");
            }
            else
            {
                /* set pin output logic level 1. */
                AON_GPIO_PinWrite(DEMO_GPIO_TEST_PIN, 1U, DEMO_USER_DATA);
                PRINTF("\r\n Set pin output logic value as high.  \r\n");
            }
        }
    }
}
