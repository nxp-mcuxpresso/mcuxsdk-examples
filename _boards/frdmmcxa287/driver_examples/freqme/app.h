/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define INPUTMUX    INPUTMUX0
#define Freqme_IRQn FREQME0_IRQn

#define DEMO_FREQ_TAR_CLOCK_SOURCE_NAME                 \
    {                                                   \
        "CLK_IN", "FRO_12M", "FREQME_CLK_IN0", "OSC32K" \
    }

#define DEMO_PULSE_TAR_CLOCK_SOURCE_NAME            \
    {                                               \
        "CLK_IN", "FRO_12M", "FRO_HF_DIV", "OSC32K" \
    }

#define DEMO_PULSE_TAR_CLOCK_SOURCE_SIGNAL                                                               \
    {                                                                                                    \
        kINPUTMUX_ClkinToFreqmeastar, kINPUTMUX_Froosc12mToFreqmeastar, kINPUTMUX_FrohfdivToFreqmeastar, \
            kINPUTMUX_Osc32k1ToFreqmeastar                                                               \
    }

#define DEMO_FREQ_TAR_CLOCK_SOURCE_SIGNAL                                                                    \
    {                                                                                                        \
        kINPUTMUX_ClkinToFreqmeastar, kINPUTMUX_Froosc12mToFreqmeastar, kINPUTMUX_Freqmeclkin0ToFreqmeastar, \
            kINPUTMUX_Osc32k1ToFreqmeastar                                                                   \
    }

#define DEMO_FREQ_REF_CLK_SOURCE  kINPUTMUX_FrohfdivToFreqmeasref
#define DEMO_PULSE_REF_CLK_SOURCE kINPUTMUX_Freqmeclkin1ToFreqmeasref
#define DEMO_FREQME               FREQME0
#define FREQME_IRQHANDLER         FREQME0_IRQHandler
#define DEMO_MAXEXPECTVALUE       (0x6FFFFFFFUL)
#define DEMO_MINEXPECTVALUE       (0xFUL)
#define DEMO_REF_CLK_FREQ         CLOCK_GetFreq(kCLOCK_FroHfDiv)
#define DEMO_CLOCK_SOURCE_COUNT   4U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
