/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "board.h"
#include "app.h"
#include "fsl_eqdc.h"
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
     * The TRIGGER signal has trigger EQDC to load position and revolution
     * value to the hold registers, so read the hold registers here.
     */
    u32Position   = EQDC_GetHoldPosition(DEMO_EQDC);
    u16Revolution = EQDC_GetHoldRevolution(DEMO_EQDC);

    u32TriggerCount++;

    if (++u32TriggerCount == 500)
    {
        u32TriggerCount = 0;
        bPrintResult    = true;
    }
}

int main(void)
{
    eqdc_config_t sEqdcConfig;
    uint32_t u32PrintCount;

    BOARD_InitHardware();

    DEMO_InitEqdcClock();
    DEMO_InitEqdcInputSignal();
    DEMO_InitTrigger();

    /*
     * sEqdcConfig.bEnableReverseDirection              = false;
     * sEqdcConfig.bCountOnce                           = false;
     * sEqdcConfig.eOperateMode                         = kEQDC_QuadratureDecodeOperationMode;
     * sEqdcConfig.eHomeEnableInitPosCounterMode        = kEQDC_HomeInitPosCounterDisabled;
     * sEqdcConfig.eIndexPresetInitPosCounterMode       = kEQDC_IndexInitPosCounterDisabled;
     * sEqdcConfig.bEnableDma                           = false;
     * sEqdcConfig.bOkayLoad                            = false;
     * sEqdcConfig.bEnableTriggerInitPositionCounter    = false;
     * sEqdcConfig.bEnableTriggerClearPositionRegisters = false;
     * sEqdcConfig.bEnableTriggerHoldPositionRegisters  = false;
     * sEqdcConfig.bEnableWatchdog                      = false;
     * sEqdcConfig.u16WatchdogTimeoutValue              = 0xFFFFU;
     * sEqdcConfig.bFilterClockSourceselection          = false;
     * sEqdcConfig.eFilterSampleCount                   = kEQDC_Filter3Samples;
     * sEqdcConfig.u8FilterSamplePeriod                 = 0U;
     * sEqdcConfig.eOutputPulseMode                     = kEQDC_OutputPulseOnCounterEqualCompare;
     * sEqdcConfig.u32PositionCompareValue[0]  		    = 0xFFFFFFFFU;
     * sEqdcConfig.u32PositionCompareValue[1]           = 0xFFFFFFFFU;
     * sEqdcConfig.u32PositionCompareValue[2]           = 0xFFFFFFFFU;
     * sEqdcConfig.u32PositionCompareValue[3]           = 0xFFFFFFFFU;
     * sEqdcConfig.eRevolutionCountCondition            = kEQDC_RevolutionCountOnIndexPulse;
     * sEqdcConfig.bEnableModuloCountMode               = false;
     * sEqdcConfig.u32PositionModulusValue              = 0U;
     * sEqdcConfig.u32PositionInitialValue              = 0U;
     * sEqdcConfig.u32PositionCounterValue              = 0U;
     * sEqdcConfig.bEnablePeriodMeasurement             = false;
     * sEqdcConfig.ePrescaler                           = kEQDC_Prescaler1;
     * sEqdcConfig.u16EnabledInterruptsMask             = 0U;
     */
    EQDC_GetDefaultConfig(&sEqdcConfig);

    sEqdcConfig.eRevolutionCountCondition           = kEQDC_RevolutionCountOnRollOverModulus;
    sEqdcConfig.eOperateMode                        = kEQDC_QuadratureDecodeOperationMode;
    sEqdcConfig.bEnableModuloCountMode              = true;
    sEqdcConfig.u32PositionModulusValue             = DEMO_ENCODER_DISK_LINE_X4 - 1;
    sEqdcConfig.bEnableTriggerHoldPositionRegisters = true;

    EQDC_Init(DEMO_EQDC, &sEqdcConfig);

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
