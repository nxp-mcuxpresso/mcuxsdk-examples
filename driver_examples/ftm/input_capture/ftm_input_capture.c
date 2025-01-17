/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_ftm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool ftmIsrFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void FTM_INPUT_CAPTURE_HANDLER(void)
{
    if ((FTM_GetStatusFlags(BOARD_FTM_BASEADDR) & FTM_CHANNEL_FLAG) == FTM_CHANNEL_FLAG)
    {
        /* Clear interrupt flag.*/
        FTM_ClearStatusFlags(BOARD_FTM_BASEADDR, FTM_CHANNEL_FLAG);
    }
    ftmIsrFlag = true;
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    ftm_config_t ftmInfo;
    uint32_t captureVal;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    /* Print a note to terminal */
    PRINTF("\r\nFTM input capture example\r\n");
    PRINTF("\r\nOnce the input signal is received the input capture value is printed\r\n");

    FTM_GetDefaultConfig(&ftmInfo);
    /* Initialize FTM module */
    FTM_Init(BOARD_FTM_BASEADDR, &ftmInfo);

    /* Setup dual-edge capture on a FTM channel pair */
    FTM_SetupInputCapture(BOARD_FTM_BASEADDR, BOARD_FTM_INPUT_CAPTURE_CHANNEL, kFTM_FallingEdge, 0);

    /* Set the timer to be in free-running mode */
    FTM_SetTimerPeriod(BOARD_FTM_BASEADDR, 0xFFFF);

    /* Enable channel interrupt when the second edge is detected */
    FTM_EnableInterrupts(BOARD_FTM_BASEADDR, FTM_CHANNEL_INTERRUPT_ENABLE);

    /* Enable at the NVIC */
    EnableIRQ(FTM_INTERRUPT_NUMBER);

    FTM_StartTimer(BOARD_FTM_BASEADDR, kFTM_SystemClock);

    while (ftmIsrFlag != true)
    {
    }

    captureVal = FTM_GetInputCaptureValue(BOARD_FTM_BASEADDR, BOARD_FTM_INPUT_CAPTURE_CHANNEL);
    PRINTF("\r\nCapture value C(n)V=%x\r\n", captureVal);

    while (1)
    {
    }
}
