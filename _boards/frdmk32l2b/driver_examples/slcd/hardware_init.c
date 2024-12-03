/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_slcd.h"
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*${variable:start}*/
slcd_clock_config_t slcdClkConfig = {kSLCD_AlternateClk1, kSLCD_AltClkDivFactor256, kSLCD_ClkPrescaler01
#if FSL_FEATURE_SLCD_HAS_FAST_FRAME_RATE
                                     ,
                                     false
#endif
};

const uint8_t slcd_lcd_gpio_seg_pin[] = {
    20, /* SLCD P05 --- LCD_P20. */
    24, /* SLCD P06 --- LCD_P24. */
    26, /* SLCD P07 --- LCD_P26. */
    27, /* SLCD P08 --- LCD_P27. */
    40, /* SLCD P09 --- LCD_P40. */
    42, /* SLCD P10 --- LCD_P42. */
    43, /* SLCD P11 --- LCD_P43. */
    44, /* SLCD P12 --- LCD_P44. */
};
/*${variable:end}*/

/*${function:start}*/
void BOARD_SetSlcdBackPlanePhase(void)
{
    SLCD_SetBackPlanePhase(LCD, 59, kSLCD_PhaseAActivate); /* SLCD COM1 --- LCD_P59. */
    SLCD_SetBackPlanePhase(LCD, 60, kSLCD_PhaseBActivate); /* SLCD COM2 --- LCD_P60. */
    SLCD_SetBackPlanePhase(LCD, 14, kSLCD_PhaseCActivate); /* SLCD COM3 --- LCD_P14. */
    SLCD_SetBackPlanePhase(LCD, 15, kSLCD_PhaseDActivate); /* SLCD COM4 --- LCD_P15. */
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    /* Enable the MCGIRCLK */
    MCG->C1 |= MCG_C1_IRCLKEN_MASK;
}
/*${function:end}*/
