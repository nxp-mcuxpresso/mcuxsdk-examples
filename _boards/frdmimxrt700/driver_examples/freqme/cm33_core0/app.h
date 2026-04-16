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

#define DEMO_FREQ_TAR_CLOCK_SOURCE_NAME                  \
    {                                           \
        "OSC_CLK", "FRO1_DIV8", "LPOSC"         \
    }

#define DEMO_PULSE_TAR_CLOCK_SOURCE_NAME            \
    {                                           \
        "OSC_CLK", "FRO1_DIV8", "FRO1"       \
    }

#define DEMO_PULSE_TAR_CLOCK_SOURCE_SIGNAL                                                   \
    {                                                                                        \
        kINPUTMUX_OscClkToFreqmeasTar, kINPUTMUX_Fro1Div8ToFreqmeasTar, kINPUTMUX_Fro1ToFreqmeasTar \
    }

#define DEMO_FREQ_TAR_CLOCK_SOURCE_SIGNAL                                                            \
    {                                                                                              \
        kINPUTMUX_OscClkToFreqmeasTar, kINPUTMUX_Fro1Div8ToFreqmeasTar, kINPUTMUX_LposcToFreqmeasTar \
    }

#define DEMO_FREQ_REF_CLK_SOURCE    kINPUTMUX_Fro1ToFreqmeasRef
#define DEMO_PULSE_REF_CLK_SOURCE kINPUTMUX_32KhzOscToFreqmeasRef
#define DEMO_FREQME            FREQME
#define FREQME_IRQHANDLER      Freqme_IRQHandler
#define DEMO_MAXEXPECTVALUE    (0x6FFFFFFFUL)
#define DEMO_MINEXPECTVALUE    (0xFUL)
#define INPUTMUX               INPUTMUX0
#define DEMO_REF_CLK_FREQ      CLOCK_GetFroClkFreq(1U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
