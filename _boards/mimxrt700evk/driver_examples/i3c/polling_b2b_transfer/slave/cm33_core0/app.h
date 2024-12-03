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
#define EXAMPLE_SLAVE              I3C0
#define I3C_SLAVE_CLOCK_FREQUENCY  CLOCK_GetI3cClkFreq()
#define I3C_ASYNC_WAKE_UP_INTR_CLEAR                    \
    {                                                   \
        SYSCON0->I3C_ASYNC_WAKEUP_CTRL = SYSCON0_I3C_ASYNC_WAKEUP_CTRL_IRQ_CLR0_MASK; \
    }
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
