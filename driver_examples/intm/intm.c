/*
 * Copyright 2022 NXP
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
#include "fsl_intm.h"
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
volatile bool g_statusFlag  = 0U;
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Interrupt service fuction of switch.
 *
 * Key Interrupt
 */
void BOARD_SW_IRQ_HANDLER(void)
{
    /* delay 1000us to let intm timeout occur */
    SDK_DelayAtLeastUs(1000U, INTM_SOURCE_CLOCK);

    /* Stop timer regist count */
    INTM_AckIrq(INTM0, BOARD_SW_IRQ);
    /* Get interrupt monitor channel status */
    g_statusFlag = INTM_GetStatusFlags(INTM0, kINTM_Monitor1);

    /* clear time out */
    INTM_ClearTimeCount(INTM0, kINTM_Monitor1);
    /* Clear external interrupt flag. */
    GPIO_GpioClearInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);

    /* Change state of button. */
    g_ButtonPress = true;
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Key Config.
 */
void GPIO_InitConfig()
{
    /* Define the init structure for the input switch pin */
    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput,
        0,
    };

    /* Init input switch GPIO */
    GPIO_SetPinInterruptConfig(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, kGPIO_InterruptFallingEdge);

    /* Enable interrupt*/
    EnableIRQ(BOARD_SW_IRQ);

    /* GPIO pin init */
    GPIO_PinInit(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, &sw_config);
}

/*!
 * @brief Main function
 */
int main(void)
{
    intm_config_t intmConfig[INTM_CHANNEL];

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    /* GPIO Config */
    GPIO_InitConfig();

    PRINTF("INTM Example Start:\r\n");
    PRINTF("\r\nPress %s to trigger interrupt. \r\n", BOARD_SW_NAME);

    /* Init the INTM module */
    INTM_GetDefaultConfig(intmConfig);

    intmConfig[kINTM_Monitor1].intm->irqnumber = BOARD_SW_IRQ;
    intmConfig[kINTM_Monitor1].intm->maxtimer  = USEC_TO_COUNT(1000U, INTM_SOURCE_CLOCK);
    intmConfig->enable                         = true;

    INTM_Init(INTM0, intmConfig);

    while (!g_ButtonPress)
    {
    }

    if (g_ButtonPress)
    {
        if (g_statusFlag == 1U)
        {
            PRINTF("INTM timeout. \r\n");
        }
        else
        {
            PRINTF("INTM no timeout. \r\n");
        }
    }

    PRINTF("\r\nINTM Example End.\r\n");

    while (1)
    {
    }
}
