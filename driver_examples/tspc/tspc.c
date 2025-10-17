/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_tspc.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile bool pad2IsrFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void DEMO_TSPC_PAD2_IRQHandler(void)
{
    PRINTF("\n\rPAD2 interrupt is occurred !\n\r");

    pad2IsrFlag = true;

    /* Clear pad interrupt flags. */
    DEMO_ClearPad2InterruptStatusFlags();

    PRINTF("\n\rPAD2 interrupt flag is cleared !\n\r");

    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);
    
    PRINTF("\n\rTSPC Example started !\n\r");

    /* Init TSPC pad1(pull-up, input buffer enable, output buffer enable). */
    DEMO_Pad1Init();

    /* Init TSPC pad2(pull-up, input buffer enable, output buffer enable, interrupt enable). */
    DEMO_Pad2Init();

    /* Init TSPC module. */
    TSPC_Init(DEMO_TSPC);

    /* Init TSPC group. */
    TSPC_InitGroup(DEMO_TSPC, DEMO_TSPC_GROUP, (DEMO_TSPC_PAD1 | DEMO_TSPC_PAD2));

    /* Disable pad1 output buffer. */
    DEMO_DisablePad1OutputBuffer();

    while(!pad2IsrFlag)
    {
    }

    /* Deinit LCU output. */
    TSPC_Deinit(DEMO_TSPC);
    PRINTF("\n\rTSPC example is finished !\n\r");

    while (true)
    {
    }
}
