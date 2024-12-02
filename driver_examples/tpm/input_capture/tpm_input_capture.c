/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_tpm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool tpmIsrFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void TPM_INPUT_CAPTURE_HANDLER(void)
{
    tpmIsrFlag = true;

    /* Clear interrupt flag.*/
    TPM_ClearStatusFlags(DEMO_TPM_BASEADDR, TPM_CHANNEL_FLAG);
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    tpm_config_t tpmInfo;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    /* Print a note to terminal */
    PRINTF("\r\nTPM input capture example\r\n");
    PRINTF("\r\nOnce the input signal is received the input capture value is printed\r\n");

    TPM_GetDefaultConfig(&tpmInfo);
    /* Initialize TPM module */
    TPM_Init(DEMO_TPM_BASEADDR, &tpmInfo);

    /* Setup input capture on a TPM channel */
    TPM_SetupInputCapture(DEMO_TPM_BASEADDR, BOARD_TPM_INPUT_CAPTURE_CHANNEL, kTPM_FallingEdge);

    /* Set the timer to be in free-running mode */
    TPM_SetTimerPeriod(DEMO_TPM_BASEADDR, TPM_MAX_COUNTER_VALUE(DEMO_TPM_BASEADDR));

    /* Enable channel interrupt when the second edge is detected */
    TPM_EnableInterrupts(DEMO_TPM_BASEADDR, TPM_CHANNEL_INTERRUPT_ENABLE);

    /* Enable at the NVIC */
    EnableIRQ(TPM_INTERRUPT_NUMBER);

    TPM_StartTimer(DEMO_TPM_BASEADDR, kTPM_SystemClock);

    while (tpmIsrFlag != true)
    {
    }

    PRINTF("\r\nCapture value C(n)V=%x\r\n", TPM_GetChannelValue(DEMO_TPM_BASEADDR, BOARD_TPM_INPUT_CAPTURE_CHANNEL));
    while (1)
    {
    }
}
