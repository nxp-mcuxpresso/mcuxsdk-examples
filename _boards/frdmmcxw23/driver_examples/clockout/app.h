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
#define APP_CLOCK_OUT_SELECT_ARRAY                                                                     \
    {                                                                                                  \
        kMAIN_CLK_to_CLKOUT, kEXT_CLK_to_CLKOUT, kFRO_HF_to_CLKOUT, kFRO1M_to_CLKOUT, kFRO24M_to_CLKOUT, \
            kOSC32K_to_CLKOUT,                                                        \
    }
#define APP_CLOCK_OUT_NAME_ARRAY                                                                        \
    {                                                                                                   \
        "Main Clock", "CLKIN Clock", "FRO 32 MHz Clock", "FRO 1 MHz Clock", "FRO 24 MHz clock", \
            "Oscillator 32 kHz Clock",                                                                  \
    }
#define APP_CLK_DIV_SUPPORT_RESET 1
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
