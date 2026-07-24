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
#define INPUTMUX    SYSCON__INPUTMUX
#define Freqme_IRQn SYSCON_FREQME_IRQn

#define DEMO_FREQ_TAR_CLOCK_SOURCE_NAME \
    {                                   \
        "FRO24M_ROOTCLK",               \
        "FRO48M_ROOTCLK",               \
        "FRO96M_ROOTCLK",               \
    }

#define DEMO_PULSE_TAR_CLOCK_SOURCE_NAME \
    {                                    \
        "FRO24M_ROOTCLK",                \
        "FRO48M_ROOTCLK",                \
        "FRO96M_ROOTCLK",                \
    }

#define DEMO_PULSE_TAR_CLOCK_SOURCE_SIGNAL   \
    {                                        \
        kINPUTMUX_SYSCON_fro24m_rootclk_tar, \
        kINPUTMUX_SYSCON_fro48m_rootclk_tar, \
        kINPUTMUX_SYSCON_fro96m_rootclk_tar, \
    }

#define DEMO_FREQ_TAR_CLOCK_SOURCE_SIGNAL    \
    {                                        \
        kINPUTMUX_SYSCON_fro24m_rootclk_tar, \
        kINPUTMUX_SYSCON_fro48m_rootclk_tar, \
        kINPUTMUX_SYSCON_fro96m_rootclk_tar, \
    }

#define DEMO_FREQ_REF_CLK_SOURCE  kINPUTMUX_SYSCON_base_clk_ref
#define DEMO_PULSE_REF_CLK_SOURCE kINPUTMUX_SYSCON_ulp32k_rootclk_ref
#define DEMO_FREQME               SYSCON__FREQME
#define FREQME_IRQHANDLER         SYSCON_FREQME_IRQHandler
#define DEMO_MAXEXPECTVALUE       (0x6FFFFFFFUL)
#define DEMO_MINEXPECTVALUE       (0xFUL)
#define DEMO_REF_CLK_FREQ         CLOCK_GetRootClockFreq(kCLOCK_Root_CGU_BASE_CLK)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
