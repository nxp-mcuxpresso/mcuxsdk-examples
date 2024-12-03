/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_pdb.h"
#include "fsl_adc.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void DEMO_InitPDB(void);
void DEMO_InitADC(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint16_t g_PdbDelayInterruptCounter;
volatile bool g_PdbDelayInterruptFlag = false;
volatile uint32_t g_Adc16InterruptCounter;
volatile uint32_t g_Adc16ConversionValue;
volatile bool g_Adc16ConversionDoneFlag = false;
const uint32_t g_Adc16_12bitFullRange   = 4096U;
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief ISR for PDB interrupt function
 */
#pragma interrupt alignsp saveall
void DEMO_PDB_IRQHandler(void)
{
    PDB_ClearStatusFlags(DEMO_PDB_BASE, kPDB_DelayAStatusFlag);
    g_PdbDelayInterruptCounter++;
    g_PdbDelayInterruptFlag = true;
}
#pragma interrupt off

/*!
 * @brief ISR for ADC16 interrupt function
 */
#pragma interrupt alignsp saveall
void DEMO_ADC16_IRQHandler(void)
{
    g_Adc16ConversionDoneFlag = true;
    /* Read conversion result to clear the conversion completed flag. */
    g_Adc16ConversionValue = ADC16_GetGroupConversionValue(DEMO_ADC16_BASE, DEMO_ADC16_GROUPID);
    g_Adc16InterruptCounter++;
}
#pragma interrupt off

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("\r\nPDB ADC16 Trigger Example.\r\n");

    DEMO_InitPDB();
    DEMO_InitADC();

    PRINTF("ADC Full Range: %ld\r\n", g_Adc16_12bitFullRange);

    g_PdbDelayInterruptCounter = 0;
    g_Adc16InterruptCounter    = 0;

    while (1)
    {
        PRINTF("\r\nType any key into terminal to trigger the PDB and then trigger the ADC's conversion ...\r\n");
        GETCHAR();
        g_PdbDelayInterruptFlag   = false;
        g_Adc16ConversionDoneFlag = false;

        PDB_DoSoftwareTrigger(DEMO_PDB_BASE);
        while ((!g_PdbDelayInterruptFlag) || (!g_Adc16ConversionDoneFlag))
        {
        }
        PRINTF("PDB Delay Interrupt Counter: %d\r\n", g_PdbDelayInterruptCounter);
        PRINTF("ADC Interrupt Count: %ld\r\n", g_Adc16InterruptCounter);
        PRINTF("ADC Value: %ld\r\n", g_Adc16ConversionValue);
    }
}

/* Initialize the PDB. */
void DEMO_InitPDB(void)
{
    pdb_config_t pdbConfigStruct;

    /* Configure the PDB counter. */
    memset(&pdbConfigStruct, 0, sizeof(pdb_config_t));

    PDB_GetDefaultConfig(&pdbConfigStruct);

    pdbConfigStruct.sPdbTriggerABOutputConfig.bEnableTrigger1 = true;
    pdbConfigStruct.u16EnableInterruptMask                    = kPDB_EnableDelayAInterrupt;
    pdbConfigStruct.bEnablePDB                                = true;
    PDB_Init(DEMO_PDB_BASE, &pdbConfigStruct);

    EnableIRQWithPriority(DEMO_PDB_IRQ_ID, 1);
}

/* Initialize the ADC16. */
void DEMO_InitADC(void)
{
    adc16_config_t adc16ConfigStruct;
    /* Configure the ADC. */
    ADC16_GetDefaultConfig(&adc16ConfigStruct);

    adc16ConfigStruct.eReferenceVoltageSource               = kADC16_ReferenceVoltageSourceVref;
    adc16ConfigStruct.eChannelInput                         = kADC16_Input17Channel;
    adc16ConfigStruct.u32GroupId                            = DEMO_ADC16_GROUPID;
    adc16ConfigStruct.eHardwareAverageMode                  = kADC16_HardwareAverageCount4;
    adc16ConfigStruct.psHardwareCompare->eCompareMode       = kADC16_LessThanThreshold;
    adc16ConfigStruct.bEnableInterruptOnConversionCompleted = true;

    ADC16_Init(DEMO_ADC16_BASE, &adc16ConfigStruct);

    if (kStatus_Success == ADC16_DoAutoCalibration(DEMO_ADC16_BASE))
    {
        PRINTF("ADC16_DoAutoCalibration() Done.\r\n");
    }
    else
    {
        PRINTF("ADC16_DoAutoCalibration() Failed.\r\n");
    }

    ADC16_SetTriggerSource(DEMO_ADC16_BASE, kADC16_HardWareTrigger);

    EnableIRQWithPriority(DEMO_ADC16_IRQ_ID, 2);
}
