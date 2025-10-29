/*
 * Copyright 2018, 2025 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_KPP_ACTIVE_COLUMROWS  (0x03) // COL/ROW 0,1
#define EXAMPLE_KPP_MATRX_NUM         (2)
#define EXAMPLE_KPP_COLROW_START_INDX (0)
#define EXAMPLE_KPP                   AON__KPP0
#define EXAMPLE_KPP_SYS_CLK_FREQ      SystemCoreClock
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*${variable:start}*/
char *keyMap[EXAMPLE_KPP_MATRX_NUM][EXAMPLE_KPP_MATRX_NUM] = {
    {
        "SW1",
        "SW2",
    },
    {
        "SW3",
        "SW4",
    },
};
/*${variable:end}*/

#endif /* _APP_H_ */
/*******************************************************************************
 * EOF
 ******************************************************************************/