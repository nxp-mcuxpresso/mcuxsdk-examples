/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define INPUTMUX                INPUTMUX0
#define Freqme_IRQn             FREQME0_IRQn
#define DEMO_CLOCK_SOURCE_COUNT 4U
#define DEMO_CLOCK_SOURCE_NAME                                  \
    {                                                           \
        "CLK_IN", "FRO_12M", "FREQME_CLK_IN0", "FREQME_CLK_IN1" \
    }

#define DEMO_REFERENCE_CLOCK_SOURCE_SIGNAL                                                                   \
    {                                                                                                        \
        kINPUTMUX_ClkInToFreqmeasRef, kINPUTMUX_FroOsc12MToFreqmeasRef, kINPUTMUX_FreqmeClkIn0ToFreqmeasRef, \
            kINPUTMUX_FreqmeClkIn1ToFreqmeasRef                                                              \
    }

#define DEMO_TARGET_CLOCK_SOURCE_SIGNAL                                                                      \
    {                                                                                                        \
        kINPUTMUX_ClkInToFreqmeasTar, kINPUTMUX_FroOsc12MToFreqmeasTar, kINPUTMUX_FreqmeClkIn0ToFreqmeasTar, \
            kINPUTMUX_FreqmeClkIn1ToFreqmeasTar                                                              \
    }

#define DEMO_REF_CLK_SOURCE    kINPUTMUX_FroHfDivToFreqmeasRef
#define DEMO_TARGET_CLK_SOURCE kINPUTMUX_FroHfDivToFreqmeasTar
#define DEMO_FREQME            FREQME0
#define FREQME_IRQHANDLER      FREQME0_IRQHandler
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
