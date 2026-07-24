/*
 * Copyright 2022, 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_SLAVE              HSP__I3C
#define I3C_SLAVE_CLOCK_FREQUENCY  CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_i3c0_fclk)
#if 0
#define I3C_ASYNC_WAKE_UP_INTR_CLEAR                    \
    {                                                   \
        BLK_CTRL_WAKEUPMIX->I3C2_ASYNC_WAKEUP_CTRL = 1; \
    }
#endif
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
