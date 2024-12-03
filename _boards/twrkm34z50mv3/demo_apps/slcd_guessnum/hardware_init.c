/*
 * Copyright 2021 NXP
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
slcd_clock_config_t slcdClkConfig = {kSLCD_DefaultClk, kSLCD_AltClkDivFactor1, kSLCD_ClkPrescaler01
#if FSL_FEATURE_SLCD_HAS_FAST_FRAME_RATE
                                     ,
                                     false
#endif
};

const uint8_t slcd_lcd_gpio_seg_pin[] = {43, 37, 36, 35, 34, 33, 32, 31, 28, 27,
                                         26, 25, 24, 23, 22, 21, 20, 19, 18, 17};
/*${variable:end}*/

/*${function:start}*/
void BOARD_SetSlcdBackPlanePhase(void)
{
    SLCD_SetBackPlanePhase(LCD, 16, kSLCD_PhaseAActivate); /* SLCD_COM1 --- LCD_P16. */
    SLCD_SetBackPlanePhase(LCD, 15, kSLCD_PhaseBActivate); /* SLCD_COM2 --- LCD_P15. */
    SLCD_SetBackPlanePhase(LCD, 14, kSLCD_PhaseCActivate); /* SLCD_COM3 --- LCD_P14. */
    SLCD_SetBackPlanePhase(LCD, 13, kSLCD_PhaseDActivate); /* SLCD_COM4 --- LCD_P13. */
    SLCD_SetBackPlanePhase(LCD, 12, kSLCD_PhaseEActivate); /* SLCD_COM5 --- LCD_P12. */
    SLCD_SetBackPlanePhase(LCD, 11, kSLCD_PhaseFActivate); /* SLCD_COM6 --- LCD_P11. */
    SLCD_SetBackPlanePhase(LCD, 10, kSLCD_PhaseGActivate); /* SLCD_COM7 --- LCD_P10. */
    SLCD_SetBackPlanePhase(LCD, 9, kSLCD_PhaseHActivate);  /* SLCD_COM8 --- LCD_P9. */
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
