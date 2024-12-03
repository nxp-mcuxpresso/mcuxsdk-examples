/*
 * Copyright  2018 NXP
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
static volatile bool s_cmpStatusChanged = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
void ANA_COMP_IRQHandler(void)
{
    if (CMP_GetStatus())
    {
        CMP_DisableInterrupt();
        s_cmpStatusChanged = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    cmp_config_t cmpConfig;
    cmp_interrupt_mask_t intType = kCMP_LevelHigh;
    /* Board pin, clock, debug console initialization */
    BOARD_InitHardware();

    PRINTF("CMP input external driver example\r\n");

    /* CMP initialization */
    cmpConfig.enLowHysteris = true;
    cmpConfig.src           = kCMP_InputAllExternal;
    cmpConfig.mode          = kCMP_LowpowerMode;
    CMP_Init(&cmpConfig);

    /* capture rasing edge and falling edge interrupt */
    CMP_SetInterruptConfig(intType);
    /* enable CMP interrupt */
    CMP_EnableInterrupt();
    CMP_ClearStatus();
    EnableIRQ(ANA_COMP_IRQn);

    while (1)
    {
        if (s_cmpStatusChanged)
        {
            if (CMP_GetOutput())
            {
                PRINTF("Input source 0 voltage range higher than source 1\r\n");
            }
            else
            {
                PRINTF("Input source 0 voltage range lower than source 1\r\n");
                break;
            }
            s_cmpStatusChanged = false;
            intType            = (intType == kCMP_LevelHigh ? kCMP_LevelLow : kCMP_LevelHigh);
            CMP_SetInterruptConfig(intType);
            CMP_ClearStatus();
            CMP_EnableInterrupt();
        }
    }

    PRINTF("CMP input external driver example finish\r\n");
    POWER_DisablePD(kPDRUNCFG_PD_BOD_ANA_COMP_EN);
}
