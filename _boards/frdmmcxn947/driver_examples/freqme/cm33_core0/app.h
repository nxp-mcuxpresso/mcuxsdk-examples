/*
 * Copyright 2022 NXP
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

#define DEMO_FREQ_TAR_CLOCK_SOURCE_NAME                                  \
    {                                                           \
        "CLK_IN", "FRO_12M", "FREQME_CLK_IN0" \
    }

#define DEMO_PULSE_TAR_CLOCK_SOURCE_NAME            \
    {                                           \
        "CLK_IN", "FRO_12M", "FRO_144M"       \
    }

#define DEMO_PULSE_TAR_CLOCK_SOURCE_SIGNAL                                                                \
    {                                                                                                     \
        kINPUTMUX_ClkInToFreqmeasTar, kINPUTMUX_Fro12MToFreqmeasTar, kINPUTMUX_Fro144MToFreqmeasTar, \
    }

#define DEMO_FREQ_TAR_CLOCK_SOURCE_SIGNAL                                                                   \
    {                                                                                                     \
        kINPUTMUX_ClkInToFreqmeasTar, kINPUTMUX_Fro12MToFreqmeasTar, kINPUTMUX_FreqmeClkIn0ToFreqmeasTar, \
    }

#define DEMO_FREQ_REF_CLK_SOURCE    kINPUTMUX_Fro144MToFreqmeasRef
#define DEMO_PULSE_REF_CLK_SOURCE kINPUTMUX_FreqmeClkIn1ToFreqmeasRef
#define DEMO_FREQME            FREQME0
#define FREQME_IRQHANDLER      Freqme_IRQHandler
#define DEMO_MAXEXPECTVALUE    (0x6FFFFFFFUL)
#define DEMO_MINEXPECTVALUE    (0xFUL)
#define DEMO_REF_CLK_FREQ      CLOCK_GetFreq(kCLOCK_Clk144M)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
