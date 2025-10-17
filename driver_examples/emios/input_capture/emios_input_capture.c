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

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool captureFlag = false;
uint32_t leadingEdge;
uint32_t trailingEdge;
uint32_t pulseWidth;
volatile uint32_t count = 0U;

/*******************************************************************************
 * Code
 ******************************************************************************/
void EXAMPLE_EMIOS_IRQHandler(void)
{
    uint32_t status;

    status = EMIOS_GetUCStatusFlag(EXAMPLE_EMIOS, EXAMPLE_IC_CHANNEL);

    if ((status & kEMIOS_EventFlag) != 0U)
    {
        EMIOS_ClearUCStatusFlag(EXAMPLE_EMIOS, kEMIOS_EventFlag, EXAMPLE_IC_CHANNEL);

        /* Get fifth high pulse. */
        count++;
        if (count == 5U)
        {
            /* Get leading edge and trailing edge capture value. */
            trailingEdge = EMIOS_GetAn(EXAMPLE_EMIOS, EXAMPLE_IC_CHANNEL);
            leadingEdge = EMIOS_GetBn(EXAMPLE_EMIOS, EXAMPLE_IC_CHANNEL);

            /* Disable interrupt and global prescaler clock. */
            EMIOS_DisableUCInterruptAndDMA(EXAMPLE_EMIOS, EXAMPLE_IC_CHANNEL);
            EMIOS_DisableGlobalPrescaler(EXAMPLE_EMIOS);
            captureFlag = true;
        }
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    emios_config_t emiosConfig;
    emios_uc_mc_config_t ucMCConfig;
    emios_uc_ic_config_t ucICConfig;

    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("\r\neMIOS Input Capture Example");

    EMIOS_GetDefaultConfig(&emiosConfig);

    emiosConfig.prescale = EXAMPLE_EMIOS_CLK_FREQ / EXAMPLE_COUNTERBUS_FREQ - 1U;
    emiosConfig.useGlobalTimeBase = true;
    EMIOS_Init(EXAMPLE_EMIOS, &emiosConfig);

    ucMCConfig.period = 0xFFFFU;
    ucMCConfig.prescale = 0U;
    ucMCConfig.ucMode = kEMIOS_MC_UpCount;
    ucMCConfig.clockSource = kEMIOS_ClkSrcInternal;
    ucMCConfig.prescalerSource = kEMIOS_Prescaler_PrescaledClock;
    ucMCConfig.counterResetTiming = kEMIOS_ResetMatchEnd;
    ucMCConfig.reloadOutputDelay = 0U;
    ucMCConfig.enableFreeze = false;
    EMIOS_ConfigModulusCounter(EXAMPLE_EMIOS, &ucMCConfig, EXAMPLE_COUNTERBUS_CHANNEL);

    ucICConfig.ucMode = kEMIOS_IPWM;
    ucICConfig.counterBus = kEMIOS_CounterBus_A;
    ucICConfig.edgePolarity = kEMIOS_IC_RisingEdge;
    ucICConfig.filterClock = kEMIOS_Filter_PrescaledClock;
    ucICConfig.filterWidth = kEMIOS_Filter_16_Cycle;
    ucICConfig.enableFreeze = false;
    EMIOS_ConfigInputCapture(EXAMPLE_EMIOS, &ucICConfig, EXAMPLE_IC_CHANNEL);

    EMIOS_EnableUCInterrupt(EXAMPLE_EMIOS, EXAMPLE_IC_CHANNEL);
    (void)EnableIRQ(EXAMPLE_EMIOS_IRQn);

    EMIOS_EnableUCPrescaler(EXAMPLE_EMIOS, EXAMPLE_COUNTERBUS_CHANNEL);
    EMIOS_EnableUCPrescaler(EXAMPLE_EMIOS, EXAMPLE_IC_CHANNEL);

    EMIOS_EnableGlobalPrescaler(EXAMPLE_EMIOS);

    while (captureFlag != true)
    {
    }

    pulseWidth = (trailingEdge >= leadingEdge) ? (trailingEdge - leadingEdge) :
                                                 (0xFFFFU - leadingEdge + trailingEdge + 1U);
    PRINTF("\r\nPulse Width count %d", pulseWidth);

    while (1)
    {
    }
}
