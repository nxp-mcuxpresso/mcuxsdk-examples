/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_tsc.h"
#include "fsl_debug_console.h"
#include "app.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void PortPreCharge(void);
void PortXMeasure(void);
void PortYMeasure(void);
void PortTouchDetect(void);
void PortIntermediate(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*
 * The example indicate how to make TSC, ADC  and touchscreen work together in debug mode.
 * In the debug mode, all TSC outputs will be controlled by software. Software can also observe
 * all TSC inputs through debug interface.
 * This example simulates non-debug(hardware) mode. It has six state: 1st-pre-charge, 1st-detect,
 * x-measure, y-measure, and 2nd-pre-charge, 2nd-detect.
 * Once touchscreen is touched, serial terminal will print the information about x/y-coordinate.
 */
int main(void)
{
    uint32_t i, k_XMeasureValue, k_YMeasureValue;
    tsc_config_t k_tscConfig;

    BOARD_InitHardware();

    PRINTF("TSC DebugMode Example Start!\r\n");

    TSC_GetDefaultConfig(&k_tscConfig);
    TSC_Init(DEMO_TSC_BASE, &k_tscConfig);
    TSC_EnableDebugMode(DEMO_TSC_BASE, true);

    PRINTF("Please touch screen!\r\n");

    while (1)
    {
        /* 1st Pre-charge stage. */
        PortPreCharge(); /* Set correct port mode to pre-charge. */
                         /* Wait enough time for the lower screen layer to achieve even-potential status.
                            Note: Using timer to delay could get more accurate results. */
        for (i = 0; i < 0xFFFFU; ++i)
        {
            __ASM("NOP");
        }
        PortIntermediate();

        /* 1st Screen detection stage. */
        PortTouchDetect(); /* Set correct port mode to detect. */
        TSC_DebugEnableDetection(DEMO_TSC_BASE, kTSC_Detection4WireMode, true);
        while (kTSC_DetectFourWireFlag != (kTSC_DetectFourWireFlag & TSC_GetStatusFlags(DEMO_TSC_BASE)))
        {
        }
        TSC_DebugEnableDetection(DEMO_TSC_BASE, kTSC_Detection4WireMode, false);
        PortIntermediate();

        /* X-measure stage. */
        PortXMeasure(); /* Set correct port mode to measure X-axis. */
        /* Before X-axis or Y-axis measurement, the screen need some time before even potential distribution ready.
           Note: Using timer to delay could get more accurate results. */
        for (i = 0; i < 0xFFFFU; ++i)
        {
            __ASM("NOP");
        }
        /* Hardware tirgger to ADC_HC3. */
        TSC_DebugTriggerSignalToADC(DEMO_TSC_BASE, kTSC_TriggerToChannel3, true);
        TSC_DebugTriggerSignalToADC(DEMO_TSC_BASE, kTSC_TriggerToChannel3, false);
        while (kTSC_ADCCOCOSignalFlag != (kTSC_ADCCOCOSignalFlag & TSC_GetADCStatusFlags(DEMO_TSC_BASE)))
        {
        }
        /* Clear ADC COCO flag. */
        TSC_DebugClearSignalToADC(DEMO_TSC_BASE, true);
        TSC_DebugClearSignalToADC(DEMO_TSC_BASE, false);
        k_XMeasureValue = kTSC_ADCConversionValueFlag & TSC_GetADCStatusFlags(DEMO_TSC_BASE);
        PortIntermediate();

        /* Y-measure stage. */
        PortYMeasure(); /* Set correct port mode to measure Y-axis. */
        /* Before X-axis or Y-axis measurement, the screen need some time before even potential distribution ready.
           Note: Using timer to delay could get more accurate results. */
        for (i = 0; i < 0xFFFFU; ++i)
        {
            __ASM("NOP");
        }
        /* Hardware tirgger to ADC_HC1. */
        TSC_DebugTriggerSignalToADC(DEMO_TSC_BASE, kTSC_TriggerToChannel1, true);
        TSC_DebugTriggerSignalToADC(DEMO_TSC_BASE, kTSC_TriggerToChannel1, false);
        while (kTSC_ADCCOCOSignalFlag != (kTSC_ADCCOCOSignalFlag & TSC_GetADCStatusFlags(DEMO_TSC_BASE)))
        {
        }
        /* Clear ADC COCO flag. */
        TSC_DebugClearSignalToADC(DEMO_TSC_BASE, true);
        TSC_DebugClearSignalToADC(DEMO_TSC_BASE, false);
        k_YMeasureValue = kTSC_ADCConversionValueFlag & TSC_GetADCStatusFlags(DEMO_TSC_BASE);
        PortIntermediate();

        /* 2nd Pre-charge stage. */
        PortPreCharge(); /* Set correct port mode to pre-charge. */
                         /* Wait enough time for the lower screen layer to achieve even-potential status.
                            Note: Using timer to delay could get more accurate results. */
        for (i = 0; i < 0xFFFFU; ++i)
        {
            __ASM("NOP");
        }
        PortIntermediate();

        /* 2nd Screen detectiopn stage. */
        PortTouchDetect(); /* Set correct port mode to detect. */
        TSC_DebugEnableDetection(DEMO_TSC_BASE, kTSC_Detection4WireMode, true);
        /* Wait enough time to read four wire detect status from analogue.
           Note: Using timer to delay could get more accurate results. */
        for (i = 0; i < 0xFFFFU; ++i)
        {
            __ASM("NOP");
        }
        /* If no touch has been detected, then previous measured coordinates¡¯ value is invalid. */
        if (kTSC_DetectFourWireFlag == (kTSC_DetectFourWireFlag & TSC_GetStatusFlags(DEMO_TSC_BASE)))
        {
            TSC_DebugEnableDetection(DEMO_TSC_BASE, kTSC_Detection4WireMode, false);
            PRINTF("x = %d, y = %d\r\n", k_XMeasureValue, k_YMeasureValue);
        }
        PortIntermediate();
    }
}

/*!
 * @brief Set TSC port mode to work in pre-charge state.
 */
void PortPreCharge(void)
{
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_WiperPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_YnlrPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_YpllPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_XnurPortSource, kTSC_Port200k_PullUpMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_XpulPortSource, kTSC_Port200k_PullUpMode);
}

/*!
 * @brief Set TSC port mode to work in X-axis measure state.
 */
void PortXMeasure(void)
{
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_WiperPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_YnlrPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_YpllPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_XnurPortSource, kTSC_PortPullDownMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_XpulPortSource, kTSC_PortPullUpMode);
}

/*!
 * @brief Set TSC port mode to work in Y-axis measure state.
 */
void PortYMeasure(void)
{
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_WiperPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_YnlrPortSource, kTSC_PortPullDownMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_YpllPortSource, kTSC_PortPullUpMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_XnurPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_XpulPortSource, kTSC_PortOffMode);
}

/*!
 * @brief Set TSC port mode to work in detect state.
 */
void PortTouchDetect(void)
{
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_WiperPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_YnlrPortSource, kTSC_PortPullDownMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_YpllPortSource, kTSC_PortPullDownMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_XnurPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_XpulPortSource, kTSC_Port200k_PullUpMode);
}

/*!
 * @brief Set TSC port mode to work in intermediate state.
 *        Intermediate state exists after pre-charge, detect, x- measure, y-measure, and
 *        2nd-pre-charge, 2nd-detect.
 */
void PortIntermediate(void)
{
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_WiperPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_YnlrPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_YpllPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_XnurPortSource, kTSC_PortOffMode);
    TSC_DebugSetPortMode(DEMO_TSC_BASE, kTSC_XpulPortSource, kTSC_Port200k_PullUpMode);
}
