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

#include "fsl_gpio.h"
#include "fsl_ewm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef WDOG_EWM_PRIO
#define WDOG_EWM_PRIO 0
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool bEwmIsrFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Configure gpio as input for button
 *
 */
static void gpio_configure(void)
{
    gpio_config_t sSwConfig = {
        .eDirection     = kGPIO_DigitalInput,
        .ePull          = kGPIO_PullDisable,
        .eInterruptMode = kGPIO_InterruptFallingEdge,
    };
    GPIO_PinInit(SW_GPIO, SW_GPIO_PIN, &sSwConfig);
}

/*!
 * @brief Check if button is pressed.
 *
 * This function gets the state of button.
 *
 * @return 0 if button is not pressed.
 *         1 if button is pressed
 */
static uint32_t is_key_pressed(void)
{
    return (GPIO_PinRead(SW_GPIO, SW_GPIO_PIN) == SW_GPIO_PRESSED_VALUE);
}

/*!
 * @brief EWM interrupt handler
 *
 */
#pragma interrupt alignsp saveall
void WDOG_EWM_IRQHandler(void);
void WDOG_EWM_IRQHandler(void)
{
    EWM_DisableInterrupt(EXAMPLE_EWM); /*!< de-assert interrupt request */
    bEwmIsrFlag = true;
}
#pragma interrupt off

/*!
 * @brief Main function
 */
int main(void)
{
    ewm_config_t sConfig;

    /* Init hardware*/
    BOARD_InitHardware();
    gpio_configure();

    /* EWM peripheral driver test */
    /*
     * sConfig.enableEwm = true;
     * sConfig.enableEwmInput = false;
     * sConfig.setInputAssertLogic = false;
     * sConfig.enableInterrupt = false;
     * sConfig.clockSource = kEWM_LpoClockSource0;
     * sConfig.clockDivder = 0U;
     * sConfig.compareLowValue = 0U;
     * sConfig.compareHighValue = 0xFEU;
     */
    EWM_GetDefaultConfig(&sConfig);
    sConfig.bEnableInterrupt = true;
    EnableIRQWithPriority(WDOG_EWM_IRQn, WDOG_EWM_PRIO);
    EWM_Init(EXAMPLE_EWM, &sConfig);

    PRINTF("\r\n EWM example ");
    PRINTF("\r\n Press %s to expire EWM ", SW_NAME);
    while (1)
    {
        /* Restart counter*/
        EWM_Refresh(EXAMPLE_EWM);

        /* Check for SW button push*/
        if (is_key_pressed())
        {
            /* Check ewm interrupt occur*/
            while (!bEwmIsrFlag)
            {
            }
            PRINTF("\r\n EWM interrupt is occurred");
            PRINTF("\r\n Press %s to expire EWM again", SW_NAME);

            /* Wait for the key to release*/
            while (is_key_pressed())
            {
            }
            /* Clear interrupt flag*/
            bEwmIsrFlag = false;
            /* Restart counter and enable interrupt for next run*/
            EWM_Refresh(EXAMPLE_EWM);
            /* Enable EWM interrupt*/
            EWM_EnableInterrupt(EXAMPLE_EWM);
        }
    }
}
