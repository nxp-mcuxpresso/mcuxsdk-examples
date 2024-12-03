/*
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_rgpio.h"
#include "veneer_table.h"
#include "board.h"
#include "app.h"
#include "clock_config.h"
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
        if (RGPIO_PinRead(TEST_SW7_GPIO, TEST_SW7_GPIO_PIN))
        {
            RGPIO_PinWrite(TEST_NS_GPIOA, TEST_GPIOA_PIN18, 1);
        }
        else
        {
            RGPIO_PinWrite(TEST_NS_GPIOA, TEST_GPIOA_PIN18, 0);
        }

        SDK_DelayAtLeastUs(1000, SystemCoreClock);
    }
}
