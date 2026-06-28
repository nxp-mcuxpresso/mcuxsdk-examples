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
typedef uint32_t clock_output_divider_t;

#define CLOCK_DIVIDER_NEED_ATCUAL_VALUE 1

#define APP_OUTPUT1_CLOCK_NAME_ARRAY                                                                \
    {                                                                                               \
        "OSC_RC_48M_DIV2", "OSC_24M", "OSC_RC_400M", "OSC_RC_16M", "SYS_PLL2_PFD2", "SYS_PLL2_CLK", \
            "SYS_PLL3_PFD1", "SYS_PLL1_DIV5",                                                       \
    }

#define APP_OUTPUT1_CLOCK_NAME_ENUM_ARRAY                                                            \
    {                                                                                                \
        kCLOCK_CKO1OutputMuxOscRc48MDiv2, kCLOCK_CKO1OutputMuxOsc24MOut, kCLOCK_CKO1OutputMuxOscRc400M, \
            kCLOCK_CKO1OutputMuxOscRc16M, kCLOCK_CKO1OutputMuxSysPll2Pfd2, kCLOCK_CKO1OutputMuxSysPll2Out, \
            kkCLOCK_CKO1OutputMuxSysPll3Pfd1, kCLOCK_CKO1OutputMuxSysPll1Div5,              \
    }

#define APP_OUTPUT2_CLOCK_NAME_ARRAY                                                              \
    {                                                                                             \
        "OSC_RC_48M_DIV2", "OSC_24M", "OSC_RC_400M", "OSC_RC_16M", "SYS_PLL2_PFD3", "OSC_RC_48M", \
            "SYS_PLL3_PFD1", "AUDIO_PLL_CLK",                                                      \
    }

#define APP_OUTPUT2_CLOCK_NAME_ENUM_ARRAY                                                               \
    {                                                                                                   \
        kCLOCK_CKO2OutputOscRc48MDiv2, kCLOCK_CKO2OutputOsc24MOut, kCLOCK_CKO2OutputOscRc400M, \
            kCLOCK_CKO2OutputOscRc16M, kCLOCK_CKO2OutputSysPll2Pfd3, kCLOCK_CKO2OutputMuxOscRc48M, \
            kCLOCK_CKO2OutputMuxSysPll3Pfd1, kCLOCK_CKO2OutputMuxAudioPllOut, \
    }

#define EXAMPLE_CLKO1_MAX_DIVIDER_VALUE 256U
#define EXAMPLE_CLKO2_MAX_DIVIDER_VALUE 256U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
