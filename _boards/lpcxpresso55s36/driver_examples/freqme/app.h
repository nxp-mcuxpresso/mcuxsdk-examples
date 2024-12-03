/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_CLOCK_SOURCE_COUNT 4U
#define DEMO_CLOCK_SOURCE_NAME                                               \
    {                                                                        \
        "XTAL32MHz", "FRO_OSC_12M", "FREQME_GPIO_CLK_A", "FREQME_GPIO_CLK_B" \
    }

#define DEMO_REFERENCE_CLOCK_SOURCE_SIGNAL                                                                           \
    {                                                                                                                \
        kINPUTMUX_Xtal32MhzToFreqmeasRef, kINPUTMUX_FroOsc12MhzToFreqmeasRef, kINPUTMUX_FreqmeGpioClkAToFreqmeasRef, \
            kINPUTMUX_FreqmeGpioClkBToFreqmeasRef                                                                    \
    }

#define DEMO_TARGET_CLOCK_SOURCE_SIGNAL                                                        \
    {                                                                                          \
        kINPUTMUX_Xtal32MhzToFreqmeasTarget, kINPUTMUX_FroOsc12MhzToFreqmeasTarget,            \
            kINPUTMUX_FreqmeGpioClkAToFreqmeasTarget, kINPUTMUX_FreqmeGpioClkBToFreqmeasTarget \
    }

#define DEMO_REF_CLK_SOURCE    kINPUTMUX_FroOsc96MhzToFreqmeasRef
#define DEMO_TARGET_CLK_SOURCE kINPUTMUX_FroOsc96MhzToFreqmeasTarget
#define DEMO_FREQME            FREQME
#define FREQME_IRQHANDLER      Freqme_IRQHandler
#define DEMO_MAXEXPECTVALUE    (0x6FFFFFFFUL)
#define DEMO_MINEXPECTVALUE    (0xFUL)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
