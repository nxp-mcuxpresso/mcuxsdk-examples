/*
 * Copyright 2024, 2026 NXP
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

#define DEMO_FREQ_TAR_CLOCK_SOURCE_NAME         \
    {                                           \
        "CLK_IN", "FRO_12M", "FREQME_CLK_IN0"   \
    }

#define DEMO_FREQ_TAR_CLOCK_SOURCE_SIGNAL                                   \
    {                                                                       \
        (uint32_t)kFREQME_TarClkIn, (uint32_t)kFREQME_TarFroOsc12M, (uint32_t)kFREQME_TarFreqmeClkIn0,  \
    }

#define DEMO_PULSE_TAR_CLOCK_SOURCE_NAME        \
    {                                           \
        "CLK_IN", "FRO_12M", "FRO_HF_DIV"       \
    }

#define DEMO_PULSE_TAR_CLOCK_SOURCE_SIGNAL                              \
    {                                                                   \
        (uint32_t)kFREQME_TarClkIn, (uint32_t)kFREQME_TarFroOsc12M, (uint32_t)kFREQME_TarFroHfDiv,  \
    }

#define DEMO_FREQ_REF_CLK_SOURCE    (uint32_t)kFREQME_RefFroHfDiv
#define DEMO_PULSE_REF_CLK_SOURCE   (uint32_t)kFREQME_RefFreqmeClkIn1
#define DEMO_FREQME                 FREQME0
#define FREQME_IRQHANDLER           FREQME0_IRQHandler
#define DEMO_MAXEXPECTVALUE         (0x6FFFFFFFUL)
#define DEMO_MINEXPECTVALUE         (0xFUL)
#define DEMO_REF_CLK_FREQ           CLOCK_GetFreq(kCLOCK_FroHfDiv)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
