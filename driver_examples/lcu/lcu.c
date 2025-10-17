/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_lcu.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define DEMO_LCU_OUTPUT_MASK (1U << DEMO_LCU_OUTPUT)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile bool lcuIsrFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void DEMO_LCU_IRQHandler(void)
{
    if (LCU_GetLutInterruptStatus(DEMO_LCU) & DEMO_LCU_OUTPUT_MASK)
    {
        PRINTF("\n\rLCU interrupt is occurred !\n\r");

        /* Clear interrupt flag. */
        LCU_ClearLutInterruptStatus(DEMO_LCU, DEMO_LCU_OUTPUT_MASK);
        lcuIsrFlag = true;

        PRINTF("\n\rLCU interrupt flag is cleared !\n\r");
    }
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Structure of initialize LCU. */
    lcu_output_config_t outputConfig;

    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);
    
    PRINTF("\n\rLCU interrupt Example started !\n\r");

    /* Init pit module. */
    LCU_Init(DEMO_LCU);

    /* Get LCU output default configuration. */
    LCU_GetOutputDefaultConfig(&outputConfig);
    /* Set LUT value. */
    outputConfig.lutValue = DEMO_LCU_LUT_VALUE;

    /* Select LC input source. */
    LCU_MuxSelect(DEMO_LCU, DEMO_LCU_INPUT, DEMO_LCU_INPUT_SOURCE);

    /* Init LCU output. */
    LCU_OutputInit(DEMO_LCU, DEMO_LCU_OUTPUT, &outputConfig);

    /* Enable LUT interrupt. */
    LCU_SetLutInterrupt(DEMO_LCU, DEMO_LCU_OUTPUT_MASK, true);

    /* Enable LCU output. */
    LCU_EnableOutput(DEMO_LCU, DEMO_LCU_OUTPUT, true);

    /* Enable at the NVIC. */
    EnableIRQ(DEMO_LCU_IRQn);

    while(!lcuIsrFlag)
    {
    }

    /* Deinit LCU output. */
    LCU_Deinit(DEMO_LCU);
    PRINTF("\n\rLCU interrupt example is finished !\n\r");

    while (true)
    {
    }
}
