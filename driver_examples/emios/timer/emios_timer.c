/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_emios.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_MC_FREQ     (1000U)     /* Modulus Counter frequency 1kHz */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t count;

/*******************************************************************************
 * Code
 ******************************************************************************/
void EXAMPLE_EMIOS_IRQHandler(void)
{
    uint32_t status;

    status = EMIOS_GetUCStatusFlag(EXAMPLE_EMIOS, EXAMPLE_MC_CHANNEL);

    if ((status & kEMIOS_EventFlag) != 0U)
    {
        EMIOS_ClearUCStatusFlag(EXAMPLE_EMIOS, kEMIOS_EventFlag, EXAMPLE_MC_CHANNEL);
        count++;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    emios_config_t emiosConfig;
    emios_uc_mc_config_t ucConfig;

    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("\r\neMIOS Timer Example");

    EMIOS_GetDefaultConfig(&emiosConfig);

    emiosConfig.prescale = EXAMPLE_EMIOS_CLK_FREQ / EXAMPLE_COUNTERBUS_FREQ - 1U;
    emiosConfig.useGlobalTimeBase = true;
    EMIOS_Init(EXAMPLE_EMIOS, &emiosConfig);

    ucConfig.period = EXAMPLE_COUNTERBUS_FREQ / EXAMPLE_MC_FREQ - 1U;
    ucConfig.prescale = 0U;
    ucConfig.ucMode = kEMIOS_MC_UpCount;
    ucConfig.clockSource = kEMIOS_ClkSrcInternal;
    ucConfig.prescalerSource = kEMIOS_Prescaler_PrescaledClock;
    ucConfig.counterResetTiming = kEMIOS_ResetMatchEnd;
    ucConfig.reloadOutputDelay = 0U;
    ucConfig.enableFreeze = false;
    EMIOS_ConfigModulusCounter(EXAMPLE_EMIOS, &ucConfig, EXAMPLE_MC_CHANNEL);

    EMIOS_EnableUCInterrupt(EXAMPLE_EMIOS, EXAMPLE_MC_CHANNEL);
    (void)EnableIRQ(EXAMPLE_EMIOS_IRQn);

    EMIOS_EnableUCPrescaler(EXAMPLE_EMIOS, EXAMPLE_MC_CHANNEL);

    EMIOS_EnableGlobalPrescaler(EXAMPLE_EMIOS);

    while (1)
    {
        if (count == EXAMPLE_MC_FREQ)
        {
            count = 0;
            PRINTF("\r\n1s elapsed");
        }
    }
}
