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

#define DEMO_FREQ_TAR_CLOCK_SOURCE_NAME                                               \
    {                                                                        \
        "XTAL32MHz", "FRO_OSC_12M", "FREQME_GPIO_CLK_A" \
    }

#define DEMO_PULSE_TAR_CLOCK_SOURCE_NAME            \
    {                                           \
        "XTAL32MHz", "FRO_OSC_12M", "FRO_OSC_96M"       \
    }

#define DEMO_PULSE_TAR_CLOCK_SOURCE_SIGNAL                                                      \
    {                                                                                           \
        kINPUTMUX_Xtal32MhzToFreqmeasTarget, kINPUTMUX_FroOsc12MhzToFreqmeasTarget,             \
        kINPUTMUX_FroOsc96MhzToFreqmeasTarget,                                                  \
    }

#define DEMO_FREQ_TAR_CLOCK_SOURCE_SIGNAL                                                        \
    {                                                                                          \
        kINPUTMUX_Xtal32MhzToFreqmeasTarget, kINPUTMUX_FroOsc12MhzToFreqmeasTarget,            \
            kINPUTMUX_FreqmeGpioClkAToFreqmeasTarget \
    }

#define DEMO_FREQ_REF_CLK_SOURCE    kINPUTMUX_FroOsc96MhzToFreqmeasRef
#define DEMO_PULSE_REF_CLK_SOURCE kINPUTMUX_FreqmeGpioClkBToFreqmeasRef
#define DEMO_FREQME            FREQME
#define FREQME_IRQHANDLER      Freqme_IRQHandler
#define DEMO_MAXEXPECTVALUE    (0x6FFFFFFFUL)
#define DEMO_MINEXPECTVALUE    (0xFUL)
#define DEMO_REF_CLK_FREQ      CLOCK_GetFreq(kCLOCK_FroHf)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
