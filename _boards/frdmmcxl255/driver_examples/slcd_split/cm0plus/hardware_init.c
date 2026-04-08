/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_slcd.h"
#include "app.h"
#include "mcmgr.h"
/*${header:end}*/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*${variable:start}*/
SLCD_Type g_slcd = {
    .control = AON__SGLCD0_AON,
};

const uint8_t slcd_lcd_gpio_seg_pin[] = {
    6,  /* SLCD P05 --- LCD_P6. */
    7,  /* SLCD P06 --- LCD_P7. */
    30, /* SLCD P07 --- LCD_P30. */
    31, /* SLCD P08 --- LCD_P31. */
    41, /* SLCD P09 --- LCD_P41. */
    33, /* SLCD P10 --- LCD_P33. */
    34, /* SLCD P11 --- LCD_P34. */
    38, /* SLCD P12 --- LCD_P38. */
    39, /* SLCD P13 --- LCD_P39. */
    46, /* SLCD P14 --- LCD_P46. */
    53, /* SLCD P15 --- LCD_P53. */
    48, /* SLCD P16 --- LCD_P48. */
    49, /* SLCD P17 --- LCD_P49. */
    50, /* SLCD P18 --- LCD_P50. */
    51, /* SLCD P19 --- LCD_P51. */
    55, /* SLCD P20 --- LCD_P55. */
};
/*${variable:end}*/

/*${function:start}*/
void BOARD_SetSlcdBackPlanePhase(void)
{
    SLCD_SetBackPlanePhase(LCD, 1, kSLCD_PhaseAActivate);  /* SLCD COM1 --- LCD_P1. */
    SLCD_SetBackPlanePhase(LCD, 15, kSLCD_PhaseBActivate); /* SLCD COM2 --- LCD_P15. */
    SLCD_SetBackPlanePhase(LCD, 16, kSLCD_PhaseCActivate); /* SLCD COM3 --- LCD_P16. */
    SLCD_SetBackPlanePhase(LCD, 22, kSLCD_PhaseDActivate); /* SLCD COM4 --- LCD_P22. */
}

void BOARD_InitHardware(void)
{
    BOARD_InitDebugConsole();
    BOARD_InitDEBUG_UARTPins();

    /* Release peripheral reset */
    RESET_ReleasePeripheralReset(kAonLCD_RST_SHIFT_RSTn);
}

/*${function:end}*/