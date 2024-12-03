/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_cmp.h"
#include "fsl_inputmux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_CmpInterruptFlag = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
void ACMP_IRQHandler(void)
{
    CMP_ClearInterrupt();
    if (true == CMP_GetOutput())
    {
        /* Turn on LED when P-side voltage is greater than that of N-side. */
        GPIO_PinWrite(DEMO_GPIO_BASE, DEMO_GPIO_PORT, DEMO_GPIO_PIN, DEMO_LED_ON);
    }
    else
    {
        /* Turn off LED when P-side voltage is lower than that of N-side. */
        GPIO_PinWrite(DEMO_GPIO_BASE, DEMO_GPIO_PORT, DEMO_GPIO_PIN, DEMO_LED_OFF);
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    cmp_config_t mCmpConfigStruct;
    cmp_vref_config_t mCmpVrefConfigStruct;

    /* Initialize hardware. */
    BOARD_InitHardware();
    EnableIRQ(ACMP_IRQn);

    PRINTF("CMP interrupt driver example\r\n");

    /*
     * config->enableHysteresis    = true;
     * config->enableLowPower      = true;
     * config->filterClockDivider  = kCMP_FilterClockDivide1;
     * config->filterSampleMode    = kCMP_FilterSampleMode0;
     */
    CMP_GetDefaultConfig(&mCmpConfigStruct);
    CMP_Init(&mCmpConfigStruct);

    /* Set VREF source. */
    mCmpVrefConfigStruct.vrefSource = KCMP_VREFSourceVDDA;
    mCmpVrefConfigStruct.vrefValue  = 15U; /* Select half of the reference voltage. */
    CMP_SetVREF(&mCmpVrefConfigStruct);

    /* Set P-side and N-side input channels. */
    CMP_SetInputChannels(DEMO_CMP_P_CHANNEL, DEMO_CMP_N_CHANNEL);

    /* Enable CMP interrupt. */
    CMP_EnableInterrupt(kCMP_EdgeRisingFalling);

    while (1)
    {
    }
}
