/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define Freqme_IRQn             FREQME0_IRQn
#define DEMO_FREQME             FREQME0
#define FREQME_IRQHANDLER       FREQME0_IRQHandler

#define DEMO_FREQ_TAR_CLOCK_SOURCE_NAME                                  \
    {                                                           \
        "CLK_IN", "FRO_12M", "FREQME_CLK_IN0" \
    }

#define DEMO_PULSE_TAR_CLOCK_SOURCE_NAME            \
    {                                           \
        "CLK_IN", "FRO_12M", "FRO_HF_DIV"       \
    }

#define DEMO_PULSE_TAR_CLOCK_SOURCE_SIGNAL                                                                  \
    {                                                                                                       \
        kINPUTMUX_SlowClkToFreqmeasTar, kINPUTMUX_Fro12mToFreqmeasTar, kINPUTMUX_FroHfDivToFreqmeasTar, \
    }

#define DEMO_FREQ_TAR_CLOCK_SOURCE_SIGNAL                                                                     \
    {                                                                                                       \
        kINPUTMUX_SlowClkToFreqmeasTar, kINPUTMUX_Fro12mToFreqmeasTar, kINPUTMUX_FreqmeClkIn0ToFreqmeasTar, \
    }

#define DEMO_FREQ_REF_CLK_SOURCE    kINPUTMUX_FroHfDivToFreqmeasRef
#define DEMO_PULSE_REF_CLK_SOURCE kINPUTMUX_FreqmeClkIn1ToFreqmeasRef

#define DEMO_MAXEXPECTVALUE    (0x6FFFFFFFUL)
#define DEMO_MINEXPECTVALUE    (0xFUL)
#define DEMO_REF_CLK_FREQ      CLOCK_GetFreq(kCLOCK_FroHfDiv)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
/*******************************************************************************
 * EOF
 ******************************************************************************/