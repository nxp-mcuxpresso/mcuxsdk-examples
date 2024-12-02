/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "board.h"
#include "app.h"
#include "fsl_qdc.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_ENCODER_DISK_LINE 1000

#define DEMO_ENCODER_DISK_LINE_X4 (4 * DEMO_ENCODER_DISK_LINE)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void DEMO_TriggerSignalISR(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t u32Position;
volatile uint16_t u16Revolution;
/* How many times the TRIGGER signal has happend. */
volatile uint32_t u32TriggerCount;
volatile bool bPrintResult = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void DEMO_TriggerSignalISR(void)
{
    /*
     * The TRIGGER signal has trigger QDC to load position and revolution
     * value to the hold registers, so read the hold registers here.
     */
    u32Position   = QDC_GetHoldPosition(DEMO_QDC);
    u16Revolution = QDC_GetHoldRevolution(DEMO_QDC);

    u32TriggerCount++;

    if (++u32TriggerCount == 500)
    {
        u32TriggerCount = 0;
        bPrintResult    = true;
    }
}

int main(void)
{
    qdc_config_t sQdcConfig;
    uint32_t u32PrintCount;

    BOARD_InitHardware();

    DEMO_InitQdcClock();
    DEMO_InitQdcInputSignal();
    DEMO_InitTrigger();

    /*
     * sQdcConfig.bEnableReverseDirection              = false;
     * sQdcConfig.eDecoderWorkMode                     = kQDC_DecoderQuadratureMode;
     * sQdcConfig.eHomeInitPosCounterMode              = kQDC_HomeInitPosCounterDisabled;
     * sQdcConfig.eIndexInitPosCounterMode             = kQDC_IndexInitPosCounterDisabled;
     * sQdcConfig.bEnableTriggerInitPositionCounter    = false;
     * sQdcConfig.bEnableTriggerClearPositionRegisters = false;
     * sQdcConfig.bEnableTriggerHoldPositionRegisters  = false;
     * sQdcConfig.bEnableWatchdog                      = false;
     * sQdcConfig.u16WatchdogTimeoutValue              = 0xFFFFU;
     * sQdcConfig.eFilterSampleCount                   = kQDC_Filter3Samples;
     * sQdcConfig.u8FilterSamplePeriod                 = 0U;
     * sQdcConfig.eOutputPulseMode                     = kQDC_OutputPulseOnCounterEqualCompare;
     * sQdcConfig.u32PositionCompareValue              = 0xFFFFFFFFU;
     * sQdcConfig.u32PositionCompare1Value             = 0xFFFFFFFFU;
     * sQdcConfig.eRevolutionCountCondition            = kQDC_RevolutionCountOnIndexPulse;
     * sQdcConfig.bEnableModuloCountMode               = false;
     * sQdcConfig.u32PositionModulusValue              = 0U;
     * sQdcConfig.u32PositionInitialValue              = 0U;
     * sQdcConfig.u32PositionCounterValue              = 0U;
     * sQdcConfig.bEnablePeriodMeasurement             = false;
     * sQdcConfig.ePrescaler                           = kQDC_Prescaler1;
     * sQdcConfig.u16EnabledInterruptsMask             = 0U;
     */
    QDC_GetDefaultConfig(&sQdcConfig);

    sQdcConfig.eRevolutionCountCondition           = kQDC_RevolutionCountOnRollOverModulus;
    sQdcConfig.bEnableModuloCountMode              = true;
    sQdcConfig.u32PositionModulusValue             = DEMO_ENCODER_DISK_LINE_X4 - 1;
    sQdcConfig.bEnableTriggerHoldPositionRegisters = true;

    QDC_Init(DEMO_QDC, &sQdcConfig);

    u32TriggerCount = 0;
    bPrintResult    = false;
    u32PrintCount   = 0;
    DEMO_StartTrigger();

    while (1)
    {
        while (!bPrintResult)
        {
        }
        bPrintResult = false;

        PRINTF("Print %lu:\t", u32PrintCount);
        PRINTF("Position: %ld\t", u32Position);
        PRINTF("Revolution: %d\t", u16Revolution);
        PRINTF("\r\n");

        u32PrintCount++;
    }
}
