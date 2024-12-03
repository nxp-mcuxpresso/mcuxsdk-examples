/*
 * Copyright 2019 NXP
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

const uint8_t slcd_lcd_gpio_seg_pin[] = {38, 36, 34, 32, 31, 29, 25, 23, 43, 37,
                                         35, 33, 50, 30, 45, 24, 26, 28, 44, 59};
/*${variable:end}*/

/*${function:start}*/
void BOARD_SetSlcdBackPlanePhase(void)
{
    SLCD_SetBackPlanePhase(LCD, 57, kSLCD_PhaseAActivate); /* SLCD_COM1 --- LCD_P57. */
    SLCD_SetBackPlanePhase(LCD, 19, kSLCD_PhaseBActivate); /* SLCD_COM2 --- LCD_P19. */
    SLCD_SetBackPlanePhase(LCD, 13, kSLCD_PhaseCActivate); /* SLCD_COM3 --- LCD_P13. */
    SLCD_SetBackPlanePhase(LCD, 58, kSLCD_PhaseDActivate); /* SLCD_COM4 --- LCD_P58. */
    SLCD_SetBackPlanePhase(LCD, 56, kSLCD_PhaseEActivate); /* SLCD_COM5 --- LCD_P56. */
    SLCD_SetBackPlanePhase(LCD, 22, kSLCD_PhaseFActivate); /* SLCD_COM6 --- LCD_P22. */
    SLCD_SetBackPlanePhase(LCD, 20, kSLCD_PhaseGActivate); /* SLCD_COM7 --- LCD_P20. */
    SLCD_SetBackPlanePhase(LCD, 14, kSLCD_PhaseHActivate); /* SLCD_COM8 --- LCD_P14. */
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* RNGA clock bug fix. */
    CLOCK_EnableClock(kCLOCK_Rnga0);
    CLOCK_DisableClock(kCLOCK_Rnga0);
    CLOCK_EnableClock(kCLOCK_Rnga0);
}
/*${function:end}*/
