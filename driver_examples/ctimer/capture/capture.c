/*
 * Copyright 2023 NXP
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
#include "fsl_ctimer.h"

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
    ctimer_config_t ctimerConfig;
    uint32_t captureValue;

    /* Init hardware*/
    BOARD_InitHardware();

    PRINTF("CTimer capture example\r\n");
    PRINTF("Rising edge triggered and CTimer capture the edge periodically\r\n\r\n");

    DEMO_InitGpioPin();
    DEMO_PullGpioPin(0);
    DEMO_InitCtimerInput();

    /*
     * ctimerConfig.mode = kCTIMER_TimerMode;
     * ctimerConfig.input = kCTIMER_Capture_0;
     * ctimerConfig.prescale = 0;
     */
    CTIMER_GetDefaultConfig(&ctimerConfig);

    CTIMER_Init(DEMO_CTIMER, &ctimerConfig);

    /*
     * Setup the capture, but don't enable the interrupt. And enable the interrupt
     * later using CTIMER_EnableInterrupts. Because if enable interrupt usig
     * CTIMER_SetupCapture, the CTIMER interrupt is also enabled in NVIC, then default
     * driver IRQ handler is called, and callback is involed. To show the capture
     * function easily, the default ISR and callback is not used.
     */
    CTIMER_SetupCapture(DEMO_CTIMER, kCTIMER_Capture_0, kCTIMER_Capture_RiseEdge, false);

    /*
     * Enable the interrupt, so that the kCTIMER_Capture0Flag can be set when
     * edge captured.
     */
    CTIMER_EnableInterrupts(DEMO_CTIMER, kCTIMER_Capture0InterruptEnable);

    CTIMER_StartTimer(DEMO_CTIMER);

    while (1)
    {
        /* Pull up the capture pin, CTIMER will capture the edge. */
        DEMO_PullGpioPin(1);

        /* Wait until edge detected, and timer count saved to capture register */
        while (0U == (kCTIMER_Capture0Flag & CTIMER_GetStatusFlags(DEMO_CTIMER)))
        {
        }

        captureValue = CTIMER_GetCaptureValue(DEMO_CTIMER, kCTIMER_Capture_0);

        CTIMER_ClearStatusFlags(DEMO_CTIMER, kCTIMER_Capture0Flag);

        PRINTF("Timer value is %d when rising edge captured\r\n", captureValue);

        /* Pull down the capture pin, prepare for next capture. */
        DEMO_PullGpioPin(0);

        /* Delay for a while for next capture. */
        SDK_DelayAtLeastUs(1 * 1000 * 1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    }
}