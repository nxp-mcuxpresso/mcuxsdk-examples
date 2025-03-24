/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_slcd.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*${variable:start}*/
const uint8_t slcd_lcd_gpio_seg_pin[] = {
    16, /* SLCD P01 --- LCD_P16. */
    17, /* SLCD P02 --- LCD_P17. */
    18, /* SLCD P03 --- LCD_P18. */
    19, /* SLCD P04 --- LCD_P19. */
    20, /* SLCD P05 --- LCD_P20. */
    21, /* SLCD P06 --- LCD_P21. */
    22, /* SLCD P07 --- LCD_P22. */
    23, /* SLCD P08 --- LCD_P23. */
    24, /* SLCD P09 --- LCD_P24. */
    25, /* SLCD P10 --- LCD_P25. */
    26, /* SLCD P11 --- LCD_P26. */
    27, /* SLCD P12 --- LCD_P27. */
};
/*${variable:end}*/

/*${function:start}*/
void BOARD_SetSlcdBackPlanePhase(void)
{
    SLCD_SetBackPlanePhase(LCD, 28, kSLCD_PhaseAActivate); /* SLCD COM13 --- LCD_P28. */
    SLCD_SetBackPlanePhase(LCD, 29, kSLCD_PhaseBActivate); /* SLCD COM14 --- LCD_P29. */
    SLCD_SetBackPlanePhase(LCD, 30, kSLCD_PhaseCActivate); /* SLCD COM15 --- LCD_P30. */
    SLCD_SetBackPlanePhase(LCD, 31, kSLCD_PhaseDActivate); /* SLCD COM16 --- LCD_P31. */
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitSLCDPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Release peripheral reset */
    RESET_ReleasePeripheralReset(kSLCD0_RST_SHIFT_RSTn);
}
/*${function:end}*/
